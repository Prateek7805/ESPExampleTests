#include "WiFiSecrets.h"
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>


ESP8266WebServer s1(80), s2(80);
bool server_count = 0;
void setup() {
  Serial.begin(115200);
  s1.on("/", HTTP_GET, [](){
    s1.send(200, "text/plain", "hello from server 1");
  });
  s2.on("/", HTTP_GET, [](){
    s2.send(200, "text/plain", "hello from server 2");
  });
  WiFi.begin(STA_SSID, STA_PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
  
}
unsigned long timer = millis();
void loop() {
  if(millis() - timer > 10000){
    server_count = !server_count;
    if(server_count){
      s1.stop();
      s2.begin();
    }else{
      s2.stop();
      s1.begin();
    }
    timer = millis();
  }
  if(server_count){
    s2.handleClient();
  }else{
    s1.handleClient();
  }
}
