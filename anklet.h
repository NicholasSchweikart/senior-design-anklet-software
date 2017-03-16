/**
 * Header file for Gait-Pro Anklet software.
 */
#ifndef ANKLET_H
#define ANKLET_H


#define DEBUG                           0       // Enable Debug Output
#define OFFLINE                         0       // Enable Use w/o Bluetooth
#define ENABLE_SENSOR                   1       // Enable BNO550

#define SAMPLING_PERIOD                 20000   // Period of data sampline
#define AVGING_RATE                     25      // Samples to take before averaging

// Message Response Values
#define RUNNING_MESSAGE                 "#U"
#define READY_MESSAGE                   "#R"
#define RESET_MESSAGE                   "#Z"
#define CSVLOG_MESSAGE                  "#C"
#define CSVDIS_MESSAGE                  "#D"

enum SYSTEM_STATE
{
    READY,
    RUNNING
};

// Function prototypes
void sendStepMessage(unsigned long* stepDuration);
void readAcceleration(double* y);
#endif
