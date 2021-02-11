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
int SCROLL_INTERVAL = 250;

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
  
  //displayNumber(price);
  scrollDisplay(price);

  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    price = getData();

    }
  
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






void scrollDisplay(int price){

  int len = numdigits(price);
  char buf[len+1];
  itoa(price, buf, 10);

  unsigned long previousMillis = 0;
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

    displayNumber(int_out);

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= SCROLL_INTERVAL) {
      previousMillis = currentMillis;
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



// currently largely stolen from https://github.com/openhardwarelabs/bitcoin-ticker/blob/master/bitcoin_ticker/bitcoin_ticker.ino
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

  // Print price
  //Serial.println();
  //Serial.println("Bitcoin price: ");
  //Serial.println(price);
  int out = round(price);
  //Serial.println(out);
  return out;

  
}
