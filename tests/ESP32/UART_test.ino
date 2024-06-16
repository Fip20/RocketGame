#define RX 16 // Goes to pin 3.3 of MSP
#define TX 17 // Goes to pin 3.2 of MSP

const int baud_rate = 115200; // 115200 per second

void setup() {
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX); // Define serial channel
}

void loop() {
  // Handle commands from MSP432
  handleSerialCommand();
}

void handleSerialCommand() {
  if (Serial2.available() > 0) {
    // Read the command from MSP432
    String command = Serial2.readStringUntil('\n');

    // Print the received command to the serial monitor
    Serial.println("Received command: " + command);
  }
}
