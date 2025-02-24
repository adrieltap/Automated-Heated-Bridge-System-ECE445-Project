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