/*

  Code Dependencies : LittleFS
  This code is used to initialize a status file using LittleFS to save status variables to preserve status during powerdown.

  LittleFS status file format
  "sta_ssid\n
   sta_pass\n
   ap_ssid\n
   ap_pass\n"

*/

#include <LittleFS.h>
#define STATUS_FILE_PATH "/status.bin"
struct wc {
  String sta_ssid = "your sta ssid";
  String sta_pass = "your sta password";
  String ap_ssid = "your ap ssid";
  String ap_pass = "your ap password";
} wifiCreds;

const char* defaultStatus PROGMEM = "your sta ssid\nyour sta password\nyour ap ssid\nyour ap password\n";

bool initLittleFS() {
  return LittleFS.begin();
}


void loadDefaultStatus() {
  wifiCreds.sta_ssid = "your sta ssid";
  wifiCreds.sta_pass = "your sta password";
  wifiCreds.ap_ssid = "your ap ssid";
  wifiCreds.ap_pass = "your ap password";
  return;
}

void restoreDefaultStatus() {
  File dStatusFile =  LittleFS.open(STATUS_FILE_PATH, "w");
  if (!dStatusFile) {
    Serial.println("Failed to open status file for writing defaults");
    return;
  }
  dStatusFile.print(defaultStatus);
  dStatusFile.close();
  loadDefaultStatus();
  return;
}
void initStatus() {
  if (!initLittleFS()) {
    Serial.println("LittleFS Init Error");
    loadDefaultStatus();
    return;
  }
  //LittleFS.format();
  if (!LittleFS.exists(STATUS_FILE_PATH)) {
    Serial.println("Status File not found, restoring default status");
    restoreDefaultStatus();
  }
  File statusFile = LittleFS.open(STATUS_FILE_PATH, "r");
  wifiCreds.sta_ssid = statusFile.readStringUntil('\n');
  wifiCreds.sta_pass = statusFile.readStringUntil('\n');
  wifiCreds.ap_ssid = statusFile.readStringUntil('\n');
  wifiCreds.ap_pass = statusFile.readStringUntil('\n');
  statusFile.close();
}

void updateStatus() {
  File statusFile  = LittleFS.open(STATUS_FILE_PATH, "w");
  if (!statusFile) {
    Serial.println("Failed to open status file for updating status");
    return;
  }
  statusFile.printf("%s\n%s\n%s\n%s\n", wifiCreds.sta_ssid.c_str(), wifiCreds.sta_pass.c_str(), wifiCreds.ap_ssid.c_str(), wifiCreds.ap_pass.c_str());
  statusFile.close();
}



void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud
  initStatus();

}

unsigned long ticker = millis(); // Initialize a variable to track time
unsigned long interval = 5000; // Set the interval between each update
unsigned long counter = 0;
void loop() {
  if (millis() - ticker > interval) {
    counter++;
    wifiCreds.sta_ssid += String(counter);
    wifiCreds.sta_pass += String(counter);
    wifiCreds.ap_ssid +=  String(counter);
    wifiCreds.ap_pass +=  String(counter);

    if (wifiCreds.sta_ssid.length() > 20) {
      restoreDefaultStatus();
    }

    Serial.println(ESP.getFreeHeap());
    updateStatus();
    Serial.printf("*===============================Status :  %d===============================*\n", counter);
    Serial.printf("sta_ssid : %s, sta_pass : %s, ap_ssid : %s, ap_pass : %s\n", wifiCreds.sta_ssid.c_str(), wifiCreds.sta_pass.c_str(), wifiCreds.ap_ssid.c_str(), wifiCreds.ap_pass.c_str());
    Serial.printf("*===============================End : %d===============================*\n", counter);
    ticker = millis();
  }
}
