#include <math.h>
#include <stdlib.h> 


void setup() {
    Serial.begin(9600);
    Serial.println("test");

}

void loop() {
  
  scrollNumber(12345);

}

void scrollNumber(long price){

  int len = numdigits(price);
  char buf[len+1];
  itoa(price, buf, 10);

  int counter = 0;
  while (true){
    int a = counter%len;
    int b = (counter + 1)%len;
    int c = (counter + 2)%len;
    int d = (counter + 3)%len;

    char char_in[4]; 
    int int_out;
    
    char_in[0] = buf[a];
    char_in[1] = buf[b];  
    char_in[2] = buf[c];  
    char_in[3] = buf[d];

    int_out = atoi(char_in);

    Serial.println(int_out);

    counter++;
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
