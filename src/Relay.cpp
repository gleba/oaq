//
// Created by soul on 05.02.2020.
//

#include <esp32-hal-gpio.h>
#include <HardwareSerial.h>
#include "Relay.h"


const int pins[4] = {4, 13, 12, 25};

//bool relay_state[4] = {false, false, false, false};

void initRelay(){
    Serial.println("initRelay");
    for(const int &pin : pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
}
void relayPower(int num, bool value){
    int i = num-1;
    int pin = pins[i];
//    relay_state[i] = value;
    if (value)
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
}
