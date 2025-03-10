#include "main.h"
#include "pros/vision.hpp"
#include "pros/misc.hpp"
#include "pros/screen.hpp"
#include <vector>
#include <cmath>

// Define the AI Vision sensor
pros::Vision ai_vision(20); // Replace 20 with your actual vision sensor port

// Struct to represent detected game elements
struct GameElement {
    double distance;  // Estimated distance to element
    double angle;     // Angle relative to robot heading
    int size;         // Size of element (for determining type)
    int confidence;   // Detection confidence
};

// Function to convert vision coordinates to radar coordinates
std::vector<GameElement> detectGameElements() {
    std::vector<GameElement> elements;
    
    // Take a snapshot with the vision sensor
    // For AI Vision, we need to check for specific game elements
    // Assuming you've already trained the AI Vision to detect game elements
    ai_vision.get_vision(); // Replace 0 with your game element signature ID
    
    // Get number of objects detected
    int numElements = ai_vision.get_object_count();
    
    // Process each detected game element
    for (int i = 0; i < numElements && i < 15; i++) { // Limit to 15 elements max
        pros::vision_object_s_t obj = ai_vision.get_by_size(i);
        
        // Skip invalid objects
        if (obj.width <= 0 || obj.height <= 0) continue;
        
        GameElement element;
        
        // Calculate distance based on object size
        // The formula may need calibration for your specific game elements
        element.size = (obj.width + obj.height) / 2;
        
        // More sophisticated distance calculation based on known game element dimensions
        // Assuming a game element of known size (adjust constants based on your game elements)
        double knownWidth = 20.0; // Width of game element in cm
        double focalLength = 300.0; // Calibration constant for your vision sensor
        element.distance = (knownWidth * focalLength) / obj.width;
        
        // Calculate angle (vision sensor x-coordinate range is typically -158 to 158)
        // Center of vision is x=0, left edge is x=-158, right edge is x=158
        element.angle = obj.x_middle_coord * (M_PI / 316.0); // Convert to radians
        
        // Store confidence level (for AI Vision)
        element.confidence = obj.type;
        
        // Add to our list if it's a valid detection
        if (element.confidence > 50 && element.distance < 300) { // Adjust thresholds as needed
            elements.push_back(element);
        }
    }
    
    return elements;
}

// Function to draw the radar display on the controller
void drawRadar(std::vector<GameElement>& elements) {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);
    
    // Clear the controller screen
    controller.clear();
    
    // Radar display settings
    int centerX = 7; // Center column of the radar display
    int centerY = 2; // Center row of the radar display
    double maxRadius = 6.0; // Maximum display radius in character units
    
    // Create empty radar display grid (8 rows, 16 columns)
    char display[8][17]; // +1 for null terminator
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 16; x++) {
            display[y][x] = ' ';
        }
        display[y][16] = '\0'; // Null terminate each row
    }
    
    // Draw radar border (circle approximation using characters)
    display[centerY-2][centerX] = '-';
    display[centerY+2][centerX] = '-';
    display[centerY][centerX-2] = '|';
    display[centerY][centerX+2] = '|';
    display[centerY-1][centerX-1] = '/';
    display[centerY-1][centerX+1] = '\\';
    display[centerY+1][centerX-1] = '\\';
    display[centerY+1][centerX+1] = '/';
    
    // Mark robot position at center
    display[centerY][centerX] = 'X';
    
    // Add sweep line animation
    static int sweep = 0;
    sweep = (sweep + 30) % 360; // Increment by 30 degrees each update
    int sweepX = centerX + static_cast<int>(cos(sweep * M_PI / 180.0) * 2.0);
    int sweepY = centerY - static_cast<int>(sin(sweep * M_PI / 180.0) * 2.0);
    if (sweepX >= 0 && sweepX < 16 && sweepY >= 0 && sweepY < 8) {
        display[sweepY][sweepX] = '*';
    }
    
    // Plot each game element on the radar
    for (const auto& element : elements) {
        // Scale distance for display
        double scaledDistance = element.distance / 50.0; // Adjust divisor to change scale
        if (scaledDistance > maxRadius) scaledDistance = maxRadius;
        
        // Convert polar coordinates to cartesian for display
        int x = centerX + static_cast<int>(scaledDistance * cos(element.angle));
        int y = centerY - static_cast<int>(scaledDistance * sin(element.angle));
        
        // Ensure coordinates are within screen bounds
        if (x >= 0 && x < 16 && y >= 0 && y < 8) {
            // Different symbols based on confidence/size
            char symbol = 'o';
            if (element.confidence > 90) symbol = 'O';
            if (element.size > 100) symbol = '#'; // Large object
            
            display[y][x] = symbol;
        }
    }
    
    // Print the radar display to controller
    for (int y = 0; y < 8; y++) {
        controller.print(y, 0, "%s", display[y]);
    }
}

void aivision() {
    // Configure AI Vision sensor
    ai_vision.set_zero_point(pros::E_VISION_ZERO_CENTER);
    ai_vision.set_wifi_mode(0); // Turn off WiFi to save power
    
    // Main AI vision radar loop
    while (true) {
        // Get objects from vision sensor
        std::vector<GameElement> gameElements = detectGameElements();
        
        // Draw radar display
        drawRadar(gameElements);
        
        // Optional: Also display on brain screen for debugging
        pros::screen::erase();
        pros::screen::print(TEXT_MEDIUM, 1, "AI Radar Active");
        pros::screen::print(TEXT_MEDIUM, 2, "Elements: %d", gameElements.size());
        
        // Sleep to avoid overwhelming the controller
        pros::delay(100);
    }
}
