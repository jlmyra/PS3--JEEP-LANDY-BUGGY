![pic one](https://github.com/jlmyra/PS3---JEEP/blob/main/Jeep_Landy.png)
# DIY Radio Control System for 3D-Sets Jeep & Landy Using PS3 Contoller and ESP32
3DSets (https://www.3dsets.com/) offers excellent 3d printable Jeep and Landy models complete with .stl's and gcode.
As an alternative to using traditional RC parts for the radio control system the following is an alternative DIY bluetooth radio control
system using inexpensive off-the-shelf parts. These parts will fit within the electronics box for each vehicle. 

### PS3 Controller Parts:
- MH-ET ESP32 Microcontroller - https://acrobotic.com/collections/esp8266/products/acr-00024 ($6.95)
- Adafruit DRV8871 Motor Controller - https://tinyurl.com/y2jta8pr ($7.50)
- Cytron MD13S Motor Controller - https://www.cytron.io/p-13amp-6v-30v-dc-motor-driver ($9.90)
- PS3 Controller - Amazon : https://tinyurl.com/y3ootahd ($19.90)
- 425K resistor
- 680K resistor
- 0.1uf capacitor
- 5V, 3A BEC/UBEC (battery elimination circuit) - https://tinyurl.com/y3t4wqus (~$4.00)
### Other Parts:
- 180 deg Servo (steering) - https://tinyurl.com/y5thqy46 ($19.00)
- Drive Motor - https://tinyurl.com/y549xxzk ($16.00)
- Winch motor - https://tinyurl.com/y6g5tkvd ($8.00)
- Battery - https://tinyurl.com/y5fdwxl3 ($31.00)

### Background:
This project is a continuation of a previously posted project - Wemos - Blynk - iPhone Remote-Controlled 3D-Printed Jeep. That project describes hardware and software to control a 3D Sets 3D printed remote controlled vehicle using an ESP8266 and Blynk on an iPhone as the remote control. While it was an elegant solution requiring a Raspberry Pi as a server between the car and phone it has some shortcomings. The Blynk interface on the iPhone does not give much precision on the steering joystick and inadvertent touching of the iphone-Blynk interface can cause the app to disappear leaving the vehicle momentarily uncontrolled.  More importantly it didn't really pass the grandson test! This effort began when my grandson asked if we could use a Playstation 3 controller instead the iPhone.  In this arrangement the ESP8266 is replaced with a ESP32 to take advantage of the built-in bluetooth radio. More importantly all of the hard work involved with connecting an ESP32 to a PS3 controller has been done by JVPernis(https://github.com/jvpernis/esp32-ps3). I simply took advantage of his work to make this setup functional.
### Software Implementation:
- Set-up ESP32 in Arduino IDE (for help see: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-mac-and-linux-instructions/)
- Install PS3 Library in Arduino IDE https://github.com/jvpernis/esp32-ps3 (for help see: https://techtutorialsx.com/2020/02/16/esp32-connecting-a-ps3-controller/)
- Install ESP32Servo Library in Arduino IDE https://github.com/jkb-git/ESP32Servo (for help see:https://dronebotworkshop.com/esp32-servo/) - I had to make a small modification to espservo.h . This library uses all of the pwm channels for servos leaving none for motors. You will need to change line 96 of espservo.h to 2 servos max leaving channels 2-16 available for motors
- Description: The sketch is presented in a 'tabbed' format for ease of editing. JEEP_LANDY_PS3_ESP32.ino contains the variables, setup, and loop. Pay attention to the steering servo section. You can 'align' the front end by changing 'servomin' and 'servomax'. The ESP32Servo library needs a minor modification. It is written in a way that dedicates all of the pwm channels to servos. Two pwm channels are required for the drive and winch motors. Line 96 of espservo.h needs to be set to 2 servos max.  The sketch is designed to produce forward and backward movment of the vehicle by moving the left joystick forward and back. The 'normal' speed is throttled to 45% of the available speed. Pressing the L1 button before moving the left joystick will produce a "nitro' mode producing 65% of the available speed. The right joystick controls the steering left-right. The sketch also 'ramps' the acceleration gradually increasing the pwm duty cycle to the motor. This prevents all of the motor torque from being delivered instantaneously to the drive train. It accelerates more like it has a clutch or torque convertor in the system. The rate of change can be modified by changing the 'motorSpeedIncrement' variable. The winch is operated using the triangle button to unwind the winch and the x button to rewind the winch. There is also a circuit for monitoring the charge state of the battery in the vehicle. There is a tap on the + side of the battery that goes to a voltage divider. The output of the voltage divider is read on GPIO32 an ADC pin. The charge state of the battery is displayed on the controller LEDS. 4 LEDS - full charge, 3 LEDS 75%, 2 LEDS 50%, and 1 LED- time to recharge. When the voltage goes below 7 volts for 15 seconds the controller will begin vibrating to remind the operator to recharge the battery.
### Wiring Diagram:
![pic one](https://github.com/jlmyra/PS3--JEEP-LANDY/blob/main/JEEP-LANDY_Wiring_Diagram.png)

