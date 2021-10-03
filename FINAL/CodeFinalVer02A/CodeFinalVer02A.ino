/*********************************************************************************
         D       R       Ô       L       E        V   E   R         0    2
 *********************************************************************************/
 

#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

//Reconocimiento de VOZ
VR myVR(0,1);    // 3/13:RX 2/12:TX, you can choose your favourite pins.
uint8_t records[7]; // save record
uint8_t buf[64];
int led = 4;
#define Encender    (0)
#define Apagar   (1) 
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


/*Sensor de temperatura y humedad
int SENSORTH = 9;   //Cambió de 2 a 5
int temp, humedad;
DHT dht (SENSORTH, DHT11); */

/*Sensor de llama
//int Led = 13 ;// define LED Interface
int buttonpin = 8; // define the flame sensor interface - Cambió de 3 a 6
int analoog = A3; // define the flame sensor interface
int val ;// define numeric variables val 
float sensor; //read analoog value */




//Código para comunicación con tecnología de reconocimiento de voz por Google
String mensaje;

//Valriable para dar un consejo aleatorio al niño/a
int randNumber;

//Servo
//Servo myServo;

void setup(){

  //mySoftwareSerial.begin(9600);
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
  if(myVR.load((uint8_t)Encender) >= 0){
    Serial.println("Encender loaded");
  }
  if(myVR.load((uint8_t)Apagar) >= 0){
    Serial.println("Apagar loaded");
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
  //dht.begin();
 
  
  

  /*Flama
  //pinMode (Led, OUTPUT) ;// define LED as output interface
  pinMode (buttonpin, INPUT) ;// output interface defines the flame sensor
  pinMode (analoog, INPUT) ;// output interface defines the flame sensor
  */
 

  //myServo.attach(38);
}



void loop() {

  //myServo.write(0);
  //myServo.write(45);
  
  //Reconocimiento de VOZ
 int ret;
 ret = myVR.recognize(buf, 50);
 if(ret>0){
    switch(buf[1]){
      case Encender:
        digitalWrite(led, HIGH);
        break;
      case Apagar:
        digitalWrite(led, LOW);
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
  
  

  /*Sensor flama
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
  
  //delay(1000);

  /*Sensor de temperatura y humedad
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
  */

  
  
}
