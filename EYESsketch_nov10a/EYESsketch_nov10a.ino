#include "LedControl.h"
#include <Wire.h>


/*
 pin 2/12/6 is connected to the DataIn 
 pin 4/11/8 is connected to the CLK 
 pin 3/10/7 is connected to LOAD 
 ***** Please set the number of devices you have *****
 */
LedControl lc=LedControl(10,12,11,2); //Naranja (10), Amarillo (11), Verde (12)

const int addrL = 0;  // first LED matrix - Left robot eye
const int addrR = 1;  // second LED matrix - Right robot eye

void setup() {
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


  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  
}

void showNeutral01() {
  byte left[8] = {
B00000000,
B00111100,
B01001110,
B01001110,
B01111110,
B01111110,
B00111100,
B00000000};
 displayEmotion(left, left);
}

void showNeutral02() {
  byte left[8] = {
B00000000,
B00011110,
B10111111,
B00000000,
B10110011,
B10110011,
B00011110,
B00000000};

 byte right[8] = {
B00011110,
B00111111,
B01111111,
B00000000,
B01100011,
B01100011,
B00111111,
B00011110};

displayEmotion(left, right);
}

void showNeutral03() {
  byte left[8] = {
B00000000,
B00111100,
B01110010,
B01110010,
B01111110,
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

void NewNeutral() {
  byte right[8] = {
B00000000,
B00011110,
B00110011,
B01100001,
B01100001,
B00110011,
B00011110,
B00000000};
  byte left[8] = {
B00000000,
B00011000,
B00111100,
B11100110,
B11100110,
B00111100,
B00011000,
B00000000};

  displayEmotion(left, right);
}

void showNew01() {
  byte left[8] = {
/*B00000000,
B00011110,
B10111111,
B00000000,
B10110011,
B10110011,
B00011110,
B00000000}; */

B00000000,
B00011110,
B10110001,
B10110101,
B00000100,
B10111111,
B00011110,
B00000000};

 byte right[8] = {
/*B00011110,
B00111111,
B01111111,
B00000000,
B01100011,
B01100011,
B00111111,
B00011110};*/

B00011110,
B00111111,
B01100011,
B01101011,
B00001000,
B01111111,
B00111111,
B00011110};

displayEmotion(left, right);
}

void showNew02() {
  byte left[8] = {
B00000000,
B00000000,
B00011110,
B10111111,
B00110011,
B10110011,
B00011110,
B00000000};

 byte right[8] = {
B00000000,
B00011110,
B00111111,
B01111111,
B01100011,
B01100011,
B00111111,
B00000000};

displayEmotion(left, right);
}


void showNew03() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B10111111,
B00110011,
B00011110,
B00000000,
B00000000};

 byte right[8] = {
B00000000,
B00000000,
B00011110,
B01111111,
B01100011,
B00111110,
B00000000,
B00000000};

displayEmotion(left, right);
}

void showNew04() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B10111111,
B00011110,
B00000000,
B00000000,
B00000000};

 byte right[8] = {
B00000000,
B00000000,
B00000000,
B01111111,
B00111110,
B00000000,
B00000000,
B00000000};

displayEmotion(left, right);
}

void showNew05() {
  byte left[8] = {
B00000000,
B00000000,
B00000000,
B10111111,
B00000000,
B00000000,
B00000000,
B00000000};

 byte right[8] = {
B00000000,
B00000000,
B00000000,
B01111111,
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


   showNew01();
   delay(3000);
   showNew02();
   delay(50);
   showNew03();
   delay(5);
   showNew04();
   delay(5);
   showNew05();
   delay(50);

   showNew04();
   delay(5);
   showNew03();
   delay(5);
   showNew02();
   delay(5);
  /*/OJOS DE ZURI
  showNeutral01();
  delay(250);
  showNeutral02();
  delay(250);
  showNeutral03();
  delay(250);

  //OJOS DE ZURI
 showNeutral03();
 delay(250);
 showNeutral02();
 delay(250);
 showNeutral01();
 delay(250);*/
  
  /*showNeutral();
  delay(4000);
  showAnger();
  delay(4000);
  showSadness();
  delay(4000);
  showWink();
  delay(4000);
  showLove();
  delay(4000);
  showScared();
  delay(4000);
  showHappy();
  delay(4000);*/
}

void receiveEvent(int howMany) {
 
  int pinOut = 0;
  int estado = 0;
 
  // Si hay dos bytes disponibles
  if (Wire.available() == 2){
    // Leemos el primero que será el pin
    pinOut = Wire.read();
    switch (pinOut) {
      case 0:
        showWink();
        delay(4000);
        break;
      
      case 1:
        showWink();
        delay(4000);
        break;
        
      case 2:
        showWink();
        delay(4000);
        break;
        
      case 3:
        showWink();
        delay(4000);
        break;

      case 4:
        showWink();
        delay(4000);
        break;

      case 5:
        showWink();
        delay(4000);
        break;

      case 6:
        showWink();
        delay(4000);
        break;

      case 7:
        showWink();
        delay(4000);
        break;

      case 8:
        showWink();
        delay(4000);
        break;
      
      default:
        Serial.println("Ninguna");
        break;
    }
  }
  // Si hay un byte disponible
  if (Wire.available() == 1){
    estado = Wire.read();
    switch (estado) {
        case 0:
        showWink();
        delay(4000);
        break;
      
      case 1:
        showWink();
        delay(4000);
        break;
        
      case 2:
        showWink();
        delay(4000);
        break;
        
      case 3:
        showWink();
        delay(4000);
        break;

      case 4:
        showWink();
        delay(4000);
        break;

      case 5:
        showWink();
        delay(4000);
        break;

      case 6:
        showWink();
        delay(4000);
        break;

      case 7:
        showWink();
        delay(4000);
        break;

      case 8:
        showWink();
        delay(4000);
        break;
      
      default:
        Serial.println("Ninguna");
        break;
    }
  }
  // Activamos/desactivamos salida
  //digitalWrite(pinOut,estado);
  
}
