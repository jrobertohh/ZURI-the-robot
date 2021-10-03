/*********************************************************************************
         D       R       Ô       L       E        V   E   R         0    2
 *********************************************************************************/
 
#include <Adafruit_Sensor.h> 
#include "DHT.h"
#include <Servo.h>
#include "LedControl.h"
#include "VoiceRecognitionV3.h"
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

//Reconocimiento de VOZ
VR myVR(2,3);    // 3/13:RX 2/12:TX, you can choose your favourite pins.
uint8_t records[7]; // save record
uint8_t buf[64];
int led = 13;
#define Vida    (0)
#define Clima   (1) 
#define Jugar   (2)
#define Hola    (3)
#define Busca   (4)
#define Otra    (5)
#define Fin     (6)
void printSignature(uint8_t *buf, int len){
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}
void printVR(uint8_t *buf){
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");
  Serial.print(buf[2], DEC);
  Serial.print("\t\t");
  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");
  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

//Ojos de ZURI
/*
 pin 2/12/12/12/7/4 is connected to the DataIn 
 pin 4/11/13/ 8/9/6 is connected to the CLK 
 pin 3/10/ 9/ 9/8/5 is connected to LOAD 
 ***** Please set the number of devices you have ***** 
                        (DIN, CLK, CS)
 */
LedControl lc=LedControl(6,5,4,2);
const int addrL = 0;  // first LED matrix - Left robot eye
const int addrR = 1;  // second LED matrix - Right robot eye


//Sensor de temperatura y humedad
int SENSORTH = 9;   //Cambió de 2 a 5
int temp, humedad;
DHT dht (SENSORTH, DHT11);

//Sensor de llama
//int Led = 13 ;// define LED Interface
int buttonpin = 8; // define the flame sensor interface - Cambió de 3 a 6
int analoog = A3; // define the flame sensor interface
int val ;// define numeric variables val 
float sensor; //read analoog value

//MPlayerMini
SoftwareSerial mySoftwareSerial(11, 12); // RX=10, TX=11
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

//Bluetooth
int bandera=0;
int bandera1=0;
int bandera2=0;
int bandera3=0;

//Código para comunicación con tecnología de reconocimiento de voz por Google
String mensaje;

//Valriable para dar un consejo aleatorio al niño/a
int randNumber;

//Servo
Servo myServo;

void setup(){

  mySoftwareSerial.begin(9600);
  myVR.begin(9600);
  Serial.begin(115200);

  
  
  //if (Serial1) Serial.println("Serial1 Activado"); else Serial.println("Serial1 NO Activado");
  //Reconocimiento de VOZ
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  pinMode(led, OUTPUT);
  //digitalWrite(led, LOW);
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    //while(1);
  }
  if(myVR.load((uint8_t)Vida) >= 0){
    Serial.println("Vida loaded");
  }
  if(myVR.load((uint8_t)Clima) >= 0){
    Serial.println("Clima loaded");
  }
  //Agregado
  if(myVR.load((uint8_t)Jugar) >= 0){
    Serial.println("Jugar loaded");
  }
  if(myVR.load((uint8_t)Hola) >= 0){
    Serial.println("Hola loaded");
  }
  if(myVR.load((uint8_t)Busca) >= 0){
    Serial.println("Busca loaded");
  }
  if(myVR.load((uint8_t)Otra) >= 0){
    Serial.println("Otra loaded");
  }
  if(myVR.load((uint8_t)Fin) >= 0){
    Serial.println("Fin loaded");
  }
  
  

  //Sensor de Temperatura y humedad
  dht.begin();
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(s)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  //Saludos inicial
  delay(2500);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(2);  //Play the first mp3 1(y), 2(n), 3(y), 4(y)
  delay(5000);

  //Flama
  //pinMode (Led, OUTPUT) ;// define LED as output interface
  pinMode (buttonpin, INPUT) ;// output interface defines the flame sensor
  pinMode (analoog, INPUT) ;// output interface defines the flame sensor

  //OJOS DE ZURI
   /*The MAX72XX is in power-saving mode on startup*/
  lc.shutdown(addrL,false);
  lc.shutdown(addrR,false);
  /* Set the brightness to max values */
  lc.setIntensity(addrL,3);
  lc.setIntensity(addrR,3);
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

  myServo.attach(38);
}

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

  byte right[8] = {
B00000000,
B00111100,
B01111110,
B01001110,
B01001110,
B01111110,
B00111100,
B00000000};

  displayEmotion(left, right);
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

  byte right[8] = {

B00000000,
B00111100,
B01111110,
B01100110,
B01100110,
B01111110,
B00111100,
B00000000};   

