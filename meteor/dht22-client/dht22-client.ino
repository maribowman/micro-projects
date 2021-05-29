#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#include "settings.h"

#define FORCE_DEEPSLEEP
#define DHT_TYPE DHT22
int dataPinD4 = 2;

ESP8266WiFiMulti WiFiMulti;
DHT dht(dataPinD4, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();
  pinMode(dataPinD4, INPUT);
  dht.begin();
  startWifi();
}


void loop() {
  sendSensorData();
  delay(60*1000);
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
  float temperature(NAN), humidity(NAN), heatindex(NAN);

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  heatindex = dht.computeHeatIndex(temperature, humidity, false);

  Serial.println("---");
  Serial.println("[DATA] temperature: " + String(temperature) + " °C");
  Serial.println("[DATA] humidity: " + String(humidity) + " %");
  Serial.println("[DATA] heatindex: " + String(heatindex) + " °C");

  Serial.println("[HTTP] sending meteor data");

  WiFiClient client;
  HTTPClient http;
  int httpCode = -1;
  http.begin(client, url);
  http.addHeader("Content-Type", contentType);
  httpCode = http.POST("{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}");
  if (httpCode > 0) {
    Serial.printf("[HTTP] received status: %d\n", httpCode);
  }
  http.end();
}

/*
 * deep sleep in min
 */
void sleep (int minutes) {
  #ifdef FORCE_DEEPSLEEP
    ESP.deepSleep(minutes * 60 * 1000000);
  #endif
}
