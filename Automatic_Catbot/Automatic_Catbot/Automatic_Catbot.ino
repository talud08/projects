

// *******Variables y libreria //


// Sensor IR del mando de la TV
#include <Arduino.h>
#include <NECIRrcv.h>
#define IRPIN 7   // pin al que conectamos el receptor de IR
static int aux = 1;
NECIRrcv ir(IRPIN) ;

// Ultrasonido //
#include <Ultrasonic.h>
Ultrasonic ultrasonic(12, 11);
int distSeguridad = 35;
int distancia;
int i = 90;
int sentido_giro = 1;
boolean izquierda = false;
boolean derecha = false;

// Servos //
int pinMotorIzqdo = 8;
int pinMotorDcho = 9;
int pinServoUltrasonido = 10;
#include <Servo.h>
Servo servoDcho;
Servo servoIzqdo;
Servo servoUltrasonido;

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
  servoUltrasonido.attach(pinServoUltrasonido);
  servoUltrasonido.write (60);


  // Inicia el vehiculo detenido
  parar();

  delay (200);
}

void loop() {

  leerIR();
  Serial.println (estado);

  if (estado == 3108437760) { // Boton desplazar al Frente
    avanzar();
  }
  if (estado == 3141861120) { // Boton IZQ
    girarIzquierda();
  }
  if (estado == 3208707840) { // Boton Parar (OK)
    parar();
  }
  if (estado == 3158572800) { // Boton DER
    girarDerecha();
  }

  if (estado == 3927310080) { // Boton Atras
    retroceder();
  }
  if (estado == 3175284480) { // Boton Automatico (ASTERISCO)
    automatico();
  }
  if (estado == 3041591040) { // Boton Hacer Foto (ALMOHADILLA)
    // hacer_foto();
  }
}

//-------- Leer sensor IR

void leerIR() {
  while (ir.available()) { // lee el IR y almacena en estado
    estado = ir.read();
  }
}

//-------- Función de funcionamiento automático
void automatico() {
  if (sentido_giro == 1) {
    moverUltrasonidoDerecha();
    movimiento();
  }
  else {
    moverUltrasonidoIzquierda();
    movimiento();
  }
}


// *********  Chequea en qué posiciones hay obstáculos y mueve el vehículo en consecuencia
void movimiento() {
  if (izquierda == false && derecha == false) {
    avanzar();
  }
  else if (izquierda == true) {
    retroceder();
    delay(1000);
    girarDerecha();
    delay(1000);
    parar();
  }
  else {
    retroceder();
    delay(1000);
    girarIzquierda();
    delay(1000);
    parar();
  }

  izquierda = false;
  derecha = false;
}


// --------------  Función que mueve el servo hacia la izquierda para medir distancias
void moverUltrasonidoIzquierda() {
  i = i - 2;
  servoUltrasonido.write(i);
  if ( i < 56) {
    sentido_giro = 1;
  }
  lecturaDistancia();
  delay(10);
}

// --------------  Función que mueve el servo hacia la derecha para medir distancias
void moverUltrasonidoDerecha() {
  i = i + 2;
  servoUltrasonido.write(i);
  if ( i > 140) {
    sentido_giro = 2;
  }
  lecturaDistancia();
  delay(10);
}

// --------------  Función que lee la distancia
void lecturaDistancia() // Lectura de distancia y comparación con distancia de seguridad
{
  // Lectura del sensor de ultrasonido
  distancia = ultrasonic.distanceRead();
  Serial.print("distancia = ");
  Serial.println(distancia);
  delay(50);
  // Comparación con distancia de seguridad
  if (distancia < distSeguridad) {
    if (i < 90) {
      derecha = true;
    }
    else {
      izquierda = true;
    }
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




