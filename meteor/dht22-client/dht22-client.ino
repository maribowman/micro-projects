//#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#include "settings.h"

ESP8266WiFiMulti WiFiMulti;

#define FORCE_DEEPSLEEP
#define DHT_TYPE DHT22
int dataPinD4 = 2;

DHT dht(dataPinD4, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  delay(500);
//  Wire.begin();
  pinMode(dataPinD4, INPUT);
  dht.begin();
//  startWifi();
}


void loop() {
  sendSensorData();
  delay(3000);
//  sleep(sleepMinutes);
}


/*
 * establish wifi-connection
 * 
 * sleep 5min if connection failed
 */
void startWifi() {
  Serial.println("---");
  WiFi.mode(WIFI_STA);
  Serial.println("(Re)Connecting to Wifi-Network with following credentials:");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Key: ");
  Serial.println(password);
  Serial.print("Device-Name: ");
  Serial.println(d1Name);
  
  WiFi.hostname(d1Name);
  WiFiMulti.addAP(ssid, password);
  int tries = 0;
  
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    tries++;

    if (tries > 60) {
      Serial.println("could not connect to wifi. retrying after sleep.");
      sleep(5);
    }
  }

  Serial.println("wifi connected");  
  Serial.print("ip address: ");
  Serial.println(WiFi.localIP());
}


/*
 * post-request to send meteo data
 */
void sendSensorData () {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float heatindex = dht.computeHeatIndex(temperature, humidity, false);

  Serial.println("---");
  Serial.println("temperature: " + String(temperature) + " °C");
  Serial.println ("humidity: " + String(humidity) + " %");
  Serial.println("relativ: " + String(heatindex) + " °C");

  
//  String payload = "token=&temperature=" + String(temp) + "&humidity=" + String(hum) + "&pressure=" + String(pres);
//  Serial.println("---");
//  Serial.println("[HTTP] Start connection info request...");
//
//  Serial.println("[HTTP] Sending data.");
//  HTTPClient http;
//  int httpCode = -1;
//
//  Serial.print("[HTTP] URL: ");
//  Serial.println(url);
//  Serial.print("[HTTP] Payload: ");
//  Serial.println(payload);
//  httpCode = http.POST(payload);
//
//  if (httpCode > 0) {
//    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//    if (httpCode == HTTP_CODE_OK) {
//      Serial.println("[HTTP] Result: OK.");
//    }
//  }
//
//  Serial.println("[HTTP] End connection.");
//  Serial.println("---");
}

/*
 * deep sleep in min
 */
void sleep (int minutes) {
  #ifdef FORCE_DEEPSLEEP
    ESP.deepSleep(minutes * 60 * 1000000);
  #endif
}
