//#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include "settings.h"

ESP8266WiFiMulti WiFiMulti;

#define FORCE_DEEPSLEEP
//#define DHT_TYPE        DHT22
//#define DHT_PIN         3
//
//DHT_Unified dht(DHT_PIN, DHT_TYPE);


void setup() {
  Serial.begin(9600);
  //while(!Serial) {} // Wait
  Wire.begin();

  splashScreen();

  Serial.println("---");
  Serial.println("Searching for sensor:");
  Serial.print("Result: ");
//  while(!dht.begin())
//  {
//    Serial.println("Could not find BME280 sensor!");
//    delay(500);
//  }
  startWifi();
}


void loop() {
//  sendSensorData();
  sleep(sleepMinutes);
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
    Serial.print(".");
    tries++;

    if (tries > 60) {
      Serial.println("Could not connect to WiFi. Sending device to bed.");
      sleep(5);
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


/*
 * post-request to send meteo data
 */
void sendSensorData () {
  float temp(NAN), hum(NAN), pres(NAN);
  
//  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
//  BME280::PresUnit presUnit(BME280::PresUnit_bar);
//  bme.read(pres, temp, hum, tempUnit, presUnit);
//  pres = pres * 1000; // convert to millibar
  
  String payload = "token=&temperature=" + String(temp) + "&humidity=" + String(hum) + "&pressure=" + String(pres);
  Serial.println("---");
  Serial.println("[HTTP] Start connection info request...");

  Serial.println("[HTTP] Sending data.");
  HTTPClient http;
  int httpCode = -1;

  Serial.print("[HTTP] URL: ");
  Serial.println(url);
  Serial.print("[HTTP] Payload: ");
  Serial.println(payload);
  httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("[HTTP] Result: OK.");
    }
  }

  Serial.println("[HTTP] End connection.");
  Serial.println("---");
}

/*
 * deep sleep in min
 */
void sleep (int minutes) {
  #ifdef FORCE_DEEPSLEEP
    ESP.deepSleep(minutes * 60 * 1000000);
  #endif
}


/**
 * Dump some information on startup.
 */
void splashScreen() {
  for (int i=0; i<=2; i++) Serial.println();
  Serial.println("#######################################");
  Serial.print("# DeviceName: ");
  Serial.println(d1Name);
  Serial.print("# Configured Endpoint: ");
  Serial.println(url);
  Serial.println("#######################################");
  for (int i=0; i<2; i++) Serial.println();
}
