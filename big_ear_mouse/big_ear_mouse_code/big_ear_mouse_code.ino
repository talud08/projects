

// *******Variables y libreria //


// Sensor IR del mando de la TV
#include <Arduino.h>
#include <NECIRrcv.h>
#define IRPIN 7   // pin al que conectamos el receptor de IR
static int aux = 1;
NECIRrcv ir(IRPIN) ;



// Servos //
int pinServoCola = 6;
int pinMotorIzqdo = 9;
int pinMotorDcho = 8;
int i = 90;  // Valor inicial cola
bool derecha = HIGH;  // Controla si la cola va hacia la derecha o hacia la izquierda

// LEDS //
int ojos = 5;
int cola = 4;
bool encendido = HIGH;

#include <Servo.h>
Servo servoDcho;
Servo servoIzqdo;
Servo servoCola;

// Variable IR //
unsigned long estado;

void setup() {
  Serial.begin(9600); // inicia el puerto serial para comunicacion con el IR

  // Infrarrojos //
  ir.begin() ;

  // Servos //
  servoDcho.attach(pinMotorDcho);
  servoDcho.write (1500);
  servoIzqdo.attach(pinMotorIzqdo);
  servoIzqdo.write (1500);
  servoCola.attach(pinServoCola);
  servoCola.write (i);

  // LEDS //
  pinMode (ojos, OUTPUT);
  pinMode (cola, OUTPUT);
  digitalWrite (ojos, encendido);
  digitalWrite (cola, encendido);

  // Inicia el vehiculo detenido
  parar();
  delay (200);
}

void loop() {

  leerIR();


  if (estado == 3108437760) { // Boton desplazar al Frente
    avanzar();
    blinkleds();
    moverCola();
  }
  if (estado == 3141861120) { // Boton IZQ
    girarDerecha();
    blinkleds();
    moverCola();
  }
  if (estado == 3208707840) { // Boton Parar
    parar();
    blinkleds();
    delay(400);
  }
  if (estado == 3158572800) { // Boton DER
    girarIzquierda();
    blinkleds();
    moverCola();
  }

  if (estado == 3927310080) { // Boton Reversa
    retroceder();
    blinkleds();
    moverCola();
  }

}

//-------- Leer sensor IR

void leerIR() {
  while (ir.available()) { // lee el IR y almacena en estado
    estado = ir.read();
  }
}

// ----------  Función que apaga y enciende leds  -----
void blinkleds() {
  encendido = !encendido;
  digitalWrite (ojos, encendido);
  digitalWrite (cola, encendido);
  delay(100);
}

// ----------  FUNCIONES QUE MUEVEN LA COLA  --------
void moverCola() {
  switch (derecha) {
    case HIGH:
      moverColaDerecha();
      break;
    case LOW:
      moverColaIzquierda();
      break;

  }
}
// --------------  Función que mueve el servo hacia la izquierda una posición
void moverColaDerecha() {
  i = i - 2;
  if (i > 60) {
    servoCola.write(i);
    delay(10);
  }
  else {
    derecha = LOW;
  }
}
// --------------  Función que mueve el servo hacia la derecha una posición
void moverColaIzquierda() {
  i = i + 2;
  if (i < 126) {
    servoCola.write(i);
    delay(10);
  }
  else {
    derecha = HIGH;
  }
}



// *******  Parar  *********
void parar() {
  servoDcho.write (1500);
  servoIzqdo.write (1500);
}

// ****** Avanzar ********
void avanzar() {
  servoDcho.write (1300);
  servoIzqdo.write (1675);
}

// ****** Retroceder ********
void retroceder() {
  servoDcho.write (1575);
  servoIzqdo.write (1400);
}

// *****Girar a la derecha ****
void girarDerecha() {
  servoDcho.write (1600);
  servoIzqdo.write (1675);
}

// *****Girar a la izquierda ****
void girarIzquierda() {
  servoDcho.write (1312);
  servoIzqdo.write (1400);
}




