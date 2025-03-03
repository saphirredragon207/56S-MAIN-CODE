#include "main.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

pros::MotorGroup hook_group({14, -12},pros::MotorGearset::blue); 

void setintake(int intakepower) {
    hook_group.move(intakepower);
}
