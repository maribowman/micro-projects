#include <Wire.h>
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
  Wire.begin();
  pinMode(dataPinD4, INPUT);
  dht.begin();
  startWifi();
}


void loop() {
  sendSensorData();
  delay(5000);
//  delay(5*60*1000);
//  sleep(sleepMinutes);
}


/*
 * establish wifi-connection
 *
 * sleep 5min if connection failed
 */
void startWifi() {
  WiFi.mode(WIFI_STA);
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
 * post-request to send meteor data
 */
void sendSensorData () {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float heatindex = dht.computeHeatIndex(temperature, humidity, false);

  Serial.println("---");
  Serial.println("[DATA] temperature: " + String(temperature) + " °C");
  Serial.println ("[DATA] humidity: " + String(humidity) + " %");
  Serial.println("[DATA] relativ: " + String(heatindex) + " °C");


  String payload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
//  String payload = "{temperature:" + String(temperature) + ",humidity:" + String(humidity) + ",pressure:" + String(pres) + "}";
  Serial.println("[HTTP] sending meteor data");
  HTTPClient http;
  int httpCode = -1;

  Serial.print("[HTTP] url: ");
  Serial.println(url);
  Serial.print("[HTTP] payload: ");
  Serial.println(payload);
  httpCode = http.POST(payload);
  if (httpCode > 0) {
    Serial.printf("[HTTP] received status: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("[HTTP] result: OK.");
    }
  }
}

/*
 * deep sleep in min
 */
void sleep (int minutes) {
  #ifdef FORCE_DEEPSLEEP
    ESP.deepSleep(minutes * 60 * 1000000);
  #endif
}
