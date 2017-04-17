/**
* Header file for Gait-Pro Anklet software.
*/

/*
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
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
