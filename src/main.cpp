#include <Arduino.h>
#include <Servo.h>

/* Humidity And Thermometer Sensor Pin */
#define FC_28 A0
const int DSdigital = 7;

/* Stepper Motor Pins */  
/*
#define PIN_STEPPER_1_EN 14
#define PIN_STEPPER_1_STEP 8
#define PIN_STEPPER_1_DIR 16
#define PIN_STEPPER_2_STEP 7
#define PIN_STEPPER_2_DIR 21
*/

/* Stepper Motor Pins */  
#define PIN_STEPPER_1_EN 10
#define PIN_STEPPER_1_STEP 9
#define PIN_STEPPER_1_DIR 17
#define PIN_STEPPER_2_STEP 0
#define PIN_STEPPER_2_DIR 0

/* DC Motor Pins */
#define PIN_DC_MOTOR_1_ENABLE 9
#define PIN_DC_MOTOR_1_IN_A 2
#define PIN_DC_MOTOR_1_IN_B 3


#define PIN_DC_MOTOR_2_ENABLE 0
#define PIN_DC_MOTOR_2_IN_A 10
#define PIN_DC_MOTOR_2_IN_B 11

/* Servo Motor Pins */
#define PIN_SERVO_1 13

const String _cmd_run = "RUN";
const String _msg_data = "DATA";
const String _msg_ready = "READY";

/* Stepper STEP Quantities */
#define DRILL_15CM_STEP_COUNT 30
#define PLATE_10CM_STEP_COUNT 30
#define PROBE_15CM_STEP_COUNT 30

// Create a servo object 
Servo myServo; 

void setup() {

  myServo.attach(PIN_SERVO_1); // Specify the pin for the servo

  Serial.begin(9600);
  /* Set Stepper Motor Pins as OUTPUT */
  pinMode(PIN_STEPPER_1_EN, OUTPUT);
  pinMode(PIN_STEPPER_1_STEP, OUTPUT);
  pinMode(PIN_STEPPER_1_DIR, OUTPUT);
  pinMode(PIN_STEPPER_2_STEP, OUTPUT);
  pinMode(PIN_STEPPER_2_DIR, OUTPUT);

  /* Set DC Motor Pins as OUTPUT */
  pinMode(PIN_DC_MOTOR_1_ENABLE, OUTPUT);
  pinMode(PIN_DC_MOTOR_1_IN_A, OUTPUT);
  pinMode(PIN_DC_MOTOR_1_IN_B, OUTPUT);
  pinMode(PIN_DC_MOTOR_2_ENABLE, OUTPUT);
  pinMode(PIN_DC_MOTOR_2_IN_A, OUTPUT);
  pinMode(PIN_DC_MOTOR_2_IN_B, OUTPUT);

  /* Set Servo Motor Pins as OUTPUT */
  pinMode(PIN_SERVO_1, OUTPUT);

  /* Set Directions */
  digitalWrite(PIN_STEPPER_1_EN, LOW);
//  digitalWrite(PIN_STEPPER_1_STEP, LOW);
  digitalWrite(PIN_STEPPER_2_STEP, LOW);
  digitalWrite(PIN_STEPPER_1_DIR, HIGH);
  digitalWrite(PIN_STEPPER_2_DIR, HIGH);
  digitalWrite(PIN_DC_MOTOR_1_IN_A, HIGH);
  digitalWrite(PIN_DC_MOTOR_1_IN_B, LOW);
  digitalWrite(PIN_DC_MOTOR_2_IN_A, HIGH);
  digitalWrite(PIN_DC_MOTOR_2_IN_B, LOW);
  
  /* Set Servo to 0*/
  myServo.write(0);
  
}

void loop() {
  /* Check if _cmd_run has been inputted*/
  if (Serial.available() > 0) {
    String input = Serial.readString();
    if (input == _cmd_run) { // Check if input is equal to _cmd_run
      /* BEGIN SPECTROMETER & PROBE MEASUREMENT */
        analogWrite(PIN_DC_MOTOR_2_ENABLE, 20); // Start Drill

        for (int i = 0; i < DRILL_15CM_STEP_COUNT*2; i++) { // Bring Drill Down 15CM
          digitalWrite(PIN_STEPPER_1_STEP, HIGH);
          delay(20);
          digitalWrite(PIN_STEPPER_1_STEP, LOW);
          delay(20);
        }
        
        for (int i = 0; i < 60; i++) { // Wait 1 Minute
          delay(1000);      
        }

        digitalWrite(PIN_STEPPER_1_DIR, 0); // Set Drill Direction
        for (int i = 0; i < DRILL_15CM_STEP_COUNT*2; i++) { // Bring Drill Up 15CM
          digitalWrite(PIN_STEPPER_1_STEP, HIGH);
          delay(20);
          digitalWrite(PIN_STEPPER_1_STEP, LOW);
          delay(20);
        }

        analogWrite(PIN_DC_MOTOR_2_ENABLE, 0); // Shutdown Drill

        digitalWrite(PIN_STEPPER_2_DIR, 0); // Set Plate Direction
        for (int i = 0; i < PLATE_10CM_STEP_COUNT*2; i++) { // Bring Plate Down 10Cm
          digitalWrite(PIN_STEPPER_2_STEP, HIGH);
          delay(20);
          digitalWrite(PIN_STEPPER_2_STEP, LOW);
          delay(20);
        }

        analogWrite(PIN_DC_MOTOR_1_ENABLE, 20); //Start DC-1
        digitalWrite(PIN_DC_MOTOR_1_IN_A, HIGH);
        digitalWrite(PIN_DC_MOTOR_1_IN_B, LOW);

        for (int i = 0; i < 5; i++) { // Wait 5 Seconds
          delay(1000);
        }

        for (int i = 0; i < PROBE_15CM_STEP_COUNT*2; i++) { // Bring Probe Down 15CM
          digitalWrite(PIN_STEPPER_1_STEP, HIGH);
          delay(20);
          digitalWrite(PIN_STEPPER_1_STEP, LOW);
          delay(20);
        }

         for (int i = 0; i < 60; i++) { // Wait 1 Minute
          delay(1000);
        }

        /* Measure and Send Data */
        uint32_t FCHumiditysensorValue = analogRead(A0);
        uint32_t DSdigitalSensorValue = digitalRead(DSdigital);   
        Serial.write(FCHumiditysensorValue);
        Serial.write(DSdigitalSensorValue);

        Serial.write(_msg_data.c_str(), _msg_data.length());

        digitalWrite(PIN_STEPPER_1_DIR, 1); // Set Probe Direction
        for (int i = 0; i < PROBE_15CM_STEP_COUNT*2; i++) { // Bring Probe Up 15CM
          digitalWrite(PIN_STEPPER_1_STEP, HIGH);
          delay(20);
          digitalWrite(PIN_STEPPER_1_STEP, LOW);
          delay(20);
        }

        analogWrite(PIN_DC_MOTOR_1_ENABLE, 20); //Start DC-1 
        digitalWrite(PIN_DC_MOTOR_1_IN_A, HIGH);
        digitalWrite(PIN_DC_MOTOR_1_IN_B, LOW);

        for (int i = 0; i < 5; i++) {
          delay(1000);
        }

        myServo.write(45);
        
      
        Serial.write(_cmd_run.c_str(), _cmd_run.length());

        
      /* END SPECTROMETER & PROBE MEASUREMENT */
    }
  }
}