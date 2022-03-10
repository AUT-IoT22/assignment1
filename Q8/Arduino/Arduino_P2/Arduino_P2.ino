#define BUZZER_PIN D0
#define LED_PIN D1
#define LDR_PIN A0



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT); // Here BUZZER is determined as an output or an indicator.
  pinMode(LED_PIN, OUTPUT); // Here LED is determined as an output or an indicator.
  pinMode(LDR_PIN, INPUT); // Here LDR sensor is determined as input.
  digitalWrite(BUZZER_PIN, LOW);
}


int ldrStatus = 0;
void loop() {
  // put your main code here, to run repeatedly:
  int ldrStatus_1024 = analogRead(LDR_PIN);
  int ldrStatus_100 = (ldrStatus_1024*100)/1024;
  Serial.println(ldrStatus_100);
  if(ldrStatus_100>70)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
  }
}
