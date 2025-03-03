#ifndef COLORSORT_HPP
#define COLORSORT_HPP



// Define Alliance enum if not already defined elsewhere
enum class Alliance {
    RED_ALLIANCE,
    BLUE_ALLIANCE
};

/**
 * Start the autonomous intake color sorting task
 * 
 * @param alliance The alliance color (RED_ALLIANCE or BLUE_ALLIANCE)
 */
void startAutonomousIntakeColorsort(Alliance alliance);
/**
 * Check if the autonomous intake color sorting task is running
 * 
 * @return true if the task is running, false otherwise
 */

/**
 * Stop the autonomous intake color sorting task
 */
void stopAutonomousIntakeColorsort();

/**
 * The main task function for color sorting during autonomous
 * 
 * @param pAlliance Pointer to the alliance color
 */
void autonomousIntakeColorsortTask(void* pAlliance);

#endif // COLORSORT_HPP
