int ledPin = D1; // digital pin D1 
int ldrPin = A0; // analog pin A0
void setup() { 
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Here LED is determined as an output or an indicator.
  pinMode(ldrPin, INPUT); // Here LDR sensor is determined as input.
}
int ldrStatus = 0;
int ledStatus = 0;
int sensitivity = 420;
void loop() { // Void loop is ran again and again and contains main code.
  ldrStatus = analogRead(ldrPin);
  Serial.println(ldrStatus);
  if (ldrStatus < sensitivity && ledStatus == 0) 
  {
    digitalWrite(ledPin, HIGH); // If LDR senses darkness led pin high that means led will glow.
    ledStatus = 1;
    delay(750);
  }
  
  else if (ldrStatus < sensitivity && ledStatus == 1) 
  {
    digitalWrite(ledPin, LOW); // If LDR senses darkness led pin low that means led will stop glowing.
    ledStatus = 0;
    delay(750);
  }
}
