## Arduino

The `arduino` folder contains all the code related to the Arduino component of our project.

The ESP32 uses a BME688 sensor to monitor air quality, humidity, and temperature and sends the data to a central hub.
This data can be queried via: https://app-side.net:10000/get-sensor-data
To be able to use it, you will have to install two external libraries.
