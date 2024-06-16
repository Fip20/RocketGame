#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Federico's Galaxy A54 5G";
const char *password = ""; // Add password
const char *serverUrl = "https://federico1110.pythonanywhere.com/get_ranking";

void setup() {
  Serial.begin(115200);

  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected");

  // Perform GET request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);

    Serial.print("Send GET request to ");
    Serial.println(serverUrl);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      // Get payload
      String payload = http.getString();
      Serial.println("Request succeed!");
      Serial.println("Response from server:");
      Serial.println(payload);
    } else {
      Serial.println("Error in HTTP request");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    // End HTTP connection
    http.end();
  }
}

void loop() {
  
}
