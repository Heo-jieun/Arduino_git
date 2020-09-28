#define PIN_LED 7
unsigned int count, state ;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  count = state = 0;
  digitalWrite(PIN_LED,state); // turn on LED
 
}

void loop() {
  state = toggle_state(count);
  digitalWrite(PIN_LED, state);
  count += 1 ;
  delay(100); // wait for 200 milliseconds  
}
int toggle_state(int count){
  if(count < 11){
    state = 0;
  }
  else if( 11 <= count && count < 21 && count%2==0){
    state = 0;
  }
  else if( 11 <= count && count < 21 && count%2==1){
    state = 1;
  }
  else{
    state = 1;
  }
  return state;
  while(1){
    ;
  }
}
