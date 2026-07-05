#include <Arduino.h>
#include "ControlPantalla.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void iniciarPantalla() {
  Wire.begin(21, 17);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error OLED"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void mostrarInicio() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println(F("BOMBA DE"));
  display.setCursor(30, 30);
  display.println(F("VACIO"));
  display.setTextSize(1);
  display.setCursor(35, 50);
  display.println(F("Iniciando..."));
  display.display();
}

void mostrarEsperandoModo() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 5);
  display.println(F("SISTEMA OK"));
  display.setCursor(0, 25);
  display.println(F("Seleccione modo en"));
  display.setCursor(0, 40);
  display.println(F("el Monitor Serial..."));
  display.display();
}

void mostrarEsperandoPotencia() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("MANUAL"));
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.println(F("Ingrese potencia %"));
  display.setCursor(0, 48);
  display.println(F("en Monitor Serial."));
  display.display();
}

void mostrarInfoAuto() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("AUTO"));
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.println(F("Potencia fija: 50%"));
  display.setCursor(0, 48);
  display.println(F("Iniciando ciclo..."));
  display.display();
}

void mostrarEsperandoBoton(int pot) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("MANUAL"));
  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print(F("Potencia Conf: "));
  display.print(pot);
  display.println(F("%"));
  display.setCursor(0, 45);
  display.println(F("Presione boton (13)"));
  display.display();
}

void mostrarEjecutando(int pot) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println(F("CALENTANDO"));
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print(F("Salida AC: "));
  display.print(pot);
  display.println(F("% Pot."));
  display.display();
}

void mostrarEsperandoSuccion() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("VALVULA CALOR: ABIERTA"));
  display.setCursor(0, 25);
  display.println(F("Presione boton (13)"));
  display.setCursor(0, 40);
  display.println(F("para abrir Succion"));
  display.display();
}

void mostrarEsperandoApagar() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("SUCCION: ABIERTA"));
  display.setCursor(0, 25);
  display.println(F("Presione boton (13)"));
  display.setCursor(0, 40);
  display.println(F("para APAGAR TODO"));
  display.display();
}

void mostrarFinalizado(int disparos) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("CICLO FIN"));
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.println(F("Disparos termicos:"));
  display.setTextSize(2);
  display.setCursor(35, 45);
  display.println(disparos);
  display.display();
}