//D0-D7 -> a-h

int DUTY_CYCLE = 1;
int BLANKING_DELAY = 100;

int C1 = 8;
int C2 = 9;
int C3 = 10;
int C4 = 11;

void setup() {
  DDRA = B11111111;  // set port D as output
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);

  digitalWrite(C1, HIGH);
  digitalWrite(C2, HIGH);
  digitalWrite(C3, HIGH);
  digitalWrite(C4, HIGH);


  Serial.begin(9600);
  delay(1000);

}




int digit1;
int digit2;
int digit3;
int digit4;
int price=0;

void loop() {

  
  
  if (Serial.available()>0)
  {      
    String a = Serial.readString();
    price = a.toInt();
  }
 

  if(price>=1000){
    digit1 = (price/1000)%10;
    digitalWrite(C1, LOW);
  }
  displayDigit(digit1, false);
  delay(DUTY_CYCLE);
  digitalWrite(C1, HIGH);
  delayMicroseconds(BLANKING_DELAY);

  if(price>=100){
    digit2 = (price/100)%10;
    digitalWrite(C2, LOW);
  }
  displayDigit(digit2, false);
  delay(DUTY_CYCLE);
  digitalWrite(C2, HIGH);
  delayMicroseconds(BLANKING_DELAY);

  if(price>=10){
    digit3 = (price/10)%10;
    digitalWrite(C3, LOW);
  }
  displayDigit(digit3, true);
  delay(DUTY_CYCLE);
  digitalWrite(C3, HIGH);
  delayMicroseconds(BLANKING_DELAY);

  if(price>=0){
   digit4 = (price)%10;
   digitalWrite(C4, LOW);
  }
  displayDigit(digit4, false);
  delay(DUTY_CYCLE);
  digitalWrite(C4, HIGH);
  delayMicroseconds(BLANKING_DELAY);

}

void displayDigit(int value, bool DP){
  if(DP==false){
    if(value==0){
      PORTA = B00111111;
    }
    else if(value ==1){
      PORTA = B00000110;
    }
      else if(value ==2){
      PORTA = B01011011;
    }
      else if(value ==3){
      PORTA = B01001111;
    }
      else if(value ==4){
      PORTA = B01100110;
    }
      else if(value ==5){
      PORTA = B01101101;
    }
      else if(value ==6){
      PORTA = B01111101;
    }
      else if(value ==7){
      PORTA = B00000111;
    }
        else if(value ==8){
      PORTA = B01111111;
    }
        else if(value ==9){
      PORTA = B01101111;
    }
    else{
      PORTA=B00000000;
    }

  } else{

          if(value==0){
      PORTA = B10111111;
    }
    else if(value ==1){
      PORTA = B10000110;
    }
      else if(value ==2){
      PORTA = B11011011;
    }
      else if(value ==3){
      PORTA = B11001111;
    }
      else if(value ==4){
      PORTA = B11100110;
    }
      else if(value ==5){
      PORTA = B11101101;
    }
      else if(value ==6){
      PORTA = B11111101;
    }
      else if(value ==7){
      PORTA = B10000111;
    }
        else if(value ==8){
      PORTA = B11111111;
    }
        else if(value ==9){
      PORTA = B11101111;
    }
    else{
      PORTA=B00000000;
    }
    
  }
}
