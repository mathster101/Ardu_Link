#include <Arduino.h>
#include <MPU9250.h>


/*
 * general data container
*/
struct telemdata{
  float voltage;
  uint16_t altitude;
  float roll,pitch,yaw;
  uint8_t temp;
} telemetry;

int countled = 0;
float pitch_off,roll_off;
MPU9250 mpu;

void offsets();
void led_control();
float getBattery();
void printall();

/* 
 *initialize serial comms and pin modes
*/
void inits(){
    pinMode(A0,INPUT);
    pinMode(8,OUTPUT); //red led
    pinMode(7,OUTPUT); //green led
    digitalWrite(8,HIGH);
    digitalWrite(7,LOW);
    Serial.begin(115200);
    for(int i =0;i<10;i++){
        digitalWrite(7,!digitalRead(7));
        delay(50);
    }
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
    //vol = vol *5 / 1024;
    //vol = (12 + 6.8) * vol /6.8;
    level = vol;
    return level;
}

/*
 * controls led flashing
*/
void led_control(){
    if(countled == 20){
        digitalWrite(8,HIGH);
        countled = 0;
    }
    else
    {
        countled ++;
    }

    if(countled<2){
        digitalWrite(7,HIGH);
    }
    else
    {
        digitalWrite(7,LOW);
    }
    
    
}

/*
 *calculates initial angle offsets
*/
void offsets(){
    int num = 500;
    roll_off = pitch_off = 0;
    for(int k = 0;k<200;k++){

        delay(2);
        mpu.update();
    }
    for(int k =0;k<num;k++){
        mpu.update();
        roll_off += mpu.getRoll();
        pitch_off += mpu.getPitch();
        //delay(2);
    }
    roll_off /= num;
    pitch_off /= num;
}