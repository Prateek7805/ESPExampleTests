/*
Code Dependencies : ESPAsyncTCP, ESPAsyncWebServer
The WiFiSecrets.h is just a header file created with the following format

#ifndef _WIFI_SECRETS_H
  #define _WIFI_SECRETS_H
  #define STA_SSID "your_SSID"
  #define STA_PASS "your_Password"
#endif _WIFI_SECRETS_H

The STA_SSID and STA_PASS are used in connecting to your Access point (Hotspot or WiFi Router)

*/

#include "WiFiSecrets.h"  

#include <ESP8266WiFi.h>  // Include the ESP8266WiFi library
#include <ESPAsyncWebServer.h>  // Include the ESPAsyncWebServer library
#include <ESPAsyncTCP.h>  // Include the ESPAsyncTCP library

AsyncWebServer server(80);  // Create an AsyncWebServer object and listen on port 80

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud

  WiFi.mode(WIFI_STA);  // Set WiFi mode to Station mode

  WiFi.begin(STA_SSID, STA_PASS);  // Connect to the WiFi network using the SSID and password from WiFiSecrets.h
  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED){  // Wait until connected to the WiFi network
    Serial.print(".");  // Print dots to indicate the connection process
  }
  Serial.println("WiFi Connected");  // Print "WiFi Connected" when connected to the WiFi network
  Serial.println(WiFi.localIP());  // Print the local IP address assigned to the ESP8266 by the router

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * req){  // Register a handler for the root URL
    req->send(200, "text/plain", "ok");  // Send a "200 OK" response with "ok" as the response body
  });
  server.begin();  // Start the server
}

unsigned long ticker = millis();  // Initialize a variable to track time
bool serverStarted = true;  // Initialize a flag to indicate whether the server is started or not
unsigned long interval = 10000; // Set the interval between server status toggle.
 
void loop() {
  if(millis() - ticker > interval){  // Check if 10 seconds have elapsed
    if(serverStarted) server.end();  // If the server is started, stop it
    else server.begin();  // If the server is stopped, start it
    serverStarted = !serverStarted;  // Toggle the serverStarted flag
    Serial.println(serverStarted ? "Server Enabled" : "Server Disabled");  // Print whether the server is enabled or disabled
    ticker = millis();  // Reset the ticker
  }
}
