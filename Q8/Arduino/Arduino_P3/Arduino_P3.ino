#include <Servo.h>

#define SERVO_PIN D8
#define LDR_PIN_Y D0
#define LDR_PIN_G D1
#define LDR_PIN_O D2

Servo servo;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LDR_PIN_Y, INPUT); // Here LDR sensor is determined as input.
  pinMode(LDR_PIN_G, INPUT); // Here LDR sensor is determined as input.
  pinMode(LDR_PIN_O, INPUT); // Here LDR sensor is determined as input.
  servo.attach(SERVO_PIN);
  servo.write(90); 
}

int ldrStatus_Y = 0;
int ldrStatus_G = 0;
int ldrStatus_O = 0;

void loop() {
  // put your main code here, to run repeatedly:
  ldrStatus_Y = digitalRead(LDR_PIN_Y);
  ldrStatus_G = digitalRead(LDR_PIN_G);
  ldrStatus_O = digitalRead(LDR_PIN_O);
  Serial.print(ldrStatus_Y);
  Serial.print("     ");
  Serial.print(ldrStatus_G);
  Serial.print("     ");
  Serial.println(ldrStatus_O);
  if(ldrStatus_G==0){
      servo.write(90);   
  }
  else{
    if(ldrStatus_Y==0){
      servo.write(45);
    }

    else if(ldrStatus_O==0){
      servo.write(135);
    }

    else{
    servo.write(90); 
     }
  }
}
