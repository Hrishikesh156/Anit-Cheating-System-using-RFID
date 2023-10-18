#include <time.h>
char timeString[9]; // HH:MM:SS format
char dateString[12]; // YYYY-MM-DD format
void printCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }


  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
  strftime(dateString, sizeof(dateString), "%Y-%m-%d", &timeinfo);

  // Serial.print("Current time in IST: ");
  // Serial.println(timeString);
  // Serial.print("Current date in IST: ");
  // Serial.println(dateString);
}
void getTime(){
  const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // IST offset in seconds (5 hours and 30 minutes)
const int daylightOffset_sec = 0;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    while (!time(nullptr)) {
    delay(1000);
    Serial.println("Waiting for time synchronization...");
  }
  Serial.println("Time synchronized");

  // Print the current time and date in IST
  printCurrentTime();
}
