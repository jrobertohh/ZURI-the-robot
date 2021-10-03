   /*********************************************************************************
         D       R       Ô       L       E        V   E   R         0    4 
 *********************************************************************************/

 /* VERSIÓN ESTABLE EN INGLÉS*/


#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "VoiceRecognitionV3.h"
#include <Adafruit_Sensor.h> 
#include "DHT.h"
#include <Wire.h>
#include "LedControl.h"


//Reproductor de MP3
SoftwareSerial mySoftwareSerial(11, 12); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
//void printDetail(uint8_t type, int value);

/** Reconocimiento de VOZ
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2, 3);   // 2:RX 3:TX, you can choose your favourite pins.
uint8_t records[7]; // save record
uint8_t buf[64];
int Led = 13;
#define Life      (7)
#define Weather   (8) 
#define Play      (9)
#define Hello    (10)
#define Okay   (11)
#define Other    (12)
#define Finish      (13)
/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    if (buf[i] > 0x19 && buf[i] < 0x7F) {
      Serial.write(buf[i]);
    }
    else {
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}
/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized.
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if (buf[0] == 0xFF) {
    Serial.print("NONE");
  }
  else if (buf[0] & 0x80) {
    Serial.print("UG ");
    Serial.print(buf[0] & (~0x80), DEC);
  }
  else {
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if (buf[3] > 0) {
    printSignature(buf + 4, buf[3]);
  }
  else {
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

//Valriable para dar un consejo aleatorio al niño/a
int randNumber;

//Banderas
int bandera=0;
int bandera1=0;
int bandera2=0;
int bandera3=0;
int CONT=0;

//Sensor de temperatura y humedad
int SENSORTH = 5;   //Cambió de 2 a 5
int temp, humedad;
DHT dht (SENSORTH, DHT11);

//Sensor de llama
int buttonpin = 4; // define the flame sensor interface - Cambió de 3 a 6
int analoog = A3; // define the flame sensor interface
int val ;// define numeric variables val 
float sensor; //read analoog value

//OJOS DE ZURI
/*
 pin 2/12/6 is connected to the DataIn 
 pin 4/11/8 is connected to the CLK 
 pin 3/10/7 is connected to LOAD 
 ***** Please set the number of devices you have *****
 */
LedControl lc=LedControl(6,8,7,2);
const int addrL = 0;  // first LED matrix - Left robot eye
const int addrR = 1;  // second LED matrix - Right robot eye


void setup() {
  
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  //Reproductor MP3
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  //delay(2500);
    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
    myDFPlayer.play(24);  //Play the first mp3 1(y), 2(n), 3(y), 4(y) 24
    delay(5000);
  
  /** Reconocimiento de VOZ */
  myVR.begin(9600);
  delay(5000);
  Serial.println("Elechouse Voice Recognition V3 Prueba ZURI");
  if (myVR.clear() == 0) {
    Serial.println("Recognizer cleared.");
  } else {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while (1);
  }
  if(myVR.load((uint8_t)Life) >= 0){
    Serial.println("Comando Life leído");
  }
  if(myVR.load((uint8_t)Weather) >= 0){
    Serial.println("Comando Weather  leído");
  }
  //Agregado
  if(myVR.load((uint8_t)Play) >= 0){
    Serial.println("Comando Play leído");
  }
  if(myVR.load((uint8_t)Hello) >= 0){
    Serial.println("Comando Hello leído");
  }
  if(myVR.load((uint8_t)Okay) >= 0){
    Serial.println("Comando Okay leído");
  }
  if(myVR.load((uint8_t)Other) >= 0){
    Serial.println("Comando Other leído");
  }
  if(myVR.load((uint8_t)Finish) >= 0){
    Serial.println("Comando Finish leído");
  }

  
  //Sensor de Temperatura y humedad
  dht.begin();

  //Flama
  //pinMode (Led, OUTPUT) ;// define LED as output interface
  pinMode (buttonpin, INPUT) ;// output interface defines the flame sensor
  pinMode (analoog, INPUT) ;// output interface defines the flame sensor

  //OJOS DE ZURI
  /*The MAX72XX is in power-saving mode on startup*/
  lc.shutdown(addrL,false);
  lc.shutdown(addrR,false);
  /* Set the brightness to max values */
  lc.setIntensity(addrL,1);
  lc.setIntensity(addrR,1);
  /* and clear the display */
  lc.clearDisplay(addrL);
  lc.clearDisplay(addrR);

  // turn on all LEDs for a test
  for(int row=0;row<8;row++) {
    lc.setRow(addrL, row, 255);
    lc.setRow(addrR, row, 255);
    delay(100);
  }
  delay(300);

}

