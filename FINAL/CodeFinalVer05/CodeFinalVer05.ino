/*********************************************************************************
         D       R       Ô       L       E        V   E   R         0    1
 *********************************************************************************/
#include <SdFatConfig.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SPI.h>   //Biblioteca para la comunicación vía SPI.
#include <SdFat.h>        // Biblioteca para acceder a la tarjeta SD (necesaria para la biblioteca vs1053_SdFat)
#include <vs1053_SdFat.h> // Biblioteca para reproducir archivos MP3

//Comunicación I2C
#include <Wire.h>
int eyes = 0;
int estado = 0;

#include <Adafruit_Sensor.h> 
#include "DHT.h"

#include <Servo.h>
Servo servo1;

SdFat sd;         //Generar el objeto para la tarjeta SD
vs1053 MP3player; //Crear el objeto reproductor multimedia
 
// Definiendo la pausa entre la reproducción de canciones.
//const int PAUSE = 2000; 

//Sensor de temperatura y humedad
int SENSORTH = 5;   //Cambió de 2 a 5
int temp, humedad;
DHT dht (SENSORTH, DHT11);

//Sensor de llama
int Led = 13 ;// define LED Interface
int buttonpin = 4; // define the flame sensor interface - Cambió de 3 a 6
int analoog = A3; // define the flame sensor interface
int val; // define numeric variables val 
float sensor; //read analoog value

//Bluetooth
int bandera=0;
int bandera1=0;
int bandera2=0;
int bandera3=0;
int idioma = 1;
int temp2 = 0;

//Código para comunicación con tecnología de reconocimiento de voz por Google
String mensaje;

//Valriable para dar un consejo aleatorio al niño/a
int randNumber;

void setup() {
  
  Serial.begin(9600);

  // Unimos este dispositivo al bus I2C
  Wire.begin();
  
  // Iniciar la comunicación con la tarjeta SD. Si ocurre un error, detente aquí
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED))
    sd.initErrorHalt();
 
// Cambiar al directorio raíz de la tarjeta SD. Si ocurre un error, se muestra el mensaje de error "sd.chdir".
  if (!sd.chdir("/"))
      sd.errorHalt("sd.chdir");
  
  //MP3 Shield starten
  MP3player.begin();
 
  // Establece el volumen. El primer valor es para el canal de audio izquierdo. El segundo para el canal de audio correcto
  MP3player.setVolume(15,15); //Ajustar el volumen
  MP3player.playTrack(1);
  

  //Sensor de Temperatura y humedad
  dht.begin();

  //Flama
  pinMode (Led, OUTPUT) ;// define LED as output interface
  pinMode (buttonpin, INPUT) ;// output interface defines the flame sensor
  pinMode (analoog, INPUT) ;// output interface defines the flame sensor

  servo1.attach(10);

}
 
