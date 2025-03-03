#include "helpfull.hpp"
#include "lift_control.hpp"
#include "main.h"
#include "pros/motors.h" // IWYU pragma: keep
#include "main.h"
/*
AUTON CODE DEFINITIONS
*/
// Constants for speeds (out of 127)
const int DRIVE_SPEED = 80;
const int TURN_SPEED = 110;
const int SWING_SPEED = 110;


// Red Left Elims Auton
void Red_Left_Elims() {
  chassis.setPose(-54, 21, 270);
  startAutonomousIntakeColorsort(Alliance::RED_ALLIANCE);
  chassis.moveToPoint(-22, 24, 2000, {.forwards = false, .maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  clamp_piston.set_value(true);
  pros::delay(500);
  setintake(127);
  
  chassis.moveToPoint(-14, 37, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPoint(-14, 58, 2000, {.maxSpeed = 90});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPose(-24, 49, 240, 2000, {.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  pros::delay(300);
  chassis.moveToPose(-30, 30, 240, 2000, {.maxSpeed = 127});
  pros::delay(300);
  setintake(0);
  while(chassis.isInMotion()) pros::delay(10);
  pros::delay(1000);
  

}

void AWP_Blue_Left() {
  setintake(127);
}
void Blue_Left_Elims() {
  setintake(127);
}
// Red Right Elims Auton
void Red_Right_Elims() {
  setintake(127);
}

// Blue Right Elims Auton
void Blue_Right_Elims() {
  
  chassis.setPose(54, 21, 90);
  startAutonomousIntakeColorsort(Alliance::BLUE_ALLIANCE);
  chassis.moveToPoint(24, 24, 2000, {.forwards = false, .maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  clamp_piston.set_value(true);
  pros::delay(500);
  setintake(127);
  startAutonomousIntakeColorsort(Alliance::BLUE_ALLIANCE);
  chassis.moveToPoint(17, 37, 2000, {.minSpeed = 1,});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPoint(17, 55, 2000, {.minSpeed = 1});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPose(24, 47, 120, 2000, {.minSpeed = 1});
  while(chassis.isInMotion()) pros::delay(10);
  chassis.moveToPose(30, 30, 120, 2000, {.minSpeed = 1});
  while(chassis.isInMotion()) pros::delay(10);
  pros::delay(200);
  setintake(0);
  /*
  stopAutonomousIntakeColorsort();
  startAutonomousIntakeColorsort(Alliance::BLUE_ALLIANCE);
  doinker_piston.set_value(true);
  chassis.moveToPoint(61, -59, 2000, {.minSpeed = 1});
  while(chassis.isInMotion()) pros::delay(10);
  doinker_piston.set_value(true);
  pros::delay(500);
  chassis.turnToPoint(24, 24, 2000);
  doinker_piston.set_value(false);
  pros::delay(500);
  */
}

// AWP Red Left Auton
void AWP_Red_Left() {
  chassis.setPose(-51, 9, 240);
  startAutonomousIntakeColorsort(Alliance::RED_ALLIANCE);
  chassis.moveToPoint(-56, 6, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  liftPositionsAuton(position_4);
  pros::delay(500);
  
  chassis.moveToPoint(-24, 24, 2000, {.forwards = false, .maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  liftPositionsAuton(position_0);
  clamp_piston.set_value(true);
  pros::delay(500);
  setintake(127);
  
  chassis.moveToPose(-10, 48,30, 2000, {.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  

  pros::delay(500);
  
  chassis.moveToPoint(-12, 30, 2000, {.forwards= false ,.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPoint(-24, 50, 2000, {.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  
  pros::delay(1000);
  clamp_piston.set_value(false);
  setintake(0);
  
  chassis.moveToPoint(-40, 0, 2000, {.maxSpeed = 110});
  while(chassis.isInMotion()) pros::delay(10);
  setintake(-127);
  chassis.moveToPoint(-20, -16, 2000, {.forwards = false, .maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  clamp_piston.set_value(true);
  pros::delay(500);
  setintake(127);
  
  chassis.moveToPoint(-24, -43, 2000, {.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  
  
  chassis.moveToPoint(-12, -11,  2000, {.maxSpeed = 90});
  pros::delay(100);
  liftPositionsAuton(position_1);
  while(chassis.isInMotion()) pros::delay(10);
  
}

// AWP Red Right Auton
void AWP_Red_Right() {
  setintake(127);
}

// AWP Blue Right Auton
void AWP_Blue_Right() {
  chassis.setPose(51, 9, 120);
  startAutonomousIntakeColorsort(Alliance::BLUE_ALLIANCE);
  chassis.moveToPoint(56, 6, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  liftPositionsAuton(position_4);
  pros::delay(500);
  
  chassis.moveToPoint(24, 24, 2000, {.forwards = false, .maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  liftPositionsAuton(position_0);
  clamp_piston.set_value(true);
  pros::delay(500);
  setintake(127);
  
  chassis.moveToPose(6, 40,30, 2000, {.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  

  pros::delay(500);
  
  chassis.moveToPoint(12, 30, 2000, {.forwards= false ,.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPoint(24, 50, 2000, {.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  
  pros::delay(1000);
  clamp_piston.set_value(false);
  setintake(0);
  
  chassis.moveToPoint(40, 0, 2000, {.maxSpeed = 110});
  while(chassis.isInMotion()) pros::delay(10);
  setintake(127);
  chassis.moveToPoint(15, -23, 2000, {.forwards = false, .maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  clamp_piston.set_value(true);
  pros::delay(500);
  setintake(127);
  
  chassis.moveToPoint(19, -48, 2000, {.maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  
  
  chassis.moveToPoint(12, -11,  2000, {.maxSpeed = 90});
  pros::delay(100);
  liftPositionsAuton(position_1);
  while(chassis.isInMotion()) pros::delay(10);
  
}


// Skills Auton
void skills_auton_1() {
  chassis.setPose(-59, 0, 90);
  //aliance steak 
  setintake(127);
  pros::delay(500);
  //end alligance steak

  // clamp goal 1
  chassis.moveToPoint(-48, 0, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPoint(-47, -25, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  clamp_piston.set_value(true);
  pros::delay(1000);
  //end clamp goal 1

  //wallsteak 1 sequence 
  chassis.moveToPoint(-47, -25, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPoint(-24, -24, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  
  chassis.moveToPoint(0, -40, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  //liftPositionsAuton(position_1);
  
  chassis.moveToPoint(28, -48, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  /*
  //align ring for wall 1
  pros::delay(1000);
  // Intake pulsing
  for(int i = 0; i < 3; i++) {
    setintake(127);
    pros::delay(160);
    setintake(0);
    pros::delay(160);
  }

  
  
  liftPositionsAuton(position_2);
  pros::delay(1000);
  setintake(127);
  
  chassis.moveToPose(5, -50,180, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPose(5, -66, 180, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  // place ring on steak 
  liftPositionsAuton(position_3);
  pros::delay(1000);
  
  chassis.moveToPose(5, -48,180, 2000, {.forwards = false,.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  liftPositionsAuton(position_0);
  */
  // end wallsteak 1 sequence

  // gal fill 1 sequence
  chassis.moveToPose(-47, -47, 270, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPose(-52, -47, 270, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);

  
  chassis.moveToPoint(-46, -58, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  

  chassis.moveToPose(-55, -55, 60, 2000, {.forwards = true, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPose(-57, -59, 60, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  setintake(-127);
  clamp_piston.set_value(false);
  pros::delay(500);
  setintake(127);


  // end goal fill 1 sequence
  chassis.moveToPose(-46, -48, 60, 2000, {.forwards = true, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPoint(-48, 0, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPoint(-48, 23, 2000, {.forwards = false, .maxSpeed = 50});
  while(chassis.isInMotion()) pros::delay(10);
  
  // clamp goal 2
  clamp_piston.set_value(true);
  pros::delay(1000);
  //wallsteak 2 sequence
  chassis.moveToPoint(-23, 23, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  /*
  chassis.moveToPoint(0, 35, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPoint(24, 47, 2000, {.maxSpeed = DRIVE_SPEED});
  liftPositionsAuton(position_1);
  while(chassis.isInMotion()) pros::delay(10);
  
  pros::delay(1000);

  // align ring for wallsetak 2
  // Intake pulsing again
  for(int i = 0; i < 3; i++) {
    setintake(127);
    pros::delay(160);
    setintake(0);
    pros::delay(160);
  }
  
  liftPositionsAuton(position_2);
  pros::delay(1000);
  setintake(127);
  
  chassis.moveToPoint(8, 47, 2000, {.maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPose(8, 62, 0, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);

  //place ring on wallsteak 2
  liftPositionsAuton(position_3);
  pros::delay(1000);

  chassis.moveToPoint(10, 47, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  liftPositionsAuton(position_0);
  while(chassis.isInMotion()) pros::delay(10);
  // end wallsteak 2 sequence
  */
  // goal fill 2 sequence
  chassis.moveToPoint(-47, 47, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPose(-59, 47, 0, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);
  
  chassis.moveToPoint(-57, 59, 2000, {.maxSpeed = 60});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPose(-60, 60, 140, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  setintake(-127);
  clamp_piston.set_value(false);
  pros::delay(500);
  // end goal fill 2 sequence

  //position for left goal push sequence
  chassis.moveToPose(-50, 50, 140, 2000, {.forwards = true, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPoint(0, 47, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  liftPositionsAuton(position_0);
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPoint(30, 33, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPose(67, 23, 0, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPose(75, 75, 0, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  // end left goal push sequence

  // goal with 3 sequence
  chassis.moveToPose(50, 0, 0, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  // clamp goal 3
  clamp_piston.set_value(true);
  pros::delay(500);

  chassis.moveToPoint(24, 24, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  setintake(0);
  chassis.moveToPoint(0, 0, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  setintake(127);
  pros::delay(100);
  setintake(0);
  chassis.moveToPoint(23, -24, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  setintake(127);
  chassis.moveToPoint(59, -59, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  setintake(-127);
  clamp_piston.set_value(false);
  liftPositionsAuton(position_4);
  chassis.moveToPoint(10, -13, 2000, {.forwards = false, .maxSpeed = 127});
  while(chassis.isInMotion()) pros::delay(10);
  // end goal with 3 sequence

  /*
  // second alliance steak
  chassis.moveToPoint(47, -47, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  pros::delay(100);
  setintake(0);
  chassis.moveToPoint(41, 0, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPose(60, 0, 270, 2000, {.forwards = false, .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  setintake(127);
  pros::delay(500);
  // end second alliance steak

  


  // right goal push sequence
  chassis.moveToPose(59, -22, 160, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);

  chassis.moveToPose(60, -57, 160, 2000, { .maxSpeed = DRIVE_SPEED});
  while(chassis.isInMotion()) pros::delay(10);
  // end right goal push sequence

  */
  // hang on middle lader
  liftPositionsAuton(position_4);
  
  chassis.moveToPoint(10, -12, 2000, {.forwards = false, .maxSpeed = 100});
  setintake(0);
  while(chassis.isInMotion()) pros::delay(10);
}