/* Valores a enviar
2  (pos 0) - Neutral01
3  (pos 1) - Neutral02
4  (pos 2) - Neutral03
5  (pos 3) - showAnger
6  (pos 4) - showSadness
7  (pos 5) - showScared
8  (pos 6) - showWink
9  (pos 7) - showHappy
10 (pos 8) - showLove

*/
byte pin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
byte estado = 0;

//OJOS DE ZURI
void showNeutral01() {
  byte left[8] = {
B00000000,
B00111100,
B01111110,
B01001110,
B01001110,
B01111110,
B00111100,
B00000000};
 displayEmotion(left, left);
}

void showNeutral02() {
  byte left[8] = {
B00000000,
B00111100,
B01111110,
B01100110,
B01100110,
B01111110,
B00111100,
B00000000};
displayEmotion(left, left);
}

void showNeutral03() {
  byte left[8] = {
B00000000,
B00111100,
B01111110,
B01110010,
B01110010,
B01111110,
B00111100,
B00000000};
displayEmotion(left, left);
}

void showAnger() {
  byte left[8] = {
B00000000,
B00000000,
B01111110,
B01011010,
B01011010,
B00100100,
B00011000,
B00000000};
  byte right[8] = {
B00000000,
B00000000,
B01111110,
B01011010,
B01011010,
B00100100,
B00011000,
B00000000};

  displayEmotion(left, right);
}

void showSadness() {
  byte left[8] = {
B00000000,
B00001100,
B00010010,
B00111010,
B01011010,
B01000010,
B00111100,
B00000000};
  byte right[8] = {
B00000000,
B00110000,
B01001000,
B01011100,
B01011010,
B01000010,
B00111100,
B00000000};

  displayEmotion(left, right);
}

void showScared() {
  byte right[8] = {
B00000000,
B00001100,
B00010010,
B00111010,
B01011010,
B01000010,
B00111100,
B00000000};
  byte left[8] = {
B00000000,
B00110000,
B01001000,
B01011100,
B01011010,
B01000010,
B00111100,
B00000000};

  displayEmotion(left, right);
}

void showWink() {
  byte right[8] = {
B00000000,
B00000000,
B00000000,
B00000000,
B00111100,
B01000010,
B00000000,
B00000000};
  byte left[8] = {
B00000000,
B00111100,
B01000010,
B01011010,
B01011010,
B01000010,
B00111100,
B00000000};

  displayEmotion(left, right);
}

void showHappy() {
  byte right[8] = {
B00000000,
B00000000,
B00000000,
B00000000,
B00111100,
B01000010,
B00000000,
B00000000};

  displayEmotion(right, right);
}

void showLove() {
  byte right[8] = {
B01100110,
B10011001,
B10000001,
B10000001,
B01000010,
B00100100,
B00011000,
B00000000};
  byte left[8] = {
B01100110,
B10011001,
B10000001,
B10000001,
B01000010,
B00100100,
B00011000,
B00000000};

  displayEmotion(left, right);
}

void showParpadeo01() {
  byte left[8] = {
B00000000,
B00111100,
B01111110,
B01100110,
B01100110,
B01111110,
B00111100,
B00000000};
displayEmotion(left, left);
}

void showParpadeo02() {
  byte left[8] = {
B00000000,
B00000000,
B01111110,
B01100110,
B01100110,
B01111110,
B00111100,
B00000000};
displayEmotion(left, left);
}

void showParpadeo03() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B01111110,
B01100110,
B01100110,
B00111100,
B00000000};
displayEmotion(left, left);
}

void showParpadeo04() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B00000000,
B01111110,
B01100110,
B00111100,
B00000000};
displayEmotion(left, left);
}

void showParpadeo05() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B01111110,
B00111100,
B00000000};
displayEmotion(left, left);
}

void displayEmotion(byte left[8], byte right[8]) {
  lc.clearDisplay(addrL);
  lc.clearDisplay(addrR);
  for(int row=0;row<8;row++) {
    lc.setRow(addrL,row,left[row]);
    lc.setRow(addrR,row,right[row]);
  }
}


