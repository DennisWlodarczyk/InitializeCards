#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

Adafruit_PN532 nfc(-1, -1);
void rewriteCards();
void printArray(uint8_t array[]);

void setup() {
  Serial.begin(115200);

  nfc.begin();
  if (! nfc.getFirmwareVersion()) {
    Serial.print("Didn't find PN53x board");
    while (true) { delay(1); }
  }
}

void loop() {
  rewriteCards();
  delay(1000);
}


void rewriteCards() {

  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  uint8_t correctAuth[4] = {4, 101, 21, 69};
  uint8_t wrongAuth[4] = {6, 6, 6, 6};
  uint8_t cardOne[4] = {0, 0, 0, 0};
  uint8_t cardTwo[4] = {0, 2, 0, 0};
  uint8_t outputArray[4] = {7, 7, 7, 7};

  //Writing the needed numbers on the first card
  Serial.println("Please present the first card");
  nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  nfc.mifareultralight_WritePage(4, correctAuth);
  nfc.mifareultralight_WritePage(5, cardOne);
  Serial.println("Writing done");

  nfc.mifareultralight_ReadPage(4, outputArray);
  Serial.println("This card has on page 4 the following numbers: ");
  printArray(outputArray);
  nfc.mifareultralight_ReadPage(5, outputArray);
  Serial.println("This card has on page 5 the following numbers: ");
  printArray(outputArray);

  delay(3000);

  //Writing the needed numbers on the second card
  Serial.println("Please present the second card");
  nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  nfc.mifareultralight_WritePage(4, correctAuth);
  nfc.mifareultralight_WritePage(5, cardTwo);
  Serial.println("Writing done");

  nfc.mifareultralight_ReadPage(4, outputArray);
  Serial.println("This card has on page 4 the following numbers: ");
  printArray(outputArray);
  nfc.mifareultralight_ReadPage(5, outputArray);
  Serial.println("This card has on page 5 the following numbers: ");
  printArray(outputArray);

  delay(3000);

  //Writing the needed numbers on the third card
  Serial.println("Please present the third card");
  nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  nfc.mifareultralight_WritePage(4, wrongAuth);
  Serial.println("Writing done");

  nfc.mifareultralight_ReadPage(4, outputArray);
  Serial.println("This card has on page 4 the following numbers: ");
  printArray(outputArray);

  Serial.println("All cards are initialized!");
}

void printArray(uint8_t array[]){
  for (int i = 0; i < 4; i++){
    Serial.println(array[i]);
  }
}