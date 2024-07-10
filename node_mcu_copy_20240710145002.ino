#include <ESP8266WiFi.h>
#include "DHT.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = "";   // your network SSID (name) 
char pass[] = "";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 2587305;
const char * myWriteAPIKey = "Z4X08NT9BR356PYQ";

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  WiFi.mode(WIFI_STA); 
  dht.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  float water_level = 0.0;
  float moister = 0.0;
  if (Serial.available()) {
  String receivedData = Serial.readStringUntil('\n');
  Serial.print("Received data: ");
  Serial.println(receivedData);
  int commaIndex1 = receivedData.indexOf(',');
  int commaIndex2 = receivedData.indexOf(',', commaIndex1 + 1);
  water_level = receivedData.substring(0, commaIndex1).toFloat();
  moister = receivedData.substring(commaIndex1 + 1, commaIndex2).toFloat();
  Serial.println(water_level);
  Serial.println(moister);
  }
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int gasSensorValue = analogRead(A0);
  Serial.println(water_level);
  Serial.println(moister);

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, gasSensorValue);
  ThingSpeak.setField(4, water_level);
  ThingSpeak.setField(5, moister);

 int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(x == 200){
    Serial.println("Channel update successful.");
    Serial.print(F("Humidity: "));Serial.print(h);Serial.print(F("%  Temperature: "));Serial.print(t);Serial.print(F("°C "));
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); // Wait 20 seconds to update the channel again
}