#include <DS3231.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define SWSERIAL_GPS_TX 10
#define SWSERIAL_GPS_RX 9

DS3231 rtc(SDA,SCL);
TinyGPS gps;
SoftwareSerial gps_s(SWSERIAL_GPS_TX, SWSERIAL_GPS_RX);

bool sync() {
  gps_s.begin(9600);
  while(true) {
    if(gps_s.available()) {
      char c = gps_s.read();
      if(gps.encode(c)) {
        int year;
        byte month,day,hour,min,sec,hundos;
        unsigned long age;
        gps.crack_datetime(&year,&month,&day,&hour,&min,&sec,&hundos,&age);
        rtc.setTime(static_cast<int>(hour),static_cast<int>(min),static_cast<int>(sec));
        rtc.setDate(static_cast<int>(day),static_cast<int>(month),static_cast<int>(year));
        rtc.begin();
        return true;
      }
    }
  }
}


void setup() {
  Serial.begin(9600);
  rtc.begin();
  sync();
}
void loop() {
  Serial.print(rtc.getDateStr(FORMAT_LONG,FORMAT_BIGENDIAN,'-'));
  Serial.print("T");
  Serial.print(rtc.getTimeStr(FORMAT_LONG));
  Serial.print(" ");
  long ts = rtc.getUnixTime(rtc.getTime());
  Serial.println(ts);

  
  delay(1000);
  
}







/*
// #include <LiquidCrystal_I2C.h>
#include <sha1.h>
#include <TOTP.h>

const unsigned int keysize = 16;
const uint8_t private_key[keysize] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d,0x4d};
//const unsigned int keysize = 10;
//const uint8_t private_key[keysize] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
//MyLegoDoorMMMMMM



DS3231 rtc(SDA,SCL);
//TOTP totp = TOTP(private_key,keysize);

uint8_t* _hmacKey = *private_key;
int _keyLength = keysize; //
//int _keyLength;
int _timeStep = 30; //
//int _timeStep;
uint8_t _byteArray[8];
// uint8_t _byteArray[14]; //
uint8_t* _hash;
int _offset;
long _truncatedHash;
char _code[7];

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  //rtc.setClockMode(false);
  rtc.begin();

  //rtc.setTime(4,26,25);
  //rtc.setDOW(3);
  //rtc.setDate(18,9,2019);
}

void loop() {
  //if(Serial.available() > 0) {
  //Serial.read();
  //}
  Serial.print(rtc.getDateStr(FORMAT_LONG,FORMAT_BIGENDIAN,'-'));
  Serial.print("T");
  Serial.print(rtc.getTimeStr(FORMAT_LONG));
  Serial.print(" ");
  long ts = rtc.getUnixTime(rtc.getTime());
  Serial.print(ts);
  Serial.print("  ");
  update(ts);
  
  delay(1000);
}
void update(long timestamp) {
  char* code = getCode(timestamp);
  Serial.println(code);
}

char* getHOTP(unsigned long steps,char* secret,int keylength=16, int digits=6) {
  byte* hmac;
  Sha1.initHmac(secret,keylength);
  Sha1.write((byte*)(steps & 0xFF),8);
  hmac = Sha1.resultHmac();
  Serial.println(hmac);
  return *hmac;
}
char* getTOTP(unsigned long timestamp, int timestep = 30) {
  return getHOTP(timestamp/timestep);
}


char* getCode(long timeStamp) {
  long steps = timeStamp / _timeStep;
  
  // STEP 0, map the number of steps in a 8-bytes array (counter value)
  _byteArray[0] = 0x00;
  _byteArray[1] = 0x00;
  _byteArray[2] = 0x00;
  _byteArray[3] = 0x00;
  _byteArray[4] = (int)((steps >> 24) & 0xFF);
  _byteArray[5] = (int)((steps >> 16) & 0xFF);
  _byteArray[6] = (int)((steps >> 8) & 0XFF);
  _byteArray[7] = (int)((steps & 0XFF));

  // STEP 1, get the HMAC-SHA1 hash from counter and key
  Sha1.initHmac(_hmacKey, _keyLength);
  Sha1.write(_byteArray, 8);
  _hash = Sha1.resultHmac();
  
  // STEP 2, apply dynamic truncation to obtain a 4-bytes string
  _offset = _hash[20 - 1] & 0xF; 
  _truncatedHash = 0;
  for (int j = 0; j < 4; ++j) {
    _truncatedHash <<= 8;
    _truncatedHash  |= _hash[_offset + j];
  }

  // STEP 3, compute the OTP value
  _truncatedHash &= 0x7FFFFFFF;
  _truncatedHash %= 1000000;
  
  // convert the value in string, with heading zeroes
  sprintf(_code, "%06ld", _truncatedHash);
  return _code;
}


*/
