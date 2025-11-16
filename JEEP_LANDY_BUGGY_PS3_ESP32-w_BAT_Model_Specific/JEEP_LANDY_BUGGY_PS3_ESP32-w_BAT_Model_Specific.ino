

#include <Ps3Controller.h>
#include "esp_adc_cal.h"
#include "Model_Variables.h"

//****************Motor PWM******************************/

uint32_t freq = 30000;  //PWM Frequency
uint8_t driveMotorChannel = 14;//changed line 96 espservo.h to 2 servos max leaving channels 2-16 available for motors
uint8_t resolution = 8; //8 bit resolution PWM
int driveMotorPin = 25; //Cytron white wire (pwm) GPIO25
int driveMotorDirection = 27;//Cytron yellow wire (DIR) GPIO27

//****************END Motor PWM******************************/

//****************Winch Motor PWM******************************/

uint8_t winchPWMChannel_1 = 4;
uint8_t winchPWMChannel_2 = 2;

//int winchPWMPin = 26; //Winch Unwind  A1A
//int winchDirectionPin = 18; //Winch Rewind A1B

int winchDirection_1 = 26; //GPIO26
int winchDirection_2 = 18; //GPIO18

//****************END Winch Motor PWM******************************/

//****************Steering Servo******************************/

#include <ESP32Servo.h>  //changed line 96 espservo.h to 2 servos max leaving channels 2-16 availble for motors

Servo steerServo;  // create servo object to control a servo
                   // 2 servo objects can be created on the ESP32
int servoPin = 4; //  GPIO4
// ServoMin and ServoMax can be used to 'align' the front wheels so that they are centered when the joystick position
// is zeroed. On my vehicles the range is 800 between wheels with different range endpoints. Your vehicle may
// required different endpoints to get the wheels centered.

int servoMin = MV_servoMin;// Drivers Side  range 800
int servoMax = MV_servoMax;// Passenger side

//****************END Steering Servo******************************/

//*********************Motor Speed Variables***********************/
int motorSpeed = 0;
int motorSpeedSlow = 0;
float nitroSpeed = 1;
float normalSpeed = 0.6;
int joystickPos = 0;

//*********************Motor Smoothing Variables***********************/
int currentMotorSpeed = 0;      // Current actual motor speed (smoothed)
int targetMotorSpeed = 0;       // Target motor speed from joystick
int targetDirection = LOW;      // Target motor direction (LOW=forward, HIGH=reverse)
int currentDirection = LOW;     // Current motor direction

// Ramping rates - adjust these values to control acceleration/deceleration speed
// Higher values = faster acceleration/deceleration
// Lower values = smoother but slower transitions
int accelerationRate = 3;       // Speed increase per update cycle
int decelerationRate = 5;       // Speed decrease per update cycle (usually faster than accel)

unsigned long lastRampUpdate = 0;
unsigned long rampUpdateInterval = 10; // Update motor speed every 10ms (100 times per second)

//*********************End Motor Ramp Variables***********************/

//****************Battery Voltage Alarm******************************/
int batStatusLED = 0; //Variable to hold rover battery status on controller

// Battery Voltage Corrections
int R1 = MV_R1;
int R2 = MV_R2;

float vOutMax = 8.4 * R2 / (R1 + R2); // Calculate output of voltage divider with fully charged batteries
float mSlope = 1 / (vOutMax / 8.4); //Calculate slope of voltage correction curve

float adcRead = 0;
float batteryVoltage = 0; //computed battery voltage
float batteryVoltageSum = 0;
float batteryVoltageAvg = 0;
float batteryVoltageCorr = 0;
float batCorrFactor = MV_batCorrFactor;

unsigned long analogReadCounter = 0;

const int batPin =  32; //GPIO32 - ADC1_CH4 Variable that holds the GPIO Address 
unsigned long previousMillis = 0; //for timer reading A0
unsigned long interval = 2; // millis between read A0

int rumbleCounter = 0;

