#ifndef CONTROL_PANTALLA_H
#define CONTROL_PANTALLA_H
#include <Arduino.h>

void iniciarPantalla();
void mostrarInicio();
void mostrarEsperandoModo();
void mostrarEsperandoPotencia();
void mostrarInfoAuto();
void mostrarEsperandoBoton(int pot);
void mostrarEjecutando(int pot);
void mostrarEsperandoSuccion();
void mostrarEsperandoApagar();
void mostrarFinalizado(int disparos);
#endif