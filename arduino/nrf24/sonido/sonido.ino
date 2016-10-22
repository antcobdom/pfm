int LED = 3 ; 
int sensor = 2 ;
bool estado = false ;

void setup()
{
  pinMode( LED, OUTPUT) ;
  pinMode( sensor , INPUT_PULLUP) ;
  digitalWrite(LED , LOW) ; // Apagamos el LED al empezar
}
void loop()
{
  bool  valor =  digitalRead(sensor) ; //leemos el estado del sensor
  if ( valor == true ) //Si está activada la salida D0
  {
    estado = ! estado ;                       // cambiamos el estado del LED
    digitalWrite(LED, estado) ;            // escribimos el nuevo valor
    delay (1000);
  }
}
