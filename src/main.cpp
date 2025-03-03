#include "main.h"
#include "colorsort.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "lvgl_touchscreen.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "helpfull.hpp"
#include "lift_control.hpp"// IWYU pragma: keep
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"

int battery_charge = 0 ;

// touchscreen declarations
static lv_indev_drv_t indev_drv;
static lv_disp_t* disp;
// device ports



// high 
// end port includes

// motor groups
pros::MotorGroup leftDrive({-4, -5, -6},pros::MotorGearset::blue); // left motor group 
pros::MotorGroup rightDrive({1, 2, 3}, pros::MotorGearset::blue); // right motor group 

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftDrive, // left motor group
                              &rightDrive, // right motor group
                              14, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              450, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// inertial sensor
pros::Imu imu(7);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor
//pros::Rotation horizontalEnc(-11);
//vertical tracking wheel encoder. Rotation sensor

lemlib::TrackingWheel right_vertical_encoder(&rightDrive, lemlib::Omniwheel::NEW_325, 6.5,450);
lemlib::TrackingWheel left_vertical_encoder(&leftDrive, lemlib::Omniwheel::NEW_325, -6.5,450);




/*
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_325, 7);

*/

// vertical tracking wheel
/*
lemlib::TrackingWheel right_vertical_tracking_wheel(&rightDrive, lemlib::Omniwheel::NEW_325, 6.5,450);
lemlib::TrackingWheel left_vertical_tracking_wheel(&leftDrive, lemlib::Omniwheel::NEW_325, 6.5,450);
*/


// sensors for odometry
lemlib::OdomSensors sensors(&right_vertical_encoder, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);



// lateral motion controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);


// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);


// create the chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors, &throttleCurve, &steerCurve);






/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    //pros::lcd::initialize();
    chassis.calibrate();
    rotation_sensor.set_position(0);
    // Initialize LVGL
    lv_init();

    // Setup display buffer and driver
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, 480 * 10);
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 240;
    lv_disp_drv_register(&disp_drv);

    // Register touchscreen
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchscreen_read;
    lv_indev_drv_register(&indev_drv);

    // Display the splash screen; the continue button will initialize the selector
    init_splash_screen();

    // Initialize the banner immediately so it is always visible.
    init_banner();


    /**/
    pros::Task selected_auton_update([&]() {
        while (true) {
            update_selected_autons();
            pros::delay(100);
        }
    });
    
    // Periodically update the banner via a dedicated task.
    pros::Task BatteryTask([&]() {
        while (true) {
            update_init_banner();
            pros::delay(100);
        }
    });


    pros::Task screenTask([&]() {
        while (true) {
            update_manual_tab();
            pros::delay(50);
        }
    });
    
    pros::Task lift_button_task([&]() {
        while (true) {
            handleLiftInput();
            pros::delay(50);
        }
    });
    pros::Task liftControlTask([]{
        while (true) {
            liftControl();
            pros::delay(10);
        }
    });
    lift_motor.set_brake_mode(MOTOR_BRAKE_BRAKE);
}
/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}





void autonomous() {
    update_match_data();


    Blue_Right_Elims();
    //Red_Left_Elims(); 
    //skills_auton_1();
    //AWP_Blue_Right();
    //AWP_Red_Left(); 
    /*
    switch(selected_auton) {
        case 0:
            Red_Left_Elims(); 
            break;
        case 1:
            Red_Right_Elims(); 
            break;
        case 2:
            Blue_Left_Elims(); 
            break;
        case 3:
            Blue_Right_Elims(); 
            break;
        case 4:
            AWP_Red_Left(); 
            break;
        case 5:
            AWP_Blue_Left(); 
            break;
        case 6:
            AWP_Red_Right(); 
            break;
        case 7:
            AWP_Blue_Right(); 
            break;
        case 8:
            skills_auton_1(); 
            break;
        
        default:
            break;
    }
    */
    
}


/**
 * Runs in driver control
 */
void opcontrol() {
    static uint32_t lastUpdate = 0;
    const uint32_t UPDATE_INTERVAL = 50;
    optical.set_led_pwm(100); // Initialize sensor LED brightness
    
    while (true) {
        // Drive Control
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
        chassis.tank(leftY, rightY);
        
        
        // Intake Control
        if (controller.get_digital(DIGITAL_R1)) {
            setintake(127);
        } else if (controller.get_digital(DIGITAL_R2)) {
            setintake(-127);
        } else {
            setintake(0);
        }
        

        // Clamp Control
        if (controller.get_digital(DIGITAL_UP)) {
            clamp_piston.set_value(true);
        } else if (controller.get_digital(DIGITAL_DOWN)) {
            clamp_piston.set_value(false);
        }

        // Doinker Control
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            doinker_piston.set_value(true);
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            doinker_piston.set_value(false);
        }

        if (controller.get_digital(DIGITAL_L1)) {
            liftPositionsAuton(position_5);
        } else if (controller.get_digital(DIGITAL_L2)) {
            liftPositionsAuton(position_4);
        }

        // Update GUI at intervals
        double current_x = chassis.getPose().x;
        double current_y = chassis.getPose().y;
        double current_heading = imu.get_heading();
        update_odom_data(current_x, current_y, current_heading);

        // Also update match tab if needed
        update_match_data();

        pros::delay(10);
    }
}




