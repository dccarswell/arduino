#include <TM1637Display.h>
//#include <avr/wdt.h>

#define float_switch 4
#define relay 3
#define red_pin 9
#define green_pin 10
#define blue_pin 11
#define CLK7 7
#define DIO7 8

const unsigned long countdown_blink_duration = 250L; //ms
const unsigned long busy_blink_duration = 100L; //ms

const unsigned long idle_delay = 500L; //ms
const unsigned long countdown_delay = 250L; //ms
const unsigned long busy_delay = 100L; //ms

const unsigned long countdown_duration = 150000L; //ms
const unsigned long max_open_time = 120000L; //ms

TM1637Display display(CLK7, DIO7);
#define BRIGHTNESS7 0x0f

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

  display.setBrightness(BRIGHTNESS7,false);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(float_switch,HIGH);
  set_color(0,0,255);
  boolean switch_state = digitalRead(float_switch);

  if(not(switch_state)) {
    unsigned long start_time = millis();
    unsigned long elapsed_time = millis() - start_time;
    unsigned long remaining_time = countdown_duration - elapsed_time;
    display.setBrightness(BRIGHTNESS7,true);
    while(elapsed_time < countdown_duration) {
      switch_state = digitalRead(float_switch);
      if(switch_state) {
        display.clear();
        display.setBrightness(BRIGHTNESS7,false);
        break;
      }
      Serial.print("Countdown ");
      Serial.println(remaining_time/1000);
      display.showNumberDecEx(remaining_time/1000,0,false);
      set_color(255,0,0);
      delay(countdown_delay);
      elapsed_time = millis() - start_time;
      remaining_time = countdown_duration - elapsed_time;
    }
    digitalWrite(relay,HIGH);
    display.clear();
    start_time = millis();
    elapsed_time = millis() - start_time;
    remaining_time = max_open_time - elapsed_time;
    while(elapsed_time < max_open_time) {
      switch_state = digitalRead(float_switch);
      if(switch_state) {
        display.clear();
        display.setBrightness(BRIGHTNESS7,false);
        break;
      }
      Serial.print("Busy ");
      display.showNumberDecEx(remaining_time/1000,0,false);
      Serial.println(remaining_time/1000);
      set_color(0,255,0);
      delay(busy_delay);
      elapsed_time = millis() - start_time;
      remaining_time = max_open_time - elapsed_time;
    }
    digitalWrite(relay,LOW);
    display.clear();
    display.setBrightness(BRIGHTNESS7,false);
  }
  Serial.println("idle");
  delay(idle_delay);
}
