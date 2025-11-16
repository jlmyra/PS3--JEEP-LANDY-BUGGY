

#include <Ps3Controller.h>
#include "esp_adc_cal.h"
#include "Model_Variables.h"

//****************Debug Configuration******************************/
// Set to 1 to enable debug Serial output, 0 to disable for better performance
#define DEBUG_MODE 1

#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
//****************END Debug Configuration******************************/

//****************Configuration Constants******************************/
// Joystick deadzone threshold
constexpr uint8_t JOYSTICK_DEADZONE = 2;

// Winch speed multiplier (0.0 to 1.0)
constexpr float WINCH_SPEED_MULTIPLIER = 0.8;

// Battery monitoring constants
constexpr uint16_t ADC_SAMPLE_COUNT = 500;           // Number of ADC samples for averaging
constexpr uint8_t ADC_SAMPLE_INTERVAL_MS = 2;        // Milliseconds between ADC samples
constexpr float BATTERY_VOLTAGE_FULL = 8.4;          // Fully charged battery voltage
constexpr float BATTERY_VOLTAGE_MIN = 6.7;           // Minimum safe battery voltage

// Battery voltage thresholds for LED indicators
constexpr float BATTERY_LEVEL_HIGH = 8.0;       // 4 LEDs
constexpr float BATTERY_LEVEL_MED_HIGH = 7.8;   // 3 LEDs threshold (upper)
constexpr float BATTERY_LEVEL_MED = 7.3;        // 2 LEDs threshold
constexpr float BATTERY_LEVEL_LOW = 6.7;        // 1 LED threshold (critical)

// Low battery rumble warning delay (in seconds)
constexpr uint8_t LOW_BATTERY_RUMBLE_DELAY = 14;    // Rumble after 15 seconds (14+1)

// Connection watchdog timeout (milliseconds)
// If no controller events received for this duration, stop motors for safety
constexpr unsigned long CONNECTION_TIMEOUT_MS = 500;  // 0.5 seconds
//****************END Configuration Constants******************************/

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
int motorSpeed = 0;           // Intermediate calculation variable
float nitroSpeed = 1.0;       // Speed multiplier for nitro mode
float normalSpeed = 0.6;      // Speed multiplier for normal mode
int8_t joystickPos = 0;       // Joystick position (-128 to +128)

//*********************Motor Smoothing Variables***********************/
uint8_t currentMotorSpeed = 0;      // Current actual motor speed (smoothed) 0-255
uint8_t targetMotorSpeed = 0;       // Target motor speed from joystick 0-255
uint8_t targetDirection = LOW;      // Target motor direction (LOW=forward, HIGH=reverse)
uint8_t currentDirection = LOW;     // Current motor direction

// Ramping rates - adjust these values to control acceleration/deceleration speed
// Higher values = faster acceleration/deceleration
// Lower values = smoother but slower transitions
uint8_t accelerationRate = 3;       // Speed increase per update cycle
uint8_t decelerationRate = 5;       // Speed decrease per update cycle (usually faster than accel)

unsigned long lastRampUpdate = 0;
unsigned long rampUpdateInterval = 10; // Update motor speed every 10ms (100 times per second)

// Connection watchdog - tracks last controller activity for safety
unsigned long lastEventTime = 0;

//*********************End Motor Ramp Variables***********************/

//****************Battery Voltage Alarm******************************/
uint8_t batStatusLED = 0; //Variable to hold rover battery status on controller (LED pattern)

// Battery Voltage Corrections
uint32_t R1 = MV_R1;
uint32_t R2 = MV_R2;

float vOutMax = BATTERY_VOLTAGE_FULL * R2 / (R1 + R2); // Calculate output of voltage divider with fully charged batteries
float mSlope = 1 / (vOutMax / BATTERY_VOLTAGE_FULL); //Calculate slope of voltage correction curve

float adcRead = 0;
float batteryVoltage = 0; //computed battery voltage
float batteryVoltageSum = 0;
float batteryVoltageAvg = 0;
float batteryVoltageCorr = 0;
float batCorrFactor = MV_batCorrFactor;

uint16_t analogReadCounter = 0;

constexpr uint8_t batPin = 32; //GPIO32 - ADC1_CH4 Variable that holds the GPIO Address
unsigned long previousMillis = 0; //for timer reading A0
unsigned long interval = ADC_SAMPLE_INTERVAL_MS; // millis between read A0

uint8_t rumbleCounter = 0;

// ADC Calibration - initialized once in setup() for efficiency
esp_adc_cal_characteristics_t adc_chars;

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
  // Calibrate ADC once at startup for efficiency (instead of every read)
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);

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
    // If disconnected, ensure motors are stopped for safety
    targetMotorSpeed = 0;
    currentMotorSpeed = 0;
    ledcWrite(driveMotorChannel, 0);
  return;

  }

  // Connection watchdog - if no events received recently, stop motors for safety
  unsigned long currentMillis = millis();
  if (lastEventTime > 0 && (currentMillis - lastEventTime) > CONNECTION_TIMEOUT_MS) {
    targetMotorSpeed = 0;  // Will smoothly decelerate via updateMotorRamping()
    DEBUG_PRINTLN("*** WATCHDOG: No controller events - stopping motors ***");
  }

  updateMotorRamping();     // Update motor speed smoothly
  computeBatteryVoltage();
}
 



 
 



   
