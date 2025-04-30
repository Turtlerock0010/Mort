/*
Program: Tralalero Tralala Code
Creation: April 30th, 2025
Contributors: Daniel Principe
Use: The code that goes into Tralalero Tralala
*/

#include <Alfredo_NoU2.h>
#include <PestoLink-Receive.h>

//drive train motors init
NoU_Motor frontleftMotor(1);
NoU_Motor frontrightMotor(2);
NoU_Motor backleftMotor(3);
NoU_Motor backrightMotor(4);

//drive train init
NoU_Drivetrain drivetrain(&frontleftMotor, &frontrightMotor, &backleftMotor, &backrightMotor);

// Quick variable changes

void setup() {
  Serial.begin(115200); // the bauder rate gotta be 9600 or 115200
  PestoLink.begin("Tralalero Tralala");
}

void loop() {
  //--Joystick Controls--
  float rotation = 0;
  float throttle = 0;

  //Set the throttle of the robot based on what key is pressed
  rotation = 1 * PestoLink.getAxis(0);
  throttle =  1 * PestoLink.getAxis(1);
  //--End of Joystick Controls--

  //--Throttle Calculations--
  double angle_radians = radians(rotation); // Convert degrees to radians

  //Combines both values of x movement and y movement into throttle
  double fl_throttle = cos(angle_radians) + sin(angle_radians);
  double fr_throttle = cos(angle_radians) - sin(angle_radians);
  double bl_throttle = cos(angle_radians) - sin(angle_radians);
  double br_throttle = cos(angle_radians) + sin(angle_radians);

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

  //Motor Set
  frontleftMotor.set(fl_throttle * throttle);
  frontrightMotor.set(fr_throttle * throttle);
  backleftMotor.set(bl_throttle * throttle);
  backrightMotor.set(br_throttle * throttle);

  PestoLink.update(); //Must be kept at the bottom
}
