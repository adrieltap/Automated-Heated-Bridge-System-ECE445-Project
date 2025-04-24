import asyncio
from bleak import BleakScanner, BleakClient
#UI pygame

# Same UUIDs as defined in the ESP32 code.
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

async def notification_handler(sender, data):
    # Convert the received bytes into a string and print.
    print("Notification from ESP32:", data.decode())

async def run():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover()
    
    # Automatically find the device with the matching name.
    esp32_address = None
    for device in devices:
        print(f"Found device: {device.name}, address: {device.address}")
        if device.name == "ESP32_BLE":
            esp32_address = device.address
            break

    if esp32_address is None:
        print("ESP32 device not found!")
        return

    print("Connecting to ESP32 at", esp32_address)
    async with BleakClient(esp32_address) as client:
        if client.is_connected:
            print("Connected to ESP32!")
            # Start notifications on the characteristic.
            await client.start_notify(CHARACTERISTIC_UUID, notification_handler)
            
            # Keep the connection open for 30 seconds to receive notifications.
            await asyncio.sleep(60 * 30)
            
            # Stop notifications and disconnect.
            await client.stop_notify(CHARACTERISTIC_UUID)
        else:
            print("Failed to connect.")

if __name__ == '__main__':
    asyncio.run(run())
