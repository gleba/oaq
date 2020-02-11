//
// Created by soul on 06.02.2020.
//

#ifndef OAQ_ROOM_H
#define OAQ_ROOM_H

struct Glass {
    int avg;
    float max;
    float min;
};

struct SensorsState {
    int lastLLevel;
    float lastHLevel;
    float lastTLevel;
    Glass oneMinuteHLevel;
    Glass oneMinuteTLevel;
};


void initRoomSensors();
void setComfortLevel(float level);
float getComfortLevel();
SensorsState getSensorsState();


#endif //OAQ_ROOM_H
