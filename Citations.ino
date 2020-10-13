/*
 * 
 
 Thanks to Jeffrey van Pernis for sharing the PS3 code. See-https://github.com/jvpernis/esp32-ps3

 Thanks to John K. Bennett for sharing the servo library for ESP32. See-https://github.com/jkb-git/ESP32Servo
 I had to make a modification to this library so that I could incorporate both servos and motors in my sketch.
 ESP32Servo.h - changed line 96 espservo.h to 2 servos max leaving channels 2-16 availble for motors

  And thanks to these sites for sharing information on coding the ESP32 with PS3, PWM motors and servos.
https://dronebotworkshop.com/esp32-servo/

https://techtutorialsx.com/2020/03/28/esp32-ps3-controller-controlling-a-dc-motor/
 
https://randomnerdtutorials.com/esp32-dc-motor-l298n-motor-driver-control-speed-direction/

https://randomnerdtutorials.com/esp32-pwm-arduino-ide/

and to:

RENZO MISCHIANTI for his write-up on voltage dividers
https://www.mischianti.org/2019/06/15/voltage-divider-calculator-and-application/

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
*/
