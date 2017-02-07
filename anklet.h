#ifndef ANKLET_H
#define ANKLET_H

#define DEBUG                           0
#define OFFLINE                         0
#define ENABLE_SENSOR                   1

#define GAIT_PERIOD                     20000
#define SAMPLING_PERIOD                 .0166
#define MAX_STRIDE_TIME                 3000000             // 2 seconds
#define HEEL_STRIKE_DETECTION_THRESHOLD -3
#define LIFT_OFF_DETECTION_THRESHOLD    -2

#define STEP_SAMPLE_SIZE                3
#define STEP_MESSAGE_LENGTH             6

#define BLOCK_DURATION                  250000               // 200 ms
#define GRAVITY                         9.82

enum SYSTEM_STATE
{
    READY,
    RUNNING
};

enum GAIT_STATE
{
    WAIT_INIT_LIFT_OFF,
    WAIT_LIFT_OFF,
    WAIT_HEEL_STRIKE,
    SWING_PHASE
};

// Function prototypes
void sendStepMessage(unsigned long* stepDuration);
void readAcceleration(double* y);
#endif
