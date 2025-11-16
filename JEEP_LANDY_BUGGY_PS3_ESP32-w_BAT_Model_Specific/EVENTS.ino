
void onEvent(){
 
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
    if((abs(Ps3.event.analog_changed.stick.ly) > 2)){      
      joystickPos = (Ps3.data.analog.stick.ly);
      motorSpeed = map(joystickPos, -128, 128, -256, 256); 
      motorSpeedSlow = map(joystickPos, -128, 128, -256, 256);
    
//***FORWARD***
    if(joystickPos < -2){

      motorSpeed = -motorSpeed; //change sign of motorSpeed

      if(Ps3.data.button.l1){ //*****NITRO****** - from pushing L1 button

      targetDirection = LOW; //Cytron MD-13S DIR pin LOW for forward
      targetMotorSpeed = (motorSpeed * nitroSpeed); //increase the motor speed by nitroSpeed

      Serial.print("Moved the left stick FORWARD NITRO:");
      Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
      Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
      Serial.print("  targetSpeed="); Serial.print(targetMotorSpeed);
      Serial.print("  joystickPos="); Serial.print(joystickPos);
      Serial.println();
      }

      if(!Ps3.data.button.l1){ //*****NORMAL SPEED***** - L1 not pushed
      targetDirection = LOW; //Cytron MD-13S DIR pin LOW for forward
      targetMotorSpeed = (motorSpeed * normalSpeed); //reduce the motor speed by normalSpeed

      Serial.print("Moved the left stick FORWARD NORMAL:");
      Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
      Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
      Serial.print("  targetSpeed="); Serial.print(targetMotorSpeed);
      Serial.print("  joystickPos="); Serial.print(joystickPos);
      Serial.println();
      }}

//***REVERSE*** - motorSpeed is greater than 0
    else if(joystickPos > 2 ){

      if(Ps3.data.button.l1){ //******NITRO********* - L1 Depressed

      targetDirection = HIGH; //Cytron MD-13S DIR pin HIGH to reverse motor direction
      targetMotorSpeed = (motorSpeed * nitroSpeed); //increase the motor speed by nitroSpeed

      Serial.print("Moved the left stick REVERSE NITRO:");
      Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
      Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
      Serial.print("  targetSpeed="); Serial.print(targetMotorSpeed);
      Serial.print("  joystickPos="); Serial.print(joystickPos);
      Serial.println();
      }

      if(!Ps3.data.button.l1){  //*******NORMAL SPEED********* - L1 not pushed

      targetDirection = HIGH; //Cytron MD-13S DIR pin HIGH to reverse motor direction
      targetMotorSpeed = (motorSpeed * normalSpeed); //reduce the motor speed by normalSpeed

      Serial.print("Moved the left stick REVERSE NORMAL:");
      Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
      Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
      Serial.print("  targetSpeed="); Serial.print(targetMotorSpeed);
      Serial.print("  joystickPos="); Serial.print(joystickPos);
      Serial.println();
      }}
      
//***STOP***
    if(joystickPos > -2 && joystickPos < 2){

      motorSpeed = 0;
      targetMotorSpeed = 0; // Smoothly decelerate to zero

      Serial.print("Moved the left stick STOP:");
      Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
      Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
      Serial.print("  targetSpeed="); Serial.print(targetMotorSpeed);
      Serial.print("  joystickPos="); Serial.print(joystickPos);
      Serial.println();
      }
    }

//***RESET VARIABLES***            
     // oldMotorSpeed = motorSpeed; //update oldMotorSpeed
      //motorSpeedSlow = motorSpeed + 100;
      //oldMotorSpeedSlow = oldMotorSpeed + 100; //Add 100 to oldMotorSpeed to allow for noticeable slowdown
      //oldJoystickPos = joystickPos; //update oldJoystickPos
      
//***********************************************************************************  
//*****************END Left Joystick - Motor Speed & Direction***********************
//***********************************************************************************





//**********************************************************************************
//*******************Right Joystick - Steering Servo Control************************ 
//**********************************************************************************

    if((Ps3.event.analog_changed.stick.rx) > 2 ){
        int joystick_Pos;
        int x_position = Ps3.data.analog.stick.rx;   //uses joystick x position for steering left/right
        joystick_Pos = map(x_position, -128, 128, servoMin, servoMax); //map joystick Blynk values to servo values
        steerServo.write(joystick_Pos);
        Serial.print("Moved the right stick:");
        Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
        Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
        Serial.println();
        Serial.print("x_Position=  "); Serial.print(x_position); Serial.print("  joystick_Pos= "); 
        Serial.println(joystick_Pos);        
      }   

    if((Ps3.event.analog_changed.stick.rx) < -2 ){
        int joystick_Pos;
        int x_position = Ps3.data.analog.stick.rx;   //uses joystick x position for steering left/right
        joystick_Pos = map(x_position, -128, 128, servoMin, servoMax); //map joystick Blynk values to servo values
        steerServo.write(joystick_Pos);  
        Serial.print("Moved the right stick:");
        Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);//serial.print DECimal
        Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
        Serial.println();
        Serial.print("x_Position=  "); Serial.print(x_position); Serial.print("  joystick_Pos= "); 
        Serial.println(joystick_Pos); 
    }

//**************************************************************************************
//*******************END Right Joystick - Steering Servo Control************************
//**************************************************************************************


//**************************************************************************************
//*************************Winch Motor - UNWIND - REWIND Winch**************************
//**************************************************************************************
   
    if((Ps3.event.analog_changed.button.up )) {
       Serial.print("Pressing the up button: ");
       digitalWrite(winchPWMChannel_1, LOW);
       ledcWrite(winchPWMChannel_2, Ps3.data.analog.button.up * .8);
       Serial.println(Ps3.data.analog.button.up, DEC);

    if (Ps3.data.analog.button.up < 1) 
       digitalWrite(winchPWMChannel_1, LOW);
       digitalWrite(winchPWMChannel_2, LOW);     
      }
    
    if((Ps3.event.analog_changed.button.down )) {
       Serial.print("Pressing the down button: ");
       digitalWrite(winchPWMChannel_2, LOW);
       ledcWrite(winchPWMChannel_1, Ps3.data.analog.button.down * .8);
       Serial.println(Ps3.data.analog.button.down, DEC);
   
    if (Ps3.data.analog.button.down < 1) 
       digitalWrite(winchPWMChannel_1, LOW);
       digitalWrite(winchPWMChannel_2, LOW);
      }
    }
    
//***************************************************************************************
//**********************END Winch Motor - UNWIND - REWIND Winch**************************
//***************************************************************************************



//******************************END void onEvent*****************************************
