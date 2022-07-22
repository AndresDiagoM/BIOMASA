int rele=35;

void setup()
{
  Serial.begin(115200);   //Se inicia la comunicación serial 
  pinMode(rele, OUTPUT);  // pin del rele,
}
void loop()
{
  
 //Se imprimen las variables
 Serial.println("\nRELE: ");
  Serial.println(analogRead(rele));
  Serial.println(digitalRead(rele));
  
  delay(2000);  
}
