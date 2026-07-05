#ifndef CONTROL_DIMMER_H
#define CONTROL_DIMMER_H
#include <Arduino.h>

void iniciarDimmer();
void actualizarPotencia(int nuevaPotencia);
int obtenerDisparos();
void reiniciarDisparos();
#endif