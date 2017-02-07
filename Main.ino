
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
int csvEnabled = 0, csvRemote = 0;

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
        while(Serial1.available()){
            Serial1.read();
        }
        switch (in)
         {
            case 's':
                system_state = RUNNING;
                Serial1.println("RUNNING");
            break;
            case 'x':
                system_state = READY;
                resetSystem(); 
                Serial1.println("READY");
            break;
            case 'r':
                Serial1.println("RESET");
                resetSystem();
            break;
            case 'e':
                Serial1.println("CSV ENABLED");
                csvRemote = 1;
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
            if(ENABLE_SENSOR) watchGait();

            // Reset Trigger
            trigger_time = micros() + GAIT_PERIOD;

            // Print the acceleration vectors
            if(csvEnabled && DEBUG)
                printVectorCSV();
            if(csvRemote)
                printVectorCSVBluetooth();
        }
    }

}

void resetSystem(){
    csvEnabled = 0; csvRemote = 0;
    valid_packet_number = 0;
}

void sendStepMessage(unsigned long* stepDuration)
{

}
