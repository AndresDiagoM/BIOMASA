#include <WiFi.h>
//#include "DHT.h"
#include <DHT.h>  
#define DHTPIN 4     // El número que se le debe asignar a DHTPIN debe ser el número del pin GPIO de la tarjeta ESP32 que se utilice para conectar el sensor DHT22.
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#include <TinyGPS++.h>                                  // Libreria Tiny GPS Plus para el uso del GPS Ublox Neo 6M
//#include <SoftwareSerial.h>                              // Librería Software Serial para utilizar otros pines como conexión serial, para comunicación con el GPS.
#include <Adafruit_ssd1306syp.h>                        // Librería de Adafruit para el display OLED
#include "Wifi.h"

#include <Q2HX711.h>
Q2HX711 MPS20N0040D(2, 18); // Pines conexión //Pin SCK: pin 3  (2,3)...Pin OUT: pin 2

DHT dht(DHTPIN, DHTTYPE);

char* ssid     = "Matta";
char* password = "DV3062021T2."; 

const char* host = "autolifegas.online";  // Dirección IP local o remota
const int   port = 80;            // Puerto, HTTP es 80 por defecto
const int   watchdog = 5000;        // Frecuencia del Watchdog
unsigned long previousMillis = millis(); 
unsigned long currentMillis;

int ID_TARJ=1;
String dato1;
String cade1;
String dato;
String cade;
String hum_max;
String temp_max;
String hum_min;
String rele_nube;
String temp_min;
String line1;
String line;
float SW;
String SWS;
float t_max, t_min;
float h_max, h_min;
float h,t;
int sensorWater1=0; 
int estado_conexion=0; 
int estado_activo=0;
int rele_medido=0; 

const int gas = 34; //pin sensor gas
const int rele = 35; //pin rele
const int led_conexion = 23; //pin indicador conexion
const int gpio5_pin=5; //pin de conexión
const int presion=2;

Adafruit_ssd1306syp display(21,22);   // (4, 5) // Definición de pines del Display OLED (SDA a Pin 4-18), (SCL a Pin 5-19)

//static const int RXPin = 3, TXPin = 1;                // Definición de pines del GPS 1 RX y 3 TX
//static const uint32_t GPSBaud = 9600;                   // Tasa de transmisión por defecto del Ublox GPS: 9600

//const double Home_LAT = 2.50378;                      // Aquí pueden definir la LATITUD de la ubicación de un SITIO, el programa cálcula la distancia en Km al sitio detectado por el GPS. Importante que usen al menos 5 decimales para que el calculo sea lo más aproximado posible.
//const double Home_LNG = -76.5792;                     // Aquí pueden definir la LONGITUD de la ubicación de un SITIO, el programa cálcula la distancia en Km al sitio detectado por el GPS. Importante que usen al menos 5 decimales para que el calculo sea lo más aproximado posible.

TinyGPSPlus gps;                                        // Crea una instancia del objeto TinyGPS++ que se denomina gps
//SoftwareSerial ss(RXPin, TXPin);                        // Determina la conexión serial con el GPS en los pines ya definidos, 12 y 13.


