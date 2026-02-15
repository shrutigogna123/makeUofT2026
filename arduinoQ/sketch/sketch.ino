#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>



int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)
int iterator = 0;

int triggerPin = 3; // New pin to talk to the ESP32
const int txTOESP32 = 1; // hardware tx pin - for serial sending to esp32

Adafruit_MPU6050 mpu;


void setup() {
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Bridge.begin();
  Monitor.begin(9600); // Set the baud rate (e.g., 9600)
  Serial.begin (9600); // Serial to talk to the esp32

  
  Monitor.println("Monitoring started!");
  if (!mpu.begin()) {
    Monitor.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  // Set sensitivity for seismic (vibration) detection
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // Highest sensitivity
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);   // Smooths out electrical noise
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);

}


void loop(){
  //mpu.update();
  Serial.write("0");
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH

  if (state == LOW) {
    Monitor.print ("Motion detected!\t");
    state = HIGH;      
    }
  }
  else {
    
     if (state == HIGH){
       Monitor.print ("Motion stopped!\t");
       state = LOW;      
   }
 }
 //delay(200);

 sensors_event_t a, g, temp;
 mpu.getEvent(&a, &g, &temp);

 float threshold = 10.0; // m/s² for walking


 float totalAcc = sqrt(a.acceleration.x*a.acceleration.x +
                      a.acceleration.y*a.acceleration.y +
                      a.acceleration.z*a.acceleration.z);
 // if (totalAcc > threshold) {
 //   //Monitor.println("Footstep detected!");
 //   // Monitor.println("");
 //   //Monitor.print(totalAcc);
 //   //Monitor.print ("\t");
 // }
  
  if (totalAcc > threshold && state == HIGH){
    Monitor.print ("3");
    Serial.write ('3');
  } else if (state == HIGH){
    Monitor.print ("1");
    Serial.write ('1');
  } else if (totalAcc > threshold){
    Serial.write ('2');
    Monitor.print ("2");
  } else {
    Serial.write('0');
    Monitor.print("0");
  }
  iterator = iterator + 1;
  if (iterator == 4){
    Monitor.println ("**");
    iterator == 0;
  } else {
    Monitor.println ("");
  }

  delay (200);

}



