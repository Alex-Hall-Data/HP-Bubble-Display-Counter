#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ShiftRegister74HC595.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <CoinMarketCapApi.h>



int numberOfShiftRegisters = 2; // number of shift registers attached in series
int latchPin = 1; //ST_CP pin
int clockPin = 0; //SH_CP pin
int dataPin = 2; //DS pin

int DUTY_CYCLE = 1;
int BLANKING_DELAY = 100;
int SCROLL_INTERVAL = 500;

ShiftRegister74HC595<2> sr(dataPin, clockPin, latchPin);

byte digit;
byte digitSelect;
byte digitArray[10]; //array of segment states
byte selectArray[4]; //array of digit onoff states


#define CD_API "/v1/bpi/currentprice.json"
#define CD_URL "api.coindesk.com"

WiFiClient client;

int price;
unsigned long interval = 100000; //ms interval between API queries

// API server
const char* host = "api.coindesk.com";

void setup() {

    //Serial.begin(9600);
    //Serial.println("test");

    
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    
    // WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;

    res = wm.autoConnect("STONKS","onlygoup"); // password protected ap
    //Serial.println("WiFi connected");  

    //TODO - add error on display if not connected

    // GPIO 1 (TX) swap the pin to a GPIO. - TODO -swap back if serial needed
  pinMode(1, FUNCTION_3); 

  //set pins to output because they are addressed in the main loop

  pinMode(latchPin, OUTPUT); //TODO - uncomment if serial needed
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

  price = getData();

}


//int i=1;

unsigned long previousMillis = 0;

void loop(){

  //displayNumber(i);
  //i++;

  //to display a static number
  /*
  displayNumber(price); 
  
  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    price = getData();

    }
   */
  
  
  //to scroll a number across the screen
  scrollDisplay(); 


  
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



void scrollDisplay(){

  int price = getData();

  unsigned long previousMillisScroll = 0;
  unsigned long previousMillisPrice = 0;
  int counter = 0;
  while (true){

  int len = numdigits(price);
  char buf[len+1];
  ltoa(price, buf, 10);

  //new character array with 3 blanking spaces at end
  char buf2[len+3];
  for(int i=0;i<len+3;i++){
    if(i<len){
      buf2[i]=buf[i];
    }
    else{
      buf2[i]='X';//blanking at end of number
    }
  }

    int a = counter%(len+3);
    int b = (counter + 1)%(len+3);
    int c = (counter + 2)%(len+3);
    int d = (counter + 3)%(len+3);

    //determine position of dp
    int dp = NULL;
    if(a==len+3-6){
      dp = 1;      
    }
    else if(b==len+3-6){
      dp=2;
    }
    else if(c==len+3-6){
      dp=3;
    }
    else if(d==len+3-6){
      dp=4;
    }

    //48 seems to be from ascii conversion. I really hate C++
    int int_out1 = buf2[a]+0-48;
    int int_out2 = buf2[b]+0-48;
    int int_out3 = buf2[c]+0-48;
    int int_out4 = buf2[d]+0-48;

    displayDigits(int_out1, int_out2, int_out3, int_out4, dp);

    unsigned long currentMillisScroll = millis();
    if (currentMillisScroll - previousMillisScroll >= SCROLL_INTERVAL) {
      previousMillisScroll = currentMillisScroll;
      counter++;
    }

    unsigned long currentMillisPrice = millis();
    if (currentMillisPrice - previousMillisPrice >= interval) {
      previousMillisPrice = currentMillisPrice;
      price=getData();
    }

    
  }
  
}



void displayDigits(int digit1, int digit2, int digit3, int digit4, int dp){


  uint8_t srData[2];

  if(digit1 < 10){
    srData[0] = selectArray[0];
    srData[1] = digitArray[digit1];
    //add decimal point if needed
    if(dp==1){
      srData[1] = srData[1] + 0x80;
    }
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


  if(digit2 < 10){
    srData[0] = selectArray[1];
    srData[1] = digitArray[digit2];
        //add decimal point if needed
    if(dp==2){
      srData[1] = srData[1] + 0x80;
    }
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



  if(digit3 < 10){
    srData[0] = selectArray[2];
    srData[1] = digitArray[digit3];
        //add decimal point if needed
    if(dp==3){
      srData[1] = srData[1] + 0x80;
    }

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



  if(digit4 < 10){
    srData[0] = selectArray[3];
    srData[1] = digitArray[digit4];
        //add decimal point if needed
    if(dp==4){
      srData[1] = srData[1] + 0x80;
    }

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



// currently largely stolen from https://github.com/openhardwarelabs/bitcoin-ticker/blob/master/bitcoin_ticker/bitcoin_ticker.ino
//returns price *100 (to allow for 2dp)
int getData(){


   // Connect to API
  //Serial.print("connecting to ");
  //Serial.println(host);
  
  // Use WiFiClient class to create TCP connections

  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    //Serial.println("connection failed");

  }
  
  // We now create a URI for the request
  String url = "/v1/bpi/currentprice.json";
  
  //Serial.print("Requesting URL: ");
  //Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(100);
  
  // Read all the lines of the reply from server and print them to Serial
  String answer;
  while(client.available()){
    String line = client.readStringUntil('\r');
    answer += line;
  }

  client.stop();
  //Serial.println();
  //Serial.println("closing connection");

  // Process answer
  //Serial.println();
  //Serial.println("Answer: ");
  //Serial.println(answer);

  // Convert to JSON
  String jsonAnswer;
  int jsonIndex;

  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {
      jsonIndex = i;
      break;
    }
  }

  // Get JSON data
  jsonAnswer = answer.substring(jsonIndex);
  //Serial.println();
  //Serial.println("JSON answer: ");
  //Serial.println(jsonAnswer);
  jsonAnswer.trim();

  // Get rate as float
  int rateIndex = jsonAnswer.indexOf("rate_float");
  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();
  float price = priceString.toFloat();
  price = price*100;

  // Print price
  //Serial.println();
  //Serial.println("Bitcoin price: ");
  //Serial.println(price);
  int out = round(price);
  //Serial.println(out);
  return out;

  
}