displayEmotion(left, right);
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

  byte right[8] = {

B00000000,
B00111100,
B01111110,
B01110010,
B01110010,
B01111110,
B00111100,
B00000000};

displayEmotion(left, right);
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
B00011110,
B00100110,
B01100110,
B01111110,
B00111100,
B00000000};
  byte left[8] = {
B00000000,
B00110000,
B01111000,
B01100100,
B01100110,
B01111110,
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
B01111110,
B01100110,
B01100110,
B01111110,
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


void showNeutral04() {
  byte left[8] = {
B00000000,
B00000000,
B01111110,
B01100110,
B01100110,
B01111110,
B00000000,
B00000000};

  byte right[8] = {

B00000000,
B00000000,
B01111110,
B01100110,
B01100110,
B01111110,
B00000000,
B00000000};  

displayEmotion(left, right);
}

void showNeutral05() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B01100110,
B01100110,
B00000000,
B00000000,
B00000000};

  byte right[8] = {

B00000000,
B00000000,
B00000000,
B01100110,
B01100110,
B00000000,
B00000000,
B00000000}; 

displayEmotion(left, right);
}

void showNeutral06() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000};

  byte right[8] = {

B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000}; 

displayEmotion(left, right);
}

void displayEmotion(byte left[8], byte right[8]) {
  lc.clearDisplay(addrL);
  lc.clearDisplay(addrR);
  for(int row=0;row<8;row++) {
    lc.setRow(addrL,row,left[row]);
    lc.setRow(addrR,row,right[row]);
  }
}

