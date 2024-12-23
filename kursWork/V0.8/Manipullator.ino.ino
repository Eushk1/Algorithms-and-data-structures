#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <cmath> 
#include <GeometricMethod.h>
#include <IterativeMethod.h>
#include <Kinimatics.h>
#include <ESP32Servo.h>
#include <utility.h>


ManipulatorState Angles;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

const int pin1 = 26;
const int pin2 = 25;
const int pin3 = 33;
const int pin4 = 32;
const int pin5 = 35;




const char* ssid = "ESP32_AP"; 
const char* password = "00000000"; 

WebServer server(80); 


const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Manipulator Control</title>
    <script>
        async function sendData(url, data) {
            const response = await fetch(url, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: data
            });
            const result = await response.text();
            document.getElementById('output').value += result + "\n";
        }

        function sendXYZaAngle() {
            const x = document.getElementById('xValue').value;
            const y = document.getElementById('yValue').value;
            const z = document.getElementById('zValue').value;
            const angle = document.getElementById('angleValue').value;

            sendData('/sendXYZaAngle', `x=${x}&y=${y}&z=${z}&angle=${angle}`);
        }

        function sendXYZ() {
            const x = document.getElementById('xValue2').value;
            const y = document.getElementById('yValue2').value;
            const z = document.getElementById('zValue2').value;

            sendData('/sendXYZ', `x=${x}&y=${y}&z=${z}`);
        }

        function sendArmAngle() {
            const angle = document.getElementById('armAngle').value;
            sendData('/sendArmAngle', `value=${angle}`);
        }
    </script>
</head>
<body>
    <h1>Manipulator Control</h1>

    <h2>Send x, y, z and effect angle(from -90 to 0)</h2>
    <input type="number" id="xValue" placeholder="Enter x">
    <input type="number" id="yValue" placeholder="Enter y">
    <input type="number" id="zValue" placeholder="Enter z">
    <input type="number" id="angleValue" placeholder="Enter angle">
    <button onclick="sendXYZaAngle()">Send</button>

    <h2>Send x, y, z</h2>
    <input type="number" id="xValue2" placeholder="Enter x">
    <input type="number" id="yValue2" placeholder="Enter y">
    <input type="number" id="zValue2" placeholder="Enter z">
    <button onclick="sendXYZ()">Send</button>

    <h2>Arm Angle (0-90)</h2>
    <input type="number" id="armAngle" min="-90" max="0" placeholder="Enter angle">
    <button onclick="sendArmAngle()">Send</button>

    <h2>Messages</h2>
    <textarea id="output" rows="10" cols="50" readonly></textarea> 
</body>
</html>
)rawliteral";


void handleSendXYZaAngle() {
    Serial.println("handleSendXYZaAngle called"); 
    if (server.hasArg("x") && server.hasArg("y") && server.hasArg("z") && server.hasArg("angle")) {
        float x = server.arg("x").toFloat();
        float y = server.arg("y").toFloat();
        float z = server.arg("z").toFloat();
        float angle = server.arg("angle").toFloat();

        Serial.print("Received x: ");
        Serial.println(x);
        Serial.print("Received y: ");
        Serial.println(y);
        Serial.print("Received z: ");
        Serial.println(z);
        Serial.print("Received angle: ");
        Serial.println(angle);

       
        String response = "Message sent: x=" + String(x) + ", y=" + String(y) + ", z=" + String(z) + ", angle=" + String(angle);

        Point3D Points= calculateRootPoint(x,y,z,angle);

        ManipulatorState Angles_temp;

        if(isPointInRegion1(Points,135)){
           Angles_temp = Gcode_command_G1(x, y, z, angle);
           if(check_angles(Angles_temp.angle1,Angles_temp.angle2,Angles_temp.angle3,Angles_temp.polarangle)){
              Angles=Angles_temp;
              response += "\nPolar Angle: " + String(Angles.polarangle) + 
                          "\nAngle 1: " + String(Angles.angle1) + 
                          "\nAngle 2: " + String(Angles.angle2) + 
                          "\nAngle 3: " + String(Angles.angle3);
              Points=calculate_position(Angles.angle1,Angles.angle2,Angles.angle3,Angles.polarangle);
              response +="\nFinal x: " + String(Points.x) + 
                          "\nFinal y: " + String(Points.y) + 
                          "\nFinal z: " + String(Points.z);
           }else{
             response += "\nAngles are not in tolerances";
           }  
        }else{
          response += "\nPoint outside the manipulation area";
        }


        server.send(200, "text/plain", response);
        Serial.println("x, y, z, angle arguments received: "+response); 
    } else {
        Serial.println("No x, y, z, angle arguments received"); // Для отладки
        server.send(400, "text/plain", "No message");
    }
}