void loop() {
  // Inicia el temporizador para reproducir las canciones.
  MP3player.available();

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
      //showScared();
      //delay(4000);

      // Comenzamos la transmisión al dispositivo 1s
      eyes = 1;
      Wire.beginTransmission(1);
      // Enviamos un byte, será el pin a encender
      Wire.write(eyes);
      // Enviamos un byte, L pondrá en estado bajo y H en estado alto
      Wire.write(estado);
      // Paramos la transmisión
      Wire.endTransmission();
      // Esperamos 1 segundo
      delay(1000);
      
      digitalWrite (Led, HIGH);
      if(idioma == 1){
        MP3player.setVolume(15,15);
        MP3player.playTrack(24);   //Verificar si no se tiene que especificar el volúmen cada que se reproduzac una pista
        delay(3000);
      }else if(idioma == 2){ 
        MP3player.setVolume(15,15);
        MP3player.playTrack(46);   //Verificar si no se tiene que especificar el volúmen cada que se reproduzac una pista
        delay(3000);
      }
  }
  else{
      digitalWrite (Led, LOW);
  }
  delay(1000);
  eyes = 0;
  
  //Sensor de temperatura y humedad
  float humedad = dht.readHumidity();
  float temp    = dht.readTemperature();
  temp = temp - 6;
  Serial.print("Temperatura : ");
  Serial.print(temp);
  if(mensaje.length()>0){
    if(mensaje=="*clima"){
       if(temp <= 10) { 
          MP3player.setVolume(15,15);
          MP3player.playTrack(21); 
          delay(3000);
       }
       else if(temp >10 && temp <= 25 ){
               MP3player.setVolume(15,15);
               MP3player.playTrack(25);           
               delay(3000);
       }
       else{
               MP3player.setVolume(15,15);
               MP3player.playTrack(23); 
               delay(3000);
        }
    }
    else{ if(mensaje=="*weather"){
       if(temp <= 10) { 
          MP3player.setVolume(15,15);
          MP3player.playTrack(43); 
          delay(3000);
       }
       else if(temp >10 && temp <= 25 ){
               MP3player.setVolume(15,15);
               MP3player.playTrack(44);           
               delay(3000);
       }
       else{
               MP3player.setVolume(15,15);
               MP3player.playTrack(45); 
               delay(3000);
        }
    }}
  }

   
     
       
      

  if(mensaje.length()>0){
    if(mensaje=="*Hola Suri"){
       MP3player.playTrack(2);
       delay(3000);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(35);
       delay(100);
       servo1.write(40);
       delay(100);
       servo1.write(180);
       delay(100);
    }else if(mensaje=="*Hello"){
       MP3player.playTrack(30);
       delay(3000);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(45);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(20);
       delay(100);
       servo1.write(25);
       delay(100);
       servo1.write(30);
       delay(100);
       servo1.write(35);
       delay(100);
       servo1.write(40);
       delay(100);
       servo1.write(180);
       delay(100);
    }
  }


  //*************************************COMANDO IDIOMA
  if(mensaje.length()>0){
    if(mensaje=="*idioma" && idioma == 1){
       MP3player.playTrack(50);
       delay(3000);
       temp2 = 0;
       //***********************
       while(temp2 == 0){
            while(Serial.available()){
              delay(10);
              char lectura = Serial.read();
              mensaje+=lectura;  
            }
           //*
           if(mensaje.length()>0){ 
               if(mensaje == "*1"){
                  MP3player.setVolume(15,15);
                  MP3player.playTrack(49);
                  delay(3000);
                  idioma = 1;
                  temp2 = 1;
               }else if(mensaje == "*2"){
                  MP3player.setVolume(15,15);
                  MP3player.playTrack(49);
                  delay(3000);
                  idioma = 2;
                  temp2 = 1;
               }
           }
        }mensaje="";
      //mensaje="";  
    }
  }


  //*************************************COMANDO IDIOMA
  if(mensaje.length()>0){
    if(mensaje=="*Language"){
       MP3player.setVolume(15,15);
       MP3player.playTrack(47);
       delay(3000);
       temp2 = 0;
       //***********************
       while(temp2 == 0){
            while(Serial.available()){
            delay(10);
            char lectura = Serial.read();
            mensaje+=lectura;  
            }
           //*
           if(mensaje.length()>0){ 
               if(mensaje == "*1"){
                  MP3player.setVolume(15,15);
                  MP3player.playTrack(49);
                  delay(3000);
                  idioma = 1;
                  temp2 = 1;
               }else if(mensaje == "*2"){
                  MP3player.setVolume(15,15);
                  MP3player.playTrack(49);
                  delay(3000);
                  idioma = 2;
                  temp2 = 1;
               }
           }
        }
    }
  }
  

  
  //Tip para la vida aleatorio
  randomSeed(millis());    // genera una semilla para aleatorio a partir de la función millis()
  randNumber = random(6);    // genera número aleatorio entre 0 y 6
  if(idioma == 1){
  if(randNumber == 0) randNumber=8;        //Lavar los dientes 8
  else if(randNumber == 1) randNumber=9;   //Agua natural      23
  else if(randNumber == 2) randNumber=10;  //Alimentación      28 
  else if(randNumber == 3) randNumber=11;  //Ejercicio         27
  else if(randNumber == 4) randNumber=12;  //No te exongas sol 30
  else if(randNumber == 5) randNumber=13;   //Ordena juguetes   29
  else if(randNumber == 6) randNumber=14;   //Haz tarea         31
  if(mensaje.length()>0){
    if(mensaje=="*vida"){
       //OJOS DE ZURI
       //showWink();
       //delay(4000);
       MP3player.setVolume(15,15);
       MP3player.playTrack(randNumber);
       delay(3000);
    }
  }}
  if(idioma == 2){
  if(randNumber == 0) randNumber=31;        //Lavar los dientes 8
  else if(randNumber == 1) randNumber=32;   //Agua natural      23
  else if(randNumber == 2) randNumber=33;  //Alimentación      28 
  else if(randNumber == 3) randNumber=34;  //Ejercicio         27
  else if(randNumber == 4) randNumber=35;  //No te exongas sol 30
  else if(randNumber == 5) randNumber=36;   //Ordena juguetes   29
  else if(randNumber == 6) randNumber=37;   //Haz tarea         31
  }
  if(mensaje.length()>0){
    if(mensaje=="*advice"){
       //OJOS DE ZURI
       //showWink();
       //delay(4000);
       MP3player.setVolume(15,15);
       MP3player.playTrack(randNumber);
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
          MP3player.setVolume(15,15);
          MP3player.playTrack(randNumber);
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
       MP3player.setVolume(15,15);
       MP3player.playTrack(35);
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
       MP3player.playTrack(11);
       delay(3000);
    }
  }
  


  if(mensaje.length()>0){
    if(mensaje=="*deber"){
       MP3player.playTrack(4);
       delay(3000);
    }
    
  } 

  //**********************************************ACTIVIDAD EN ESPAÑOL
 if(mensaje.length()>0){
        if(mensaje=="*jugar"){
             MP3player.setVolume(15,15);
             MP3player.playTrack(15);
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
                                    MP3player.setVolume(15,15);
                                    MP3player.playTrack(17);
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
                                                  MP3player.setVolume(15,15);
                                                  MP3player.playTrack(16);
                                                  delay(3000);

                                                }
                                            }
                                            if(mensaje.length()>0){ 
                                               if(mensaje == "*9"){
                                                  MP3player.setVolume(15,15);
                                                  MP3player.playTrack(18);
                                                  delay(3000);
                                                  bandera2 = 1;
                                                  bandera1 = 1;
                                               }
                                             }
                                             if(mensaje.length()<=3 && mensaje.length()>=1){
                                               if(mensaje != "*9"){
                                                  MP3player.setVolume(15,15);
                                                  MP3player.playTrack(22);
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


   //**********************************************ACTIVIDAD EN INGLÉS
  if(mensaje.length()>0){
        if(mensaje=="*Play"){
             MP3player.setVolume(15,15);
             MP3player.playTrack(38);
             delay(3000);
             bandera1 = 0;
             while(bandera1 == 0 ){
                    
                          while(Serial.available()){
                            delay(10);
                            char lectura = Serial.read();
                            mensaje+=lectura;
                          }
                          if(mensaje.length()>0){
                             if(mensaje == "*search"){
                                    MP3player.setVolume(15,15);
                                    MP3player.playTrack(39);
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
                                               if(mensaje == "*Finish"){
                                                  MP3player.setVolume(15,15);
                                                  MP3player.playTrack(40);
                                                  delay(3000);

                                                }
                                            }
                                            if(mensaje.length()>0){ 
                                               if(mensaje == "*Nine"){
                                                  MP3player.setVolume(15,15);
                                                  MP3player.playTrack(42);
                                                  delay(3000);
                                                  bandera2 = 1;
                                                  bandera1 = 1;
                                               }
                                             }
                                             if(mensaje.length()<=3 && mensaje.length()>=1){
                                               if(mensaje != "*Nine"){
                                                  MP3player.setVolume(15,15);
                                                  MP3player.playTrack(41);
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
}
