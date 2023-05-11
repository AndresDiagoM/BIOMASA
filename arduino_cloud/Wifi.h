#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Ticker.h>
#define pinLedWifi 2//D4

Ticker ticker; //instancia de la clase ticker
IPAddress ip;

//DECLARACIÓN DE FUNCIONES
void Wifi();
void parpadeoLedWifi();


//
void parpadeoLedWifi(){
  byte estado = digitalRead(pinLedWifi);
  digitalWrite(pinLedWifi,!estado);
}
void Wifi() {
  //Serial.begin(115200);
  //pinMode(pinLedWifi, OUTPUT);

  //Empezamos el temporizador que hace parpadear el led
  //ticker.attach(0.2, parpadeoLedWifi);
      
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    //res = wm.autoConnect("AutoConnectESP32","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("Succesfully connected  :)");
    }
  //Paramos el temporizador
  //ticker.detach();

  //apagamos el led
  //digitalWrite(pinLedWifi, HIGH);   
  ip=WiFi.localIP();
}
