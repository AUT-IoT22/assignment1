const uint8_t LEDpin = 5;
const uint8_t LDRpin = A0;

int ldrValue = 0;
int ledValue = 0;

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);

//  pinMode(LEDpin, OUTPUT);
//  digitalWrite(LEDpin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  ldrValue = analogRead(LDRpin);
  ledValue = rToLightLinear(ldrValue);

  analogWrite(LEDpin, ledValue);

//  Serial.println(ldrValue);
//
//  delay(200);
}

int rToLightQuadratic(int x) {
  return -1.8*1e-4 * x * x + 0.4615 * x -28.799;
}

int rToLightLinear(int x) {
  return (int) (0.2656 * x) -17;
}
