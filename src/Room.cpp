//
// Created by soul on 06.02.2020.
//

#include <DHT.h>
#include <mutex>
#include <thread>
#include <stack>
#include "Room.h"
#include "Relay.h"


const auto check_interval = std::chrono::seconds{2};

DHT dht(GPIO_NUM_27, DHT11);

float round(float var) {
    float value = (int) (var * 100 + .5);
    return (float) value / 100;
}
//void add(std::stack<float> tail, float value){
//    if (tail.size()>=30)
//        tail.pop();
//    tail.push(value);
//};
//
//std::stack<float> tailH;
//std::stack<float> tailT;

SensorsState sensorsState;
float comfortLevel = 42;

void castData() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    sensorsState.lastHLevel = humidity;
    sensorsState.lastTLevel = temperature;
    if (humidity >= comfortLevel + 2)
        relayPower(4, false);
    if (humidity <= comfortLevel - 2)
        relayPower(4, true);
}


void checker() {
    while (true) {
        std::this_thread::sleep_for(check_interval);
        castData();
    }
}

void initRoomSensors() {
    Serial.println("init room sensors");
    dht.begin();
    new std::thread(checker);
}

SensorsState getSensorsState() {
    return sensorsState;
}

float getComfortLevel(){
    return comfortLevel;
}
void setComfortLevel(float level) {
    comfortLevel = level;
}