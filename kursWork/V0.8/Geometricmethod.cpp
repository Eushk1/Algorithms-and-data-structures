#include <GeometricMethod.h>
#include <Arduino.h>
#include <Kinimatics.h>

ManipulatorState Gcode_command_G1(float x, float y, float z, float GridAngle) {
    ManipulatorState manipulatorstate_temp;

    // Вычисляем полярный угол
    manipulatorstate_temp.polarangle = atan2(y, x) * (180.0 / M_PI);

    // Вычисляем длину радиус-вектора
    x = sqrt(x * x + y * y);
    x = x - (hinge_3_len * cos((GridAngle) * (M_PI / 180.0)));
    z = z - (hinge_3_len * sin((GridAngle) * (M_PI / 180.0)));

    Serial.print("Т1: ");
    Serial.println(x);
    Serial.print("Т2: ");
    Serial.println(z);

    // Вычисляем длину прямой и угол
    float direct_line_len = sqrt(x * x + z * z);
    float direct_line_angle = atan2(z, x);

    // Вычисляем углы
    manipulatorstate_temp.angle3 = acos(((((hinge_2_len * hinge_2_len) + (direct_line_len * direct_line_len) - (hinge_1_len * hinge_1_len))) / (2 * hinge_2_len * direct_line_len)));
    manipulatorstate_temp.angle1 = acos(((((hinge_1_len * hinge_1_len) + (direct_line_len * direct_line_len) - (hinge_2_len * hinge_2_len))) / (2 * hinge_1_len * direct_line_len)));
    manipulatorstate_temp.angle2 = acos(((((hinge_1_len * hinge_1_len) + (hinge_2_len * hinge_2_len) - (direct_line_len * direct_line_len))) / (2 * hinge_1_len * hinge_2_len)));

    Serial.print("Т1: ");
    Serial.println(direct_line_len);
    Serial.print("Т2: ");
    Serial.println(direct_line_angle * (180.0 / M_PI));

    Serial.print("A1: ");
    Serial.println(manipulatorstate_temp.angle1);
    Serial.print("A2: ");
    Serial.println(manipulatorstate_temp.angle2);
    Serial.print("A3: ");
    Serial.println(manipulatorstate_temp.angle3);




    // Преобразование радиан в градусы
    manipulatorstate_temp.angle3 = -(180 -  (manipulatorstate_temp.angle3 * (180.0 / M_PI))-(180-(direct_line_angle * (180.0 / M_PI))-GridAngle));
    manipulatorstate_temp.angle1 = (manipulatorstate_temp.angle1 * (180.0 / M_PI)) + (direct_line_angle * (180.0 / M_PI));
    manipulatorstate_temp.angle2 = -(180-manipulatorstate_temp.angle2 * (180.0 / M_PI));

    return manipulatorstate_temp;
}