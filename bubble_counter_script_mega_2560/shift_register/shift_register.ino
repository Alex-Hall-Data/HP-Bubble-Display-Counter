#include <ShiftRegister74HC595.h>

//control hp led with shift register
//daisychain sn74hc595 registers and hookup as below and with Q4-7 of register 1 to cathode pins1-4 of display
//Q0-7 of register 2 go to anodes a-f-dp of display


int numberOfShiftRegisters = 2; // number of shift registers attached in series
int latchPin = 1; //ST_CP pin
int clockPin = 0; //SH_CP pin
int dataPin = 2; //DS pin

int DUTY_CYCLE = 1;
int BLANKING_DELAY = 100;

ShiftRegister74HC595<2> sr(dataPin, clockPin, latchPin);

byte digit;
byte digitSelect;
byte digitArray[10]; //array of segment states
byte selectArray[4]; //array of digit onoff states

void setup() {

  //GPIO 1 (TX) swap the pin to a GPIO.
  pinMode(1, FUNCTION_3); 

  //set pins to output because they are addressed in the main loop

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //define arrays

  digitArray[0] = 0x3F; //B00111111
  digitArray[1] = 0x6; //B00000110
  digitArray[2] = 0x5B; //B01011011
  digitArray[3] = 0x4F; //0B01001111
  digitArray[4] = 0x66; //0B01100110
  digitArray[5] = 0x6D; //0B01101101
  digitArray[6] = 0x7D; //0B01111101
  digitArray[7] = 0x7; //0B00000111
  digitArray[8] = 0x7F; //0B01111111
  digitArray[9] = 0x6F; //0B01101111

  selectArray[3] = 0x7F; //0B01111111
  selectArray[2] = 0xBF; //0B10111111
  selectArray[1] = 0xDF; //0B11011111
  selectArray[0] = 0xEF; //0B11101111

  sr.setAllLow(); // set all pins LOW

}

int i=1;

void loop(){

  //uint8_t srData[] = {selectArray[2], digitArray[7]};
  //sr.setAll(srData);
  displayNumber(i);

  i++;
}


void displayNumber(int num){
  
  int digit1;
  int digit2;
  int digit3;
  int digit4;

  uint8_t srData[2];

  if(num >= 1000){
    digit1 = (num/1000)%10;
    srData[0] = selectArray[0];
    srData[1] = digitArray[digit1];
  }
  else{
    srData[0] = 0b11111111;
    srData[1] = 0b00000000;
  }
  sr.setAll(srData);

  delay(DUTY_CYCLE);
  srData[0] = 0b11111111;
  srData[1] = 0b00000000;
  sr.setAll(srData);
  delayMicroseconds(BLANKING_DELAY);


  if(num >= 100){
    digit2 = (num/100)%10;
    srData[0] = selectArray[1];
    srData[1] = digitArray[digit2];
  }
  else{
    srData[0] = 0b11111111;
    srData[1] = 0b00000000;
  }
  sr.setAll(srData);

  delay(DUTY_CYCLE);
  srData[0] = 0b11111111;
  srData[1] = 0b00000000;
  sr.setAll(srData);
  delayMicroseconds(BLANKING_DELAY);



  if(num >= 10){
    digit3 = (num/10)%10;
    srData[0] = selectArray[2];
    srData[1] = digitArray[digit3];

  }
  else{
   srData[0] = 0b11111111;
   srData[1] = 0b00000000;
  }
  sr.setAll(srData);

  delay(DUTY_CYCLE);
  srData[0] = 0b11111111;
  srData[1] = 0b00000000;
  sr.setAll(srData);
  delayMicroseconds(BLANKING_DELAY);



  if(num >= 1){
    digit4 = (num)%10;
    srData[0] = selectArray[3];
    srData[1] = digitArray[digit4];

  }
  else{
    srData[0] = 0b11111111;
    srData[1] = 0b00000000;
  }
  sr.setAll(srData);

  delay(DUTY_CYCLE);
  srData[0] = 0b11111111;
  srData[1] = 0b00000000;
  sr.setAll(srData);
  delayMicroseconds(BLANKING_DELAY);

  
  
}
