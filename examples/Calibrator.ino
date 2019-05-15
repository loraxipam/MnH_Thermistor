/* Moles n Halitosis NTC calibration example. This shows how to calibrate your thermistor */

#include <MnH_Thermistor.h>

// A thermistor on analog port 2 and you know its temperature curve
MnH_Thermistor  thermo2(A2, -0.1522, 123.2, false);

// Another thermistor on analog port 3 but figure its curve
MnH_Thermistor  thermo3(A3);

void setup(){
  delay(1000);
  Serial.begin(115200);
  delay(60);
  Serial.println("***** Moles and Halitosis Thermistor Calibration Example *****");
  Serial.println("Put the sensors on your desk and wait for V3min to settle. Measure the temp (celsius).");
  Serial.println("Now put them in your freezer and measure the temperature there. Wait for V3max to settle.");
  Serial.println("Use these values to figure your temperature curve using the converterHelper() function.");
  Serial.println("You don't need two sensors, you know. You can go olde fashioned and use a real thermometer.\n");
  // Here's my real thermistor values as an example.
  MnH_Thermistor::converterHelper(623, 12.2, 448, 33.9);
  Serial.println("");

  Serial.println("V2\t\tT2C\t\tT2F\t\tV3\t\tV3max\t\tV3min");
}


void loop(){
  // Read the thermistors
  float t2val = thermo2.readSensor();
  float t3val = thermo3.readSensor();

  // Print the values and temperatures of thermo2
  Serial.print(t2val);Serial.print("\t\t");
  Serial.print(thermo2.degC());Serial.print("\t\t");
  Serial.print(MnH_Thermistor::degCtoDegF(thermo2.degC()));Serial.print("\t\t");

  // Print the values and maximum and minimum for thermo3
  Serial.print(t3val);Serial.print("\t\t");
  Serial.print(thermo3.maxRaw());Serial.print("\t\t");
  Serial.print(thermo3.minRaw());Serial.println("");

  delay(500);
}
