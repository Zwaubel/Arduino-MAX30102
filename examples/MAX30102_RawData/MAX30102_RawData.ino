/*
  Arduino-MAX30102 oximetry / heart rate integrated sensor library by Shivam Gupta (gupta.shivam1996@gmail.com)

  Based on MAX30100 library, Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>
  All alogrithms and methods used are from the above author,
  I have only modified this enough to make it work with the new MAX30102 sensor.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// The example shows how to retrieve raw values from the sensor
// experimenting with the most relevant configuration parameters.
// Use the "Serial Plotter" app from arduino IDE 1.6.7+ to plot the output

#include <Wire.h>
#include "MAX30102.h"

// Sampling is tightly related to the dynamic range of the ADC.
// refer to the datasheet for further info
#define SAMPLING_RATE                       MAX30102_SAMPRATE_400HZ

// The LEDs currents must be set to a level that avoids clipping and maximises the
// dynamic range
#define IR_LED_CURRENT                      0x16
#define RED_LED_CURRENT                     0x1b

// The pulse width of the LEDs driving determines the resolution of
// the ADC (which is a Sigma-Delta).
// set HIGHRES_MODE to true only when setting PULSE_WIDTH to MAX30102_SPC_PW_411US_18BITS
#define PULSE_WIDTH                         MAX30102_SPC_PW_411US_18BITS
#define HIGHRES_MODE                        true


// Instantiate a MAX30102 sensor class
MAX30102 sensor;
uint32_t ir, red;

void setup()
{
  Serial.begin(115200);

  // Initialize the sensor
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!sensor.begin()) {
    Serial.println("MAX30102 INIT FAILED");
    while (true);
  }
  Serial.println("IR\tRed");

  // Set up the wanted parameters
  sensor.setMode(MAX30102_MODE_SPO2_HR);
  //sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
  sensor.setIRLedCurrent(IR_LED_CURRENT);
  sensor.setRedLedCurrent(RED_LED_CURRENT);
  sensor.setLedsPulseWidth(PULSE_WIDTH);
  sensor.setSamplingRate(SAMPLING_RATE);
  sensor.setSampleAveraging(MAX30102_SMP_AVE_16);
  sensor.setRangeADC(MAX30102_ADCRange_16384);
}

void loop()
{
  sensor.update();

  while (sensor.getRawValues(&ir, &red)) {
    Serial.print(ir);
    Serial.print('\t');
    Serial.println(red);
  }
}