#include <stdlib.h>
// Arrays of predefined subjects, student IDs, and card IDs
const char* subjects[] = {"Math", "Science", "History", "English", "Physics"};
const char* studentIDs[] = {"S123", "S456", "S789", "S101", "S202"};
const char* cardIDs[] = {"C111", "C222", "C333", "C444", "C555"};

const char* randomSubject;
const char* randomStudentID;
const char* randomCardID;

void generateAndPrintRandomData() {
  // Generate random indices to select values from the arrays
  int randomSubjectIndex = random(sizeof(subjects) / sizeof(subjects[0]));
  int randomStudentIDIndex = random(sizeof(studentIDs) / sizeof(studentIDs[0]));
  int randomCardIDIndex = random(sizeof(cardIDs) / sizeof(cardIDs[0]));

  // Select random values from the arrays
  randomSubject = subjects[randomSubjectIndex];
  randomStudentID = studentIDs[randomStudentIDIndex];
  randomCardID = cardIDs[randomCardIDIndex];

  // Print the randomly generated data
  // Serial.println("Randomly Generated Data:");
  // Serial.print("Subject: ");
  // Serial.println(randomSubject);
  // Serial.print("Student ID: ");
  // Serial.println(randomStudentID);
  // Serial.print("Card ID: ");
  // Serial.println(randomCardID);
}