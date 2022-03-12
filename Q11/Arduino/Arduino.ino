#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define CLASS_SIZE 2

constexpr uint8_t RFID_RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t RFID_SS_PIN = D4;     // Configurable, see typical pin layout above

const uint8_t SERVO_PIN = D8;
const uint8_t BUZZ_PIN = 10;  // SD3 
const uint8_t LED_PIN = 9;  // SD2
const uint8_t LDR_PIN = A0;

const unsigned int LDR_TRIGGER_LEVEL = 700;

const unsigned int ALLOWED_TIME = 10;
const unsigned int CLASS_TIME = 15;

// Network credentials
const char *ssid     = "RADIN-L";
const char *password = "radinradin";

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

Servo servo;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String names[] = {"Mohammaderfan Ghasemi", "Radin Shayanfar"};
String tags[] = {"1795490155", "1627116127"};
bool aList[CLASS_SIZE] = {0};

String tag;
byte servoDeg = 0;
long epochTime = 0;
bool classRunning = false;
long classStartEpochTime = 0;

// ===================================================================

void setup() {
  Serial.begin(9600);
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  servo.attach(SERVO_PIN);
  servo.write(servoDeg);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZ_PIN, LOW);

  connectToWifi();

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.update();
}

void loop() {
  
  if (!classRunning) {
    if (isLdrTriggered()) {  // starting class...
      classStartEpochTime = getEpochTime();
      classRunning = true;
      Serial.printf("Class started at epoch time: %ld\n", classStartEpochTime);
      buzz1s();
    }
    return;
  }

  // Class is running at this point

  epochTime = getEpochTime();
  
  if (epochTime > classStartEpochTime+CLASS_TIME) {  // stopping class...
    classRunning = false;
    Serial.printf("Class ended at epoch time: %ld\n", epochTime);
    printAbsents();
    newAList();
    buzz1s();
    return;
  }
  
  tag = readCard();
  if (tag != "") {  // Card is hold on reader
    Serial.printf("Tag ID: %s\n", tag);
    
    if (epochTime < classStartEpochTime+ALLOWED_TIME) {  // Enter is allowed
      Serial.printf("%Tag ID %s entered the class.\n", tag );
      rollStudent(tag);

      openDoor();
      blinkLed3s();
      closeDoor();
    } else {
      buzz1s();
    }   
  }
}
// ===================================================================

String tagTemp;

String readCard() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return "";
  tagTemp = "";
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tagTemp += rfid.uid.uidByte[i];
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  return tagTemp;
}

int ldrStatus = 0;

bool isLdrTriggered() {
  ldrStatus = analogRead(LDR_PIN);
//  Serial.println(ldrStatus);
  if (ldrStatus > LDR_TRIGGER_LEVEL)
    return true;
  else
    return false;
}

void blinkLed3s() {
  digitalWrite(LED_PIN, HIGH);
  delay(3000);
  digitalWrite(LED_PIN, LOW);
}

void buzz1s() {
  digitalWrite(BUZZ_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZ_PIN, LOW);
}

void openDoor() {
  servoDeg = 180;
  servo.write(servoDeg);
}

void closeDoor() {
  servoDeg = 0;
  servo.write(servoDeg);
}

void connectToWifi() {
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnedted.");
}

long getEpochTime() {
  return timeClient.getEpochTime();
}

void printAbsents() {
  Serial.println("=== Absents ===");
  for (int i = 0; i < CLASS_SIZE; i++) {
    if (! aList[i]) {
      Serial.println(names[i]);
    }
  }
}

int tIndex = -1;
void rollStudent(String tag) {
  tIndex = searchArray(tags, tag);
  if (tIndex >= 0) {
    aList[tIndex] = true;
  }
}

void newAList() {
  for (int i = 0; i < CLASS_SIZE; i++) {
    aList[i] = false;
  }
}

int searchArray(String arr[], String s) {
  for (int i = 0; i < CLASS_SIZE; i++) {
    if (arr[i] == s) {
      return i;
    }
  }
  return -1;
}
