#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9
#define SS_PIN          10
#define BUZZER_PIN      7  
#define LED1_PIN        4  
#define LED2_PIN        5  
#define MAX_TAGS        4
#define SERVO_PIN       6
#define LDR_PIN         A0
#define LDR_THRESHOLD   1000  

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo servoMotor;  // Create a servo object

unsigned long startTime[MAX_TAGS] = {0};
boolean tagActive[MAX_TAGS] = {false};
byte lastScannedTag[MAX_TAGS][MFRC522::MF_KEY_SIZE] = {{0}};
int activeTagCount = 0;

void setup_rfid() {
    //Serial.begin(9600);
    while (!Serial);
    SPI.begin();
    mfrc522.PCD_Init();
    delay(4);
    mfrc522.PCD_DumpVersionToSerial();
    // Serial.println(F("Scan PICC to see UID..."));
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    servoMotor.attach(SERVO_PIN);  // Attach the servo to the specified pin
    pinMode(LDR_PIN, INPUT);
}

int findTagIndex(byte* uid, byte uidSize) {
    for (int i = 0; i < MAX_TAGS; i++) {
        if (tagActive[i] && compareTagID(uid, lastScannedTag[i])) {
            return i;
        }
    }
    return -1;
}

int findInactiveTag() {
    for (int i = 0; i < MAX_TAGS; i++) {
        if (!tagActive[i]) {
            return i;
        }
    }
    return -1;
}

void copyTagID(byte* source, byte* destination) {
    for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
        destination[i] = source[i];
    }
}

boolean compareTagID(byte* tag1, byte* tag2) {
    for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
        if (tag1[i] != tag2[i]) {
            return false;
        }
    }
    return true;
}

void moveServo() {
    // Serial.println("Moving Servo");
    servoMotor.write(90);  // Move the servo to 90 degrees
    delay(1000);  // Wait for 1 second
    servoMotor.write(0);  // Move the servo back to 0 degrees
}
void loop_rfid() {
  
    // Reset the loop if no new card present
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Check if the tag is already active
    int tagIndex = findTagIndex(mfrc522.uid.uidByte, mfrc522.uid.size);

    // If it's a new tag, start the timer and ring the buzzer
    if (tagIndex == -1) {
        tagIndex = findInactiveTag();
        if (tagIndex != -1) {
            startTime[tagIndex] = millis();
            tagActive[tagIndex] = true;
            copyTagID(mfrc522.uid.uidByte, lastScannedTag[tagIndex]);
            activeTagCount++;
            digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON
            delay(100);  // Buzzer ON duration
            digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF
            // Move the servo motor when a new tag is scanned for the first time
            if (activeTagCount == 1) {
                moveServo();
            }
        }
    } else {
        // If it's an active tag, check if it's the same tag
        if (compareTagID(mfrc522.uid.uidByte, lastScannedTag[tagIndex])) {
            // If it's the same tag, stop the timer, calculate elapsed time, and ring the buzzer
            unsigned long endTime = millis();
            unsigned long elapsedTime = endTime - startTime[tagIndex];

            // Serial.print("Time for Tag ");
            // Serial.print(tagIndex + 1);
            Serial.print("T:");
            Serial.println(elapsedTime / 1000.0);  // Convert milliseconds to seconds
            // Serial.println(" seconds");

            // Check if the final time exceeds 10 seconds
            if (elapsedTime > 10000) {
                digitalWrite(LED1_PIN, HIGH);  // LED1 ON
                delay(50);
                digitalWrite(LED1_PIN, LOW);  // LED1 OFF  
                digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON
                delay(700);  // Buzzer ON duration
                digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF              
            } else {
                digitalWrite(LED2_PIN, HIGH);  // LED2 ON
                delay(50);
                digitalWrite(LED2_PIN, LOW);  // LED1 OFF
            }

            // Ring the buzzer
            digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON
            delay(300);  // Buzzer ON duration
            digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF

            // Reset the timer and deactivate the tag
            startTime[tagIndex] = 0;
            tagActive[tagIndex] = false;
            activeTagCount--;
        } else {
            // If it's a different tag, update the last scanned tag
            copyTagID(mfrc522.uid.uidByte, lastScannedTag[tagIndex]);
        }
    }

    // Retrieve and print the tag UID
    // Serial.print("Tag UID: ");
    // for (byte i = 0; i < mfrc522.uid.size; i++) {
        // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        // Serial.print(mfrc522.uid.uidByte[i], HEX);
    // }
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; ++i) {
      cardUID += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
    }
    // Print the UID (Card ID)
    Serial.print(F("C: "));
    Serial.println(cardUID);
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();

    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

    // Check LDR sensor value and activate LED2_PIN if above threshold
    int ldrValue = analogRead(LDR_PIN);
    // Serial.print("LDR Value: ");
    // Serial.println(ldrValue);
    if (ldrValue > LDR_THRESHOLD) {
        digitalWrite(LED2_PIN, HIGH);  // Activate LED2_PIN
    } else {
        digitalWrite(LED2_PIN, LOW);   // Deactivate LED2_PIN
    }
}
