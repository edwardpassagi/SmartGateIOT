/**
 * This script is responsible to manage the authentication flow
 * and core logic of our ESP8266 microcontroller.
 */
#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#define SD3 10
#define allowLCD true //enable lcd, disable when debugging

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

const int RS = D3, EN = D4, d4 = D0, d5 = SD3, d6 = D9, d7 = D10;
// D9 is RX, D10 is TX
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7); // Create LED panel instance

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

// Active buzzer
const int BUZZER = D8;

// WiFi Credentials
const char* ssid = "hacker";
const char* password = "nicememe";

// http variable
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

// Authorized User variable
String authorizedName = "";

void setup()
{
  initializer();
  Serial.println("Ready to scan:");
  
  lcdLine1("Scan Ready");
  durationBeep(200);
}

void loop()
{
  digitalWrite(BUZZER,LOW);
  
  // Look for new cards or select one of the cards
  if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  String UID = getUid();

  // TODO: make HTTP get call to validate entry
  if (isValidUid(UID)) //change UID of the card that you want to give access
  {
    authorizedSequence();
  }
  else   {
    rejectedSequence();
  }

  // cleared sequence, ready to scan again
  lcdLine1("Scan Ready");
  durationBeep(200);
}

/*
   This script is responsible to initialize all of our modules and IOs.
*/
void initializer()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(BUZZER, OUTPUT); // Initiate Buzzer
  digitalWrite(BUZZER,LOW);
  
  // this line overrides our serial monitor output!
  if (allowLCD) lcd.begin(16, 2); // Initiate LCD panel

  connectToWiFi();
}

void connectToWiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    lcdConnectingBlink();
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
void lcdConnectingBlink()
{
  // print on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Conn...");
  delay(500);
  lcd.clear();
  delay(300);
}

/*
   getContent returns the UID of the current card being read.
*/
String getUid()
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
  String UID = content.substring(1);
  UID.replace(" ","-");
  Serial.println();
  return UID;
}

/**
   validateContent() makes determines wether current UID exists in our database or not.
*/
bool isValidUid(String UID)
{
  //TODO: add HTTP get call to validate with database
  String uri = "http://pure-atoll-60128.herokuapp.com/validate?uid=" + UID;
  Serial.println(uri);
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, uri);
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, http.getStream());;
        
        if (httpResponseCode == 200) {
          authorizedName = doc["name"].as<String>();
          return true;
        }
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
      lcdLine1("WiFi disc..");
      delay(1000);
      connectToWiFi();
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
  Serial.println("Authenticated");
  Serial.println("Hi, " + authorizedName);
  authorizedBeep();
  // print on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Door Unlocking");
  lcd.setCursor(0, 1);
  lcd.print("Hi, " + authorizedName);

  statuss = 1;

  // simulate time it takes to open and close door
  delay(5000);
}

void authorizedBeep()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(BUZZER,HIGH);
    delay(50);
    digitalWrite(BUZZER,LOW);
    delay(50);
  }
}
/*
   Sequence upon rejection.
*/
void rejectedSequence()
{
  Serial.println(" Access Denied ");

  durationBeep(500);
  // print on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Denied!");

  // some delay before user can retry
  delay (3000);
}

void durationBeep(int milliseconds)
{
  digitalWrite(BUZZER,HIGH);
  delay(milliseconds);
  digitalWrite(BUZZER,LOW);
}

void lcdLine1(String content)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(content);
}