void setup() {
  Wifi();
  //Serial.begin(115200);
  pinMode(gas, INPUT); //entrada sensor de gas 
  pinMode(rele, OUTPUT);  // pin del rele,
  pinMode(led_conexion, OUTPUT);
  pinMode(gpio5_pin, OUTPUT);
  
  display.initialize();                                 // Inicializa el display OLED 
  display.clear();                                      // Borra lo presentado en el display OLED
  display.setTextSize(1);                               // Fija el tamaño del texto en el display OLED 
  display.setTextColor(WHITE);                          // Fija el color del texto
  display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
  display.println("Localizacion por GPS");  
  display.println(TinyGPSPlus::libraryVersion());
  display.update();                                     // Actualiza el display, para que lo que se ha enviado hasta el momento, después de borrar, se presente en pantalla.
  delay(2500);                                          // Pausa de 2,5 segundos, para poder ver lo presentado en pantalla  
  display.clear();                                      // Borra lo presentado en el display OLED
  display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
  display.println("Conectando a...");  
  display.println(ssid);
  display.update();                                     // Actualiza el display, para que lo que se ha enviado hasta el momento, después de borrar, se presente en pantalla.
  Serial.print("Conectando a...");
  Serial.println(ssid);
  delay(1000);

  //Wifi();
  
  String sid=WiFi.SSID();
  String pas=WiFi.psk();
  char Buf[50];
  sid.toCharArray(Buf, 50);
  char Buf2[50];
  pas.toCharArray(Buf2, 50); 
  //Serial.print();
  WiFi.begin(Buf, Buf2);
  //WiFi.begin(ssid, password);
  
  /*display.clear();                                      // Borra lo presentado en el display OLED
  display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
  display.print("Esperando conexion WiFi");
  display.update();*/
  
  /*while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    Serial.print(".");
    display.print(".");
    display.update(); 
    delay(500);
    Serial.print(".");
  }*/

  display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
  display.clear();                                      // Borra lo presentado en el display OLED
  display.println("");
  display.println("WiFi conectado");  
  display.println("Direccion IP: ");
  display.println(ip);                      // Envía al display la IP asignada
  display.update();                                     // Actualiza el display, para que lo que se ha enviado hasta el momento, se presente en pantalla.
  delay(2000);                                        // Pausa de 2 segundos, para poder ver lo presentado en pantalla

  
  Serial.println("");
  Serial.println("WiFi conectado");  
  Serial.println("Dirección IP: ");
  Serial.println(ip); //WiFi.localIP();
  dht.begin();
  //ss.begin(GPSBaud);
}
 
