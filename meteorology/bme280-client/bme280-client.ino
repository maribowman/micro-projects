#include <Adafruit_BME280.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include "settings.h"

ESP8266WiFiMulti WiFiMulti;

#define SERIAL_BAUD 115200
#define FORCE_DEEPSLEEP

Adafruit_BME280 bme;


void setup() {
  Serial.begin(SERIAL_BAUD);
  //while(!Serial) {} // Wait
  Wire.begin();

  splashScreen();

  Serial.println("---");
  Serial.println("Searching for sensor:");
  Serial.print("Result: ");
  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(500);
  }
  switch(bme.chipModel()) {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }
  startWIFI();
}


void loop() {
  sendSensorData(); //send data
  sleep(sleepMinutes); //sending into deep sleep
}


/*
 * establish wifi-connection
 * 
 * sleep 5min if connection failed
 */
void startWIFI() {
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
    tryCnt++;

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
  
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_bar);
  bme.read(pres, temp, hum, tempUnit, presUnit);
  pres = pres * 1000; // convert to millibar
  
  String payload = "token=" + token + "&temperature=" + String(temp) + "&humidity=" + String(hum) + "&pressure=" + String(pres);
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
    Serial.print(minutes);
    ESP.deepSleep(minutes * 60 * 1000000);
  #endif
}


/**
 * Dump some information on startup.
 */
void splashScreen() {
  for (int i=0; i<=5; i++) Serial.println();
  Serial.println("#######################################");
  Serial.print("# ");

  Serial.println("# -----------");
  Serial.println("# Chris Figge (flazer)");
  Serial.println("# Mail: info@flazer.net");
  Serial.println("# -----------");
  Serial.print("# DeviceName: ");
  Serial.println(d1Name);
  Serial.print("# Configured Endpoint: ");
  Serial.println(url);
  Serial.println("#######################################");
  for (int i=0; i<2; i++) Serial.println();
}
