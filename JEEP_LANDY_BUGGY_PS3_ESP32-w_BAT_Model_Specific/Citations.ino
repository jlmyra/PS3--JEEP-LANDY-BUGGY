/*
 
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

 KHALED MAGDY for his outstanding write-up on the ESP32 ADC and ADC Calibration
 https://deepbluembedded.com/esp32-adc-tutorial-read-analog-voltage-arduino/

****************************************************************************************
*************************************** LICENSE ****************************************
****************************************************************************************
      This is free and unencumbered software released into the public domain.
      
      Anyone is free to copy, modify, publish, use, compile, sell, or
      distribute this software, either in source code form or as a compiled
      binary, for any purpose, commercial or non-commercial, and by any
      means.
      
      In jurisdictions that recognize copyright laws, the author or authors
      of this software dedicate any and all copyright interest in the
      software to the public domain. We make this dedication for the benefit
      of the public at large and to the detriment of our heirs and
      successors. We intend this dedication to be an overt act of
      relinquishment in perpetuity of all present and future rights to this
      software under copyright law.
      
      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
      IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
      OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
      ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
      OTHER DEALINGS IN THE SOFTWARE.
      
      For more information, please refer to <http://unlicense.org/>

****************************************************************************************
*************************************** LICENSE ****************************************
****************************************************************************************
*
*
*
*
*
*
 */
