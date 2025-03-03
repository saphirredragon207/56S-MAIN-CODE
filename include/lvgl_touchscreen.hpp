//──────────────────────────────
// lvgl_touchscreen.hpp
//──────────────────────────────

#ifndef LVGL_TOUCHSCREEN_HPP
#define LVGL_TOUCHSCREEN_HPP

#include "pros/screen.hpp"   //iwyu pragma: keep
#include "pros/motors.hpp"   //iwyu pragma: keep
#include "pros/imu.hpp"      //iwyu pragma: keep
#include "pros/rtos.hpp"     //iwyu pragma: keep
#include "liblvgl/lvgl.h"    //iwyu pragma: keep
#include "pros/misc.hpp"     //iwyu pragma: keep

// Structure representing the robot status (for display updates)
struct RobotStatus {
    double x;
    double y;
    double heading;
    int main_battery;
    int controller_battery;
    bool controller_connected;
    bool imu_connected;
    bool imu_calibrated;
    double motor_temps[12];
};

// -----------------------------------------------------------------------------
// Extern declarations for Display Buffer, Driver, and GUI objects
extern lv_disp_draw_buf_t draw_buf;
extern lv_color_t buf1[480 * 10];
extern lv_disp_drv_t disp_drv;

extern lv_obj_t* tabview;
extern lv_obj_t* selector_tab;    // Renamed from match_tab to selector_tab if needed
extern lv_obj_t* skills_tab;
extern lv_obj_t* odom_tab;
extern lv_obj_t* manual_tab;

extern lv_obj_t* status_container;
extern lv_obj_t* coords_label;
extern lv_obj_t* heading_label;
extern lv_obj_t* temp_label;       // Used in manual tab
extern lv_obj_t* battery_label;
extern lv_obj_t* imu_btn;
extern lv_obj_t* controller_icon;
extern lv_obj_t* field_display;

// Autonomous selection variables
extern int selected_auton;
extern const char* auton_names[];
void update_selected_autons();
// -----------------------------------------------------------------------------
// Function prototypes for initialization and callbacks

// LVGL and touchscreen initialization
void init_lvgl();
void init_splash_screen();

// Selector (GUI) initialization for Selector, Skills, Odom, and Manual tabs.
void init_selector();
void init_banner();
void update_init_banner();

// Update functions
void create_status_display();  // (if used)
void update_odom_data(double x, double y, double heading);
void update_field_position(double x, double y, double heading);
void update_match_data();
void update_manual_tab();

// Touchscreen and display callbacks
void disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
void touchscreen_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);

// Button event callback for autonomous selector
// Uses dynamic allocation for the button IDs.
void btn_event_cb(lv_event_t* e);

// Switch callbacks for manual tab
void clamp_switch_event_cb(lv_event_t* e);
void intake_switch_event_cb(lv_event_t* e);

// -----------------------------------------------------------------------------
// Autonomous routine prototypes
void Red_Left_Elims();
void Red_Right_Elims();
void Blue_Left_Elims();
void Blue_Right_Elims();
void AWP_Red_Left();
void AWP_Red_Right();
void AWP_Blue_Left();
void AWP_Blue_Right();
void skills_auton_1();
void skills_auton_2();

// -----------------------------------------------------------------------------
// Optional: dedicated LVGL task function prototype
void lvglTask();

#endif  // LVGL_TOUCHSCREEN_HPP
