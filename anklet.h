#ifndef ANKLET_H
#define ANKLET_H

#define DEBUG                           1
#define OFFLINE                         0
#define ENABLE_SENSOR                   1

#define GAIT_PERIOD                     20000
#define SAMPLING_PERIOD                 200

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
