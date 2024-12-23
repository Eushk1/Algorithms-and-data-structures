#ifndef KINIMATICS_H
#define KINIMATICS_H

#include <Arduino.h>

struct ManipulatorState {
    float polarangle; 
    float angle1;
    float angle2;
    float angle3;
    int gridangle;
};

const float hinge_1_len = 10.5f;
const float hinge_2_len = 14.5f;
const float hinge_3_len = 18.0f;
const float cPI = 3.1415f;

#endif