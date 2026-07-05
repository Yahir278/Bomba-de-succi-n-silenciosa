#include "ControlDimmer.h"
#include "ControlPantalla.h"
#include "ControlValvulas.h"

const int PIN_BOTON = 13; 

enum EstadoSistema {
  INICIO,
  ESPERANDO_MODO,
  ESPERANDO_POTENCIA_MANUAL,
  MOSTRANDO_INFO_AUTO,
  ESPERANDO_BOTON_INICIO,
  EJECUTANDO_CALOR,
  ESPERANDO_BOTON_SUCCION,  
  ESPERANDO_BOTON_APAGAR,   
  FINALIZADO
};
EstadoSistema estadoActual = INICIO;

unsigned long tiempoInicio = 0;
unsigned long tiempoEsperaInfo = 0;

const unsigned long TIEMPO_OPERACION_MS = 3000;  
const unsigned long TIEMPO_INFO_MS = 2000;       

int potenciaSeleccionada = 0; 
bool esModoAutomatico = false; 

// Variables de Antirrebote Industrial
bool ultimoEstadoBoton = HIGH;
unsigned long tiempoUltimoRebote = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  
  // 1. EL HARDWARE NEUMÁTICO NACE SELLADO Y ZUMBANDO
  iniciarValvulas(); 
  iniciarDimmer();
  iniciarPantalla(); 
  
  mostrarInicio();
  delay(2000); 
  
  estadoActual = ESPERANDO_MODO;
  mostrarEsperandoModo();
  
  Serial.println("\n==================================================");
  Serial.println("  SISTEMA DE VACÍO DEFINITIVO (v3.0 - ESPOL)      ");
  Serial.println("==================================================");
  Serial.println("Escribe 'manual' o 'automatico' y presiona Enter:");
}

void loop() {
  bool lecturaBoton = digitalRead(PIN_BOTON);
  
  if (lecturaBoton != ultimoEstadoBoton) {
    tiempoUltimoRebote = millis();
  }

  switch (estadoActual) {
    
    case ESPERANDO_MODO:
      if (Serial.available() > 0) {
        String entrada = Serial.readStringUntil('\n');
        entrada.trim();
        entrada.toLowerCase();

        // Candado: Garantizamos que el ciclo arranque totalmente hermético
        cerrarAmbasValvulas();

        if (entrada == "automatico") {
          Serial.println("\n[Modo Auto] Configurado al 50%...");
          potenciaSeleccionada = 50; 
          esModoAutomatico = true;
          tiempoEsperaInfo = millis();
          
          estadoActual = MOSTRANDO_INFO_AUTO;
          mostrarInfoAuto();
        } 
        else if (entrada == "manual") {
          Serial.println("\n[Modo Manual] Por favor ingresa la potencia (0-100):");
          esModoAutomatico = false;
          
          estadoActual = ESPERANDO_POTENCIA_MANUAL;
          mostrarEsperandoPotencia();
        }
      }
      break;

    case ESPERANDO_POTENCIA_MANUAL:
      if (Serial.available() > 0) {
        String entrada = Serial.readStringUntil('\n');
        entrada.trim();
        if (entrada.length() > 0) {
          int valor = entrada.toInt();
          if (valor >= 0 && valor <= 100) {
            potenciaSeleccionada = valor;
            Serial.print("\n>>> Potencia fijada al: ");
            Serial.print(potenciaSeleccionada);
            Serial.println("%");
            
            estadoActual = ESPERANDO_BOTON_INICIO;
            mostrarEsperandoBoton(potenciaSeleccionada);
          }
        }
      }
      break;

    case MOSTRANDO_INFO_AUTO:
      if (millis() - tiempoEsperaInfo >= TIEMPO_INFO_MS) {
        Serial.println("\n[!] Iniciando calor Auto de 3s...");
        reiniciarDisparos();
        actualizarPotencia(potenciaSeleccionada); 
        tiempoInicio = millis();
        
        estadoActual = EJECUTANDO_CALOR;
        mostrarEjecutando(potenciaSeleccionada);
      }
      break;

    case ESPERANDO_BOTON_INICIO:
      if ((millis() - tiempoUltimoRebote) > 50) {
        if (lecturaBoton == LOW) {
          Serial.println("\n[!] Botón presionado. Calentando niquelina 3s...");
          reiniciarDisparos();
          actualizarPotencia(potenciaSeleccionada);
          tiempoInicio = millis();
          
          estadoActual = EJECUTANDO_CALOR;
          mostrarEjecutando(potenciaSeleccionada);
          
          while(digitalRead(PIN_BOTON) == LOW) { delay(10); } 
        }
      }
      break;

    case EJECUTANDO_CALOR:
      if (millis() - tiempoInicio >= TIEMPO_OPERACION_MS) {
        actualizarPotencia(0); // Apaga Triac
        
        if (esModoAutomatico) {
          Serial.println("\n[OK] Fin de ciclo Auto.");
          estadoActual = FINALIZADO;
          mostrarFinalizado(obtenerDisparos());
        } else {
          // SE CUMPLEN LOS 3s: Abrimos escape de calor (El pin 25 sube a 3.3V)
          Serial.println("\n[OK] 3s listos. Abriendo escape de calor (Pin 25)...");
          abrirValvulaCalor(); 
          
          estadoActual = ESPERANDO_BOTON_SUCCION;
          mostrarEsperandoSuccion();
        }
      }
      break;

    case ESPERANDO_BOTON_SUCCION:
      if ((millis() - tiempoUltimoRebote) > 50) {
        if (lecturaBoton == LOW) {
          // CLIC 2: Abrimos succión de la bomba (El pin 26 sube a 3.3V)
          Serial.println("\n[!] Botón presionado. Abriendo Succión (Pin 26)...");
          abrirValvulaSuccion();
          
          estadoActual = ESPERANDO_BOTON_APAGAR;
          mostrarEsperandoApagar();
          
          while(digitalRead(PIN_BOTON) == LOW) { delay(10); }
        }
      }
      break;

    case ESPERANDO_BOTON_APAGAR:
      if ((millis() - tiempoUltimoRebote) > 50) {
        if (lecturaBoton == LOW) {
          // CLIC 3: Volvemos a meter LOW a la ESP32 -> NPN se apaga -> Pega 12V a ambas
          Serial.println("\n[!] Botón presionado. Sellando ambas válvulas (Zumbido ON).");
          cerrarAmbasValvulas();
          
          estadoActual = FINALIZADO;
          mostrarFinalizado(obtenerDisparos());
          
          while(digitalRead(PIN_BOTON) == LOW) { delay(10); }
        }
      }
      break;

    case FINALIZADO:
      if ((millis() - tiempoUltimoRebote) > 50) {
        if (lecturaBoton == LOW) {
          // CLIC 4: El usuario da la orden de reiniciar el sistema
          Serial.println("\n--- SISTEMA REINICIADO ---");
          estadoActual = ESPERANDO_MODO;
          mostrarEsperandoModo();
          
          while(digitalRead(PIN_BOTON) == LOW) { delay(10); }
        }
      }
      break;
  }
  ultimoEstadoBoton = lecturaBoton;
}