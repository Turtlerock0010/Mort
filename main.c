/*
Program: Tralalero Tralala Code
Creation: April 30th, 2025
Contributors: Daniel Principe
Use: The code that goes into Tralalero Tralala
*/




#include <Alfredo_NoU2.h>
#include <PestoLink-Receive.h>
#include <stdio.h>




//drive train motors init
NoU_Motor frontleftMotor(1);
NoU_Motor frontrightMotor(2);
NoU_Motor backleftMotor(3);
NoU_Motor backrightMotor(4);




// Quick variable changes




bool zeroPressed = false;




void setup() {
 Serial.begin(115200); // the bauder rate gotta be 9600 or 115200
 PestoLink.begin("Tralalero Tralala");
}




void loop() {
 //--Joystick Controls--
 float horizontalThrottle = 0;
 float verticalThrottle = 0;
 float rotationalThrottle = 0;


 double fl_throttle = 0;
 double fr_throttle = 0;
 double bl_throttle = 0;
 double br_throttle = 0;




 //Set the throttle of the robot based on what key is pressed
 horizontalThrottle = 1 * PestoLink.getAxis(0);
 verticalThrottle =  -1 * PestoLink.getAxis(1);
 rotationalThrottle = 1 * PestoLink.getAxis(2);

 horizontalThrottle = round(horizontalThrottle * 100.0) / 100.0;
 verticalThrottle = round(verticalThrottle * 100.0) / 100.0;
 rotationalThrottle = round(rotationalThrottle * 100.0) / 100.0;


 //--End of Joystick Controls--

 //--Throttle Calculations--
 double angle_radians = atan2(horizontalThrottle, verticalThrottle); // Convert degrees to radians

 //Combines both values of x movement and y movement into throttle
 if (horizontalThrottle != 0 || verticalThrottle != 0) {
   fl_throttle = cos(angle_radians) + sin(angle_radians);
   fr_throttle = cos(angle_radians) - sin(angle_radians);
   bl_throttle = cos(angle_radians) - sin(angle_radians);
   br_throttle = cos(angle_radians) + sin(angle_radians);
 } else {
   fl_throttle = 0;
   fr_throttle = 0;
   bl_throttle = 0;
   br_throttle = 0;
 }


 //Normalize throttles
 double max_throttle = fabs(fl_throttle);
 if (fabs(fr_throttle) > max_throttle) max_throttle = fabs(fr_throttle);
 if (fabs(bl_throttle) > max_throttle) max_throttle = fabs(bl_throttle);
 if (fabs(br_throttle) > max_throttle) max_throttle = fabs(br_throttle);




 if (max_throttle > 1.0) {
   fl_throttle /= max_throttle;
   fr_throttle /= max_throttle;
   bl_throttle /= max_throttle;
   br_throttle /= max_throttle;
 }
 //--End of Throttle Calculations--


 if (rotationalThrottle != 0) {
   if (rotationalThrottle > 0) {
     fl_throttle = rotationalThrottle;
     fr_throttle = -rotationalThrottle;
     bl_throttle = rotationalThrottle;
     br_throttle = -rotationalThrottle;
   }
   if (rotationalThrottle < 0) {
     fl_throttle = rotationalThrottle;
     fr_throttle = -rotationalThrottle;
     bl_throttle = rotationalThrottle;
     br_throttle = -rotationalThrottle;
   }
 }
 if (PestoLink.keyHeld(Key::Digit0) and not zeroPressed) {
   zeroPressed = true;
   Serial.print("\n");
   Serial.print(rotationalThrottle);
 }
 if (!PestoLink.keyHeld(Key::Digit0)) {
   zeroPressed = false;
 }
 //Motor Set
 frontleftMotor.set(fl_throttle);
 frontrightMotor.set(-fr_throttle);
 backleftMotor.set(bl_throttle);
 backrightMotor.set(-br_throttle);


 PestoLink.update(); //Must be kept at the bottom
}


