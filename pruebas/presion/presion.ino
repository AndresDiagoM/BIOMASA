//https://electronperdido.com/shop/sensores/presion/mps20n0040d-sensor-presion-0-40kpa/
#include <Q2HX711.h>
Q2HX711 MPS20N0040D(2, 18); // Pines conexión //Pin SCK: pin 3  (2,3)...Pin OUT: pin 2
// *** CONFIGURACIÓN ***
void setup() {
  Serial.begin(9600); // Inicia monitor serie a 9600bps
}
// *** CÓDIGO PRINCIPAL ***
void loop() {
  float valor = 0.0;
  valor = MPS20N0040D.read(); // Lectura valor del sensor

  if(valor <=17000000 && valor>13000000 ){
    Serial.println("presion baja");
  }else if(valor <=13000000 && valor>10000000){
    Serial.println("presion alta");
  }else if(valor <10000000 ){
    Serial.println("presion media");
  }
  
  Serial.println(valor,0);
  delay(500);
}
