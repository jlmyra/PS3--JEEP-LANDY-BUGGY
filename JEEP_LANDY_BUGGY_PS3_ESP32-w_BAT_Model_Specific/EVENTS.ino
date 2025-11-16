
void onEvent(){

  // Update watchdog timer - we received a controller event
  lastEventTime = millis();

//*****************Left Joystick - Motor Speed & Direction***********************
//Pushing the joystick forward creates negative values from 0 to -128
//Those values get mapped to 2^8 bits or 0 to -256.
//When the joystickPos values go negative or less than zero we want to ramp up the motor speed and 
//create forward motion.
//After all of the 'if' tests motorSpeed and joystickPos values are stored in new variables oldMotorSpeed
//and oldJoystickPos as 'previous values" so that joystick movement can be determined
  //    =============================================================
  //    |         Forward       |                                   |  
  //    |          -128         |                                   |
  //    |           /\          |               /\                  |
  //    |        <  ly  >       | Left -128  <  rx  > +128 Right    |
  //    |           \/          |               \/                  |
  //    |          +128         |                                   |
  //    |        Reverse        |                                   |
  //    |                       |                                   |
  //    |      LEFT JOYSTICK    |         RIGHT JOYSTICK            |
  //    |         Motion        |            Steering               |
  //    =============================================================

//***Read the left joystick***
    if((abs(Ps3.event.analog_changed.stick.ly) > JOYSTICK_DEADZONE)){
      joystickPos = (Ps3.data.analog.stick.ly);
      motorSpeed = map(joystickPos, -128, 128, -256, 256);

//***FORWARD***
    if(joystickPos < -JOYSTICK_DEADZONE){

      motorSpeed = -motorSpeed; //change sign of motorSpeed

      // Determine speed multiplier based on L1 button (nitro vs normal)
      float speedMultiplier = Ps3.data.button.l1 ? nitroSpeed : normalSpeed;

      targetDirection = LOW; //Cytron MD-13S DIR pin LOW for forward
      targetMotorSpeed = constrain(motorSpeed * speedMultiplier, 0, 255); //Apply speed and bounds check

      DEBUG_PRINT("Moved the left stick FORWARD ");
      DEBUG_PRINT(Ps3.data.button.l1 ? "NITRO:" : "NORMAL:");
      DEBUG_PRINT(" x="); DEBUG_PRINT(Ps3.data.analog.stick.lx);
      DEBUG_PRINT(" y="); DEBUG_PRINT(Ps3.data.analog.stick.ly);
      DEBUG_PRINT("  targetSpeed="); DEBUG_PRINT(targetMotorSpeed);
      DEBUG_PRINT("  joystickPos="); DEBUG_PRINTLN(joystickPos);
      }

//***REVERSE*** - motorSpeed is greater than 0
    else if(joystickPos > JOYSTICK_DEADZONE ){

      // Determine speed multiplier based on L1 button (nitro vs normal)
      float speedMultiplier = Ps3.data.button.l1 ? nitroSpeed : normalSpeed;

      targetDirection = HIGH; //Cytron MD-13S DIR pin HIGH to reverse motor direction
      targetMotorSpeed = constrain(motorSpeed * speedMultiplier, 0, 255); //Apply speed and bounds check

      DEBUG_PRINT("Moved the left stick REVERSE ");
      DEBUG_PRINT(Ps3.data.button.l1 ? "NITRO:" : "NORMAL:");
      DEBUG_PRINT(" x="); DEBUG_PRINT(Ps3.data.analog.stick.lx);
      DEBUG_PRINT(" y="); DEBUG_PRINT(Ps3.data.analog.stick.ly);
      DEBUG_PRINT("  targetSpeed="); DEBUG_PRINT(targetMotorSpeed);
      DEBUG_PRINT("  joystickPos="); DEBUG_PRINTLN(joystickPos);
      }
      
//***STOP***
    if(joystickPos > -JOYSTICK_DEADZONE && joystickPos < JOYSTICK_DEADZONE){

      motorSpeed = 0;
      targetMotorSpeed = 0; // Smoothly decelerate to zero

      DEBUG_PRINT("Moved the left stick STOP:");
      DEBUG_PRINT(" x="); DEBUG_PRINT(Ps3.data.analog.stick.lx);
      DEBUG_PRINT(" y="); DEBUG_PRINT(Ps3.data.analog.stick.ly);
      DEBUG_PRINT("  targetSpeed="); DEBUG_PRINT(targetMotorSpeed);
      DEBUG_PRINT("  joystickPos="); DEBUG_PRINTLN(joystickPos);
      }
    }

//***********************************************************************************
//*****************END Left Joystick - Motor Speed & Direction***********************
//***********************************************************************************





//**********************************************************************************
//*******************Right Joystick - Steering Servo Control************************
//**********************************************************************************

    // Consolidated steering control - handles both left and right movement
    if(abs(Ps3.event.analog_changed.stick.rx) > JOYSTICK_DEADZONE ){
        int joystick_Pos;
        int x_position = Ps3.data.analog.stick.rx;   //uses joystick x position for steering left/right
        joystick_Pos = map(x_position, -128, 128, servoMin, servoMax); //map joystick values to servo values
        steerServo.write(joystick_Pos);
        DEBUG_PRINT("Moved the right stick:");
        DEBUG_PRINT(" x="); DEBUG_PRINT(Ps3.data.analog.stick.rx);
        DEBUG_PRINT(" y="); DEBUG_PRINTLN(Ps3.data.analog.stick.ry);
        DEBUG_PRINT("x_Position=  "); DEBUG_PRINT(x_position); DEBUG_PRINT("  joystick_Pos= ");
        DEBUG_PRINTLN(joystick_Pos);
      }

//**************************************************************************************
//*******************END Right Joystick - Steering Servo Control************************
//**************************************************************************************


//**************************************************************************************
//*************************Winch Motor - UNWIND - REWIND Winch**************************
//**************************************************************************************

    if((Ps3.event.analog_changed.button.up )) {
       DEBUG_PRINT("Pressing the up button: ");
       digitalWrite(winchPWMChannel_1, LOW);
       ledcWrite(winchPWMChannel_2, Ps3.data.analog.button.up * WINCH_SPEED_MULTIPLIER);
       DEBUG_PRINTLN(Ps3.data.analog.button.up);

       if (Ps3.data.analog.button.up < 1) {
          digitalWrite(winchPWMChannel_1, LOW);
          digitalWrite(winchPWMChannel_2, LOW);
       }
    }

    if((Ps3.event.analog_changed.button.down )) {
       DEBUG_PRINT("Pressing the down button: ");
       digitalWrite(winchPWMChannel_2, LOW);
       ledcWrite(winchPWMChannel_1, Ps3.data.analog.button.down * WINCH_SPEED_MULTIPLIER);
       DEBUG_PRINTLN(Ps3.data.analog.button.down);

       if (Ps3.data.analog.button.down < 1) {
          digitalWrite(winchPWMChannel_1, LOW);
          digitalWrite(winchPWMChannel_2, LOW);
       }
    }
    
//***************************************************************************************
//**********************END Winch Motor - UNWIND - REWIND Winch**************************
//***************************************************************************************



//******************************END void onEvent*****************************************
