#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "current_time.h"
#include "rand_generator.h"
#include "unix_epoch_time.h"
#define TIME_ZONE -5
#include "capture_aws.h"

#include "read_arduino.h"
// float time_elapsed =-1;
// String receivedCardID = "";

// #define DHTPIN 4        // Digital pin connected to the DHT sensor
// #define DHTTYPE DHT11   // DHT 11
 
// DHT dht(DHTPIN, DHTTYPE);
 
float h = 60.3;
float t = 29.5;
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;
 
#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"
 
WiFiClientSecure net;
 
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
 
PubSubClient client(net);
 
time_t now;
time_t nowish = 1510592825;
 
 
void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}
 
 
void messageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
 
 
void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
 
  NTPConnect();
 
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);
 
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  // doc["time"] = millis();
  // doc["humidity"] = 60.0;
  // doc["temperature"] = 30.2;
  // data to be sent
  getTime();
  generateAndPrintRandomData();
  setup_capture();
  doc["student_id"] = randomStudentID;
  doc["card_id"] = receivedCardID;
  doc["subject"] = randomSubject;
  doc["date"] = dateString;
  doc["out_time"] = get_unixTime();
  doc["in_time"] = get_unixTime() - (int)time_elapsed;
  doc["total_time"] = (int)time_elapsed;
  doc["image_url"] = imageUrl;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.print(F("Student ID: "));
  Serial.println(randomStudentID);
  Serial.print(F("Card ID: "));
  Serial.println(receivedCardID);
  
  time_elapsed = -1;
  receivedCardID = "";

}
 
 
void setup()
{
  Serial.begin(9600);
  // start_reading();
  setup_unixTime();
  
  connectAWS();
  // dht.begin();
}
 
 
void loop()
{
  timeClient.update();
  // h = dht.readHumidity();
  // t = dht.readTemperature();
 
  // if (isnan(h) || isnan(t) )  // Check if any reads failed and exit early (to try again).
  // {
  //   Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }
 
  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.println(F("°C "));
  //delay(2000);
    loop_read_arduino();
  // now = time(nullptr);
  // loop_read_arduino();
  // Serial.println(time_elapsed, 2);
  // Serial.println(receivedCardID);
  if (!client.connected())
  {
    connectAWS();
  }
  // else
  // {
  //   client.loop();
  //   if (millis() - lastMillis > 5000)
  //   {
  //     lastMillis = millis();
  //     publishMessage();
  //   }
      
      if(time_elapsed != -1 && receivedCardID != ""){
          publishMessage();

      }
  // }
}