void handleSendXYZ() {
    Serial.println("handleSendXYZ called"); // Для отладки
    if (server.hasArg("x") && server.hasArg("y") && server.hasArg("z")) {
        float x = server.arg("x").toFloat();
        float y = server.arg("y").toFloat();
        float z = server.arg("z").toFloat();

        Serial.print("Received x: ");
        Serial.println(x);
        Serial.print("Received y: ");
        Serial.println(y);
        Serial.print("Received z: ");
        Serial.println(z);

        
        Manipulator manipulator;

        String response = "Message sent: x=" + String(x) + ", y=" + String(y) + ", z=" + String(z);

        Point3D Points;
        Points.x=x;
        Points.y=y;
        Points.y=y;

         ManipulatorState Angles_temp;


        if(isPointInRegion2(Points,-135)){
           Angles_temp = manipulator.Gcode_command_G2(x, y, z);
           if(check_angles(Angles_temp.angle1,Angles_temp.angle2,Angles_temp.angle3,Angles_temp.polarangle)){
              Angles=Angles_temp;
              response += "\nPolar Angle: " + String(Angles.polarangle) + 
                          "\nAngle 1: " + String(Angles.angle1) + 
                          "\nAngle 2: " + String(Angles.angle2) + 
                          "\nAngle 3: " + String(Angles.angle3);
              Points=calculate_position(Angles.angle1,Angles.angle2,Angles.angle3,Angles.polarangle);
              response +="\nFinal x: " + String(Points.x) + 
                          "\nFinal y: " + String(Points.y) + 
                          "\nFinal z: " + String(Points.z);
           }else{
             response += "\nAngles are not in tolerances";
           }  
        }else{
          response += "\nPoint outside the manipulation area";
        }
        
        
        server.send(200, "text/plain", response);
        Serial.println("x, y, z arguments received: "+response); 
    } else {
        Serial.println("No x, y, z arguments received"); // Для отладки
        server.send(400, "text/plain", "No message");
    }
}

void handleSendArmAngle() {
    if (server.hasArg("value")) {
        float angle = server.arg("value").toFloat();
        Serial.print("Received arm angle: ");
        Serial.println(angle);
        Angles.gridangle=angle;
        server.send(200, "text/plain", "Angle sent: " + String(angle));
    } else {
        server.send(400, "text/plain", "No message");
    }
}

void setup() {
    Serial.begin(115200);


    WiFi.softAP(ssid, password);
    Serial.println("Access Point created");


    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());


    server.on("/", []() {
        server.send(200, "text/html", html);
    });
    server.on("/sendXYZaAngle", handleSendXYZaAngle);
    server.on("/sendXYZ", handleSendXYZ);
    server.on("/sendArmAngle", handleSendArmAngle);

    server.begin();
    Serial.println("Server started");


    servo1.attach(pin1);
    servo2.attach(pin2);
    servo3.attach(pin3);
    servo4.attach(pin4);
    servo5.attach(pin5);

}

void loop() {
   server.handleClient(); // Обработка входящих клиентов
   servo1.write((90+Angles.polarangle));
   servo2.write(180-Angles.angle1);
   servo3.write(135+Angles.angle2);
   servo4.write(Angles.gridangle);
   servo5.write(135+Angles.angle3);
}
