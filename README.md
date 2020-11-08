# DIY Radio Control System for 3D-Sets Jeep & Landy Using PS3 Contoller and ESP32
3DSets (https://www.3dsets.com/) offers excellent 3d printable Jeep and Landy models complete with .stl's and gcode.
As an alternative to using traditional RC parts for the readio control system the following is an alternative DIY bluetooth radio control
system using inexpensive off-the-shelf parts. 

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
This project is a continuation of a previously posted project - Wemos - Blynk - iPhone Remote-Controlled 3D-Printed Jeep. That project describes hardware and software to control a 3D Sets 3D printed remote controlled vehicle using an ESP8266 and Blynk on an iPhone as the remote control. While it was an elegant solution requiring a Raspberry Pi as a server between the car and phone it has some shortcomings. The Blynk interface on the iPhone does not give much precision on the steering joystick and inadvertent touching of the interface could cause the app to disappear.  It didn't really pass the grandson test! My grandson asked if we could use a Playstation 3 controller.  I began exploring the possibility of using a cloned Playstation 3 wireless bluetooth controller instead of Blynk on the iPhone. Since the ESP8266 does not have bluetooth capability an ESP32 would be necessary. I wanted to keep everything in the electronics box provided for in the 3D-Sets design. Most ESP32's that was was familiar with would take up too much space to be feasible. That was until I found the MH-ET ESP 32 mini. This board packs an ESP32 on a board just slightly wider than the WEMOS ESP8266.
### Software Implemntation:
- Set-up ESP32 in Arduino IDE (for help see: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-mac-and-linux-instructions/)
- Install PS3 Library in Arduino IDE https://github.com/jvpernis/esp32-ps3 (for help see: https://techtutorialsx.com/2020/02/16/esp32-connecting-a-ps3-controller/)
- Install ESP32Servo Library in Arduino IDE https://github.com/jkb-git/ESP32Servo (for help see:https://dronebotworkshop.com/esp32-servo/) - I had to make a small modification to espservo.h . This library uses all of the pwm channels for servos leaving none for motors. You will need to change line 96 of espservo.h to 2 servos max leaving channels 2-16 available for motors
-
### Hardware Implementation:

