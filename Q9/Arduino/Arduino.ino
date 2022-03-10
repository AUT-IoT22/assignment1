#include "NewPing.h"
#include <Servo.h>

#define TRIGGER_PIN D0
#define ECHO_PIN D1
#define SERVO_PIN D8

#define MAX_DISTANCE 120
#define SENSITIVITY 0.8

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servo;

float duration, distance;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
  servo.write(0);
}

int detection_counter = 1;
int succeed_detection = 0;
void loop() {
  // put your main code here, to run repeatedly:
  distance = sonar.ping_cm();
  detection_counter+=1;
  // Send results to Serial Monitor
  Serial.print("Distance = ");
  
  if (distance >= 90 || distance <= 2) 
  {
    Serial.println("Out of range");
  }
  else 
  {
    succeed_detection+=1;
    Serial.print(distance);
    Serial.println(" cm");
  }
  if(detection_counter%200==0)
  {
    if(succeed_detection>200*SENSITIVITY)
    {
      servo.write(180);
      delay(3500);
      servo.write(0);
    }
    
    detection_counter = 1;
    succeed_detection = 0;
  }
}
