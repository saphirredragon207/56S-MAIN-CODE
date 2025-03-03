#ifndef HELPFULL_HPP
#define HELPFULL_HPP

#include "pros/motors.hpp"
#include "pros/rtos.hpp"

// Motor declarations
extern pros::Motor intake11w;
extern pros::Motor intake55w;

// Global variables
extern int current_power;
extern pros::Task* intake_task;

/**
 * @brief Task function to control the intake motors
 * 
 * @param param Pointer to intake power value
 */
void intake_control(void* param);

/**
 * @brief Sets the intake power and manages the intake task
 * 
 * @param intakepower Power level for the intake motors
 */
void setintake(int intakepower);

#endif // HELPFULL_HPP
