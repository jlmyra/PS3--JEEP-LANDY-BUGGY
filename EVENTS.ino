


void onEvent(){
 
  //*****************Left Joystick - Motor Speed & Direction***********************
/*
 
Ps3.event.button_down.____
Ps3.event.button_up.
Ps3.data.button.
Ps3.event.analog_changed.button.___


    if( Ps3.event.button_down.l3 )
        Serial.println("Started pressing the left stick button");
    if( Ps3.event.button_up.l3 )
        Serial.println("Released the left stick button");

   if( abs(Ps3.event.analog_changed.button.l1)){
       Serial.print("Pressing the left shoulder button: ");
       Serial.println(Ps3.data.analog.button.l1, DEC);
   }
   if( Ps3.data.button.cross && Ps3.data.button.down )
        Serial.println("Pressing both the down and cross buttons");
    if( Ps3.data.button.square && Ps3.data.button.left )
        Serial.println("Pressing both the square and left buttons");
    if( Ps3.data.button.triangle && Ps3.data.button.up )
        Serial.println("Pressing both the triangle and up buttons");
    if( Ps3.data.button.circle && Ps3.data.button.right )
        Serial.println("Pressing both the circle and right buttons");
        */
 //***TURBO***
  if((abs(Ps3.event.analog_changed.stick.ly) > 2)  ){      
       int joystickPos = (Ps3.data.analog.stick.ly);
       int motorSpeed = map(joystickPos, -128, 128, -256, 256); 
//FORWARD - TURBO
    if(joystickPos < -2){
       motorSpeed = -motorSpeed; 
       if(Ps3.data.button.l1){ 
       ledcWrite(driveMotorChannel, (motorSpeed * .8)); //reduce the motor speed by 40%
       }
       if(!Ps3.data.button.l1){ 
       ledcWrite(driveMotorChannel, (motorSpeed * .4)); //reduce the motor speed by 40%
       }
       digitalWrite(driveMotorDirection, LOW);
       Serial.print("Moved the left stick Fast FORWARD:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("***FAST*** FORWARD motorSpeed= "); Serial.println(motorSpeed * .8);
    }
//REVERSE - TURBO
    else if(joystickPos > 2 ){ 
       if(Ps3.data.button.l1){ 
       ledcWrite(driveMotorChannel, (motorSpeed * .8)); //reduce the motor speed by 40%
       }
       if(!Ps3.data.button.l1){
       ledcWrite(driveMotorChannel, (motorSpeed * .4)); 
       }
       digitalWrite(driveMotorDirection, HIGH);
       Serial.print("Moved the left stick Fast REVERSE:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("***FAST*** REVERSE motorSpeed= "); Serial.println(motorSpeed * .1);
    } 
//NEUTRAL - TURBO     
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
/*
//***TOURING SPeED***  
  if((! Ps3.data.button.l1) && (abs(Ps3.event.analog_changed.stick.ly) > 2)){      
       int joystickPos = (Ps3.data.analog.stick.ly);
       int motorSpeed = map(joystickPos, -128, 128, -256, 256); 
//FORWARD
    if(joystickPos < -2){
       motorSpeed = -motorSpeed;  
       ledcWrite(driveMotorChannel, (motorSpeed * .4)); //reduce the motor speed by 40%
       digitalWrite(driveMotorDirection, LOW);
       Serial.print("Moved the left stick FORWARD:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("FORWARD motorSpeed= "); Serial.println(motorSpeed * .4);
    }
//REVERSE
    else if(joystickPos > 2 ){   
       ledcWrite(driveMotorChannel, (motorSpeed * .4)); //reduce the motor speed by 40%
       digitalWrite(driveMotorDirection, HIGH);
       Serial.print("Moved the left stick REVERSE:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("REVERSE motorSpeed= "); Serial.println(motorSpeed * .4);
    } 
//NEUTRAL     
    else{
      motorSpeed = 0;
      ledcWrite(driveMotorChannel, (motorSpeed));
      digitalWrite(driveMotorDirection, LOW);
      Serial.print("Moved the left stick NEUTRAL:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       Serial.print("NEUTRAL motorSpeed= "); Serial.println(motorSpeed); 
    }

  }
  */
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
       digitalWrite(winchDirectionPin, LOW);
       ledcWrite(winchPWMChannel, Ps3.data.analog.button.up * .5);
       Serial.println(Ps3.data.analog.button.up, DEC);


        
    //if( Ps3.event.button_up.l3 )
        //Serial.println("Released the left stick button");

    if (Ps3.data.analog.button.up < 1) 
       digitalWrite(winchDirectionPin, LOW);
       digitalWrite(winchPWMChannel, LOW);
       //ledcWrite(winchPWMChannel, 0 );
        
   }
    
       

    if((Ps3.event.analog_changed.button.down )) {
       Serial.print("Pressing the down button: ");
       digitalWrite(winchDirectionPin, HIGH);
       ledcWrite(winchPWMChannel, Ps3.data.analog.button.down * .5);
       Serial.println(Ps3.data.analog.button.down, DEC);
   
    if (Ps3.data.analog.button.down < 1) 
       digitalWrite(winchDirectionPin, LOW);
       digitalWrite(winchPWMChannel, LOW);
       //ledcWrite(winchPWMChannel, 0 ); 
       
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
