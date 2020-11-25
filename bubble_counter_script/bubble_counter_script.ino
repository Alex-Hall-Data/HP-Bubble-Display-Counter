//D0-D7 -> a-h

int DUTY_CYCLE = 1;
int BLANKING_DELAY = 100;

int C1 = 8;
int C2 = 9;
int C3 = 10;
int C4 = 11;

void setup() {
  DDRD = B11111111;  // set port D as output
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);

  digitalWrite(C1, HIGH);
  digitalWrite(C2, HIGH);
  digitalWrite(C3, HIGH);
  digitalWrite(C4, HIGH);

}

int i=0;
int interval=20; //interval between digit changes
int previousMillis = 0;
int currentMillis;

int digit1;
int digit2;
int digit3;
int digit4;

void loop() {
  currentMillis = millis();
  if(currentMillis-previousMillis>interval){
    i++;
    previousMillis = currentMillis;
  }

  

  
 

  if(i>=1000){
    digit1 = (i/1000)%10;
    digitalWrite(C1, LOW);
  }
  displayDigit(digit1);
  delay(DUTY_CYCLE);
  digitalWrite(C1, HIGH);
  delayMicroseconds(BLANKING_DELAY);

  if(i>=100){
    digit2 = (i/100)%10;
    digitalWrite(C2, LOW);
  }
  displayDigit(digit2);
  delay(DUTY_CYCLE);
  digitalWrite(C2, HIGH);
  delayMicroseconds(BLANKING_DELAY);

  if(i>=10){
    digit3 = (i/10)%10;
    digitalWrite(C3, LOW);
  }
  displayDigit(digit3);
  delay(DUTY_CYCLE);
  digitalWrite(C3, HIGH);
  delayMicroseconds(BLANKING_DELAY);

  if(i>=0){
   digit4 = (i)%10;
   digitalWrite(C4, LOW);
  }
  displayDigit(digit4);
  delay(DUTY_CYCLE);
  digitalWrite(C4, HIGH);
  delayMicroseconds(BLANKING_DELAY);

}

void displayDigit(int value){

  if(value==0){
    PORTD = B00111111;
  }
  else if(value ==1){
    PORTD = B00000110;
  }
    else if(value ==2){
    PORTD = B01011011;
  }
    else if(value ==3){
    PORTD = B01001111;
  }
    else if(value ==4){
    PORTD = B01100110;
  }
    else if(value ==5){
    PORTD = B01101101;
  }
    else if(value ==6){
    PORTD = B01111101;
  }
    else if(value ==7){
    PORTD = B00000111;
  }
      else if(value ==8){
    PORTD = B01111111;
  }
      else if(value ==9){
    PORTD = B01101111;
  }
  else{
    PORTD=B00000000;
  }
}
