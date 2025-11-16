//*************************Compute Battery Voltage function***********************
  // The max voltage of the ESP32 ADC at the analogSetPinAttenuation() of 11db setting is 3.3V (see void setup()). 
  // The ESP32 ADC is non-linear from about 2.6V to 3.3V. A voltage divider is needed to drop the battery voltage
  // from 8.4V (fully charged) to ~ 2.5V where the ADC becomes linear (but noisy). To smooth out the noise the 
  // sketch reads the battery voltage every 2ms, 500 times and computes an average. In other words every second 
  // a new voltage measurement is computed~.The voltage divider uses an R1 of 68k ohms and R2 of ~29,500 ohms. 
  // A 0.1uf capacitor should be connected across Vout and ground to reduce noise.
  // The ESP32 can communicate the status of the battery charge to the PS3 controller by using 
  // the 4 connection leds. LED 4 is full charge, LED 3 is 3/4. LED 2 is 50%. LED 1 means it time to recharge
  // the battery.
  // Full charge is ~8.4V Should not use the battery below 6.7V (80% of 8.4V or 3.2V/cell*2 cells)
  // The ESP32 comes with an embedded ADC calibration curve. This curve is called with this function-
  // readADC_Cal(int ADC_Raw) and it corrects the adc voltage reading accordingly. The computeBatteryVoltage
  // function 
  // see - DeepBlueEmbedded describes a method for using the ESP32 internal reference voltage calibration and 
  // correction.
  // This sketch follows the description here:
  // https://deepbluembedded.com/esp32-adc-tutorial-read-analog-voltage-arduino/ for more detail

  void computeBatteryVoltage(){
    
  unsigned long currentMillis = millis(); //set the current time
  if (currentMillis - previousMillis > interval) { //check to see if the interval has been met
  previousMillis = currentMillis; //reset the time

  if (analogReadCounter < ADC_SAMPLE_COUNT){ //Read the ADC multiple times for averaging
    analogReadCounter = analogReadCounter + 1;
    adcRead = analogRead(batPin); //Read the battery voltage on the ADC and correct it on the next line
    batteryVoltage = readADC_Cal(adcRead); //ADC Calibrated read function - see below returns mV
    batteryVoltage = ((batteryVoltage) * .001) - batCorrFactor; //convert mV to V & add offset to agree with voltmeter
    batteryVoltageSum = batteryVoltageSum + batteryVoltage; //Sum the voltage readings
    //DEBUG_PRINT("batteryVoltageSum= "); DEBUG_PRINTLN(batteryVoltage);
  }

  if (analogReadCounter == ADC_SAMPLE_COUNT){
    //DEBUG_PRINT("analogReadCounter= "); DEBUG_PRINTLN(analogReadCounter);
    batteryVoltageAvg = batteryVoltageSum / ADC_SAMPLE_COUNT; //Compute the Average
    batteryVoltageCorr = (batteryVoltageAvg ) * mSlope; //Correct the ADC reading to Actual Battery Voltage

    DEBUG_PRINT("batteryVoltageAvg= "); DEBUG_PRINT(batteryVoltageAvg); DEBUG_PRINT("\t"); DEBUG_PRINT("batteryVoltageCorr= ");
    DEBUG_PRINT(batteryVoltageCorr); DEBUG_PRINT("  RumbleCounter= "); DEBUG_PRINTLN(rumbleCounter);

    batteryVoltageSum = 0;
    analogReadCounter = 0;
 // Test the battery voltage and set the led's accordingly
  if (batteryVoltageCorr > BATTERY_LEVEL_HIGH) {
    Ps3.setPlayer(batStatusLED);
    batStatusLED = 10; //4 LED's
    rumbleCounter = 0;
    batteryVoltageCorr = 0;
  }
  else if(batteryVoltageCorr <= BATTERY_LEVEL_MED_HIGH && batteryVoltageCorr > BATTERY_LEVEL_MED ) {
    Ps3.setPlayer(batStatusLED);
    batStatusLED = 9; // 3 LEDS
    rumbleCounter = 0;
    batteryVoltageCorr = 0;
  }
  else if(batteryVoltageCorr <= BATTERY_LEVEL_MED && batteryVoltageCorr > BATTERY_LEVEL_LOW ) {
    Ps3.setPlayer(batStatusLED);
    batStatusLED = 7; // 2 LEDS
    rumbleCounter = 0;
    batteryVoltageCorr = 0;
  }
    else if(batteryVoltageCorr < BATTERY_LEVEL_LOW ) {
    Ps3.setPlayer(batStatusLED);
    batStatusLED = 4;
    batteryVoltageCorr = 0;

//************************SET RUMBLE*************************
// if the battery voltage drops below min voltage for sustained period turn on the PS3 Rumble
// to alert the user

   if (rumbleCounter == LOW_BATTERY_RUMBLE_DELAY){  //Delay before activating rumble
   
    ps3_cmd_t cmd;

    cmd.rumble_left_intensity = 0x7d;
    cmd.rumble_right_intensity = 0x7d; //0xff = 255
  
    cmd.rumble_right_duration = 100;
    cmd.rumble_left_duration = 100;
  
    ps3Cmd(cmd);
    //Serial.print("rumbleCounter ="); Serial.println(rumbleCounter);
    rumbleCounter = 13; //keep the rumble on
   }
   
    rumbleCounter++; // rumbleCounter initialized at 0
  
//***********************END SET RUMBLE*********************
      } // else if <6.7
     } // if analogReadCounter
    } // if currentMillis
  } // void computeBatteryVoltage

//***********************ADC Read Calibration Function*********************
// Optimized version - uses global adc_chars struct initialized once in setup()
// instead of re-calibrating on every read (500x/sec improvement)

   uint32_t readADC_Cal(int ADC_Raw){
   return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
  }

//************************END Compute Battery Voltage function***********************
