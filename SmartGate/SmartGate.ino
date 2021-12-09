/*
 *  Created by TheCircuit
*/

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#define SD2 11
#define SD3 12
#define allowLCD true //enable lcd

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
  initializer();
  Serial.println("Setting up serial output...");
}

void loop() 
{
  // Look for new cards or select one of the cards
  if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  String content= getContent();

  // TODO: make HTTP get call to validate entry
  bool isPermitted = validateContent(content);
  if (isPermitted) //change UID of the card that you want to give access
  {
    authorizedSequence();
  }
  else   {
    rejectedSequence();
  }
}

/*
 * This script is responsible to initialize all of our modules and IOs.
 */
void initializer()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  // this line overrides our serial monitor output!
  if (allowLCD) lcd.begin(16,2); // Initiate LCD panel
}

/*
 * getContent returns the UID of the current card being read.
 */
String getContent()
{ 
  String content = "";
  Serial.println();
  Serial.print(" UID tag :");
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
  return content;
}

/**
 * validateContent() makes determines wether current UID exists in our database or not.
 */
bool validateContent(String content)
{
  //TODO: add HTTP get call to validate with database
  if (content.substring(1) == "7A E7 FD 3F"){
    return true;
  }

  return false;
}

/*
 * Sequence upon succesful authorization.
 */
void authorizedSequence()
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
  statuss = 1;
}

/*
 * Sequence upon rejection.
 */
void rejectedSequence()
{
  Serial.println(" Access Denied ");
  
  // print on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Denied!");
  delay(3000);
}
