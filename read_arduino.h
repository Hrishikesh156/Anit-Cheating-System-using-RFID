float time_elapsed;
String receivedCardID;
  // Your other ESP8266 code here
void setup_read() {
  Serial.begin(9600);  // Initialize the hardware serial port (Serial) at 9600 baud
}

void loop_read_arduino() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');  // Read until newline character
    // Serial.print("Received Data from Arduino: ");
    // Serial.println(data);

    // // Convert the received string back to a float
    // float receivedFloat = data.toFloat();
    // Serial.print("Received Float: ");
    // Serial.println(receivedFloat);

    if (data.startsWith("T:")) {
      time_elapsed = data.substring(2).toFloat();  // Extract and convert the float data
      Serial.print("Time Elapsed: ");
      Serial.println(time_elapsed, 2);  // Print with 2 decimal places
    } else if (data.startsWith("C:")) {
      receivedCardID = data.substring(2);  // Extract the string data
      Serial.print("CardID: ");
      Serial.println(receivedCardID);
    }

    // Process the received float data as needed
  }
  // Your other ESP8266 code here
}

