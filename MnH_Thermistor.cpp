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

#include <Arduino.h>
#include <MnH_Thermistor.h>

/** Assuming ten bit analog reads **/
#define _INV(VAL) (_inverted ? (1023 - (VAL)) : (VAL))

MnH_Thermistor::MnH_Thermistor(){}

/** A thermistor on an analog pin. */
MnH_Thermistor::MnH_Thermistor(byte analogPin)      {
    pinMode(analogPin, INPUT);
    _analogPin = analogPin;
                                                    }

/** The analog pin to read the resistor differential and if the sensor is inverted.
*   An inverted sensor has the sensor on the positive side of the resistor, or may be a PTC.
*/
MnH_Thermistor::MnH_Thermistor(byte analogPin, bool inverted)       {
    pinMode(analogPin, INPUT);
    _analogPin = analogPin;
    _inverted = inverted;
                                                                    }

/** A thermistor on an analog pin whose slope you know. */
MnH_Thermistor::MnH_Thermistor(byte analogPin, bool inverted, float slope, float intercept) {
    pinMode(analogPin, INPUT);
    _analogPin = analogPin;
    _inverted  = inverted;
    setConverter(slope, intercept);
                                                                                            }

/**
 * Read the sensor, update the smoothed array and return the *avg* value
 * @return current average based on a new sensor reading
 */
float MnH_Thermistor::readSensor()                                  {
    int sensorValue = analogRead(_analogPin);
    sensorValue = _INV(sensorValue);
    _runningAvg = calculateCurrentAvg(sensorValue);
    // notice that readSensor applies max/min from the avg
    _runningRawMax = max(_runningRawMax,(int)_runningAvg);
    _runningRawMin = min(_runningRawMin,(int)_runningAvg);
    return _runningAvg;     // <-returns the running average
                                                                    }

/**
 * This reads the sensor and gives *that* value. It also updates the array
 *
 * If you want to keep the array clean, just read the pin yourself.
 * @return the current sensor value, not the array average
 */
int MnH_Thermistor::readRawSensor()                                     {
    int sensorValue = analogRead(_analogPin);
    sensorValue = _INV(sensorValue);
    _runningAvg = calculateCurrentAvg(sensorValue);
    // notice that readRawSensor applies max/min from raw
    _runningRawMax = max(_runningRawMax,sensorValue);
    _runningRawMin = min(_runningRawMin,sensorValue);
    return sensorValue;     // <-returns the raw reading
                                                                        }

/**
 * What is the maximum value that the sensor has seen?
 * @return max sensor value seen so far
 */
int MnH_Thermistor::maxRaw()                                            {
    return _runningRawMax;
                                                                        }

/**
 * What is the minimum value that the sensor has seen?
 * @return min sensor value seen so far
 */
int MnH_Thermistor::minRaw()                                            {
    return _runningRawMin;
                                                                        }

/**
 * What is the current average sensor reading?
 * @return avg sensor variable value
 */
float MnH_Thermistor::currentAvg()                                          {
    return _runningAvg;
                                                                            }

/**
 * Just return the geometric mean of the array
 * @return geometric mean of sensor array values
 */
float MnH_Thermistor::currentMean()                                         {
    float f = 0.0;
    int arrayIndex = 0;
    for (arrayIndex = 0; arrayIndex < SMOOTHERSIZE; arrayIndex++){
        if (avgValues[arrayIndex] < 1) break;
        f += log(avgValues[arrayIndex]);
    }
    f = exp(f/(float)arrayIndex);
    return f;
                                                                            }

/**
 * Calculate and return the average of the array
 * @return average of the sensor array values
 */
float MnH_Thermistor::calculateCurrentAvg()                                 {
    float v = 0.0;
    int arrayIndex = 0;
    for (arrayIndex = 0; arrayIndex < SMOOTHERSIZE; arrayIndex++){
        if (avgValues[arrayIndex] < 1) break;
        v += (float)avgValues[arrayIndex];
    }
    v /= (float)arrayIndex;
    return v;
                                                                            }


/**
 * Push the new reading into the array and calculate the new average
 * @return avg of the new sensor array values
 */
float MnH_Thermistor::calculateCurrentAvg(int sensorReading)                {
    static unsigned int avgValuesCurrentIndex;
    avgValues[avgValuesCurrentIndex++ % SMOOTHERSIZE] = sensorReading;

    return calculateCurrentAvg();
                                                                            }


/**
 * Set the slope and intercept for the specific thermistor...yeah, you'll
 * have to cipher these yerself, Jethro...naught plus naught equals naught.
 * FYI, I've had NTCs that had m from -0.1228 to -0.07667 and b from,
 * respectively, 200.6 to 456.7 in the "fridge to hot room" ranges.
 */
void MnH_Thermistor::setConverter(float m, float b) {
    // y = mx + b where x is the analog value and y is the tempC
    slope = m;
    intercept = b;
                                                    }


/**
 * What's the current temperature in centigrade?
 */
float MnH_Thermistor::degC()                        {
    if (slope == 0.0) return -999.999;
    return ( intercept + (_runningAvg * slope) );
                                                    }


/**
 * What's the current temperature in fahrenheit?
 */
float MnH_Thermistor::degF()                        {
    if (slope == 0.0) return -999.999;
    return MnH_Thermistor::degCtoDegF(degC());
                                                    }


/**
 * Handy tools to convert temperatures.
 */
static const float MnH_Thermistor::degCtoDegF(float temp)   {
    return ((temp * 1.8) + 32.0);
                                                            }


static const float MnH_Thermistor::degFtoDegC(float temp)   {
    return ((temp - 32.0) / 1.8);
                                                            }

static const void MnH_Thermistor::converterHelper(int rawVal1, float tempC1, int rawVal2, float tempC2) {
  float dX, dY, slope;
  String retval = "slope: ";
  bool transform;

  dX = rawVal2 - rawVal1;
  dY = tempC2  - tempC1;
  slope = dX / dY;

  transform = retval.concat(slope);
  transform = retval.concat(", intercept: ");
  transform = retval.concat(tempC1 - slope * rawVal1);

  // if (Serial.available() > 0) {
    Serial.println("rawVal1\ttempC1\trawVal2\ttempC2");
    Serial.print(rawVal1); Serial.print("\t");
    Serial.print(tempC1); Serial.print("\t");
    Serial.print(rawVal2); Serial.print("\t");
    Serial.print(tempC2); Serial.println("\t");
    if (transform) Serial.println(retval);
  // }

}
