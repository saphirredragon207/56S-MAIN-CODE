// lift_control.hpp
#pragma once
#include "main.h" // IWYU pragma: keep
#include "pros/rtos.hpp" // IWYU pragma: keep


// Lift Positions
extern const int position_0;
extern const int position_1;
extern const int position_2;
extern const int position_3;
extern const int position_4;
extern const int position_5;
extern int lift_target_position;
extern int positions_for_ring_arm[];

// Global heading tracking
extern double cumulative_heading;

// Function declarations
double normalizeAngle(double angle);
int calculateLiftSpeed(int targetPosition, int currentPosition);
void liftControl();
void handleLiftInput();
void liftPositionsAuton(int desiredPosition);
