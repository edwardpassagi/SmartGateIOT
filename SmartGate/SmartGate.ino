/*
 *  Created by TheCircuit
*/

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

const int RS = D3, EN = D4, d4 = D0, d5 = D8, d6 = D9, d7 = D10;
// D9 is RX, D10 is TX
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7); // Create LED panel instance

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  lcd.begin(16,2); // Initiate LCD panel
  Serial.println("Setting up serial output...");
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();


  // make HTTP get call to validate entry
  if (content.substring(1) == "7A E7 FD 3F") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Mr.Circuit ");
    
    // print on lcd
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print("Welcome, Mo!");

    delay(1000);
    Serial.println(" Have FUN ");
    Serial.println();
    
    statuss = 1;
  }
  
  else   {
    Serial.println(" Access Denied ");

    // print on lcd
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied!");
    delay(3000);
  }
} 
