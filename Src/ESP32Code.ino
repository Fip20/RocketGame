#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define RX 16 // Goes to pin 3.3 of MSP
#define TX 17 // Goes to pin 3.2 of MSP

const char *ssid = "Federico's Galaxy A54 5G";
const char *password = ""; // Add password
const int baud_rate = 115200;
const char *serverUrl = "https://federico1110.pythonanywhere.com/get_ranking";

void handleSerialCommand();

void setup() {
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX); // Define serial channel

  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected");
}

void loop() {
  // Handle MSP432 commands
  handleSerialCommand();

  delay(5000); // Wait 5 sec before the next request
}

void handleSerialCommand() {
  if (Serial2.available() > 0) {
    // Read command from MSP432
    String command = Serial2.readStringUntil('\n');

    Serial.println(command);

    // Analize command
    if (command.startsWith("get_ranking")) {
      // Execute GET request and send reply to MSP432
      getAndSendRanking();
    } else {
      Serial.println("Unknown command.");
    }
  }
}

void getAndSendRanking() {

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

    // Parse JSON and extract scores
    String scores = extractScores(payload);

    // Send scores through UART to MSP432
    sendString(scores);
  } else {
    Serial.println("Error HTTP request");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  // End HTTP cnnection
  http.end();
}

String extractScores(String jsonString) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonString);
  
  String scores = "[";
  for (JsonObject obj : doc.as<JsonArray>()) {
    if (scores.length() > 1) {
      scores += ", ";
    }
    scores += obj["score"].as<String>();
  }
  scores += "]^";

  return scores;
}

void sendString(String send) {
  char c;
  int timer = 1;

  for (int i = 0; i < send.length(); i++) {
    c = send.charAt(i);
    Serial2.write(c);
    timer--;
    if (timer == 0) {
      String tmp = Serial2.readStringUntil(']');
      timer = 1;
    }
  }
}

