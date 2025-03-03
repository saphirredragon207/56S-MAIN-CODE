#include "main.h" // IWYU pragma: keep

//mototrs
pros::Motor intake11w(-12,pros::v5::MotorGears::blue); 
pros::Motor intake55w(-11,pros::v5::MotorGears::green);
pros::Motor lift_motor(19,pros::v5::MotorGears::green);

//pneumatics
pros::ADIDigitalOut clamp_piston('H',false);
pros::ADIDigitalOut intake_piston('b',true);
pros::ADIDigitalOut doinker_piston('c',false);
//sensors
pros::Optical optical(20);
pros::Vision vision_sensor(19);
pros::Rotation rotation_sensor(9);

pros::Controller controller(pros::E_CONTROLLER_MASTER);



