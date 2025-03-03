#include "main.h"
#include "pros/rotation.hpp" // IWYU pragma: keep
#include "lift_control.hpp"


// Lift Positions
const int position_0 = 0;
const int position_1 = 250;
const int position_2 = 500;
const int position_3 = 1200;
const int position_4 = 1500;
const int position_5 = 1900;
int lift_target_position = -1;
int positions_for_ring_arm[] = {position_0, position_1, position_2, position_3, position_4,position_5};
int currentPosition = position_0; // Default position
// Global heading tracking
double cumulative_heading = 0;

// Function to normalize angles to -180 to 180 degrees
double normalizeAngle(double angle) {
    while (angle > 180) angle -= 360;
    while (angle < -180) angle += 360;
    return angle;
}

// Speed Calculation Function
int calculateLiftSpeed(int targetPosition, int currentPosition) {
    int difference = abs(targetPosition - currentPosition);
    if (difference < 5) return 2;
    if (difference < 50) return 10;
    if (difference < 150) return 40;
    return 127;
}

// Lift Control Function
void liftControl() {
    while (true) {
        if (lift_target_position != -1) {
            // Move to the target position using the lift motor’s encoder
            int currentPosition = lift_motor.get_position();
            while (abs(lift_target_position - currentPosition) > 3) {
                currentPosition = lift_motor.get_position();
                int error = abs(lift_target_position - currentPosition);
                int speed = calculateLiftSpeed(lift_target_position, currentPosition);

                

                if (currentPosition < lift_target_position) {
                    lift_motor.move_velocity(speed);
                } else {
                    lift_motor.move_velocity(-speed);
                }
                pros::delay(10);
            }

            // Stop motor movement
            lift_motor.move_velocity(0);

            // Set the motor to hold mode so that it maintains the current position
            lift_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

            // Optionally: Wait until a new target is set before resuming adjustments
            // (This loop holds the position until lift_target_position changes)
            
            while (true) {
                // Check if a new command has been issued (i.e., lift_target_position reset)
                if (lift_target_position != -1 && 
                   abs(lift_target_position - lift_motor.get_position()) > 3) {
                    // If moving, change back to coast mode before adjusting speed
                    lift_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                    break;
                }
                pros::delay(20);
            }
            
        }
        pros::delay(50);
    }
}


// Controller Input Handling for Lift
void handleLiftInput() {
    static bool lastButtonStates[4] = {false, false, false, false};
    static int currentPosition = position_1; // Default position
    
    // Get current button states
    bool buttonA = controller.get_digital(DIGITAL_A);
    bool buttonB = controller.get_digital(DIGITAL_B);
    bool buttonX = controller.get_digital(DIGITAL_X);
    bool buttonY = controller.get_digital(DIGITAL_Y);
    
    // Check for button press (transition from false to true)
    if (buttonA && !lastButtonStates[0]) {
        lift_target_position = position_0;
        currentPosition = position_0;
    } else if (buttonB && !lastButtonStates[1]) {
        lift_target_position = position_1;
        currentPosition = position_1;
    } else if (buttonX && !lastButtonStates[2]) {
        lift_target_position = position_2;
        currentPosition = position_2;
    } else if (buttonY && !lastButtonStates[3]) {
        lift_target_position = position_3;
        currentPosition = position_3;
    }
    
    // Update last button states
    lastButtonStates[0] = buttonA;
    lastButtonStates[1] = buttonB;
    lastButtonStates[2] = buttonX;
    lastButtonStates[3] = buttonY;
}

// Autonomous Lift Control
void liftPositionsAuton(int desiredPosition) {
    bool positionValid = false;
    for (int pos : positions_for_ring_arm) {
        if (pos == desiredPosition) {
            positionValid = true;
            break;
        }
    }

    if (positionValid) {
        lift_target_position = desiredPosition;       
    }
    
}