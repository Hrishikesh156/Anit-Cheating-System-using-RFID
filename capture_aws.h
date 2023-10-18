// #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// #include <ArduinoJson.h>
// const char ssid[]= "Poseidon";               //TAMIM2.4G
// const char password[] = "hrishi156";  
String imageUrl;
void setup_capture() {
  // Serial.begin(9600);

  // // Connect to Wi-Fi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");

  // Define the HTTPClient object
  // HTTPClient http;
 
  // // Specify the server and port
  // http.begin("http://192.168.0.103:5000/add_data");
  WiFiClient client;
  HTTPClient http;

  // Specify the server and port
  http.begin(client, "http://192.168.245.221:5000/capture");

  // Set the HTTP method (POST)
  http.addHeader("Content-Type", "application/json"); // Set content type
  // String studentID = "asdf";
  // int Time = 32423423;
  // Your JSON data to send in the POST request
  int Time = get_unixTime();
  String studentID = "student";
  String jsonPayload = "{\"studentID\":\"" + studentID + "\",\"Time\":" + String(Time) + "}";

  // Send the POST request with the JSON payload
  int httpResponseCode = http.POST(jsonPayload);

  // Check the response code
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);

    DynamicJsonDocument doc(256); // Adjust the buffer size as needed
    deserializeJson(doc, response);

    // Extract the URL
    imageUrl = doc["url"].as<String>();
    
    Serial.print("Extracted URL: ");
    Serial.println(imageUrl);
  } else {
    Serial.print("HTTP Error code: ");
    Serial.println(httpResponseCode);
  }

  // Close the connection
  http.end();
}


