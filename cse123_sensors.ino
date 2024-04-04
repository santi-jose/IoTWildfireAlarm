/* CSE123B Team 4 Wildfire Detection IoT
   created by Aracely Cano-Gramajo & Jose Santiago
*/
#include <HardwareSerial.h>

/*Pins Used*/
#define RXD 33 //rx pin 
#define TXD 32 //tx pin
#define rainDigital 35 //rain sensor pins
#define pwmPin 34 // temp pin

/*comands for the UART*/
uint8_t  readgas[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};  // get gas command
uint8_t  cal[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};  // calibrate command
uint8_t  response[9] = {};  // holds the recieved data

/*variables used*/
uint8_t idx = 0;
bool flag = false;
uint16_t co2 = 0;
bool skip_flag = false;

void setup() {
  Serial.begin(9600, SERIAL_8N1, RXD, TXD); //baud-rate, protocol, RX pin, TX pin
  pinMode(rainDigital, INPUT); //setting rain pin as input
  pinMode(pwmPin, INPUT);
}

void loop() {
  int temp = getTemp();
  int rain = getRain();
  uint16_t CO2 = getCO2();

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Rain: ");
  Serial.println(rain);
  Serial.print("CO2 ppm: ");
  Serial.println(CO2);
  delay(10000); // 10 second delay
}

/*Helper Functions*/
uint16_t getCO2(void) {
  Serial.write(readgas, 9); //send out readgas code to uart
  delay(1000);
  while (Serial.available()) { //data is ready to read
    //Serial.println(idx);
    response[idx++] = Serial.read(); //save each bit into responce array
    flag = true; //set flag true
  }
  idx = 0; //reset idx
  if (flag) { //if response is full
    flag = false; //reset flag & get co2 -> CO2 concentration = HIGH * 256 + LOW
    co2 = 0;
    //Serial.print("high bits: ");
    //Serial.println((uint16_t)response[2]);
    //Serial.print("low bits: ");
    //Serial.println(response[3]);
    co2 += (uint16_t)response[2] << 8;
    co2 += response[3];
  }
  //Serial.print("CO2: ");
  //Serial.println(co2);
  return co2;
}

int getRain(void){
  int rainDigitalVal = digitalRead(rainDigital); // read input pin
  //Serial.print("\t");
  //Serial.println(rainDigitalVal); //print it in serial output 0 or 1 depending if wet or not
  return rainDigitalVal;
}

int getTemp(void){
  int tempVal = analogRead(pwmPin);
  //Serial.print("temp ");
  //Serial.println(tempVal);
  return tempVal;
}
