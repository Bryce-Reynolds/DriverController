#include <Arduino.h>
#include "pedal.h"

// Constants for scaling
const float ACC_BASELINE_SCALE = 0.9;
const float PEDAL_VALUE_SCALE = 0.6;

// DESC:     updates the pedal value in the drive object
// INPUT:    pass by reference the drive object
// OUT:      the pedal member of drive will be written to,
//           the reversed member of drive will be read from
void update_drive_pedal() {
    // Read raw values
    int baseRaw = analogRead(ACC_BASELINE);
    int pedalRaw = analogRead(ACC_PEDAL);

    // Calculate min and max thresholds
    int max = baseRaw * ACC_BASELINE_SCALE;
    int min = ACC_PEDAL_MIN;

    float pedalValue = pedalRaw;

    // Check for invalid pedal values
    if ((baseRaw < (pedalValue - 50)) || (pedalRaw < min)) {
        Serial.println("Error: PedalFault!");
        drive.pedalFault = true;
        return;
    }
    //Serial.println(pedalValue);   //DEBUG

    // Scale pedal value to range [0, 1]
    pedalValue -= min;
    pedalValue /= (max - min);

    // Debugging output
    Serial.print(pedalRaw);
    Serial.print(" | ");
    Serial.print(baseRaw);
    Serial.print(" | ");

    // Invert and constrain pedal value, then scale it
    pedalValue = 1 - pedalValue;
    pedalValue = constrain(pedalValue, 0, 1) * PEDAL_VALUE_SCALE;

    Serial.println(pedalValue);

    // Update pedal value in drive object
    drive.pedal = pedalValue;
}

