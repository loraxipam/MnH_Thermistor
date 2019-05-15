/***************************************************
  This is a Moles & Halitosis library.   Soli Deo gloria.
  Control your: THERMISTOR
  License: MIT
  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  * and associated documentation files (the "Software"), to deal in the Software without
  * restriction, including without limitation the rights to use, copy, modify, merge, publish,
  * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
  * Software is furnished to do so, subject to the following conditions: The above copyright notice
  * and this permission notice shall be included in all copies or substantial portions of the
  * Software.
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  ****************************************************/

#ifndef _MnH_THERMISTOR_H
#define _MnH_THERMISTOR_H

#include <Arduino.h>

#define SMOOTHERSIZE 4
/**
    The basic 10 bit analog negative thermal coefficient (NTC) thermistor

    This has some smoothing to the readings to eliminate spikes.
    If you want to measure "warm" rather than "cool", use the inverted constructor.

    Your typical cheapo NTC will have a room temperature resistance about 10 kilohms,
    a few kilohms on a hot afternoon and down to one or two kilohms on a scorching bench.
    Mine got to 75 kilohm in my freezer (0 degF).

    With a pullup resistor, your sensor (@) will be inversely proportional to temperature.
    ---- GND --- THM --- @ --- 10K OHM --- Vcc ---

 */
class MnH_Thermistor                                                                                  {
 public:
  MnH_Thermistor(byte analogPin);                 /// A sensor on pin X
  MnH_Thermistor(byte analogPin, bool inverted);  /// A sensor on pin X but invert reporting
  MnH_Thermistor(byte analogPin, bool inverted, float slope, float intercept);

  float  readSensor();                            /// read and report the current [avg] value
  int readRawSensor();                            /// read and report the current raw value

  int maxRaw();                                   /// report the max raw value seen
  int minRaw();                                   /// report the min raw value seen

  float currentAvg();                             /// report the current average. Do not read.
  float currentMean();                            /// report the current geo mean. Do not read.

  void  setConverter(float m, float b);           /// Set up the temperature slope formula
  float degC();                                   /// After setting up the slope formula first
  float degF();

  static const float degCtoDegF(float temp);      /// convert degC to degF
  static const float degFtoDegC(float temp);      /// convert degF to degC

  /*
   * Put your NTC in the freezer and check the sensor value. Again on the countertop.
   * If you know those two temps, this will spit out a straight line equation for you,
   * (assuming you will read temps in that range).
  */
  static const void converterHelper(int rawVal1, float tempC1, int rawVal2, float tempC2);

 private:
  MnH_Thermistor();                               /// A blank sensor should never be used
  byte         _analogPin;                        /// The pin to read
  bool         _inverted = false;                 /// to use inverted values
  int _runningRawMax = 0;                         /// Highest raw read value seen
  int _runningRawMin = 1023;                      /// Lowest  raw read value seen
  float _runningAvg = 0.0;                        /// The current "value"
  float intercept = 0.0, slope = 0.0;             /// Temperature slope parameters for this thermistor
  float avgValues[SMOOTHERSIZE];
  float calculateCurrentAvg();                    /// Just calculate avgs from the current array
  float calculateCurrentAvg(int sensorReading);   /// Pushes in a new reading and returns array avg

                                                                                                      };


#endif
