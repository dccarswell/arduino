//#include <avr/wdt.h>

const int float_switch = 2; // pin
const int relay = 3; // pin
int red_pin = 9; // pin
int green_pin = 10; // pin
int blue_pin = 11; // pin

const unsigned long countdown_blink_duration = 250L; //ms
const unsigned long busy_blink_duration = 100L; //ms

const unsigned long idle_delay = 500L; //ms
const unsigned long countdown_delay = 250L; //ms
const unsigned long busy_delay = 100L; //ms

const unsigned long countdown_duration = 150000L; //ms
const unsigned long max_open_time = 120000L; //ms



void set_color(byte rv, byte gv, byte bv) {
  analogWrite(red_pin,rv);
  analogWrite(green_pin, gv);
  analogWrite(blue_pin,bv);
}

void setup() {
  //wdt_enable(WDTO_1S);
  
  pinMode(float_switch,INPUT); //INPUT_PULLUP
  pinMode(relay,OUTPUT);
  
  digitalWrite(float_switch,HIGH); //INPUT_PULLUP instead!
  digitalWrite(relay,LOW);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(float_switch,HIGH);
  set_color(0,0,255);
  boolean switch_state = digitalRead(float_switch);
  if(not(switch_state)) {
    unsigned long start_time = millis(); 
    while((millis() - start_time) < countdown_duration) {
      switch_state = digitalRead(float_switch);
      if(switch_state) {
        break;
      }
      Serial.print("Countdown ");
      Serial.println((countdown_duration - (millis() - start_time))/1000);
      set_color(255,0,0);
      delay(countdown_delay);
    }
    digitalWrite(relay,HIGH);
    start_time = millis();
    while((millis() - start_time) < max_open_time) {
      switch_state = digitalRead(float_switch);
      if(switch_state) {
        break;
      }
      Serial.print("Busy ");
      Serial.println((max_open_time - (millis() - start_time))/1000);
      set_color(0,255,0);
      delay(busy_delay);
    }
    digitalWrite(relay,LOW);
  }
  Serial.println("idle");
  delay(idle_delay);
}
