#include <Arduino.h>

const int M_PWM_1 = 33; 
const int M_PWM_2 = 27; 
const int M_SHUTDOWN = 32; 
const int inputPin = 5;    // signal for the vibrations

// Function to play custom frequency and intensity
void playHaptic(int intensity, int freqDelay, int durationMs) {
  unsigned long startTime = millis();
  while (millis() - startTime < durationMs) {
    analogWrite(M_PWM_1, intensity);
    digitalWrite(M_PWM_2, LOW);
    delay(freqDelay);
    
    analogWrite(M_PWM_1, 0); // Pulse off for frequency oscillation
    delay(freqDelay);
  }
}

void setup() {
    Serial.begin(9600);
    delay(1000); 
    Serial.println("TITAN Core Initializing...");

    pinMode(M_PWM_1, OUTPUT);
    pinMode(M_PWM_2, OUTPUT);
    pinMode(M_SHUTDOWN, OUTPUT);
  
  // WAKE UP the DRV8212 driver [cite: 171]
    digitalWrite(M_SHUTDOWN, HIGH); 
    Serial.println("Motor Driver Awakened");
}

void loop() {
    if (Serial.available() > 0) {
        char dangerLevel = Serial.read();
        if (dangerLevel == '1') {      // BUNNY/WIND: Fast, light tickle
            playHaptic(60, 2, 200);
            delay (800);   
        } 
        else if (dangerLevel == '2') { // HUMAN: Medium sharp pulses
            for(int i=0; i<2; i++) {
                playHaptic(150, 15, 300);
                delay(100);
            }
            delay(200);
        } 
        else if (dangerLevel == '3') { // BEAR: Heavy, slow rumble
            playHaptic(255, 60, 1000);  
        } else{
            delay(1000);
        }
    }
}
