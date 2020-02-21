//
// Created by soul on 05.02.2020.
//

#include "Core.h"
#include "WiFi.h"
#include "Room.h"
#include "ESPAsyncWebServer.h"
#include "Movement.h"
#include "Relay.h"

AsyncWebServer server(80);


int fanSpeed = 200;
int fanDcSpeed = 100;
int subLightTime = 100;

void setUpRoutes(){
    Serial.println("set up http routes");
//    server.onNotFound(onIndex);
    server.begin();
    Serial.println("http server started");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

        int paramsNr = request->params();
        Serial.println(paramsNr);
        SensorsState sensorsState = getSensorsState();
        String content = "T:"+String(sensorsState.lastTLevel)+"\nH:"+String(sensorsState.lastHLevel);
        for(int i=0;i<paramsNr;i++){
            AsyncWebParameter* p = request->getParam(i);
            if (p->name()=="c"){
                setComfortLevel(p->value().toFloat());
            }
            if (p->name()=="l"){
                setLightSenseLevel(p->value().toFloat());
            }
            if (p->name()=="on"){
                relayPower(p->value().toFloat(), true);
            }
            if (p->name()=="off"){
                relayPower(p->value().toFloat(), false);
            }
            if (p->name()=="t"){
                setForceLight(p->value().toFloat());
            }
            if (p->name()=="fan"){
                fanSpeed = p->value().toInt();
                ledcWrite(0, fanSpeed);
            }
            if (p->name()=="dc"){
                fanDcSpeed = p->value().toInt();
                ledcWrite(1, fanDcSpeed);
            }
        }
        content = content+"\nLigthSense:"+analogRead(GPIO_NUM_14);
        content = content+"\nComfort:"+getComfortLevel();
        content = content + "\nLigthSenseTrigger:" + getLightSenseLevel();
        content = content + "\nForceLigth:" + getForceLight();
        content = content + "\nFan:" + fanSpeed;
        content = content + "\nDCFan:" + fanDcSpeed;
        content = content + "\nlastTrigger:" + getLastTrigger();
        request->send(200, "text/plain", content);
    });
}

//int Core::getSubLigthTime(){
//    return
//}


void Core::connect() {
    Serial.println("connect wi-fi");
    WiFi.begin("soulways", "soulways");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("IP address: ");  //  "IP-адрес: "
    Serial.println(WiFi.localIP());
    setUpRoutes();

    ledcSetup(0, 24, 8);
    ledcAttachPin(GPIO_NUM_32, 0);
    ledcWrite(0, fanSpeed);

    ledcSetup(1, 24, 8);
    ledcAttachPin(GPIO_NUM_33, 1);
    ledcWrite(1, fanDcSpeed);
}