//****************END Battery Voltage Alarm******************************/

  ESP32PWM pwm;

  void setup() {
 
  Serial.begin(250000);
 
  if (!Ps3.begin("b8:27:eb:37:85:b9")) { //This is unique to my controllers - you will have to change this for yours
    Serial.println("Initialization failed.");
    return;
    }
    else
    { 
  Serial.println();
  Serial.println();
  Serial.println("***********************************************************");
  Serial.println("***Initialization finished. Ready to pair PS3 controller***");
  Serial.println("***********************************************************");
  Serial.println();
  Serial.println();
    }
 
  Ps3.attach(onEvent);
  Ps3.attachOnConnect(onConnection);
  
// Battery Meter Setup
  pinMode(batPin, INPUT);
  analogSetPinAttenuation(batPin, ADC_0db);

// Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
//Drive Motor and Winch PWM Setup
  pinMode(driveMotorPin, OUTPUT); //Must have
  pinMode(driveMotorDirection, OUTPUT); //Must have
  
  pinMode(winchDirection_1, OUTPUT); //Must have
  pinMode(winchDirection_2, OUTPUT); //Must have

  
  ledcSetup(driveMotorChannel, freq, resolution);
  ledcAttachPin(driveMotorPin, driveMotorChannel); // GPIO25

  ledcSetup(winchPWMChannel_1, freq, resolution);
  ledcAttachPin(winchDirection_1, winchPWMChannel_1);

  ledcSetup(winchPWMChannel_2, freq, resolution);
  ledcAttachPin(winchDirection_2, winchPWMChannel_2);

// Steering Servo Setup
 
  steerServo.setPeriodHertz(300);    // standard 50 hz servo
  steerServo.attach(servoPin, servoMin, servoMax); // attaches the servo on pin 4 to the servo object
  
// ServoMin and ServoMax can be used to 'align' the front wheels so that they are centered when the joystick position
// is zeroed. On my vehicles the range is 800 between wheels with different range endpoints. Your vehicle may
// required different endpoints to get the wheels centered.
  
}

//*******************Motor Ramping Function for Smooth Acceleration/Deceleration***********************
void updateMotorRamping() {
  unsigned long currentMillis = millis();

  // Only update at specified interval (default 10ms)
  if (currentMillis - lastRampUpdate < rampUpdateInterval) {
    return;
  }
  lastRampUpdate = currentMillis;

  // Handle direction changes - need to ramp down to zero before changing direction
  if (targetDirection != currentDirection && currentMotorSpeed > 0) {
    // Ramp down to zero before direction change
    if (currentMotorSpeed > decelerationRate) {
      currentMotorSpeed -= decelerationRate;
    } else {
      currentMotorSpeed = 0;
      currentDirection = targetDirection; // Change direction when stopped
    }
  }
  // Normal ramping when direction matches or motor is stopped
  else {
    currentDirection = targetDirection;

    // Ramp speed up or down toward target
    if (currentMotorSpeed < targetMotorSpeed) {
      // Accelerating
      currentMotorSpeed += accelerationRate;
      if (currentMotorSpeed > targetMotorSpeed) {
        currentMotorSpeed = targetMotorSpeed; // Don't overshoot
      }
    }
    else if (currentMotorSpeed > targetMotorSpeed) {
      // Decelerating
      currentMotorSpeed -= decelerationRate;
      if (currentMotorSpeed < targetMotorSpeed) {
        currentMotorSpeed = targetMotorSpeed; // Don't undershoot
      }
    }
  }

  // Apply smoothed speed to motor
  digitalWrite(driveMotorDirection, currentDirection);
  ledcWrite(driveMotorChannel, currentMotorSpeed);
}
//*******************End Motor Ramping Function***********************

void loop() {

  if(!Ps3.isConnected()){

  return;

  }

  updateMotorRamping();     // Update motor speed smoothly
  computeBatteryVoltage();
}
 



 
 



   