void loop() {

  myServo.write(0);
  myServo.write(45);
  
  //Reconocimiento de VOZ
 int ret;
 ret = myVR.recognize(buf, 50);
 if(ret>0){
    switch(buf[1]){
      case Vida:
        digitalWrite(led, HIGH);
        break;
      case Clima:
        digitalWrite(led, HIGH);
        break;
      case Jugar:
        digitalWrite(led, LOW);
        break;
      case Hola:
        digitalWrite(led, LOW);
        break;
      case Busca:
        digitalWrite(led, LOW);
        break;
      case Otra:
        digitalWrite(led, LOW);
        break;
      case Fin:
        digitalWrite(led, LOW);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    printVR(buf);
  }
  
  //OJOS DE ZURI
  showNeutral01();
  delay(250);
  showNeutral02();
  delay(250);
  showNeutral03();
  delay(250);

  

  /*showNeutral04();
  delay(250);
  showNeutral05();
  delay(250);
  showNeutral06();
  delay(250);

  showNeutral05();
  delay(250);
  showNeutral06();
  delay(250);
  */
  
  static unsigned long timer = millis();
  
  if (millis() - timer > 3000) {
    timer = millis();
    //myDFPlayer.next();  //Play next mp3 every 3 second.
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  //Captura de comando desde smartphone
  while(Serial.available()){
    delay(10);
    char lectura = Serial.read();
    mensaje+=lectura;   
  }

  //Sensor flama
  sensor = analogRead(analoog);
  Serial.println(sensor); // display temperature
  val = digitalRead (buttonpin) ;// digital interface will be assigned a value of 3 to read val
  if (val == HIGH){   // When the flame sensor detects a signal, LED flashes
      //OJOS DE ZURI
      showScared();
      //delay(4000);
      //digitalWrite (Led, HIGH);
      myDFPlayer.volume(30);  
      myDFPlayer.play(9);
      delay(3000);
  }/*
  else{
      digitalWrite (Led, LOW);
  }*/
  delay(1000);

  //Sensor de temperatura y humedad
  float humedad = dht.readHumidity();
  float temp    = dht.readTemperature();
  temp = temp - 10;
  //Serial.print("Temperatura : ");
  //Serial.print(temp);
  if(mensaje.length()>0){
    if(mensaje=="*clima"){
       if(temp <= 17) { 
          myDFPlayer.volume(30);  
          myDFPlayer.play(14);
          delay(3000);
       }
       else if(temp >=18 && temp <= 25 ){
               myDFPlayer.volume(30);  
               myDFPlayer.play(10);
               delay(3000);
       }
       else{
               myDFPlayer.volume(30);  
               myDFPlayer.play(16);
               delay(3000);
        }
    }
  }


  if(mensaje.length()>0){
    if(mensaje=="*Hola Suri"){
       myDFPlayer.volume(30);  
       myDFPlayer.play(13);
       delay(3000);
    }
  }

  //Tip para la vida aleatorio
  randomSeed(millis());    // genera una semilla para aleatorio a partir de la función millis()
  randNumber = random(6);    // genera número aleatorio entre 0 y 6
  if(randNumber == 0) randNumber=24;        //Lavar los dientes 24
  else if(randNumber == 1) randNumber=23;   //Agua natural      23
  else if(randNumber == 2) randNumber=28;  //Alimentación      28 
  else if(randNumber == 3) randNumber=27;  //Ejercicio         27
  else if(randNumber == 4) randNumber=30;  //No te exongas sol 30
  else if(randNumber == 5) randNumber=29;   //Ordena juguetes   29
  else if(randNumber == 6) randNumber=31;   //Haz tarea         31
  if(mensaje.length()>0){
    if(mensaje=="*vida"){
       //OJOS DE ZURI
       showWink();
       //delay(4000);
       myDFPlayer.volume(30);  
       myDFPlayer.play(randNumber);
       delay(3000);
    }
  }


  //Tip para la vida aleatorio
  randomSeed(millis());    // genera una semilla para aleatorio a partir de la función millis()
  randNumber = random(2);    // genera número aleatorio entre 0 y 6
  if(randNumber == 0) randNumber=17;        //Song 17
  else if(randNumber == 1) randNumber=21;   //Song 21
  else if(randNumber == 2) randNumber=22;  //Song 22
  if(mensaje.length()>0){
    if(mensaje=="*música"){
       
       //delay(30000);
       bandera3 = 0;
       while(bandera3 == 0 && mensaje != "*parar"){
          myDFPlayer.volume(30);  
          myDFPlayer.play(randNumber);
          while(Serial.available()){
               delay(10);
               char lectura = Serial.read();
               mensaje+=lectura;   
               
           }
           if(mensaje == "*parar")
              bandera3 = 1;
           mensaje="";
       }
    }
  }


  if(mensaje.length()>0){
    if(mensaje=="*jugar 2"){
       myDFPlayer.volume(30);  
       myDFPlayer.play(35);
       delay(3000);
       bandera = 0;
       //while(mensaje != "*fin" ){
       while(bandera == 0 ){
          while(Serial.available()){
               delay(10);
               char lectura = Serial.read();
               mensaje+=lectura;   
               
           }
           if(mensaje == "*fin")
              bandera = 1;
           mensaje="";
       }

       myDFPlayer.volume(30);  
       myDFPlayer.play(11);
       delay(3000);
       
    }
  }

  
  if(mensaje.length()>0){
    if(mensaje=="*deber"){
       myDFPlayer.volume(30);  
       myDFPlayer.play(4);
       delay(3000);
    }
    
  }  


//**********************************************
 if(mensaje.length()>0){
        if(mensaje=="*jugar uno"){
             myDFPlayer.volume(30);  
             myDFPlayer.play(32);
             delay(3000);
             bandera1 = 0;
             while(bandera1 == 0 ){
                    
                          while(Serial.available()){
                            delay(10);
                            char lectura = Serial.read();
                            mensaje+=lectura;
                          }
                          if(mensaje.length()>0){
                             if(mensaje == "*busca"){
                                    myDFPlayer.volume(30);  
                                    myDFPlayer.play(2);
                                    delay(12000);
                                    bandera2 = 0;
                                    while(bandera2 == 0){
                                           while(Serial.available()){
                                               delay(10);
                                               char lectura = Serial.read();
                                               mensaje+=lectura;  
                                            }
                                            //*
                                            if(mensaje.length()>0){ 
                                               if(mensaje == "*listo Suri"){
                                                  myDFPlayer.volume(30);  
                                                  myDFPlayer.play(1);
                                                  delay(3000);

                                                }
                                            }
                                            if(mensaje.length()>0){ 
                                               if(mensaje == "*9"){
                                                  myDFPlayer.volume(30);  
                                                  myDFPlayer.play(4);
                                                  delay(3000);
                                                  bandera2 = 1;
                                                  bandera1 = 1;
                                               }
                                             }
                                             if(mensaje.length()<=3 && mensaje.length()>=1){
                                               if(mensaje != "*9"){
                                                  myDFPlayer.volume(30);  
                                                  myDFPlayer.play(15);
                                                  delay(3000);
                                               }
                                            }   
                                      
                                      mensaje="";
                                   }
                                 
                              }
                              mensaje=""; 
                           }
                          
                     
                      
             }
            
           mensaje="";  
        }
  }
 //***********************************************/
  
 
 mensaje="";

 
 
 
 //OJOS DE ZURI
 showNeutral03();
 delay(250);
 showNeutral02();
 delay(250);
 showNeutral01();
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
