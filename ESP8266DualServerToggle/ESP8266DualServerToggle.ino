/*

  Code Dependencies : ESPAsyncTCP, ESPAsyncWebServer
  Description:
  This code sets up two AsyncWebServer objects and listens on port 80 for incoming HTTP GET requests on the root URL.
  When a GET request is received, it sends a response to the client with the response body "response from server 1" or "response from server 2".
  The WiFiSecrets.h is a header file containing the following code:
  #ifndef _WIFI_SECRETS_H
  #define _WIFI_SECRETS_H
  #define STA_SSID "your_SSID"
  #define STA_PASS "your_Password"
  #endif _WIFI_SECRETS_H
  The STA_SSID and STA_PASS are used in connecting to your Access point (Hotspot or WiFi Router)
*/

#include "WiFiSecrets.h" // Include the WiFiSecrets header file

#include <ESP8266WiFi.h> // Include the ESP8266WiFi library
#include <ESPAsyncWebServer.h> // Include the ESPAsyncWebServer library
#include <ESPAsyncTCP.h> // Include the ESPAsyncTCP library

AsyncWebServer server1(80); // Create an AsyncWebServer object and listen on port 80
AsyncWebServer server2(80);

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud

  WiFi.mode(WIFI_STA); // Set WiFi mode to Station mode

  WiFi.begin(STA_SSID, STA_PASS); // Connect to the WiFi network using the SSID and password from WiFiSecrets.h
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) { // Wait until connected to the WiFi network
    Serial.print("."); // Print dots to indicate the connection process
  }
  Serial.println("WiFi Connected"); // Print "WiFi Connected" when connected to the WiFi network
  Serial.println(WiFi.localIP()); // Print the local IP address assigned to the ESP8266 by the router

  server1.on("/", HTTP_GET, [](AsyncWebServerRequest * req) { // Register a handler for the root URL
    req->send(200, "text/plain", "response from server 1"); // Send a "200 OK" response with "response from server 1" as the response body
  });
  server2.on("/", HTTP_GET, [](AsyncWebServerRequest * req) { // Register a handler for the root URL
    req->send(200, "text/plain", "response from server 2"); // Send a "200 OK" response with "response from server 2" as the response body
  });

  server1.begin(); // Start server1
}

unsigned long ticker = millis(); // Initialize a variable to track time
uint8_t serverCode = 0; // Initialize a flag to indicate whether server1 or server2 is currently running
unsigned long interval = 10000; // Set the interval between server status toggle.

void loop() {
  if (millis() - ticker > interval) { // Check if 10 seconds have elapsed
    if (serverCode == 0) {
      server1.end(); // If server 1 is started, stop it
      server2.begin(); // Start server 2
    }
    else {
      server2.end(); // If server 2 is started, stop it
      server1.begin(); // Start server 1
    }
    serverCode = 1 - serverCode; // Toggle the serverCode flag
    Serial.println(serverCode == 0 ? "Server 1 Enabled" : "Server 2 Enabled"); // Print whether server 1 or server 2 is enabled
    ticker = millis(); // Reset the ticker
  }
}
