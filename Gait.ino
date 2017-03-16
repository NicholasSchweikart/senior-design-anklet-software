/**
* Gait analysis engine for the Anklet
*
=========== Connections for BNO055 ================
Connect SCL to analog 5
Connect SDA to analog 4
Connect VDD to 3.3V DC
Connect GROUND to common ground
*
* ======== BNO055 Axis =========================
* --------------     ----------------------------------------
* |O   -x ^   O|     |                                      |
* |       |   o|     |                                      |
* | -y    |   o|     |             Micro Controller         |
* |  <----Z   o|     |                                      |
* |           o|     |                                      |
* |O          O|     |                                      |
* --------------     -----------------------------------------
* ~Positive Z is out of screen.
*/

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "anklet.h"

Adafruit_BNO055 bno = Adafruit_BNO055();

int samples = 0;
double sum = 0.0, average = 0.0, lastY = 0.0, minY = 0.0, lastSlope = 0.0;

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

/**
* Func: Continually looks for peaks in the acceleration data comming in. When
* a peak is detected, it will send off the MAXIMUM value of that peak over the
* Bluetooth link.
* NOTE: Always sends the ABS over the link
*/
void watchGaitPeaks()
{
  imu::Vector<3> accelerationVector = NULL;
  accelerationVector = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  double y = accelerationVector.y();

  // Filter out noise below 5 m/s^2
  if(y > -5.0)
  {
    y = 0;
  }

  // Update the MINIMUM if necessary. Remeber MIN is the MAX because -Y is
  // the upward axis.
  if(y < minY)
  minY = y;

  // Calculate the slope
  double slope = y - lastY;

  // If our slope switches signs, we mark a peak.
  if(lastSlope < 0 && slope > 0)
  {
    if(minY < 0)
    {
      minY *= -1;
    }

    Serial1.println(minY);
    if(DEBUG){ Serial.println(minY);}

    // Reset minY to 0 for next measurement.
    minY = 0;
  }

  lastSlope = slope;
  lastY = y;
}

/**
* Func: Prints out the x,y,z acceleration values from the BNO055 over the
* Serial Link.
* Format: "X,Y,Z\n"
*/
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

/**
* Func: Prints out the x,y,z acceleration values from the BNO055 over the
* Bluetooth link.
* Format: "X,Y,Z\n"
*/
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

/**
* Func: Resets the variables for the gait data collection.
*/
void resetGait(){
  lastY = 0.0;
  minY = 0.0;
  lastSlope = 0.0;
}
