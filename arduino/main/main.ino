#include <WiFi.h>
#include <Wire.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //setupWifi();

  Wire.setPins(21,22);
  Wire.begin();
}

void setupWifi() {
  const char* ssid = "ENO"; 
  const char* password = "02826ENO@innolabs!"; //TODO check

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
      Serial.println("Status:" + WiFi.status());
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  // put your main code here, to run repeatedly:
   byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}
