# _Moles and Halitosis_ Thermistor library

  This library provides an easy-to-use interface for an analog thermistor. It gives a simple interface for collecting the sensor readings and smoothing the data. You can read the value directly or report back the average or geometric mean of the latest N readings. There are functions to return the minimum and maximum values that the sensor has reported which are helpful for determining the thermistor's response curve. Once you do know the response curve of your thermistor, you can ask it to report the temperature directly either in Celsius or Fahrenheit. Some helper functions are provided, two to convert between degrees C and degress F as well as a simple function to provide the parameters for a linear response curve based on your empirical testing of the sensor.

# Functions

  The following functions are provided by this library. Most are used simply for retrieving and reporting the sensor values, but there are some helper functions that deal with temperatures and "thermistory" requirements.

## Constructors

  ```c++
  MnH_Thermistor(byte analogPin)
  MnH_Thermistor(byte analogPin, bool inverted)
  MnH_Thermistor(byte analogPin, bool inverted, float slope, float intercept)
  ```

## Sensor functions
  ```c++
  float  readSensor()        // read the sensor and return the running average
  int readRawSensor()        // read and return the sensor, calculate running average
  int        maxRaw()        // maximum senor value seen. See notes.
  int        minRaw()        // minimum senor value seen. See notes.
  float  currentAvg()        // current average sensor values
  float currentMean()        // current geometric mean sensor values
  void  setConverter(float m, float b)   // configure thermistor response curve
  float        degC()        // current temperature in Celsius
  float        degF()        // current temperature in Fahrenheit
  ```

## Helper functions

  ```c++
  static const float degCtoDegF(float temp)
  static const float degFtoDegC(float temp)
  static const void  converterHelper(int rawVal1, float tempC1, int rawVal2, float tempC2)
  ```

# Discussion

  Three constructors provide a simple way to create thermistor objects on a given analog pin. They provide an "inverter" in case your wiring or your device require an inversion of the slope. In addition, if you know the response curve of a given sensor, you can supply the slope and intercept for that when creating the object.

  The object provides a smoothing array which can be resized by adjusting the SMOOTHERSIZE directive. If you read the sensor using the `readSensor()` function, then the array will be updated and you'll get the smoothed average back from the function. In contrast, if you read the sensor using the `readRawSensor()` function, the behavior is the same in terms of updating the smoothing array, but the return value is the actual sensor value, not the array value. Notice, too, that the results returned from `maxRaw()` and `minRaw()` are impacted by this. If you use only `readSensor()` then the min/max functions will return only the min/max as seen by the average.
