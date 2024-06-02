#include <LiquidCrystal.h>
#include "Dinosaurio.h"
#include "Cactus.h"
#include "Ave.h"

LiquidCrystal lcd(22, 23, 5, 18, 19, 21);
Dinosaurio dino;
Cactus cactus(16);
Ave ave(16); 

byte dinoReposo[8] = {
    B00000,
    B00000,
    B00111,
    B00101,
    B10111,
    B10100,
    B01101,
    B01100
};
byte dinoCamina1[8] = {
    B00000,
    B00111,
    B00101,
    B10111,
    B10100,
    B01110,
    B01100,
    B00010
};
byte dinoCamina2[8] = {
    B00000,
    B00111,
    B00101,
    B10111,
    B10100,
    B01110,
    B01100,
    B10000
};
byte dinoSalta[8] = {
    B00111,
    B00101,
    B10111,
    B10100,
    B01101,
    B01100,
    B10000,
    B00000
};
byte cactus1[8] = {
    B00000,
    B00000,
    B00001,
    B10101,
    B01101,
    B00110,
    B00100,
    B00100
};
byte ave1[8] = {
    B00100,
    B01110,
    B11111,
    B01110,
    B00100,
    B00000,
    B00000,
    B00000
};

struct Button {
    const uint8_t PIN;
    bool pressed;
};

Button button1 = {4, false};
Button button2 = {2, false}; 

int velocidad = 300;
int puntaje = 0;
bool juegoActivo = true;

void IRAM_ATTR isr1() {
    button1.pressed = true;
}

void IRAM_ATTR isr2() {
    button2.pressed = true;
}

void setup() {
    lcd.begin(16, 2);
    Serial.begin(115200);
    Serial.println("test");

    pinMode(button1.PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button1.PIN), isr1, FALLING);

    pinMode(button2.PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button2.PIN), isr2, FALLING);

    lcd.createChar(0, dinoReposo);
    lcd.createChar(1, dinoCamina1);
    lcd.createChar(2, dinoCamina2);
    lcd.createChar(3, dinoSalta);
    lcd.createChar(4, cactus1);
    lcd.createChar(5, ave1);

    lcd.setCursor(12, 0);
    lcd.print(puntaje);

    lcd.setCursor(0, 1);
    lcd.write(byte(0));

    delay(4000);
}

void mostrarEstadoFinal() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Total:");
    lcd.print(puntaje);
    delay(1000);
}

void reiniciarJuego() {
    juegoActivo = true;
    puntaje = 0;
    lcd.clear();
    lcd.setCursor(12, 0);
    lcd.print(puntaje);
    delay(2000);
}

void loop() {
    if (juegoActivo) {
        lcd.clear();
        if (button1.pressed) {
            dino.saltar();
            lcd.setCursor(0, 0);
            button1.pressed = false;
        } else {
            lcd.setCursor(0, 1);
            dino.caminar();
        }
        lcd.write(byte(dino.obtenerEstado()));
        cactus.mover();
        lcd.setCursor(cactus.obtenerPosicion(), 1);
        lcd.write(byte(4));
        ave.mover();
        lcd.setCursor(ave.obtenerPosicion() + 5, 0); 
        lcd.write(byte(5));
        if (dino.obtenerPosicion() == ave.obtenerPosicion() && dino.obtenerEstado() != 0) {
            Serial.println("Colision con el ave!");
            juegoActivo = false;
        }
        if (dino.obtenerPosicion() == 0 && cactus.obtenerPosicion() == 0) {
            Serial.println("Colision con el cactus!");
            juegoActivo = false;
        } else {
            puntaje++;
        }
        lcd.setCursor(12, 0);
        lcd.print(puntaje);
        delay(velocidad);
    } else {
        mostrarEstadoFinal();
        if (button2.pressed) { 
            button2.pressed = false;
            reiniciarJuego();
        }
    }
}
