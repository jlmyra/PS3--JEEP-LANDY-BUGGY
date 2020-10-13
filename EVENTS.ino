
void onEvent(){
 
//*****************Left Joystick - Motor Speed & Direction***********************

 //***TURBO***
  if((abs(Ps3.event.analog_changed.stick.ly) > 2)  ){      
       int joystickPos = (Ps3.data.analog.stick.ly);
       int motorSpeed = map(joystickPos, -128, 128, -256, 256); 
//FORWARD - 
    if(joystickPos < -2){
       motorSpeed = -motorSpeed; 
       if(Ps3.data.button.l1){ //TURBO
       ledcWrite(driveMotorChannel, (motorSpeed * .6)); //reduce the motor speed by 40%
       }
       if(!Ps3.data.button.l1){ 
       ledcWrite(driveMotorChannel, (motorSpeed * .4)); //reduce the motor speed by 60%
       }
       digitalWrite(driveMotorDirection, LOW);
       Serial.print("Moved the left stick Fast FORWARD:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("***FAST*** FORWARD motorSpeed= "); Serial.println(motorSpeed * .6);
    }
//REVERSE - 
    else if(joystickPos > 2 ){ 
       if(Ps3.data.button.l1){ //TURBO
       ledcWrite(driveMotorChannel, (motorSpeed * .6)); //reduce the motor speed by 40%
       }
       if(!Ps3.data.button.l1){
       ledcWrite(driveMotorChannel, (motorSpeed * .4)); //reduce the motor speed by 60%
       }
       digitalWrite(driveMotorDirection, HIGH);
       Serial.print("Moved the left stick Fast REVERSE:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("***FAST*** REVERSE motorSpeed= "); Serial.println(motorSpeed * .1);
    } 
//NEUTRAL    
    else{
      motorSpeed = 0;
      ledcWrite(driveMotorChannel, (motorSpeed));
      digitalWrite(driveMotorDirection, LOW);
      Serial.print("Moved the left stick Fast NEUTRAL:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("Fast NEUTRAL motorSpeed= "); Serial.println(motorSpeed); 
    }
  }

//*****************END Left Joystick - Motor Speed & Direction***********************

  
//*******************Right Joystick - Steering Servo Control************************  


    if( (Ps3.event.analog_changed.stick.rx) > 2 ){
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

    if( (Ps3.event.analog_changed.stick.rx) < -2 ){
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
//*******************END Right Joystick - Steering Servo Control************************


//**************************Winch Motor - UNWIND - REWIND Winch**************************
    
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
//**********************END Winch Motor - UNWIND - REWIND Winch**************************


//*******************Check controller Connection********************************
 
void onConnection() {
   
  if (Ps3.isConnected()) {
    Serial.println("Controller connected.");
  }
}

//*******************END Check controller Connection********************************
