#include "ESP8266WebServer.h"
#define LEDPIN D1
const char* ssid = "Deine SSID";
const char* pass = "Dein Passwort";
IPAddress ip(192,168,178,xxx); // xxx = wählt eine frei IP in eurem Heimnetz
IPAddress gateway(192,168,178,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);
 
String led1= "<a href=\"/led_an\">LED An</a>";
String led0= "<a href=\"/led_aus\">LED Aus</a>";
void handleRoot() {
  String message="<h1>Testprogramm - Minimalprogramm ESP8266</h1>";
  message += "Hallo ......, das ist ein Gru&szlig vom ESP8266 Server</BR></BR>";
  message += led1;
  server.send(200, "text/html", message);
}
void ledan(){
  digitalWrite(LEDPIN, HIGH);
  server.send(200, "text/html", led0);
}
void ledaus(){
  digitalWrite(LEDPIN, LOW);
  server.send(200, "text/html", led1);
}
void setup(){
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
//  Serial.begin(9600); 
//  Serial.println("Testprogramm - Minimalprogramm ESP8266");
//  Serial.print("Verbinde mich mit Netz: ");
//  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet); 
  
//  while(WiFi.status() != WL_CONNECTED){
//  delay(500); Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi Verbindung aufgebaut");
//  Serial.print("Eigene IP des ESP-Modul: ");
//  Serial.println(WiFi.localIP());
  server.on("/",handleRoot);
  server.on("/led_an", ledan);
  server.on("/led_aus", ledaus);
  server.begin();
//  Serial.println("HTTP Server wurde gestartet!");
} 
  
void loop(){
  server.handleClient(); 
}
