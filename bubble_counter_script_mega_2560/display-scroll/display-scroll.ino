#include <math.h>
#include <stdlib.h> 


int SCROLL_INTERVAL=250;

void setup() {
    Serial.begin(9600);
    Serial.println("test");

}

void loop() {

  long pr = 12345;
  scrollNumber(pr);

}

void scrollNumber(int price){

  int len = numdigits(price);
  char buf[len+1];
  ltoa(price, buf, 10);

  //new character array with blanking spaces at end
  char buf2[len+1];
  for(int i=0;i<len+1;i++){
    if(i<len){
      buf2[i]=buf[i];
    }
    else{
      buf2[i]='X';//blanking at end of number
    }
  }

  unsigned long previousMillis = 0;
  int counter = 0;
  while (true){
    int a = counter%len;
    int b = (counter + 1)%(len+1);
    int c = (counter + 2)%(len+1);
    int d = (counter + 3)%(len+1);

    //48 seems to be from ascii conversion. I really hate C++
    int int_out1 = buf2[a]+0-48;
    int int_out2 = buf2[b]+0-48;
    int int_out3 = buf2[c]+0-48;
    int int_out4 = buf2[d]+0-48;

    


    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= SCROLL_INTERVAL) {
      previousMillis = currentMillis;
    Serial.println(int_out1);
    Serial.println(int_out2);
    Serial.println(int_out3);
    Serial.println(int_out4);
    Serial.println();
      counter++;
    }
  }
  
}


int numdigits(long i)
{
       int digits;

       i = abs(i);
       if (i < 10)
         digits = 1;
       else
         digits = (int)(log10((double)i)) + 1;
       return digits;
}
