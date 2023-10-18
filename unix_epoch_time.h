#include <NTPClient.h>
#include <WiFiUdp.h>


const long utcOffsetInSeconds = 19800; // UTC offset for India (19800 seconds)
const char* ntpServerName = "in.pool.ntp.org";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServerName, utcOffsetInSeconds);

void setup_unixTime() {
  // Serial.begin(115200);

  // // Connect to WiFi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }

  // Initialize the NTP client
  timeClient.begin();
  timeClient.update();
}

int get_unixTime() {
  // timeClient.update();
  int x=0;
  if (timeClient.forceUpdate()) {
    // Serial.print("Unix Epoch Time: ");
    x = timeClient.getEpochTime();
   
  }
  return x;
  // Add any other code you need here
}
