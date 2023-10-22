/**
 * Partially taken from:
 * Copyright (C) 2021 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* If compiling this examples leads to an 'undefined reference error', refer to the README 
 * at https://github.com/BoschSensortec/Bosch-BSEC2-Library
 */
/* The new sensor needs to be conditioned before the example can work reliably. You may run this
 * example for 24hrs to let the sensor stabilize.
 */

/**
 * basic.ino sketch :
 * This is an example for illustrating the BSEC virtual outputs and
 * which has been designed to work with Adafruit ESP8266 Board
 */


#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <bsec2.h>
#include <HTTPClient.h>


// #################### MACROS ####################
#define ERROR_DUR   1000
#define PIN_SCL		22
#define PIN_SDA		21

const char* WIFI_SSID = "ENO"; 
const char* WIFI_PASSWORD = "02826ENO@innolabs!";
const String SERVER_URL = "https://app-side.net:10000/post-sensor-data";
const long SEND_TO_PROXY_INTERVAL = 1000; // Send POST request every 60 seconds

Bsec2 envSensor;
HTTPClient http;
unsigned long previousMillis = 0;

// collected data
float iaq = 0.0;
float temperature = 0.0;
float pressure = 0.0;
float humidity = 0.0;
float gasResistance = 0.0;
float stabilizationStatus = 0.0;
float runInStatus = 0.0;

// #################### FUNCTION DECLARATIONS ####################
/**
 * @brief : This function halts the program and prints the last error when a fault was detected
 */
void halt(String lastError);

/**
 * @brief : This function checks the BSEC status, prints the respective error code. Halts in case of error
 * @param[in] bsec  : Bsec2 class object
 */
void checkBsecStatus(Bsec2 bsec);

/**
 * @brief : This function is called by the BSEC library when a new output is available
 * @param[in] input     : BME68X sensor data before processing
 * @param[in] outputs   : Processed BSEC BSEC output data
 * @param[in] bsec      : Instance of BSEC2 calling the callback
 */
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

/**
 * @brief : Sets up wifi
 */
void setupWifi(void);

/**
 * @brief : Sets up the BME688 sensor
 */
void setupBME688(void);

/**
 * @brief : Sends the data to an intermediate webserver json proxy (fully under our control) that will then serve the requests of the clients
 */
void sendDataToProxy(void);

// #################### IMPLEMENTATION ####################

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial Initialized");

  setupWifi();
  setupBME688();
}

void loop() {
    /* Call the run function often so that the library can 
     * check if it is time to read new data from the sensor  
     * and process it.
     */
    if (!envSensor.run()) {
        checkBsecStatus(envSensor);
    }

    sendDataToProxy();
}

void sendDataToProxy() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= SEND_TO_PROXY_INTERVAL) {
    // Save the last time a request was sent
    previousMillis = currentMillis;
   // Create a JSON object and add sensor data
    DynamicJsonDocument jsonDoc(256); // Adjust the size according to your data
    jsonDoc["iaq"] = iaq;
    jsonDoc["temperature"] = temperature;
    jsonDoc["pressure"] = pressure;
    jsonDoc["humidity"] = humidity;
    jsonDoc["gasResistance"] = gasResistance;
    jsonDoc["stabilizationStatus"] = stabilizationStatus;
    jsonDoc["runInStatus"] = runInStatus;

    // Serialize JSON to a string
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    http.POST(jsonString);
  }
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
      Serial.println("Status:" + WiFi.status());
  }
  
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());
}

void setupBME688() {

     /* Desired subscription list of BSEC2 outputs */
    bsecSensor sensorList[] = {
            BSEC_OUTPUT_IAQ,
            BSEC_OUTPUT_RAW_TEMPERATURE,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_RAW_HUMIDITY,
            BSEC_OUTPUT_RAW_GAS,
            BSEC_OUTPUT_STABILIZATION_STATUS,
            BSEC_OUTPUT_RUN_IN_STATUS
    };

    Wire.setPins(PIN_SDA,PIN_SCL);
    Wire.begin();
   /* Valid for boards with USB-COM. Wait until the port is open */
    while(!Serial) delay(10);

    /* Initialize the library and interfaces */
    if (!envSensor.begin(BME68X_I2C_ADDR_HIGH, Wire)) {
        checkBsecStatus(envSensor);
    }

    /* Subsribe to the desired BSEC2 outputs */
    if (!envSensor.updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_LP)) {
        checkBsecStatus(envSensor);
    }

    /* Whenever new data is available call the newDataCallback function */
    envSensor.attachCallback(newDataCallback);

    Serial.println("BSEC library version " + \
            String(envSensor.version.major) + "." \
            + String(envSensor.version.minor) + "." \
            + String(envSensor.version.major_bugfix) + "." \
            + String(envSensor.version.minor_bugfix));
}

void halt(String lastError) {
    while(1) {
        delay(ERROR_DUR);
        Serial.println("Last Error: " + lastError);
    }
}

void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec) {
    if (!outputs.nOutputs) {
        return;
    }

    // Store sensor data in global variables
    iaq = outputs.output[0].signal;
    temperature = outputs.output[1].signal;
    pressure = outputs.output[2].signal;
    humidity = outputs.output[3].signal;
    gasResistance = outputs.output[4].signal;
    stabilizationStatus = outputs.output[5].signal;
    runInStatus = outputs.output[6].signal;

    Serial.println("BSEC outputs:\n\ttimestamp = " + String((int) (outputs.output[0].time_stamp / INT64_C(1000000))));
    for (uint8_t i = 0; i < outputs.nOutputs; i++) {
        const bsecData output  = outputs.output[i];
        switch (output.sensor_id) {
            case BSEC_OUTPUT_IAQ:
                Serial.println("\tiaq = " + String(output.signal));
                Serial.println("\tiaq accuracy = " + String((int) output.accuracy));
                break;
            case BSEC_OUTPUT_RAW_TEMPERATURE:
                Serial.println("\ttemperature = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_PRESSURE:
                Serial.println("\tpressure = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_HUMIDITY:
                Serial.println("\thumidity = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_GAS:
                Serial.println("\tgas resistance = " + String(output.signal));
                break;
            case BSEC_OUTPUT_STABILIZATION_STATUS:
                Serial.println("\tstabilization status = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RUN_IN_STATUS:
                Serial.println("\trun in status = " + String(output.signal));
                break;
            default:
                break;
        }
    }
}

void checkBsecStatus(Bsec2 bsec) {
    if (bsec.status < BSEC_OK) {
        halt("BSEC error code : " + String(bsec.status));
    } else if (bsec.status > BSEC_OK) {
        Serial.println("BSEC warning code : " + String(bsec.status));
    }

    if (bsec.sensor.status < BME68X_OK) {
        halt("BME68X error code : " + String(bsec.sensor.status)); 
    }
    else if (bsec.sensor.status > BME68X_OK) {
        halt("BME68X warning code : " + String(bsec.sensor.status));
    }
}