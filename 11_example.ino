#include <Servo.h>
#define PIN_SERVO 10

Servo myservo;
int i;
int servoAngle ;
int angle[4];
 
void setup() {
  Serial.begin(115200);
  i = 0;
  servoAngle = 0;
  angle[0] = 0;
  angle[1] = 90;
  angle[2] = 180;
  angle[3] = 90;
  myservo.attach(PIN_SERVO); 
  myservo.write(servoAngle);
  delay(1000);
}

void loop() {
    // add code here. 
    if(i < 4){
       myservo.write(angle[i]);
       delay(500);
       i++ ;
    }else{
      i = 0;
      myservo.write(angle[0]);
      delay(500);
    }  
    Serial.println(angle[i]);
    
}
