#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <MPU9250.h>
#include <custom.h>

/* Object initializations*/

RF24 radio(9,10); // CE, CSN
Adafruit_BMP280 bmp;

const byte address[6] = "00001"; //remember to change



void setup() {
  bmp.begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  //radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_250); /* Standby time. */
  mpu.setup();
  telemetry.yaw = telemetry.pitch = telemetry.roll = 0;
  telemetry.temp = telemetry.altitude = telemetry.voltage = 0; 
  Serial.println("Initializations complete");
  inits();
  offsets();
}

int state = 0;
void loop() {
    float t1 = millis();
    mpu.update();
    
    switch(state){
      case 0 : telemetry.altitude = bmp.readAltitude(); 
              state = 1;
              break;
      case 1 : telemetry.temp = bmp.readTemperature();
              state = 0;
              break;
    }
    
    telemetry.yaw = mpu.getYaw();
    telemetry.roll = mpu.getRoll() - roll_off;
    telemetry.pitch = mpu.getPitch() - pitch_off;
    telemetry.voltage = getBattery();
    
    radio.writeFast(&telemetry,sizeof(telemdata));
    //Serial.println(millis() - t1);
    led_control();
    printall(telemetry);
    //delay(20);
}