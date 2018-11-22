/*
  Codigo Esclavo
  El presente codigo será utilizado para el dispositivo
  maestro en el bus de i2c, en el presente sketch
  obtenemos informacion de serial por medio de
  valores separados por comas y lo enviamos a sus respectivos
  dispositivos siendo el 0 el dispositivo maestro
  0,13,1 para encender el led del pin 13 en el maestro
  0,13,0 para apagar el led del pin 13 en el maestro

  Yeffri J. Salazar
  Xibalba Hackerspace, noviembre 2018
*/

#include <Wire.h>
//cadena de caracteres para almacenar los datos
String stringDeEntrada = "";
// variable para detectar el fin de la cadena
bool stringCompleta = false;
int datos[] = {0, 0, 0};
int  contadorSerial = 0;
void setup() {
  for ( uint8_t i = 2; i < 45; i++) {
    if (i != 20 || i != 21) {
      pinMode(i, OUTPUT);
    }
  }
  //iniciamos la comunicacion Serial a 9600 baudios
  Serial.begin(9600);
  //empezamos la comunicacion i2c como maestro
  Wire.begin();
  stringDeEntrada.reserve(10);
}
void loop() {

  if (stringCompleta) {
    // Serial.println(stringDeEntrada);
    // Serial.println(datos[0]);
    // Serial.println(datos[1]);
    // Serial.println(datos[2]);
    //envio de datos siempre y cuando no sea para el maestro
    if (datos[0] != 0) {
      enviarDatosI2C(datos[0], datos[1], datos[2]);
    } else {
      if (datos[1] != 20 || datos[1] != 21) {
        digitalWrite(datos[1], datos[2]);
        Serial.print(datos[0]);
        Serial.print(",");
        Serial.print(datos[1]);
        Serial.print(",");
        Serial.println(datos[2]);
      }
    }
    // limpiamos la cadena de datos
    stringDeEntrada = "";
    stringCompleta = false;
    contadorSerial = 0;
  }
}

void enviarDatosI2C(uint8_t direccionEsclavo, uint8_t pin, uint8_t estado) {
  // transmitir informacion a la direccion del dispositivo deseado
  Wire.beginTransmission(direccionEsclavo);
  Wire.write(pin);
  Wire.write(estado);
  //terminar la transmision de datos
  Wire.endTransmission();
  //Pedir respuesta del dispositivo
  Wire.requestFrom(direccionEsclavo, 3);
  int respuesta[] = {0, 0, 0};
  int acontador = 0;
  while (Wire.available()) {
    respuesta[acontador] = int(Wire.read());
    Serial.print(respuesta[acontador]);
    if (acontador != 2) {
      Serial.print(",");
    }
    acontador++;
  }
  Serial.println();
}


void serialEvent() {
  while (Serial.available()) {
    //lectura de nuevos caracteres
    char inChar = (char)Serial.read();
    stringDeEntrada += inChar;
    //si el caracter es igual a una coma entonces guardarlo
    //en el arreglo llamado datos
    if (inChar == ',') {
      datos[contadorSerial] = stringDeEntrada.toInt();
      stringDeEntrada = "";
      //  Serial.println(contadorSerial);
      //Serial.println(stringDeEntrada);
      contadorSerial++;
    } else {
      datos[2] = stringDeEntrada.toInt();
    }
    //cuando el caracter sea igual a nueva linea
    if (inChar == '\n') {
      stringCompleta = true;
    }
  }
}
