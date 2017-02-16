
#include <Arduino.h>
#include "wiring_private.h" // pinPeripheral() function
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif
#include "anklet.h"

enum SYSTEM_STATE system_state = READY;

unsigned char valid_packet_number = 0;
unsigned long trigger_time = 0;
int csvEnabled = 0, csvLoggingEnabled = 0;

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/

void setup(void)
{
    if(DEBUG)
    {
        while (!Serial);  // required for Flora & Micro
        delay(500);

        Serial.begin(115200);
        Serial.println(F("Gait System Test Software"));
        Serial.println(F("------------------------------------------------"));
        Serial.println("Waiting for connection...");
    }
    Serial1.begin(9600);

    if(ENABLE_SENSOR) initializeSensor();
}

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
            case 'S':
                system_state = RUNNING;
                resetSystem();
                Serial1.println(RUNNING_MESSAGE);
            break;
            case 'X':
                system_state = READY;
                resetSystem();
                Serial1.println(READY_MESSAGE);
            break;
            case 'R':
                Serial1.println(RESET_MESSAGE);
                resetSystem();
            break;
            case 'E':
                Serial1.println(CSVLOG_MESSAGE);
                csvLoggingEnabled = 1;
            break;
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
                case 's':
                    system_state = RUNNING;
                    resetSystem();
                    Serial.println("RUNNING");
                break;
                case 'x':
                    system_state = READY;
                    resetSystem();
                    Serial.println("READY");
                break;
                case 'r':
                    Serial.println("RESET");
                    resetSystem();
                break;
                case 'e':
                    Serial.println("CSV ENABLED");
                    csvEnabled = 1;
                break;
            }
        }
    }

    /**
     * Run the main processing loop at 50 HZ
     */
    if(system_state == RUNNING)
    {
        if(trigger_time <= micros())
        {
            if(ENABLE_SENSOR && !csvEnabled && !csvLoggingEnabled) watchGait();

            // Reset Trigger
            trigger_time = micros() + SAMPLING_PERIOD;

            // Print the acceleration vectors
            if(csvEnabled && DEBUG)
                printVectorCSV();
            if(csvLoggingEnabled)
                printVectorCSVBluetooth();
        }
    }

}

void resetSystem(){
    csvEnabled = 0;
    csvLoggingEnabled = 0;
    valid_packet_number = 0;
    resetGait();
}
