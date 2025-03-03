
//──────────────────────────────
// lvgl_touchscreen.cpp
//──────────────────────────────
#include "main.h" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep
#include "pros/misc.hpp"
#include "lvgl_touchscreen.hpp"
#include <cstdio>
#include <cstdlib>
#include "pros/motors.h"

#include "lemlib/api.hpp"
// ----------------------
// Global variables

lv_disp_draw_buf_t draw_buf;
lv_color_t buf1[480 * 10];
lv_disp_drv_t disp_drv;

lv_obj_t* tabview = nullptr;
lv_obj_t* match_tab = nullptr;
lv_obj_t* skills_tab = nullptr;
lv_obj_t* odom_tab = nullptr;
lv_obj_t* manual_tab = nullptr;
lv_obj_t* red_options = nullptr;    // Red autonomous page container
lv_obj_t* blue_options = nullptr;   // Blue autonomous page container
lv_obj_t* skills_options = nullptr; // Skills autonomous container

lv_obj_t* status_container = nullptr;
lv_obj_t* coords_label = nullptr;
lv_obj_t* heading_label = nullptr;
lv_obj_t* temp_label = nullptr;
lv_obj_t* battery_label = nullptr;
lv_obj_t* imu_btn = nullptr;
lv_obj_t* controller_icon = nullptr;
lv_obj_t* field_display = nullptr;

int selected_auton = 0;
const char* auton_names[] = {
    "Red Left Elims", "Red Right Elims",
    "Blue Left Elims", "Blue Right Elims",
    "AWP Red Left",    "AWP Blue Left",
    "AWP Red Right",   "AWP Blue Right",
    "Skills 1",        "Skills 2"
};

// ----------------------
// Display flush callback for PROS V5 Brain
void disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
    for (int32_t y = area->y1; y <= area->y2; y++) {
        for (int32_t x = area->x1; x <= area->x2; x++) {
            pros::screen::set_pen(color_p->full);
            pros::screen::draw_pixel(x, y);
            color_p++;
        }
    }
    lv_disp_flush_ready(disp_drv);
}

