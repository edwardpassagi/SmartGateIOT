/*
    Created by TheCircuit
*/

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#define SD2 11
#define SD3 12
#define allowLCD false //enable lcd, disable when debugging

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const int RS = D3, EN = D4, d4 = D0, d5 = D8, d6 = D9, d7 = D10;
// D9 is RX, D10 is TX
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7); // Create LED panel instance

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

// WiFi Credentials
const char* ssid = "hacker";
const char* password = "nicememe";

// http variable
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

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
  String content = getContent();

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
   This script is responsible to initialize all of our modules and IOs.
*/
void initializer()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  // this line overrides our serial monitor output!
  if (allowLCD) lcd.begin(16, 2); // Initiate LCD panel

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

/*
   getContent returns the UID of the current card being read.
*/
String getContent()
{
  String content = "";
  Serial.println();
  Serial.print("UID tag :");
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
   validateContent() makes determines wether current UID exists in our database or not.
*/
bool validateContent(String content)
{
  //TODO: add HTTP get call to validate with database
  String uri = "http://b0d8-130-126-255-165.ngrok.io/employee";

  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, uri.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        if (httpResponseCode == 200) return true;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
  // if all else fails
  return false;
}

/*
   Sequence upon succesful authorization.
*/
void authorizedSequence()
{
  Serial.print("Authenticated");

  // print on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Granted");
  lcd.setCursor(0, 1);
  lcd.print("Welcome, Mo!");

  statuss = 1;
}

/*
   Sequence upon rejection.
*/
void rejectedSequence()
{
  Serial.println(" Access Denied ");

  // print on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Denied!");
}