void loop() {
  
  currentMillis = millis();
  //GPS();  

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = 40;//dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = 35;//dht.readTemperature();
  //Leer sensor gas, rele, presion
  float gas_medido= 1000;//analogRead(gas);
    rele_medido=analogRead(rele);  //if..read>1000 or read<1000
      if(rele_medido>1000){
        rele_medido=1;
      }else{
        rele_medido=0;
      }
  //float presion_medido=1000;//digitalRead(presion);
  float presion_medido = 0.0;
  presion_medido = MPS20N0040D.read(); // Lectura valor del sensor
  //Serial.println(valor,0);
  
  /*sensorWater1= analogRead(sensorWater);
  Serial.print("************Sensor Watter: ");
  Serial.println(sensorWater1);
  SW = sensorWater1; */

  Serial.print("\nHumidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  Serial.print("gas: ");
  Serial.print(gas_medido); 
  Serial.print(" Rele: ");
  Serial.print(rele_medido); 
  Serial.print(" Presion: ");
  Serial.print(presion_medido);
  delay(500);
  imprimirLCD( rele_medido,  gas_medido,  t,  h,  presion_medido, ID_TARJ);

// Primero se consultan los datos maximos de temp y hum
  currentMillis = millis();
  consultarBaseDatos("/biomasa/php/programa5.php?");  
  delay(2000);
  
// Ahora se guardan los valores medidos en la base de datos

   currentMillis = millis();
   if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;
    WiFiClient client;
  
    if (!client.connect(host, port)) {
      Serial.println(" Conexión falló...(programa1.php)");
      return;
    }
                    // /biomasa/php/programa11.php?humedad=
    String url2 = "/biomasa/php/programa1.php?humedad=";
    url2 += h;
    url2 += "&temperatura=";
    url2 += t;
    url2 += "&ID_TARJ=";
    url2 += ID_TARJ;
    url2 += "&gas=";
    url2 += gas_medido;
    url2 += "&rele=";
    url2 += rele_medido;    
    url2 += "&presion=";
    url2 += presion_medido;
    //url2 += ".";
    //url2 += lat_decimal;
    //url2 += "&longitud=";
    //url2 += lon_entero;
    //url2 += ".";
    //url2 += lon_decimal;
    //url2 += "&velocidad=";
    //url2 += gps.speed.kmph();
    //url2 += "&altitud=";
    //url2 += gps.altitude.meters();
    //url2 += "&conexion=";
    //url2 += estado_conexion;


    if(!isnan(t) and !isnan(h) ){//and  gps.altitude.meters()>0
      Serial.println("   (t,h)!=NAN");
      
      // Envío de la solicitud al Servidor
      client.print(String("POST ") + url2 + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");             
      unsigned long timeout2 = millis();    
      while (client.available() == 0) {
        if (millis() - timeout2 > 5000) {
          Serial.println(">>> Superado tiempo de espera!");
          client.stop();
          return;
        }
      }    
  
    // Lee respuesta del servidor
    while(client.available()){
      line = client.readStringUntil('\r');
      //Serial.println("");
      Serial.println(line);
    }

      digitalWrite(gpio5_pin, HIGH);   
      Serial.println("DATO ENVIADO (programa1.php)");
      display.clear();                                      // Borra lo presentado en el display OLED
      display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
      display.print("DATO ENVIADO...");
      display.update();
      delay(2000);
      
    }else{
      digitalWrite(gpio5_pin, LOW);
      Serial.println("DATO NO ENVIADO (programa1.php)");
      display.clear();                                      // Borra lo presentado en el display OLED
      display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
      display.print("DATO NO ENVIADO...");
      display.update();
      delay(2000);
    }
    
   }
}

//----------FUNCIONES------------//
void consultarBaseDatos(String url){
  if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;
    WiFiClient client;
  
    if (!client.connect(host, port)) {
      Serial.println("Conexión falló...");
      //display.setCursor(0,0);                            // Si hay error en la conexión con el servidor web, se presenta un error de conexión fallida.
      //display.clear();                                      
      //isplay.println("Conex. fallida al servidor...");
      //isplay.update();
      digitalWrite(led_conexion,LOW);// LED DE CONEXION                                     
      delay(1000); 
      estado_conexion = 0; // no hay conexion 
      
      return;
    }else{
      //display.println("Hay conexion al sistema");
      digitalWrite(led_conexion,HIGH); // LED DE CONEXION, digitalWrite(led_conexion,HIGH)gpio5_pin
      estado_conexion = 1; // hay conexion
      
      }

      String url3 = "/biomasa/php/programa5.php?ID_TARJ=";
    url3 += ID_TARJ;
      // Envío de la solicitud al Servidor
      client.print(String("POST ") + url3 + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");             
      unsigned long timeout2 = millis();    
      while (client.available() == 0) {
        if (millis() - timeout2 > 5000) {
          Serial.println(">>> Superado tiempo de espera!");
          client.stop();
          return;
        }
               // "/biomasa/php/programa5.php?"
    //String url = "/biomasa/php/programa5.php?";
    /*client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Superado tiempo de espera!");
        return;
      }*/
    }
    
    // Lee respuesta del servidor
    while(client.available()){
          line = client.readString();
          line.trim();
      }
      //Serial.print("\n ");
      //Serial.print(line);
      int longitud = line.length();
      int longitud_f = longitud;
      longitud = longitud - 9;
      
      dato = line.substring(longitud,longitud_f);
      cade = "\nDato recibido es...";
      cade += dato; 
      cade += "---->";
      Serial.print(cade);

      hum_max = dato.substring(2,4);
      temp_max = dato.substring(0,2);
      hum_min = dato.substring(6,8);
      temp_min = dato.substring(4,6);
      rele_nube=dato.substring(8,9);
            
      // Lo siguiente se utiliza para pasar la cadena de texto a un flotante, para poder comparar
      char cadena[temp_max.length()+1];
      temp_max.toCharArray(cadena, temp_max.length()+1);
      t_max = atof(cadena);
      
      // Lo siguiente se utiliza para pasar la cadena de texto a un flotante, para poder comparar
      char cadena2[hum_max.length()+1];
      hum_max.toCharArray(cadena2, hum_max.length()+1);
      h_max = atof(cadena2);

      // Lo siguiente se utiliza para pasar la cadena de texto a un flotante, para poder comparar
      char cadena3[temp_min.length()+1];
      temp_min.toCharArray(cadena3, temp_min.length()+1);
      t_min = atof(cadena3);
      
      // Lo siguiente se utiliza para pasar la cadena de texto a un flotante, para poder comparar
      char cadena4[hum_min.length()+1];
      hum_min.toCharArray(cadena4, hum_min.length()+1);
      h_min = atof(cadena4);

      // Lo siguiente se utiliza para pasar la cadena de texto a un flotante, para poder comparar
      char cadena5[rele_nube.length()+1];
      rele_nube.toCharArray(cadena5, rele_nube.length()+1);
      //rele_medido = atoi(cadena5);

      cade = "Temp max: ";
      cade += t_max;
      Serial.print(cade);
      
      cade = " Humedad max:";
      cade += h_max;
      Serial.println(cade);

      cade = "Temp min:";
      cade += t_min;
      Serial.print(cade);
      
      cade = " Humedad min: ";
      cade += h_min;
      Serial.println(cade);

      cade = " Rele nube: ";
      cade += rele_nube;
      Serial.println(cade);
      
      int aux = rele_nube.toInt();
      if(aux==1 && rele_medido==0){
         digitalWrite(rele, HIGH);
         delay(10000);
         digitalWrite(rele, LOW);
         Serial.print("\n HOSTINGER(1,0):");
         Serial.print(rele_nube);
      }else if(aux==0 && rele_medido==1){
        digitalWrite(rele, LOW);
        Serial.print("\n HOSTINGER(0,1):");
        Serial.print(rele_nube);
      }else if(aux==1 && rele_medido==1){
        delay(5000);
        digitalWrite(rele, LOW);
        Serial.print("\n HOSTINGER(1,1):");
        Serial.print(rele_nube);
      }else{
        digitalWrite(rele, LOW);
        Serial.print("\n HOSTINGER(else):");
      }
      rele_medido=0;
      
      display.clear();                                      // Borra lo presentado en el display OLED
      display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
      display.println("ALERTAS:");
      if (t>t_max or t<t_min)
      {
         Serial.println("ALERTA TEMPERATURA MAX/MIN");
         //digitalWrite(gpio15_pin, HIGH);     
              if(t>t_max){
                display.println("Temp MAX:");        
              }else{
                display.println("Temp MIN:");        
              }
         display.print(" T:");
         display.println(t,5);
         display.update();
         delay(2000);
       }else if (h>h_max or h<h_min)
        {
         Serial.println("ALERTA HUMEDAD MAX/MIN");
              if(h>h_max){
                display.println("Hum MAX:");        
              }else{
                display.println("Hum MIN:");        
              }
            display.print(" H:");
             display.println(h,5);
             display.update();
             delay(2000);
         //digitalWrite(gpio2_pin, HIGH);
        } else{
          display.println("SIN ALERTAS");
          display.update();
          delay(2000);
        }
      delay(2000);
    }
}

void imprimirLCD(int rele, float gas, float t, float h, float presion, int id_tarj){
      display.clear();                                      // Borra lo presentado en el display OLED
      display.setCursor(0,0);                               // Fija el cursor en la posición 0,0 del display
      display.println("DATOS MEDIDOS:");
      display.print("ID Tarj: ");
      display.println(id_tarj);
      display.print("Rele: ");                         
      display.println(rele,5);
      display.print("Gas: ");                         
      display.println(gas,5);
      display.print("Tempe: ");                         
      display.println(t,5);
      display.print("Humedad: ");                         
      display.println(h,5);
      display.print("Presion: ");                         
      display.println(presion,5);
      display.update();
      delay(2000);
}
/*
static void smartDelay(unsigned long ms)                // Un tipo de retraso que asegura la operación del gps
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
    gps.encode(ss.read());
  } while (millis() - start < ms);
} 
void GPS(){
  double lati=0;
  double longi=0;
  display.clear();
  display.setCursor(0,0); 
  lati = gps.location.lat();                            // Obtiene el valor de latitud del GPS.  
  int lat_entero;                          // Las siguientes lineas son para almacenar en una variable el componente entero y en otra variable el componente decimal de la latitud
  int lat_decimal;                                      // para unirlos posteriormente, ya que si se deja todo en una sola variable, lo deja por defecto en dos decimales, lo cual no es suficiente
  int lat_entero_abs;                                   // para obtener una buena ubicación en un mapa. También se maneja una variable para valor absoluto, en casos de valores negativos.
  int lon_entero;                                      // Las siguientes lineas son para almacenar en una variable el componente entero y en otra variable el componente decimal de la longitud
  int lon_decimal;                                     // para unirlos posteriormente, ya que si se deja todo en una sola variable, lo deja por defecto en dos decimales, lo cual no es suficiente
  int lon_entero_abs;
  
  lat_entero = int(lati);
  lat_entero_abs = abs(lat_entero);
  lat_decimal = int((abs(lati) - lat_entero_abs) * 100000);
  if (lat_entero == 0)                                  // Se coloca éste condicional para que cuando el GPS apenas ha iniciado, no envíe valores de ubicación con 0, a la base de datos
   { 
     display.print("Esperando obtener datos del GPS...");  // en caso de que los valores recibidos del GPS de latitud sean 0, presenta en pantalla que está esperando obtener datos del GPS.
     display.update();                                     // Actualiza el display para mostrar el mensaje.
     delay (1000);
     display.clear();
     display.update();                                     // Actualiza el display para mostrar la pantalla limpia.
     delay (1000);
   }
  else
   {  
     display.print("Latit   : ");                         // Envía a la pantalla (sin presentarlo aún, porque no hay update) el valor de latitud medido por el GPS.
     display.println(lati,5);
     display.print("Longit  : ");
     longi = gps.location.lng();                          // Obtiene el valor de longitud del GPS.                                    // para obtener una buena ubicación en un mapa. También se maneja una variable para valor absoluto, en casos de valores negativos.
     lon_entero = int(longi);
     lon_entero_abs = abs(lon_entero);
     lon_decimal = int((abs(longi) - lon_entero_abs) * 100000);
     display.println(longi,5);
     display.print("Satelit : ");                         // Envía a la pantalla (sin presentarlo aún, porque no hay update) el valor del # de satelites medido por el GPS.
     display.println(gps.satellites.value());
     display.print("Altitud : ");                         // Envía a la pantalla (sin presentarlo aún, porque no hay update) el valor de altitud en metros, medido por el GPS.
     display.print(gps.altitude.meters());
     display.println("mts");                          
     display.print("Hora/Min: ");                         // Envía a la pantalla (sin presentarlo aún, porque no hay update) el valor de hora, minutos y segundos, medido por el GPS.
     
     if(gps.time.hour()<5){
      int hincorrect;
      int hcorrect;
      hincorrect=gps.time.hour()-5;
      hcorrect = 24+hincorrect;
      display.print(hcorrect);
     }
     else{
     display.print(gps.time.hour()-5); 
     } 
                         
     display.print(":");
     display.print(gps.time.minute());                     
     display.print(":");
     display.println(gps.time.second());                   
     display.print("Velocid : ");
     display.print(gps.speed.kmph());                     // Envía a la pantalla (sin presentarlo aún, porque no hay update) el valor de velocidad, calculado por el GPS.
     display.println(" km/h"); 
     unsigned long Distance_To_Home = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),Home_LAT, Home_LNG);
     display.print("Dis casa: ");                        // Envía a la pantalla (sin presentarlo aún, porque no hay update) el valor de distancia al sitio en kms, referenciado arriba, calculado por el GPS.
     display.print(Distance_To_Home);
     display.println(" km"); 
     display.update();                                  // Actualiza la pantalla para mostrar lo que se ha enviado hasta el momento y presenta un retardo, para poder visualizarlo.
     delay(3000);
  
   } 
   smartDelay(500);                                      // Ejecuta un retardo especial, para saber si hay o no respuesta del GPS.
   if (millis() > 5000 && gps.charsProcessed() < 10)
      display.println(F("No GPS data received: check wiring"));
}
*/
