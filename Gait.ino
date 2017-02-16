/**
 * Gait analysis engine for the anklet
 * Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground
 */

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "anklet.h"

Adafruit_BNO055 bno = Adafruit_BNO055();

int samples = 0;
double sum = 0.0, average = 0.0, lastY = 0.0;

void initializeSensor()
{
    /* Initialise the sensor */
    if(!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }else{
        bno.setExtCrystalUse(true);
    }
}

void watchGait()
{
    if(samples == AVGING_RATE)
    {
        double output = 0.0;
        if(lastY < 0.0){
            output = lastY * -1.0;
        }

        Serial1.println(output, 2);
        if(DEBUG) Serial.println(output, 2);
        samples = 0;
    }

    imu::Vector<3> accelerationVector = NULL;
    accelerationVector = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    lastY = ALPHA * accelerationVector.y() + ALPHA_I * lastY;
    samples += 1;
}

void printVectorCSV()
{
    double y,x,z;
    imu::Vector<3> accel = NULL;

    accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    y = accel.y();
    x = accel.x();
    z = accel.z();
    Serial.print(x,2); Serial.print(',');
    Serial.print(y,2); Serial.print(',');
    Serial.println(z,2);
}
void printVectorCSVBluetooth()
{
    double y = 0.0,x=0.0,z=0.0;
    imu::Vector<3> accel = NULL;

    accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    y = accel.y();
    x = accel.x();
    z = accel.z();
    Serial1.print(x,2); Serial1.print(',');
    Serial1.print(y,2); Serial1.print(',');
    Serial1.println(z,2);
}
void resetGait(){
    lastY = 0.0;
}
