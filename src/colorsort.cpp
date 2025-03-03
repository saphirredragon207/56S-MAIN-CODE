#include "main.h"
#include "pros/rtos.hpp"
#include "pros/optical.hpp"
#include "main.h"

// Declare the intake control function
void setintake(int speed);

static pros::Task* colorsortTask = nullptr;
static pros::Task* driverTask = nullptr;
static bool stopTask = false;

struct TaskParams {
    Alliance alliance;
};

void autonomousIntakeColorsortTask(void* params) {
    optical.set_led_pwm(100); // Initialize sensor LED brightness
    
    TaskParams* taskParams = static_cast<TaskParams*>(params);
    Alliance alliance = taskParams->alliance;
    
    
    while (!stopTask) {
        if (optical.get_proximity() > 200) { // Check if the optical sensor is near
            if (optical.get_hue() > 0 && optical.get_hue() < 20) {
                // If the ring is red
                if (alliance == Alliance::BLUE_ALLIANCE) {
                    // Eject red ring if alliance is blue
                    setintake(-127);
                    pros::delay(200);
                    setintake(127);
                    
                } else {
                    // Continue intake if alliance is red
                    pros::delay(0.000001);
                }
            } else if (optical.get_hue() > 200 && optical.get_hue() < 255) {
                // If the ring is blue
                if (alliance == Alliance::RED_ALLIANCE) {
                    // Eject blue ring if alliance is red
                    setintake(-127);
                    pros::delay(204);
                    setintake(127);
                    
                } else {
                    // Continue intake if alliance is blue
                    pros::delay(0.000001);
                }
            } else {
                // Continue intake if no specific color detected
                pros::delay(0.000001);
            }
        } else {
            // Continue intake if not near
            pros::delay(0.000001);
        }
        pros::delay(50); // Pause briefly before checking again
    }
}

void startAutonomousIntakeColorsort(Alliance alliance) {
    if (!colorsortTask) {
        stopTask = false;
        static TaskParams taskParams = {alliance}; // Static to ensure it stays alive
        colorsortTask = new pros::Task(autonomousIntakeColorsortTask, static_cast<void*>(&taskParams), "AutonomousColorsortTask");
        optical.set_led_pwm(100); // Initialize sensor LED brightness
    }
}

void stopAutonomousIntakeColorsort() {
    if (colorsortTask) {
        stopTask = true;
        pros::delay(20); // Allow time for the task loop to exit
        delete colorsortTask;
        colorsortTask = nullptr;
    }
}