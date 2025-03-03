#include <Arduino.h>

#define MOTOR_A_R_EN 2  
#define MOTOR_A_L_EN 3   
#define MOTOR_B_R_EN 4   
#define MOTOR_B_L_EN 5  

#define MOTOR_A_PWM1 6  
#define MOTOR_A_PWM2 7  
#define MOTOR_B_PWM1 8
#define MOTOR_B_PWM2 9  

int right_wheel_speed = 0;
int left_wheel_speed = 0;
float linear_x = 0.0;
float angular_z = 0.0;

void forward();
void backward();
void turn_right();
void turn_left();
void stop_motors();
void calc_wheel_speed();
void parseSerialData(String data);

void setup() {
    Serial.begin(115200);
    pinMode(MOTOR_A_R_EN, OUTPUT);
    pinMode(MOTOR_A_L_EN, OUTPUT);
    pinMode(MOTOR_B_R_EN, OUTPUT);
    pinMode(MOTOR_B_L_EN, OUTPUT);

    pinMode(MOTOR_A_PWM1, OUTPUT);
    pinMode(MOTOR_A_PWM2, OUTPUT);
    pinMode(MOTOR_B_PWM1, OUTPUT);
    pinMode(MOTOR_B_PWM2, OUTPUT);

    digitalWrite(MOTOR_A_R_EN, HIGH); 
    digitalWrite(MOTOR_A_L_EN, HIGH);  
    digitalWrite(MOTOR_B_R_EN, HIGH);  
    digitalWrite(MOTOR_B_L_EN, HIGH);  
}

void loop() {
    if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        parseSerialData(data);
        calc_wheel_speed();     

        if (linear_x > 0) {
            forward();  
        } else if (linear_x < 0) {
            backward();
        } else {
            stop_motors();
        }

        if (angular_z > 0) {
            turn_right(); 
        } else if (angular_z < 0) {
            turn_left(); 
        }
    }
}

void parseSerialData(String data) {
    int linearStart = data.indexOf("LX:") + 3;
    int linearEnd = data.indexOf(",", linearStart);
    int angularStart = data.indexOf("AZ:") + 3;

    if (linearStart != -1 && linearEnd != -1 && angularStart != -1) {
        linear_x = data.substring(linearStart, linearEnd).toFloat();
        angular_z = data.substring(angularStart).toFloat();
    }
}

void calc_wheel_speed() {
    right_wheel_speed = linear_x + angular_z;
    left_wheel_speed = linear_x - angular_z;

    // Hızları sınırla (0 ile 255 arasında) 
    right_wheel_speed = constrain(right_wheel_speed, -255, 255);
    left_wheel_speed = constrain(left_wheel_speed, -255, 255);
}


void forward() {
    // Araç ileri hareketi
    analogWrite(MOTOR_A_PWM1, right_wheel_speed);  // Sağ motoru ileri
    analogWrite(MOTOR_A_PWM2, 0);                        // Sağ motor geri
    analogWrite(MOTOR_B_PWM1, left_wheel_speed);   // Sol motoru ileri
    analogWrite(MOTOR_B_PWM2, 0);                        // Sol motor geri
}

void backward() {
    // Araç geri hareketi
    analogWrite(MOTOR_A_PWM1, 0);                        // Sağ motoru geri
    analogWrite(MOTOR_A_PWM2, -right_wheel_speed);  // Sağ motor ileri
    analogWrite(MOTOR_B_PWM1, 0);                        // Sol motoru geri
    analogWrite(MOTOR_B_PWM2, -left_wheel_speed);   // Sol motor ileri
}

void stop_motors() {
    // Motorları durdur
    analogWrite(MOTOR_A_PWM1, 0);
    analogWrite(MOTOR_A_PWM2, 0);
    analogWrite(MOTOR_B_PWM1, 0);
    analogWrite(MOTOR_B_PWM2, 0);
}

void turn_left() {
    // Sol dönme hareketi
    analogWrite(MOTOR_A_PWM1, 0);                        // Sağ motoru geri
    analogWrite(MOTOR_A_PWM2, -right_wheel_speed);  // Sağ motor ileri
    analogWrite(MOTOR_B_PWM1, left_wheel_speed);   // Sol motoru ileri
    analogWrite(MOTOR_B_PWM2, 0);                        // Sol motor geri
}

void turn_right() {
    // Sağ dönme hareketi
    analogWrite(MOTOR_A_PWM1, right_wheel_speed);  // Sağ motoru ileri
    analogWrite(MOTOR_A_PWM2, 0);                        // Sağ motor geri
    analogWrite(MOTOR_B_PWM1, 0);                        // Sol motoru geri
    analogWrite(MOTOR_B_PWM2, -left_wheel_speed);   // Sol motor ileri
}

