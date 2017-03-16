#include <Arduino.h>
#include "wiring_private.h" // pinPeripheral() function
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif
#include "anklet.h"

enum SYSTEM_STATE system_state = READY;

unsigned long trigger_time = 0;             // Holds micros for next trigger
int csvEnabled = 0, csvLoggingEnabled = 0;  // Ouput control flags

// A small print helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/**
 * Description: Does preliminary setup of device. Runs once at power up, or
 * after reset.
 * Args: void
 */
void setup(void)
{
    if(DEBUG)
    {
        while (!Serial);  // wait for the serial to be ready
        delay(500);

        Serial.begin(115200);
        Serial.println(F("Gait System In DEBUG"));
        Serial.println(F("------------------------------------------------"));
        Serial.println("Waiting for connection...");
    }
    Serial1.begin(9600);

    if(ENABLE_SENSOR) initializeSensor(); // Enable the BNO055
}

/**
 * Description: Main loop that will run forever. It parses incomming commands
 * from both Bluetooth and Seial. It also monitors the timer and runs the data
 * processing at the specified frequency.
 */
void loop(void)
{
    /**
     * Parse Bluetooth Serial Commands
     */
    if(Serial1.available() > 0)
    {
        char in = Serial1.read();
        switch (in)
         {
            // Start command
            case 'S':
                system_state = RUNNING;
                resetSystem();
                Serial1.println(RUNNING_MESSAGE);
            break;

            // Stop command.
            case 'X':
                system_state = READY;
                resetSystem();
                Serial1.println(READY_MESSAGE);
            break;

            // Reset command
            case 'R':
                Serial1.println(RESET_MESSAGE);
                resetSystem();
            break;

            // Enable CSV output command
            case 'E':
                Serial1.println(CSVLOG_MESSAGE);
                csvLoggingEnabled = 1;
            break;

            // Disable CSV output command
            case 'D':
                Serial1.println(CSVDIS_MESSAGE);
                csvLoggingEnabled = 0;
            break;
        }
    }

    if(DEBUG)
    {
        /**
         * Parse USB Serial Commands
         */
        if(Serial.available() > 0)
        {
            char in = (char)Serial.read();
            while(Serial.available()){
                Serial.read();
            }
            switch (in)
             {
                case 'S':
                    system_state = RUNNING;
                    resetSystem();
                    Serial.println("RUNNING");
                break;
                case 'X':
                    system_state = READY;
                    resetSystem();
                    Serial.println("READY");
                break;
                case 'R':
                    Serial.println("RESET");
                    resetSystem();
                break;
                case 'E':
                    Serial.println("CSV ENABLED");
                    csvEnabled = 1;
                break;
            }
        }
    }

    /**
     * Run the main processing at 50 HZ
     */
    if(system_state == RUNNING)
    {
        if(trigger_time <= micros())
        {
            if(ENABLE_SENSOR && !csvEnabled && !csvLoggingEnabled)
              watchGaitPeaks();

            // Reset Trigger
            trigger_time = micros() + SAMPLING_PERIOD;

            // Print the acceleration vectors if applicable
            if(csvEnabled && DEBUG)
                printVectorCSV();

            if(csvLoggingEnabled)
                printVectorCSVBluetooth();
        }
    }

}

/**
 * Func: Resets the flags for the system main loop 
 */
void resetSystem(){
    csvEnabled = 0;
    csvLoggingEnabled = 0;
    resetGait();
}
