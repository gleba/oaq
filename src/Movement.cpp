// Created by soul on 05.02.2020.
//
#include <esp32-hal.h>
#include <Arduino.h>
#include <thread>
#include "Movement.h"
#include "Relay.h"
#include "Core.h"

const int motion_sensor = GPIO_NUM_26;
const auto check_interval = std::chrono::seconds{2};
//unsigned long now = millis();
unsigned long last_trigger = 0;
unsigned long now = 0;
unsigned long movement_time = 0;
unsigned long high_light = 3 * 60 * 1000;
unsigned long low_light = 5 * 60 * 1000;
unsigned long sub_light = 7 * 60 * 1000;

unsigned long msInMin = 60 * 1000;

int sparseness = 3500;
int max_sparseness = 4000;

int forceLight = 0;
int getForceLight(){
    return forceLight;
}
void setForceLight(int value){
    forceLight = value;
}


void hideMovements() {
    if (forceLight>0){
        forceLight = forceLight - 2;
        last_trigger = millis();
    } else {
        movement_time = millis() - last_trigger;
        if (movement_time > high_light)
            relayPower(1, false);
        if (movement_time > sub_light)
            relayPower(2, false);
        if (movement_time > low_light)
            relayPower(3, false);
    }
}

void IRAM_ATTR detectMovement() {
    Serial.println("MOTION DETECTED");
    last_trigger = millis();

    int s_value = analogRead(GPIO_NUM_14);
    relayPower(2, true);
    if (s_value < max_sparseness)
        relayPower(3, true);
    if (s_value < sparseness)
        relayPower(1, true);

}


void movement_tail() {
    while (true) {
        std::this_thread::sleep_for(check_interval);
        hideMovements();
    }
}

void initMovement() {
    Serial.println("init movement");
    pinMode(motion_sensor, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(motion_sensor), detectMovement, RISING);
    new std::thread(movement_tail);
}

void setLightSenseLevel(int value) {
    sparseness = value;
}

int getLightSenseLevel() {
    return sparseness;
}
unsigned long getLastTrigger() {
    return last_trigger;
}

