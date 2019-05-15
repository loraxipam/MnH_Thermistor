/* Moles n Halitosis NTC example. This shows how to use your thermistor */

#include <MnH_Thermistor.h>

// A thermistor on analog port 3
MnH_Thermistor  thermo3(A3);

void setup(){
  delay(1000);
  Serial.begin(115200);
  delay(60);
  Serial.println("***** Moles and Halitosis Thermistor Example *****");
  Serial.println("V2\t\tT2C\t\tT2F\t\tV3\t\tV3max\t\tV3min");
}


void loop(){
  // Read the thermistor and show the raw analog value
  int t3val = thermo3.readRawSensor();

  // Print the values and maximum and minimum for thermo3
  Serial.print(t3val);Serial.print("\t\t");
  Serial.print(thermo3.maxRaw());Serial.print("\t\t");
  Serial.print(thermo3.minRaw());Serial.println("");

  delay(500);
}
