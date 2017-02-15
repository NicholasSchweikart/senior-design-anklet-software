#ifndef ANKLET_H
#define ANKLET_H

#define DEBUG                           0
#define OFFLINE                         0
#define ENABLE_SENSOR                   1

#define SAMPLING_PERIOD                 20000
#define AVGING_RATE                     1000
#define ALPHA                           .03
#define ALPHA_I                         .97

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