// ----------------------
// Touchscreen read callback
void touchscreen_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data) {
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    pros::screen_touch_status_s_t touch_status = pros::screen::touch_status();

    if (touch_status.touch_status) {
        data->state = LV_INDEV_STATE_PRESSED;
        last_x = touch_status.x;
        last_y = touch_status.y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    data->point.x = last_x;
    data->point.y = last_y;
}

// ----------------------
// Splash screen and continue button
void init_splash_screen() {
    lv_obj_t* splash = lv_obj_create(lv_scr_act());
    lv_obj_set_size(splash, 480, 210);
    lv_obj_set_style_bg_color(splash, lv_color_hex(0x202020), 0);
    lv_obj_set_style_bg_opa(splash, LV_OPA_COVER, 0);

    // Team logo/name
    lv_obj_t* logo = lv_label_create(splash);
    lv_obj_set_style_text_font(logo, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(logo, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(logo, "56S\nSELF DESTRUCT SYSTEM");
    lv_obj_align(logo, LV_ALIGN_CENTER, 0, -30);

    // Continue button
    lv_obj_t* cont_btn = lv_btn_create(splash);
    lv_obj_set_size(cont_btn, 160, 40);
    lv_obj_align(cont_btn, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t* btn_label = lv_label_create(cont_btn);
    lv_label_set_text(btn_label, "CONTINUE");
    lv_obj_center(btn_label);

    // When pressed, delete splash and initialize GUI.
    lv_obj_add_event_cb(cont_btn, [](lv_event_t* e) {
        lv_obj_t* splash = lv_obj_get_parent(e->target);
        lv_obj_del(splash);
        init_selector();
    }, LV_EVENT_CLICKED, nullptr);
}

//----------------------
// Banner functions

#include <stdio.h>

// Global object handles for banner updates
static lv_obj_t* red_banner;
static lv_obj_t* controller_img;
static lv_obj_t* box3d;
static lv_obj_t* label_56S;
static lv_obj_t* label_odom;
static lv_obj_t* label_batt;

// Update the banner with current data
void update_init_banner() {
    // Dynamic data; replace these with actual readings as needed.
    float odom_x = chassis.getPose().x;    // Actual odometry x-coordinate
    float odom_y = chassis.getPose().y;    // Actual odometry y-coordinate
    float odom_hdg = chassis.getPose().theta;   // Actual heading angle
    int battery_level = pros::battery::get_capacity(); // Actual battery level

    char buffer[64];

    // Update odometry label
    snprintf(buffer, sizeof(buffer), "odom: (x: %.0f, y: %.0f, hdg: %.0f)", odom_x, odom_y, odom_hdg);
    lv_label_set_text(label_odom, buffer);

    // Update battery label
    snprintf(buffer, sizeof(buffer), "Battery: %d%%", battery_level);
    lv_label_set_text(label_batt, buffer);
}

// Timer callback that updates the banner
static void banner_timer_cb(lv_timer_t* timer) {
    (void)timer;   // unused parameter
    update_init_banner();
}

// Initialize the banner and start the timer for constant updating
void init_banner() {
    // Create the red banner container on the active screen (height 30)
    red_banner = lv_obj_create(lv_scr_act());
    lv_obj_set_size(red_banner, 480, 30);
    lv_obj_set_style_bg_color(red_banner, lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_bg_opa(red_banner, LV_OPA_COVER, 0);
    lv_obj_align(red_banner, LV_ALIGN_BOTTOM_MID, 0, 0);

    // Use a flex layout to space children equally horizontally and center vertically
    lv_obj_set_flex_flow(red_banner, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(red_banner, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Controller icon (make sure controller_icon is valid)
    controller_img = lv_img_create(red_banner);
    lv_img_set_src(controller_img, &controller_icon);  // Replace with your image resource
    lv_obj_set_size(controller_img, 16, 16);

    // Simulated 3D box
    box3d = lv_obj_create(red_banner);
    lv_obj_set_size(box3d, 16, 16);
    lv_obj_clear_flag(box3d, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(box3d, lv_color_hex(0x505050), 0);
    lv_obj_set_style_border_color(box3d, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_border_width(box3d, 2, 0);

    // "56S" label
    label_56S = lv_label_create(red_banner);
    lv_obj_set_style_text_font(label_56S, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label_56S, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(label_56S, "56S");

    // Odom data label (initial text)
    label_odom = lv_label_create(red_banner);
    lv_label_set_text(label_odom, "odom: (x: n/a, y: n/a, hdg: n/a)");

    // Battery level label (initial text)
    label_batt = lv_label_create(red_banner);
    lv_label_set_text(label_batt, "Battery: 100%");

    // Create a timer that calls banner_timer_cb every 100ms
    lv_timer_create(banner_timer_cb, 100, NULL);
}

// ----------------------
// Create a styled button (used for auton options)
// Uses dynamic allocation for the button ID.
static lv_obj_t* create_styled_btn(lv_obj_t* parent, const char* text, int x, int y, int id) {
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, lv_color_hex(0x808080));
    lv_style_set_border_color(&style_btn, lv_color_hex(0x000000));
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_radius(&style_btn, 8);

    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_set_size(btn, 130, 35);
    lv_obj_set_pos(btn, x, y);

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    // Allocate an integer on the heap for this button’s ID
    int* btn_id = new int(id);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, btn_id);

    return btn;
}

// ----------------------
// Button event callback for auton selection.
// Now, before highlighting the pressed button, we reset selections in
// all autonomous containers so that only one button remains highlighted.
void btn_event_cb(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    int* btn_id = static_cast<int*>(lv_event_get_user_data(e));
    selected_auton = *btn_id;  // Update global selection

    // Clear button highlights in all containers:
    lv_obj_t* containers[] = { red_options, blue_options, skills_options };
    for (int j = 0; j < 3; j++) {
        if (containers[j] != nullptr) {
            uint32_t count = lv_obj_get_child_cnt(containers[j]);
            for (uint32_t i = 0; i < count; i++) {
                lv_obj_t* child = lv_obj_get_child(containers[j], i);
                if (lv_obj_check_type(child, &lv_btn_class)) {
                    lv_obj_set_style_bg_color(child, lv_color_hex(0x808080), 0);
                }
            }
        }
    }
    // Highlight the pressed button.
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x32CD32), 0);

    update_match_data();
}

// ----------------------
// Switch event callbacks for manual tab
void clamp_switch_event_cb(lv_event_t* e) {
    lv_obj_t* sw = lv_event_get_target(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    // Example: clamp_piston.set_value(state);
}

void intake_switch_event_cb(lv_event_t* e) {
    lv_obj_t* sw = lv_event_get_target(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    // Example: intake_piston.set_value(state);
}



// ----------------------
// Selector initialization: builds main tabview with Match, Skills, Odom, and Manual tabs.
// Modified to condense the UI to the top 210 pixels (240 - 30 for banner)
void init_selector() {
    // Create main tabview with a top bar (height 40)
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 30);
    // Set its size to the full width and available height (240 - banner height 30 = 210)
    lv_obj_set_size(tabview, 480, 210);
    // Align the tabview to the top so the banner at the bottom isn’t overlapped
    lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 0);

    // Create tabs
    match_tab  = lv_tabview_add_tab(tabview, "Match");
    skills_tab = lv_tabview_add_tab(tabview, "Skills");
    odom_tab   = lv_tabview_add_tab(tabview, "Odom");
    manual_tab = lv_tabview_add_tab(tabview, "Manual");

    // --- MATCH TAB ---
    // Nested tabview for autonomous selection in Match.
    lv_obj_t* auton_tabview = lv_tabview_create(match_tab, LV_DIR_TOP, 30);
    lv_obj_set_size(auton_tabview, 480, 200);

    // Create pages:
    red_options = lv_tabview_add_tab(auton_tabview, "Red");
    blue_options = lv_tabview_add_tab(auton_tabview, "Blue");

    // For Red page:
    create_styled_btn(red_options, auton_names[0], 15, 10, 0);
    create_styled_btn(red_options, auton_names[1], 165, 10, 1);
    create_styled_btn(red_options, auton_names[4], 15, 65, 4);
    create_styled_btn(red_options, auton_names[6], 165, 65, 6);

    // For Blue page:
    create_styled_btn(blue_options, auton_names[2], 15, 10, 2);
    create_styled_btn(blue_options, auton_names[3], 165, 10, 3);
    create_styled_btn(blue_options, auton_names[5], 15, 65, 5);
    create_styled_btn(blue_options, auton_names[7], 165, 65, 7);

    // --- SKILLS TAB ---
    skills_options = lv_obj_create(skills_tab);
    lv_obj_set_size(skills_options, 440, 140);
    lv_obj_align(skills_options, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_bg_opa(skills_options, 0, 0);
    create_styled_btn(skills_options, auton_names[8], 15, 5, 9);
    create_styled_btn(skills_options, auton_names[9], 165, 5, 10);

    // --- ODOM TAB ---
    field_display = lv_img_create(odom_tab);
    lv_obj_set_size(field_display, 180, 180);
    lv_obj_align(field_display, LV_ALIGN_LEFT_MID, 10, 0);
    lv_img_set_src(field_display, "S:/vex_field_image.png");  // Replace with your PNG filename on the SD card


    lv_obj_t* data_container = lv_obj_create(odom_tab);
    lv_obj_set_size(data_container, 180, 180);
    lv_obj_align(data_container, LV_ALIGN_RIGHT_MID, -10, 0);
    coords_label = lv_label_create(data_container);
    heading_label = lv_label_create(data_container);
    lv_obj_align(coords_label, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_align(heading_label, LV_ALIGN_TOP_LEFT, 5, 50);

    // --- MANUAL TAB ---
    lv_obj_t* main_container = lv_obj_create(manual_tab);
    lv_obj_set_size(main_container, 460, 180);
    lv_obj_align(main_container, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_pad_all(main_container, 5, 0);
    lv_obj_set_style_bg_opa(main_container, 0, 0);

    lv_obj_t* temp_container = lv_obj_create(main_container);
    lv_obj_set_size(temp_container, 220, 180);
    lv_obj_align(temp_container, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_opa(temp_container, 0, 0);
    temp_label = lv_label_create(temp_container);
    lv_obj_align(temp_label, LV_ALIGN_TOP_LEFT, 5, 5);

    lv_obj_t* control_container = lv_obj_create(main_container);
    lv_obj_set_size(control_container, 220, 180);
    lv_obj_align(control_container, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_opa(control_container, 0, 0);

    lv_obj_t* clamp_switch = lv_switch_create(control_container);
    lv_obj_align(clamp_switch, LV_ALIGN_TOP_LEFT, 10, 20);
    lv_obj_add_event_cb(clamp_switch, clamp_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_t* clamp_label = lv_label_create(control_container);
    lv_label_set_text(clamp_label, "Clamp Piston");
    lv_obj_align_to(clamp_label, clamp_switch, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_t* intake_switch = lv_switch_create(control_container);
    lv_obj_align(intake_switch, LV_ALIGN_TOP_LEFT, 10, 70);
    lv_obj_add_event_cb(intake_switch, intake_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_t* intake_label = lv_label_create(control_container);
    lv_label_set_text(intake_label, "Intake Piston");
    lv_obj_align_to(intake_label, intake_switch, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
}

// ----------------------
// Update functions

void update_odom_data(double x, double y, double heading) {
    if (!lv_obj_is_valid(coords_label) || !lv_obj_is_valid(heading_label))
        return;

    char coords_str[64];
    snprintf(coords_str, sizeof(coords_str), "X: %.2f\nY: %.2f", x, y);
    lv_label_set_text(coords_label, coords_str);

    char heading_str[32];
    snprintf(heading_str, sizeof(heading_str), "Heading: %.2f°", heading);
    lv_label_set_text(heading_label, heading_str);

    update_field_position(x, y, heading);
}

void update_field_position(double x, double y, double heading) {
    if (!lv_obj_is_valid(field_display))
        return;

    static lv_obj_t* field_label = nullptr;
    if (!field_label) {
        field_label = lv_label_create(field_display);
        lv_obj_set_style_text_font(field_label, &lv_font_montserrat_16, 0);
        lv_obj_align(field_label, LV_ALIGN_CENTER, 0, 0);
    }

    char pos_str[64];
    snprintf(pos_str, sizeof(pos_str), "X: %.2f\nY: %.2f\nH: %.2f", x, y, heading);
    lv_label_set_text(field_label, pos_str);
}

void update_match_data() {
    static lv_obj_t* auton_label = nullptr;
    if (!auton_label) {
        auton_label = lv_label_create(match_tab);
        lv_obj_set_style_text_font(auton_label, &lv_font_montserrat_16, 0);
        lv_obj_align(auton_label, LV_ALIGN_CENTER, 0, 100);
    }
    lv_label_set_text(auton_label, auton_names[selected_auton]);
}

void update_manual_tab() {
    if (temp_label) {
        // Dummy temperature values.
        double left_temp = 0, right_temp = 0, intake_temp = intake11w.get_temperature(pros::E_MOTOR_ENCODER_DEGREES), ring_arm_temp = lift_motor.get_temperature(pros::E_MOTOR_ENCODER_DEGREES);
        char temp_str[200];
        snprintf(temp_str, sizeof(temp_str),
                 "Motor Temperatures:\n\n"
                 "Left Drive:  %.1f°C\n"
                 "Right Drive: %.1f°C\n"
                 "Intake:      %.1f°C\n"
                 "lift Arm:    %.1f°C",
                 left_temp, right_temp, intake_temp, ring_arm_temp);
        lv_label_set_text(temp_label, temp_str);
    }
}

#include <iostream> // Add this include for debug prints

#include <iostream> // Add this include for debug prints

void update_selected_autons() {
    while(pros::competition::is_disabled() == true) {
        // Check which button was pressed and update selected_auton
        lv_obj_t* containers[] = { red_options, blue_options, skills_options };
        const char* container_names[] = { "Red Options", "Blue Options", "Skills Options" };
        
        for (int j = 0; j < 3; j++) {
            lv_obj_t* container = containers[j];
            if (container != nullptr) {
                uint32_t child_count = lv_obj_get_child_cnt(container);
                std::cout << "Checking container: " << container_names[j] << " with " << child_count << " children." << std::endl;
                
                for (uint32_t i = 0; i < child_count; i++) {
                    lv_obj_t* btn = lv_obj_get_child(container, i);
                    if (lv_obj_check_type(btn, &lv_btn_class)) {
                        std::cout << "Checking button " << i << " in " << container_names[j] << std::endl;
                        
                        // If this button is highlighted (selected)
                        lv_color_t btn_color = lv_obj_get_style_bg_color(btn, 0);
                        std::cout << "Button color: " << btn_color.full << std::endl;
                        
                        if (btn_color.full == lv_color_hex(0x32CD32).full) {
                            std::cout << "Button " << i << " is selected in " << container_names[j] << std::endl;
                            
                            // Get the button's ID from its user data
                            int* btn_id = (int*)lv_obj_get_user_data(btn);
                            if (btn_id != nullptr) {
                                selected_auton = *btn_id;
                                std::cout << "Selected auton ID: " << selected_auton << " from " << container_names[j] << std::endl;
                            } else {
                                std::cout << "Button ID is null in " << container_names[j] << std::endl;
                            }
                            break;
                        }
                    } else {
                        std::cout << "Object " << i << " is not a button in " << container_names[j] << std::endl;
                    }
                }
            } else {
                std::cout << "Container " << container_names[j] << " is null." << std::endl;
            }
        }
        pros::delay(10); // Add a small delay to prevent excessive CPU usage
    }
}


// ----------------------
// Dedicated LVGL task.
void lvglTask() {
    while (true) {
        lv_task_handler();
        pros::delay(5);
    }
}

// ----------------------
// End of GUI functions; now the main initialization and opcontrol routines.