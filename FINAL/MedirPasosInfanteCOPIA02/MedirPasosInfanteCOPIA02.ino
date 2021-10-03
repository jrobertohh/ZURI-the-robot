#include <ESP8266WiFi.h>


/* VERSIÓN ESTABLE*/

//Sensor Shock
int SENSOR=D0;
int CONT=0;
int BAND=0;

#define LED D4

//Configuración WiFi
const char* ssid = "iPhone"; // ApWorkersCcm   "iPhone"; "e84d76"; "iPhone de Zaira";
const char* password = "12345678"; //"12345678"; "0qgiwz43"; "272627885"; "ceconexpo" "1tjzfogugwix6";
const char* host = "jrobertohhresearch.tech";    //"droleunla.000webhostapp.com";  Dirección IPv4 */


void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  //Sensor Shock
  pinMode(SENSOR, INPUT);

  pinMode(LED, OUTPUT);
}

void loop() {
     digitalWrite(LED, HIGH);
     delay(2500);
     while(BAND == 0 && CONT < 20){
        if(digitalRead(SENSOR)==LOW){
           CONT+=2;
           Serial.print(CONT);
           delay(500);
        }
        if(CONT==19){
           BAND==1;
        }
        
      }
   if(CONT==20){
    Serial.print("connecting to ");
    Serial.println(host);

    // Usar la clase WiFiClient para crear una conexiòn TCP
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
       Serial.println("connection failed");
       return;
    }
    
    // We now create a URL for the request
    String url = "/drole/guardar.php";
    String dato1 = "?cant=";
    String dato2 = "&m_zuri=";
    String m_zuri="mz000001";
    Serial.print("Requesting URL: ");
    Serial.println(url);

    

    
    //Insertar registro en BD
    client.print(String("GET ") + url + dato1 + CONT + dato2 + m_zuri + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
    unsigned long timeout = millis();
    while (client.available() == 0) { //Devuelve la cantidad de datos que se han escrito por el cliente para el servidor al que está conectado
          if (millis() - timeout > 5000) {
             Serial.println(">>> Client Timeout !");
             client.stop();
             return;
          }
    }
    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
        String line = client.readStringUntil('\r'); //El carácter de retorno de carro
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
    delay(100); 
   }
   CONT = 50;   
}
