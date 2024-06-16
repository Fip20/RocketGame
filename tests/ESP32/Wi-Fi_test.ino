#include <WiFi.h>

const char *ssid = "Federico's Galaxy A54 5G";
const char *password = ""; // Add password

void setup() {
  Serial.begin(115200);

  // Connessione Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected");
}

void loop() {
  
}
