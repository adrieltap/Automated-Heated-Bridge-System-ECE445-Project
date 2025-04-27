import sys, time, asyncio
from collections import deque
import csv

from PySide6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QPushButton, QDoubleSpinBox, QSpinBox
)
import pyqtgraph as pg
from bleak import BleakScanner, BleakClient
from qasync import QEventLoop, asyncSlot

#Constant UUIDs
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHAR_UUID    = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

HISTORY_SEC = 60 * 20      # seconds shown on x-axis
SAMPLE_SEC  = 2        # ESP32 sends roughly every 2 s

DEF_AIR = 10
DEF_SURF = 10
DEF_MOIS = 3500
DEF_OVER = 20


class Dashboard(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ESP32 Bridge Monitor")

        #Init Plots/Curves
        self.temp_plot = pg.PlotWidget(background="w")
        self.temp_plot.setTitle("Temperature (°C)")
        self.temp_plot.addLegend()
        self.rain_plot = pg.PlotWidget(background="w")
        self.rain_plot.setTitle("Rain (raw)")
        self.rain_plot.addLegend()

        self.surf_curve = self.temp_plot.plot(pen="r", name="surface")
        self.air_curve  = self.temp_plot.plot(pen="b", name="air")
        self.rain_curve = self.rain_plot.plot(pen="g", name="rain")
        self.s_thr_curve = self.temp_plot.plot(pen="k", name="surf thr")
        self.a_thr_curve = self.temp_plot.plot(pen="c", name="air  thr")
        self.o_thr_curve = self.temp_plot.plot(pen="m", name="over thr")
        self.m_thr_curve = self.rain_plot.plot(pen="darkgreen", name="moist thr")

        #Spin-box inits for controling thresholds
        self.air_spin   = self.dbl_box("air( °C)      ", "", DEF_AIR)
        self.surf_spin  = self.dbl_box("surface(°C)  ", "", DEF_SURF)
        self.moist_spin = self.int_box("moisture(Ω) ", DEF_MOIS)
        self.over_spin  = self.dbl_box("overheat(°C) ", "", DEF_OVER)

        self.lbl_surface = QLabel("Surface: — °C")
        self.lbl_air     = QLabel("Air:     — °C")
        self.lbl_rain    = QLabel("Rain:    —")
        self.lbl_heater  = QLabel("Heater:  —")

        self.update_btn = QPushButton("Update thresholds")
        self.update_btn.clicked.connect(self.send_thresholds) #Links button press to func

        self.reset_btn  = QPushButton("Reset to defaults")
        self.reset_btn.clicked.connect(self.reset_thresholds)

        #Create the layout with right and left sides
        left = QVBoxLayout()
        left.addWidget(self.temp_plot)
        left.addWidget(self.rain_plot)

        right = QVBoxLayout()
        right.addWidget(QLabel("Thresholds:"))
        for w in (self.air_spin, self.surf_spin, self.moist_spin, self.over_spin):
            right.addWidget(w)
        right.addWidget(self.update_btn)
        right.addWidget(self.reset_btn)
        right.addStretch()

        right.addWidget(QLabel("Latest Values:"))
        for w in (self.lbl_surface, self.lbl_air, self.lbl_rain, self.lbl_heater):
            right.addWidget(w)
        right.addSpacing(10)

        main = QHBoxLayout(self)
        main.addLayout(left, 2); main.addLayout(right, 1)

        # buffers
        n = HISTORY_SEC // SAMPLE_SEC
        self.t     = deque(maxlen=n)
        self.surf  = deque(maxlen=n)
        self.air   = deque(maxlen=n)
        self.rain  = deque(maxlen=n)
        self.s_thr = deque(maxlen=n)
        self.a_thr = deque(maxlen=n)
        self.o_thr = deque(maxlen=n)
        self.m_thr = deque(maxlen=n)

        self.t0 = time.time()

        asyncio.get_event_loop().create_task(self.ble_loop())
    
    def start_csv(self):
        self.t0 = time.time()  # reset timer at start of test
        t_now = int(self.t0)   # get the start timestamp in seconds

        filename = f"test_log_{t_now}.csv"
        self.csv_file = open(filename, mode='w', newline='')
        self.csv_writer = csv.writer(self.csv_file)

        # Write CSV header
        self.csv_writer.writerow(["Surface Temp Threshold", "Air Temp Threshold", "Rain Level Threshold", "Overheat Threshold"])
        self.csv_writer.writerow([self.s_thr, self.a_thr, self.m_thr, self.o_thr])
        self.csv_writer.writerow(["Time (s)", "Surface Temp (C)", "Air Temp (C)", "Rain Level", "Heater State", "Surface Temp Threshold (C)",])

    def end_csv(self):
        if hasattr(self, 'csv_file'):
            self.csv_file.close()
    
    def closeEvent(self, event):
        self.end_csv()
        event.accept()


    # spin-box helpers
    def dbl_box(self, prefix, suffix, default):
        b = QDoubleSpinBox(decimals=1, singleStep=0.1)
        b.setPrefix(prefix); b.setSuffix(suffix); b.setRange(-50, 150); b.setValue(default)
        return b
    def int_box(self, prefix, default):
        b = QSpinBox(); b.setPrefix(prefix); b.setRange(0, 4095); b.setValue(default)
        return b

    # BLE discovery → connect → notifications
    async def ble_loop(self):
        print("Scanning…")
        dev = next((d for d in await BleakScanner.discover(5) if d.name == "ESP32_BLE"), None)
        if not dev: print("ESP32_BLE not found"); return
        self.client = BleakClient(dev.address)
        await self.client.connect()
        await self.client.start_notify(CHAR_UUID, self.handle_pkt)
        print("Connected; receiving data")
        await self.send_thresholds()
        self.send_thresholds()
        self.start_csv()
        while self.client.is_connected:
            await asyncio.sleep(1)

    # packet handler (update buffers & curves)
    def handle_pkt(self, _s, data):
        try:
            surf, air, rain, _heater = map(float, data.decode().split(";"))
        except ValueError:
            return

        print(surf, air, rain, _heater)
        self.lbl_surface.setText(f"Surface: {surf:.1f} °C")
        self.lbl_air.setText    (f"Air:     {air:.1f} °C")
        self.lbl_rain.setText   (f"Rain:    {rain:.0f}")
        self.lbl_heater.setText (f"Heater:  {'ON' if _heater else 'OFF'}")

         # --- CSV logging section ---
        if hasattr(self, 'csv_writer'):
            t_now = time.time() - self.t0 
            self.csv_writer.writerow([t_now, surf, air, rain, int(_heater)])
        # ----------------------------------

        #Update buffer vals
        t_now = time.time() - self.t0
        self.t.append(t_now)
        self.surf.append(surf)
        self.air.append(air)
        self.rain.append(rain)
        self.s_thr.append(self.surf_spin.value())
        self.a_thr.append(self.air_spin.value())
        self.o_thr.append(self.over_spin.value())
        self.m_thr.append(self.moist_spin.value())

        #Buf vals --> curve
        self.surf_curve.setData(self.t, self.surf)
        self.air_curve .setData(self.t, self.air)
        self.rain_curve.setData(self.t, self.rain)
        self.s_thr_curve.setData(self.t, self.s_thr)
        self.a_thr_curve.setData(self.t, self.a_thr)
        self.o_thr_curve.setData(self.t, self.o_thr)
        self.m_thr_curve.setData(self.t, self.m_thr)

    #Sends threshold vals through BLE
    @asyncSlot()
    async def send_thresholds(self):
        msgs = [
            f"air={self.air_spin.value():.1f}",
            f"surface={self.surf_spin.value():.1f}",
            f"moisture={self.moist_spin.value()}",
            f"overheat={self.over_spin.value():.1f}",
        ]
        for m in msgs:
            try:
                await self.client.write_gatt_char(CHAR_UUID, m.encode(), response=True)
                print("Sent:", m)
            except Exception as e:
                print("BLE write failed:", e)

    async def reset_thresholds(self):
        self.air_spin.setValue(DEF_AIR)
        self.surf_spin.setValue(DEF_SURF)
        self.moist_spin.setValue(DEF_MOIS)
        self.over_spin.setValue(DEF_OVER)
        await self.send_thresholds() 

# run app (main code)
if __name__ == "__main__":
    app = QApplication(sys.argv)
    loop = QEventLoop(app)
    asyncio.set_event_loop(loop)
    win = Dashboard()
    win.resize(960, 560)
    win.show()
    with loop: loop.run_forever() #Run with UI window until closed
