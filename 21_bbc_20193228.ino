#include <Servo.h>

// Arduino pin assignment
#define PIN_IR A0
#define PIN_LED 9
#define PIN_SERVO 10

// configurable parameters
#define SND_VEL 346.0 // sound velocity at 24 celsius degree (unit: m/s)
#define INTERVAL 25 // sampling interval (unit: ms)
#define _DIST_MIN 70 // minimum distance to be measured (unit: mm)
#define _DIST_MAX 550 // maximum distance to be measured (unit: mm)

#define _DUTY_MIN 553 // servo full clockwise position (0 degree)
#define _DUTY_NEU 1481 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counterclockwise position (180 degree)
#define _DIST_ALPHA 0.1

// global variables
float timeout; // unit: us
float dist_min, dist_max, dist_raw, dist_prev, dist_ema, EMA_k, alpha, dist_write; // unit: mm
unsigned long last_sampling_time; // unit: ms
float scale; // used for pulse duration to distance conversion
int angle_value[180] ; 
int angleSecond ;
Servo myservo;

int a, b; // unit: mm


void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);

  alpha = _DIST_ALPHA;
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

  // initialize USS related variables
  dist_min = _DIST_MIN; 
  dist_max = _DIST_MAX;
  timeout = (INTERVAL / 2) * 1000.0; // precalculate pulseIn() timeout value. (unit: us)
  dist_raw = dist_prev = 0.0; // raw distance output from USS (unit: mm)
  scale = 0.001 * 0.5 * SND_VEL;
  dist_write = 0.0;
  angleSecond = 553;
  alpha = _DIST_ALPHA;

// initialize serial port
  Serial.begin(57600);

  a = 70;
  b = 300;
}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {
  
  float raw_dist = ir_distance();  
  
  //거리값 필터링
  if(raw_dist< dist_min || raw_dist > dist_max ) {
    dist_write = dist_write;  // 센서가 측정을 제대로 하지 못하면 dist_write에 저번 거리값을 유지  
  }
  else {
    dist_write = raw_dist; // 새로운 값이 들어오면 그 값을 반영   
  }
  dist_ema = (alpha*dist_write) + (1 - alpha)*EMA_k; //EMA값을 계산
  EMA_k = dist_ema; // 이전 EMA값을 저장
  
  float dist_cali = 100 + 300.0 / (b - a) * (raw_dist - a);

  Serial.print("min:0,max:500,dist:");  
  Serial.print(",dist_ema:");
  Serial.print(dist_ema);
  Serial.print(",raw_dist:");
  Serial.println(raw_dist);
  /*
  Serial.print(",dist_cali:");
  Serial.print(dist_cali);
  */
  
  // bang-bang control///////////////
  
  if(dist_ema < 255){
    myservo.writeMicroseconds( 2000 );
  }else if(dist_ema >275){
     myservo.writeMicroseconds( 1300 );
  }
  if(dist_write > 100 && dist_write<450) digitalWrite(PIN_LED, 0);
  else digitalWrite(PIN_LED, 255);
  delay(20);
}
