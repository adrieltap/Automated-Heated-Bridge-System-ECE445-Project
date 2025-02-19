## 2/16/2025

* Met up as a group on a discord call from 3-5 PM. Talked about design choices, design review and how we are going to split work when presenting.
* Design modifications if proposal does not get accepted.
	* If more complexity is needed:
		* Add one more temperature sensor and one more moisture sensor.
			* We would then take the average between the two temperature sensors to get a more reliable reading and we could also do the same for the moisture sensor.
		* Design a UI app
			* This app would allow the user to turn "on" our system. Which means the system would start sensing the environment and heat up when necessary. 
			* It would also allow the user to turn it off
			* The app can also show the user how much money / resources they would have saved compared to if they kept the heater running the whole time.
* We also went over the placement and wiring of the sensors
	* Option 1 -> Place moisture sensor in the middle of the plate and the temperature sensor under the plate, there would be something to insulate the temperature sensor so it does not get incorrect reading if the heat is on.
		* We could ask machine shop to poke a hole between the plate to get our wires through
	* Option 2 -> Use ESP32 bluetooth and wifi features to send data to the MCU, this would require a separate battery for each sensors since they will be separated.
* Before our demo, I am planning on researching more about the ESP32, and our main event loop and how it will calculate if we would need to turn the heater on. 
	* How we decide to turn on the heat
		* If temperature < 0 Celsius AND moisture level is greater than the threshold
		* If temperature >= 0 Celsius OR moisture level is less than or equal to the threshold
	* Some pseudo code:
```
	if (temperature < 0°C AND moisture_level > threshold):
	    // Activate heating if freezing and moisture present
	    if (heating_status == OFF):
	        turn_on_heating()
	        heating_status = ON
	        log_event("Heating activated due to freezing and moisture detected.")
	else if (temperature >= 0°C OR moisture_level <= threshold):
	    // Deactivate heating if conditions are safe
	    if (heating_status == ON):
	        turn_off_heating()
	        heating_status = OFF
	        log_event("Heating deactivated - safe conditions.")
	else:
	    // Default case: system is in a neutral state, no action needed
	    log_event("System in neutral state - no heating needed.")
```
* ESP32
	* Temperature Sensor
	* Moisture Sensor
	* MOSFET-Based Switching
## 2/17/2025 - Design Review

* Met up again before our demo to prepare, talked about placement of moisture sensors and temperature sensors as well. Moisture sensor can be difficult since we would need to detect water on the whole entire plate.
	* Moisture sensor placements
		* Drainage system to detect water?
		* Flat Moisture sensors that can detect water.
* During Demo / Post Demo
	* Advice that we were told:
		* We will lose a lot of heat heating the surface because of air.
		* Look into how heated driveways work to see if we can derive it
		* Look into how bathroom heated floors work to see if we can also derive it as well
		* Fix block diagrams, have a legend for different communication protocols. Have actual square boxes with its subsystem in smaller boxes
		* Fail safe option if there is too much ice and our system cannot catch up.
		* Look into the temperatures allowed for our peripherals and parts.
		* Placements of PCB, how much we will heat and how to scale it
## 2/19/2025 

*  Peer Reviewed GymHive Tracker group
*  Different parts to consider:
	* Water sensor:  https://www.amazon.com/dp/B0BXKMLB4D?&linkCode=sl1&tag=zlufy-20&linkId=c3cc423f6887383dbbef16c3d21264c7&language=en_US&ref_=as_li_ss_tl
	* Rain Sensor: https://www.amazon.com/dp/B0CM6224T2?linkCode=sl1&tag=zlufy-20&linkId=d6c3b90b94272148cb801cf57015b64e&language=en_US&ref_=as_li_ss_tl&th=1
* Goal for the end of this week is to figure out the exact parts to buy so we can start on our PCB design.
* Plan is to meet up on Friday to discuss with machine shop with how to approach the metal plate.
* 


	
	