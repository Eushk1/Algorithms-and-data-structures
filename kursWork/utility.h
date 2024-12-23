#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>
#include <Kinimatics.h>
#include <cmath>

struct Point3D {
  double x;
  double y;
  double z;
};

bool check_angles(float theta1, float theta2, float theta3, float theta_polar);

Point3D calculate_position(float theta1, float theta2, float theta3, float theta_polar);

Point3D calculateRootPoint(float Xeff, float Yeff, float Zeff,float Oeff);

bool isPointInRegion1(Point3D point,float O2min);
bool isPointInRegion2(Point3D point,float O2min);
#endif