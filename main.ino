/*
Program: The Punkrocker Code
Creation: July 5th, 2025
Contributors: Daniel Principe
Team: Pink Fluffy Unicorns [83]
Use: The code that goes into The Punkrocker
*/

// Library Init
#include <Alfredo_NoU3.h>
#include <PestoLink-Receive.h>

// Drive Train Motors Init
NoU_Motor frontLeftMotor(8);
NoU_Motor frontRightMotor(1);
NoU_Motor backLeftMotor(7);  
NoU_Motor backRightMotor(2);

// End Effector Motors Init
NoU_Motor algaeIntakeMotors(5);
NoU_Motor coralIntakeMotors(4);
NoU_Motor coralIntakeMotors2(3);
// End Effector Arm Servos Init
NoU_Servo leftArmServo(1);

// Elevator Servos Init
NoU_Servo rightElevatorServo(3);
NoU_Servo leftElevatorServo(4);

// Drive Train Init
NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &backLeftMotor, &backRightMotor);

float rightElevatorServoDEG = 0;
float leftElevatorServoDEG = 180;

void setup() {
  NoU3.begin();
  PestoLink.begin("The Punkrocker");
  // coralIntakeMotors.setInverted(true);

}


void loop() {
  if (PestoLink.update()) {
    // Get battery voltage and display on PestoLink
    float batteryVoltage = NoU3.getBatteryVoltage();
    PestoLink.printBatteryVoltage(batteryVoltage);

    // Algae Intake Button
    if (PestoLink.buttonHeld(4)) {
      algaeIntakeMotors.set(-1);
    } else {
      algaeIntakeMotors.set(0);
    }

    // Algae Outtake Button
    if (PestoLink.buttonHeld(5)) {
      algaeIntakeMotors.set(1);
    } else {
      algaeIntakeMotors.set(0);
    }

    // Coral Intake Button
    if (PestoLink.buttonHeld(6)) {
      // coralIntakeMotors.setInverted(false);
      coralIntakeMotors.setInverted(true);
      coralIntakeMotors2.setInverted(true);
      coralIntakeMotors.set(1);
      coralIntakeMotors2.set(1);
    } else {
    //  coralIntakeMotors.setInverted(false);
      coralIntakeMotors.set(0);
      coralIntakeMotors2.set(0);
      coralIntakeMotors.setInverted(false);
      coralIntakeMotors2.setInverted(false);
    }

    // Coral Outtake Button
    if (PestoLink.buttonHeld(7) || PestoLink.keyHeld(Key::Q)) {
      coralIntakeMotors.set(1);
      coralIntakeMotors2.set(1);
    } else {
      // coralIntakeMotors.setInverted(false);
      coralIntakeMotors.set(0);
      coralIntakeMotors2.set(0);
      coralIntakeMotors.setInverted(false);
      coralIntakeMotors2.setInverted(false);
    }

    //---End Effector Servo Code---
    // Get degrees from PestoLink
    int leftArmServoDEG = PestoLink.buttonHeld(0) || PestoLink.keyHeld(Key::E) ? 5 : 35; 
    // Update arm servos
    leftArmServo.write(leftArmServoDEG);


    //---Elevator Servo Code---
    // Change degrees based on input
    if (PestoLink.keyHeld(Key::U) && leftElevatorServoDEG > 0) {
      rightElevatorServoDEG += 1;
      leftElevatorServoDEG -= 1;
    } else if (PestoLink.keyHeld(Key::I) && leftElevatorServoDEG < 180) {
      rightElevatorServoDEG -= 1;
      leftElevatorServoDEG += 1;
    }

    if (PestoLink.buttonHeld(12) || PestoLink.keyHeld(Key::R)) {
      rightElevatorServoDEG = 180 ;
      leftElevatorServoDEG = 10 ;
    } else if (PestoLink.buttonHeld(14)) {
      rightElevatorServoDEG = 120;
      leftElevatorServoDEG = 60;
    } else if (PestoLink.buttonHeld(15)) {
      rightElevatorServoDEG = 45;
      leftElevatorServoDEG = 135;
    } else if (PestoLink.buttonHeld(13) || PestoLink.keyHeld(Key::F)) {
      rightElevatorServoDEG = 0;
      leftElevatorServoDEG = 180;
    }

    // Update elevator servos
    leftElevatorServo.write(leftElevatorServoDEG);
    rightElevatorServo.write(rightElevatorServoDEG);

    //---Drive Train Code---
    // Axis Init
    float horizontalAxis = 0;
    float verticalAxis = 0;
    float rotationalAxis = 0;

    // Retrieve 3 axes from PestoLink and set it to given axis
    horizontalAxis = 1 * PestoLink.getAxis(0);
    verticalAxis = -1 * PestoLink.getAxis(1);
    rotationalAxis = -1 * PestoLink.getAxis(2);

    // Apply axes to drive train
    drivetrain.holonomicDrive(horizontalAxis, verticalAxis, rotationalAxis);

  } else {
    drivetrain.curvatureDrive(0, 0); // Stops drifting after code uploads
    PestoLink.update(); // Required piece of code to update robot on pestolink values
  }
}
