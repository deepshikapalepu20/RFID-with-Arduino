#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN    9       // Reset pin for the RFID module
#define SS_PIN     10      // Slave Select pin for SPI
#define buzzerPin  8       // Digital pin for the buzzer
#define redLedPin  2       // Digital pin for the red LED
#define greenLedPin 3      // Digital pin for the green LED

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Define the UIDs of the RFID cards
byte card1UID[] = {0x10, 0xB5, 0x80, 0x14};
byte card2UID[] = {0xDE, 0xE4, 0xCF, 0x1D};

void setup() {
  Serial.begin(9600);               // Initialize serial communications with the PC
  while (!Serial);                   // Wait for the serial port to connect
  SPI.begin();                       // Init SPI bus
  mfrc522.PCD_Init();                // Init MFRC522
  delay(4);                          // Optional delay. Some boards need more time after init to be ready
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(redLedPin, OUTPUT);        // Set the red LED pin as OUTPUT
  pinMode(greenLedPin, OUTPUT);      // Set the green LED pin as OUTPUT
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    // Turn off LEDs if no card is present
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Check the UID of the card
  if (compareUID(mfrc522.uid.uidByte, card1UID)) {
    // Card 1: Blink the green LED and print the welcome message
    blinkGreenLED();
    Serial.println("Welcome to Lovely Professional University - Faizan(12323057), Tanya(12304490), Deepshika(12322963), Pranav(12324530)");
  } else if (compareUID(mfrc522.uid.uidByte, card2UID)) {
    // Card 2: Activate the buzzer and print "Not Authorized"
    activateBuzzer();
    Serial.println("Not Authorized");
  }

  // Halt communication with the RFID card
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void blinkGreenLED() {
  digitalWrite(greenLedPin, HIGH);
  delay(1000);  // Keep the green LED on for 1 second
  digitalWrite(greenLedPin, LOW);
}

void activateBuzzer() {
  tone(buzzerPin, 1000, 1000);  // Buzzer frequency and duration
  delay(1000);                  // Wait for 1 second
  noTone(buzzerPin);            // Turn off the buzzer
}

bool compareUID(byte* uid1, byte* uid2) {
  for (int i = 0; i < 4; i++) {
    if (uid1[i] != uid2[i]) {
      return false; // UIDs are different
    }
  }
  return true; // UIDs are the same
}
