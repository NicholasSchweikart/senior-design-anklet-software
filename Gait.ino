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

enum GAIT_STATE gaitState = WAIT_INIT_LIFT_OFF;

char steps = 0;
double lastYAccel = 0;

unsigned long stepDuration = 0, blockTime = 0, averageStepDuration = 0;

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

void readAcceleration(double* y)
{
    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2
    imu::Vector<3> accel = NULL;

    accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    *y = accel.y();

    return;
}

void watchGait()
{
    // block for Xms after changing states
    if(blockTime)
    {
        if(micros() < blockTime)
            return;
        else
            blockTime = 0;
    }

    double yAcceleration;
    readAcceleration(&yAcceleration);

    if(gaitState == WAIT_INIT_LIFT_OFF )
    {
        // Detect LIFT OFF
        if(yAcceleration < LIFT_OFF_DETECTION_THRESHOLD)
        {
            // We have a HEEL_STRIKE_DETECTED so start monitoring
            if(DEBUG == 2) Serial.println("Initial LIFT_OFF_DETECTED!");

            // Get current time to start duration timer
            stepDuration = micros();

            // Block to filter out noisy spikes
            blockTime = micros() + BLOCK_DURATION;

            gaitState = SWING_PHASE;
        }

    }
    else if (gaitState == SWING_PHASE)
    {
        // Detect HEEL STRIKE
        if(yAcceleration < HEEL_STRIKE_DETECTION_THRESHOLD)
        {
            // Wait for HEEL STRIKE
            if(DEBUG == 2) Serial.println("HEEL_STRIKE_DETECTED");

            // Block to filter out noisy spikes
            blockTime = micros() + BLOCK_DURATION;

            gaitState = WAIT_LIFT_OFF;
        }
    }
    else if (gaitState == WAIT_LIFT_OFF)
    {
        // Detect LIFT OFF
        if(yAcceleration < LIFT_OFF_DETECTION_THRESHOLD)
        {
            // We have a HEEL_STRIKE_DETECTED so start monitoring
            if(DEBUG == 2) Serial.println("LIFT_OFF_DETECTED: send step message");

            // Get the time for the entire step
            stepDuration = micros() - stepDuration;

            // Send of the gait data, and reset everything
            steps += 1;
            averageStepDuration += stepDuration;

            // Reset the timers
            stepDuration = micros();

            // Block to filter out noisy spikes
            blockTime = micros() + BLOCK_DURATION;

            gaitState = SWING_PHASE;
        }
    }

    if(steps >= STEP_SAMPLE_SIZE)
    {
        averageStepDuration = averageStepDuration/STEP_SAMPLE_SIZE;
        //sendStepMessage(&averageStepDuration);
        steps = 0; averageStepDuration = 0;
    }

    if(gaitState > WAIT_INIT_LIFT_OFF)
    {
        // If stepDuration is larger than 2 seconds, take us back to idle.
        if((micros() - stepDuration ) > MAX_STRIDE_TIME)
        {
            // Clear & Reset all the vars
            stepDuration = 0;  steps = 0; averageStepDuration = 0;
            gaitState = WAIT_INIT_LIFT_OFF;
            if(DEBUG == 2) Serial.println("Detected stop of walk!");
        }
    }

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
