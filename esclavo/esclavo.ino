/*
  Codigo Esclavo
  El presente codigo será utilizado para todos los dispositivos
  esclavos del bus i2c cambiando unicamente la variable que se
  llama direccion para asignar nuevas direcciones a nuevos
  dispositivos

  Yeffri J. Salazar
  Xibalba Hackerspace, noviembre 2018
*/
/*************
   Librerias
 *************/
#include <Wire.h>
/*************
   variables
 *************/
//direccion del dispositivo
int direccion = 2;
//almacenamiento de datos recibidos
// y enviados posteriormente
int respuesta[] = {direccion, 0, 0};

void setup() {
  // en este ciclo definimos como salida
  // desde el pin numero 2 hasta el 45
  // evitando los pines 20 y 21 que son
  // los usados para comunicación
  for ( byte i = 2; i < 45; i++) {
    if (i != 20 || i != 21) {
      pinMode(i, OUTPUT);
    }
  }
  // nos unimos al bus con la direccion asignada
  Wire.begin(direccion);
  // definimos la funcion cuando se recibe informacion desde el maestro
  Wire.onReceive(recibirDatos);
  //Serial.begin(9600);           // debugging
  //envio de informacion del esclavo al maestro
  Wire.onRequest(peticionDeRespuesta);

}

void loop() {
}
void peticionDeRespuesta() {
  Wire.write(respuesta[0]);
  Wire.write(respuesta[1]);
  Wire.write(respuesta[2]);
}

void recibirDatos(int a) {
  //variable contador para llevar el control del almacenamiento
  int contador = 1;
  while (Wire.available()) {
    //almacenamiento de datos
    respuesta[contador] = int(Wire.read());
    //Serial.print(respuesta[contador]); // debugging
    contador++;// se le suma una unidad para seguir con el conteo
  }
  //Serial.println(); //debugging
  
  // siempre y cuando el valor no sea 20 o 21
  // el valor en respuesta[1] es igual al valor de pin en el
  // maestro y el valor de respuesta[2] es el estado del pin
  // definido tambien en el maestro
  if (respuesta[1] != 20 || respuesta[1] != 21) {
    digitalWrite(respuesta[1], respuesta[2]);
  }
}
