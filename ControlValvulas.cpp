#include "ControlValvulas.h"

const int PIN_VALVULA_CALOR = 25;
const int PIN_VALVULA_SUCCION = 26;

// =========================================================================
// DICCIONARIO DE HARDWARE (ESP32 -> NPN 2N2222A -> Gate MOSFET -> Válvula NO)
// =========================================================================
// LOW  (0V)   -> NPN Apagado -> Gate recibe 5V -> Válvula con 12V (CERRADA / SUENA)
// HIGH (3.3V) -> NPN Saturado -> Gate cae a GND -> Válvula con 0V  (ABIERTA / SILENCIO)

#define ENERGIZAR_BOBINA_CERRAR LOW  
#define RELAJAR_BOBINA_ABRIR    HIGH 

void iniciarValvulas() {
  // 1. Cargamos el estado de CERRADO en el procesador antes de abrir el pin
  digitalWrite(PIN_VALVULA_CALOR, ENERGIZAR_BOBINA_CERRAR);
  digitalWrite(PIN_VALVULA_SUCCION, ENERGIZAR_BOBINA_CERRAR);

  pinMode(PIN_VALVULA_CALOR, OUTPUT);
  pinMode(PIN_VALVULA_SUCCION, OUTPUT);
}

void abrirValvulaCalor() {
  digitalWrite(PIN_VALVULA_CALOR, RELAJAR_BOBINA_ABRIR); 
}

void cerrarValvulaCalor() {
  digitalWrite(PIN_VALVULA_CALOR, ENERGIZAR_BOBINA_CERRAR); 
}

void abrirValvulaSuccion() {
  digitalWrite(PIN_VALVULA_SUCCION, RELAJAR_BOBINA_ABRIR);
}

void cerrarValvulaSuccion() {
  digitalWrite(PIN_VALVULA_SUCCION, ENERGIZAR_BOBINA_CERRAR);
}

void cerrarAmbasValvulas() {
  digitalWrite(PIN_VALVULA_CALOR, ENERGIZAR_BOBINA_CERRAR);
  digitalWrite(PIN_VALVULA_SUCCION, ENERGIZAR_BOBINA_CERRAR);
}