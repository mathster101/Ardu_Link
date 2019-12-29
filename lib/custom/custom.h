#include <Arduino.h>

/*
* general data container
*/
struct telemdata{
  float voltage;
  float altitude;
  float roll,pitch,yaw;
  float temp;
} telemetry;

float * offsets();
void led_control();
float getBattery();
void printall();

/* 
*initialize serial and pins
*/
void inits(){
    pinMode(A0,INPUT);
    Serial.begin(115200);
};

/*
 * Print current data samples
*/
void printall(telemdata a){
    Serial.print("Battery Level = ");
    Serial.println(a.voltage);
    Serial.print("Altitude = ");
    Serial.println(a.altitude);
    Serial.print("Temp = ");
    Serial.println(a.temp);
    Serial.println("Roll Pitch Yaw");
    Serial.print(a.roll);Serial.print(' ');Serial.print(a.pitch);Serial.print(' ');Serial.println(a.yaw);
};

/*
 * Reads LiPo voltage.
 * @return The charge level in %age
*/
float getBattery(){         
    float level = 0;  
    float vol = analogRead(A0);
    vol = vol *5 / 1024;
    vol = (12 + 6.8) * vol /6.8;
    level = vol;
    return level;
}