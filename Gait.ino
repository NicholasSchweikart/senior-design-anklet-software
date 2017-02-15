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

int samples = 0, updates = 0;
double sum = 0.0, average = 0.0;
double lastYAccel = 0.0;

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
        average = sum/samples;
        if(average < 0.0){
            average *= -1.0;
        }

        Serial1.println(average, 2);
        if(DEBUG) Serial.println(average, 2);
        samples = 0;
        sum = 0;
    }

    imu::Vector<3> accelerationVector = NULL;
    accelerationVector = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    double yA = accelerationVector.y();
    // double xA = accelerationVector.x();
    // double zA = accelerationVector.z();
    // double output = sqrt(yA*yA + xA*xA + zA*zA);

    // Compute filtered data.
    lastYAccel = ALPHA * yA + ALPHA_I * lastYAccel;
    
    sum += lastYAccel;
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
    double y,x,z;
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
    sum = 0;
    samples = 0;
}
