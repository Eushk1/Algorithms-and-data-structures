#include <utility.h>

bool check_angles(float theta1, float theta2, float theta3, float theta_polar) {
  // Check if all angles are within their respective ranges
  if (0 <= theta1 && theta1 <= 180 &&
      -135 <= theta2 && theta2 <= 45 &&
      -105 <= theta3 && theta3 <= 75 &&
      -90 <= theta_polar && theta_polar <= 90) {
    return true;
  } else {
    return false;
  }
}


Point3D calculate_position(float theta1, float theta2, float theta3, float theta_polar) {
  float L1=hinge_1_len;
  float L2=hinge_2_len;
  float L3=hinge_3_len;

  theta1 = theta1 * M_PI / 180.0;
  theta2 = theta2 * M_PI / 180.0;
  theta3 = theta3 * M_PI / 180.0;
  theta_polar = theta_polar * M_PI / 180.0;


  double x_prime = L1 * cos(theta1) + L2 * cos(theta1 + theta2) + L3 * cos(theta1 + theta2 + theta3);
  double z_prime = L1 * sin(theta1) + L2 * sin(theta1 + theta2) + L3 * sin(theta1 + theta2 + theta3);
  
  // Project onto the main planes
  double x = x_prime * cos(theta_polar);
  double y = x_prime * sin(theta_polar);
  double z = z_prime;
  
  return {x, y, z};
}


Point3D calculateRootPoint(float Xeff, float Yeff, float Zeff,float Oeff) {
  Point3D rootPoint;

  Oeff = Oeff * M_PI / 180.0;

  float L3=hinge_3_len;

  rootPoint.z = Zeff - L3 * sin(Oeff);
  float angle = atan2(Xeff, Yeff);
  rootPoint.x = Xeff - L3 * cos(Oeff) * sin(angle);
  rootPoint.y = Yeff - L3 * cos(Oeff) * cos(angle);
  return rootPoint;
}

bool isPointInRegion1(Point3D point,float O2min) {

  O2min = O2min * M_PI / 180.0;

  float L1=hinge_1_len;
  float L2=hinge_2_len;

  if (point.x >= 0 && point.z >= 0 &&
      point.x * point.x + point.y * point.y + point.z * point.z <  (L1+L2)*(L1+L2) &&
      sqrt(point.x * point.x + point.y * point.y + point.z * point.z) > sqrt(L1 * L1 + L2 * L2 - 2 * L1 * L2 * cos(O2min))) {

    return true;
  } else {
    return false;
  }
}

bool isPointInRegion2(Point3D point,float O2min) {

  O2min = O2min * M_PI / 180.0;

  float L1=hinge_1_len;
  float L2=hinge_2_len;
  float L3=hinge_3_len;


  if (point.x >= 0 && point.z >= 0 &&
      point.x * point.x + point.y * point.y + point.z * point.z <  (L1+L2+L3)*(L1+L2+L3) &&
      sqrt(point.x * point.x + point.y * point.y + point.z * point.z) > sqrt(L1 * L1 + L2 * L2 - 2 * L1 * L2 * cos(-O2min))) {

    return true;
  } else {
    return false;
  }
}

