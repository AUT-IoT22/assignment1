#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

constexpr uint8_t RFID_RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t RFID_SS_PIN = D4;     // Configurable, see typical pin layout above

const uint8_t SERVO_PIN = D8;
const uint8_t BUZZ_PIN = 10;  // SD3 
const uint8_t LED_PIN = 9;  // SD2
const uint8_t LDR_PIN = A0;

const unsigned int LDR_TRIGGER_LEVEL = 700;

// Network credentials
const char *ssid     = "RADIN-PC 0950";
const char *password = "f:67W589";

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

Servo servo;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String tag;
byte servoDeg = 0;

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
}

void loop() {
  if (isLdrTriggered()) {
    blinkLed3s();
  }
  tag = readCard();
  if (tag != "") {  // Card is hold on reader
    Serial.println(tag);
    time_t epochTime = timeClient.getEpochTime();
    Serial.print("Epoch Time: ");
    Serial.println(epochTime);
    openDoor();
    blinkLed3s();
//    buzz1s();
    closeDoor();
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