void loop(){ 
  
  
  Wire.beginTransmission(1);
  
  //Reproductor de MP3
  static unsigned long timer = millis();
  if (millis() - timer > 3000) {
    timer = millis();
    //myDFPlayer.next();  //Play next mp3 every 3 second.
  }
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  //OJOS DE ZURI
  showNeutral01();
  delay(250);
  showNeutral02();
  delay(250);
  

  showParpadeo01();
  delay(150);
  showParpadeo02();
  delay(5);
  showParpadeo03();
  delay(5);
  showParpadeo04();
  delay(5);
  showParpadeo05();
  delay(5);

  

  //Sensor de temperatura y humedad
  float humedad = dht.readHumidity();
  float temp    = dht.readTemperature();
  temp = temp - 3;
        
  int ret;
  ret = myVR.recognize(buf, 50);
  if (ret > 0) {
    switch (buf[1]) {
      case Hello:
        /*/digitalWrite(led, LOW);
        Wire.write(pin[6]);
        Wire.write(estado);
        Wire.endTransmission();
        // Esperamos 1 segundo
        delay(1000);
        // Cambiamos el estado
        if (estado == 0){
            estado = 1;
        }
        else {
            estado = 0;
        }*/
        showWink();
        myDFPlayer.volume(30);  
        myDFPlayer.play(24);
        delay(3000);
        break;
      
      case Weather:
        if(temp <= 20) { 
          myDFPlayer.volume(30);  
          myDFPlayer.play(41);
          delay(5000);
          showAnger();
        }
        else if(temp > 20 && temp <= 25 ){
               myDFPlayer.volume(30);  
               myDFPlayer.play(38);
               delay(5000);
               showAnger();
        }
        else{
               myDFPlayer.volume(30);  
               myDFPlayer.play(37);
               delay(5000);
               showAnger();
        }
        break;
      
      /*
      case Play:
        //Serial.println("Jugar");
        showAnger();
        myDFPlayer.volume(30);  
        myDFPlayer.play(44);
        delay(10000);
        bandera1 = 0;
        while(bandera1 == 0 ){
            ret = myVR.recognize(buf, 50);
            if(buf[1] == 4){
              showAnger();
              myDFPlayer.volume(30);  
              myDFPlayer.play(45);
              delay(12000);
              bandera2 = 0;
              while(bandera2 == 0){
                  ret = myVR.recognize(buf, 50);
                  if(buf[1] == 6){
                    myDFPlayer.volume(30);  
                    myDFPlayer.play(42);
                    delay(3000);
                    showWink();
                    bandera2 = 1;
                    bandera1 = 1;
                  }
              }
            }
        }
        break;
        */

        case Play:
        //Serial.println("Jugar");
        showAnger();
        myDFPlayer.volume(30);  
        myDFPlayer.play(45);
        delay(10000);
        bandera2 = 0;
        while(bandera2 == 0){
                  ret = myVR.recognize(buf, 50);
                  if(buf[1] == 6){
                    myDFPlayer.volume(30);  
                    myDFPlayer.play(42);
                    delay(3000);
                    showWink();
                    bandera2 = 1;
                    bandera1 = 1;
            }
        }
        break;
      
        
      case Life:
        //Tip para la vida aleatorio
        randomSeed(millis());    // genera una semilla para aleatorio a partir de la función millis()
        randNumber = random(6);    // genera número aleatorio entre 0 y 6
        if(randNumber == 0) randNumber=28;        //Lavar los dientes 24
        else if(randNumber == 1) randNumber=29;   //Agua natural      23
        else if(randNumber == 2) randNumber=30;  //Alimentación      28 
        else if(randNumber == 3) randNumber=31;  //Ejercicio         27
        else if(randNumber == 4) randNumber=34;  //No te exongas sol 30
        else if(randNumber == 5) randNumber=35;   //Ordena juguetes   29
        else if(randNumber == 6) randNumber=36;   //Haz tarea         31
        //OJOS DE ZURI
        showWink();
        //delay(4000);
        myDFPlayer.volume(30);  
        myDFPlayer.play(randNumber);
        delay(3000);
        //digitalWrite(led, HIGH);
        break;
      
      
      default:
        Serial.println("Record function undefined");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }

  //Sensor flama
  sensor = analogRead(analoog);
  //Serial.println(sensor); // display temperature
  val = digitalRead (buttonpin) ;// digital interface will be assigned a value of 3 to read val
  if (val == HIGH){   // When the flame sensor detects a signal, LED flashes
      //OJOS DE ZURI
      //showScared();
      //delay(4000);
      //digitalWrite (Led, HIGH);
      myDFPlayer.volume(30);  
      myDFPlayer.play(25);
      delay(3000);
      
  }
  
  /*
  else{
      digitalWrite (Led, LOW);
  }*/
  delay(1000);

  /*/OJOS DE ZURI
 showNeutral03();
 delay(250);
 showNeutral02();
 delay(250);
 showNeutral01();
 delay(250);*/

 //OJOS DE ZURI
  showParpadeo05();
  delay(5);
  showParpadeo04();
  delay(5);
  showParpadeo03();
  delay(5);
  showParpadeo02();
  delay(5);
  showParpadeo01();
  delay(150);

  showNeutral02();
  delay(250);
  showNeutral03();
  delay(250);
 
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
