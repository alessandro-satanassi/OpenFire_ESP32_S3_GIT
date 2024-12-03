/*!
 * @file SamcoDisplay.h
 * @brief Macros for lightgun HUD display.
 *
 * @copyright That One Seong, 2024
 *
 *  SamcoDisplay is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _SAMCODISPLAY_H_
#define _SAMCODISPLAY_H_

#include <stdint.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class ExtDisplay {
public:
    /// @brief Constructor
    ExtDisplay();

    /// @brief Attempt to start display using current pin numbers from SamcoPreferences
    /// @return success (true) or fail (false)
    bool Begin();

    /// @brief Update top panel with new info
    /// @return nothing
    void TopPanelUpdate(const char textPrefix[7], const char textInput[16]);

    /// @brief Clear screen for different gun modes
    /// @return nothing
    void ScreenModeChange(int8_t screenMode, bool isAnalog = false);

    /// @brief Perform maintenance operations (WIP)
    /// @details For when values aren't being updated, but still want to change something on the screen
    /// (i.e. small text printouts when health/ammo empty)
    /// @return nothing
    void IdleOps();

    /// @brief Draw seen points here
    /// @details Should ONLY be used in scenarios where the mouse isn't being updated, i.e. calibration.
    /// @return nothing
    void DrawVisibleIR(int pointX[4], int pointY[4]);

    /// @brief Draw hotkey pause mode layout
    /// @return nothing
    void PauseScreenShow(uint8_t currentProf, char name1[16], char name2[16], char name3[16], char name4[16]);

    /// @brief Update simple pause mode list on screen
    /// @return nothing
    void PauseListUpdate(uint8_t selection);

    /// @brief Update simple pause mode profiles list on screen
    /// @return nothing
    void PauseProfileUpdate(uint8_t selection, char name1[16], char name2[16], char name3[16], char name4[16]);

    /// @brief Print save status message
    void SaveScreen(uint8_t status);

    /// @brief Update main screen ammo glyphs
    /// @return nothing
    void PrintAmmo(uint8_t ammo);

    /// @brief Update main screen life glyphs
    /// @return nothing
    void PrintLife(uint8_t life);

    enum ScreenMode_e {
        Screen_None = -1,
        Screen_Init = 0,
        Screen_Normal,
        Screen_Pause,
        Screen_Profile,
        Screen_Saving,
        Screen_SaveSuccess,
        Screen_SaveError,
        Screen_Calibrating,
        Screen_IRTest,
        Screen_Docked,
        Screen_Mamehook_Single,
        Screen_Mamehook_Dual
    };

    enum ScreenPauseList_e {
        ScreenPause_Calibrate = 0,
        ScreenPause_ProfileSelect,
        ScreenPause_Save,
        ScreenPause_Rumble,
        ScreenPause_Solenoid,
        ScreenPause_EscapeKey
    };

    enum ScreenSerialInit_e {
        ScreenSerial_None = 0,
        ScreenSerial_Life,
        ScreenSerial_Ammo,
        ScreenSerial_Both
    };

    /// @brief Whether life updates are in lifebar or life glyphs form
    bool lifeBar = false;

    /// @brief Which layout we use for serial mode
    uint8_t serialDisplayType = 0;

private:
    bool displayValid = false;

    int8_t screenState = Screen_None;

    bool ammoEmpty = false;
    bool lifeEmpty = false;

    uint8_t currentAmmo;
    uint8_t currentLife;

    // timestamps, in case we need them for periodic tasks in IdleOps()
    unsigned long ammoTimestamp = 0;
    unsigned long lifeTimestamp = 0;
    unsigned long idleTimeStamp = 0;


    #define NUMBER_GLYPH_WIDTH 21
    #define NUMBER_GLYPH_HEIGHT 36
    /// @brief Glyphs for common HUD elements
    static constexpr uint8_t number_0[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x5f, 0xff, 0xd0, 0xe0, 0x00, 0x78, 0xe0, 0x00, 0xf8, 0xe0,
        0x00, 0xf8, 0xe0, 0x01, 0xb8, 0xe0, 0x01, 0xb8, 0xe0, 0x03, 0x38, 0xe0, 0x03, 0x38, 0xe0, 0x06,
        0x38, 0xe0, 0x06, 0x38, 0xe0, 0x0c, 0x38, 0xe0, 0x0c, 0x38, 0xe0, 0x18, 0x38, 0xe0, 0x18, 0x38,
        0xef, 0xff, 0xb8, 0x5f, 0xff, 0xd0, 0xef, 0xff, 0xb8, 0xe0, 0x60, 0x38, 0xe0, 0x60, 0x38, 0xe0,
        0xc0, 0x38, 0xe0, 0xc0, 0x38, 0xe1, 0x80, 0x38, 0xe1, 0x80, 0x38, 0xe3, 0x00, 0x38, 0xe3, 0x00,
        0x38, 0xe6, 0x00, 0x38, 0xe6, 0x00, 0x38, 0xec, 0x00, 0x38, 0xec, 0x00, 0x38, 0xf8, 0x00, 0x38,
        0xf8, 0x00, 0x38, 0x5f, 0xff, 0xd0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0
    };

    static constexpr uint8_t number_1[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x00, 0x00, 0x38, 0x00, 0x00, 0x10, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x00, 0x00, 0x38, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    static constexpr uint8_t number_2[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xd0, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x0f, 0xff, 0xb8, 0x5f, 0xff, 0xd0, 0xef, 0xff, 0x80, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0,
        0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00,
        0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00,
        0xe0, 0x00, 0x00, 0x5f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0
    };

    static constexpr uint8_t number_3[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xd0, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x0f, 0xff, 0xb8, 0x1f, 0xff, 0xd0, 0x0f, 0xff, 0xb8, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x00, 0x00, 0x38, 0x1f, 0xff, 0xd0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0
    };

    static constexpr uint8_t number_4[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x10, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0,
        0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00,
        0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38,
        0xef, 0xff, 0xb8, 0x5f, 0xff, 0xd0, 0x0f, 0xff, 0xb8, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x00, 0x00, 0x38, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    static constexpr uint8_t number_5[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x5f, 0xff, 0xc0, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0,
        0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00,
        0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00,
        0xef, 0xff, 0x80, 0x5f, 0xff, 0xd0, 0x0f, 0xff, 0xb8, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x00, 0x00, 0x38, 0x1f, 0xff, 0xd0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0
    };

    static constexpr uint8_t number_6[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x5f, 0xff, 0xc0, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0,
        0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00,
        0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00,
        0xef, 0xff, 0x80, 0x5f, 0xff, 0xd0, 0xef, 0xff, 0xb8, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0,
        0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00,
        0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38,
        0xe0, 0x00, 0x38, 0x5f, 0xff, 0xd0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0
    };

    static constexpr uint8_t number_7[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x40, 0x00, 0x00, 0xc0, 0x00,
        0x00, 0xc0, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06,
        0x00, 0x00, 0x06, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00,
        0x00, 0x30, 0x00, 0x00, 0x30, 0x00, 0x00, 0x30, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00,
        0xc0, 0x00, 0x00, 0xc0, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
        0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x18, 0x00, 0x00,
        0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    static constexpr uint8_t number_8[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x5f, 0xff, 0xd0, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0,
        0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00,
        0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38,
        0xef, 0xff, 0xb8, 0x5f, 0xff, 0xd0, 0xef, 0xff, 0xb8, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0,
        0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00,
        0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38,
        0xe0, 0x00, 0x38, 0x5f, 0xff, 0xd0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0
    };

    static constexpr uint8_t number_9[] = {
        0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x5f, 0xff, 0xd0, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0,
        0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00,
        0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0x38,
        0xef, 0xff, 0xb8, 0x5f, 0xff, 0xd0, 0x0f, 0xff, 0xb8, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
        0x00, 0x00, 0x38, 0x1f, 0xff, 0xd0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xc0
    };

    #define HEART_SMALL_WIDTH 12
    #define HEART_SMALL_HEIGHT 16
    static constexpr uint8_t lifeIcoSmall[] = {
        0xff, 0xf0, 0x80, 0x10, 0x80, 0x10, 0x80, 0x10, 0x90, 0x90, 0xb9, 0xd0, 0xbf, 0xd0, 0xbf, 0xd0,
        0xbf, 0xd0, 0x9f, 0x90, 0x8f, 0x10, 0x86, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x10, 0xff, 0xf0
    };

    #define HEART_LARGE_WIDTH 16
    #define HEART_LARGE_HEIGHT 20
    static constexpr uint8_t lifeIcoLarge[] = {
        0xff, 0xff, 0xc0, 0x03, 0xbf, 0xfd, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa4, 0x25, 0xae, 0x75, 
        0xaf, 0xf5, 0xaf, 0xf5, 0xaf, 0xf5, 0xa7, 0xe5, 0xa3, 0xc5, 0xa1, 0x85, 0xa0, 0x05, 0xa0, 0x05, 
        0xa0, 0x05, 0xbf, 0xfd, 0xc0, 0x03, 0xff, 0xff
    };

    #define LIFEBAR_BANNER_WIDTH 23
    #define LIFEBAR_BANNER_HEIGHT 9
    static constexpr uint8_t lifeBarBanner[] = {
        0xc1, 0xbf, 0x7e, 0xc1, 0xbf, 0x7e, 0xc1, 0xb8, 0x60, 0xc1, 0xb0, 0x60, 0xc1, 0xbe, 0x7c, 0xc1, 
        0xbe, 0x60, 0xc1, 0xb0, 0x60, 0xfd, 0xb0, 0x7e, 0xfd, 0xb0, 0x7e
    };

    #define LIFEBAR_SMALL_WIDTH 59
    #define LIFEBAR_SMALL_HEIGHT 9
    
    
    static constexpr uint8_t lifeBarSmall[] = {
        0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
        0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
        0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
        0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
        0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80
    };

    #define LIFEBAR_LARGE_WIDTH 106
    #define LIFEBAR_LARGE_HEIGHT 13
    static constexpr uint8_t lifeBarLarge[] = {
        0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x60, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x01, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 
        0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0xe0, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x60, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xfe, 0x00
    };

    static constexpr uint8_t reloadGlyph[] = {
        // RELOAD
    };

    #define DIVIDER_WIDTH 1
    #define DIVIDER_HEIGHT 48
    // if only we could draw this vertically, sigh
    static constexpr uint8_t dividerLine[] = {
        0x40, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 
        0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 
        0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00
    };

    #define ARROW_WIDTH 9
    #define ARROW_HEIGHT 5
    static constexpr uint8_t upArrowGlyph[] = {
        0x08, 0x00, 0x14, 0x00, 0x22, 0x00, 0x41, 0x00, 0x80, 0x80
    };

    static constexpr uint8_t downArrowGlyph[] = {
        0x80, 0x80, 0x41, 0x00, 0x22, 0x00, 0x14, 0x00, 0x08, 0x00
    };

    #define CONNECTION_WIDTH 10
    #define CONNECTION_HEIGHT 17
    static constexpr uint8_t btConnectIco[] = {
        0x18, 0x00, 0x1c, 0x00, 0x1e, 0x00, 0x1b, 0x00, 0xd9, 0x80, 0x78, 0xc0, 0x39, 0x80, 0x1b, 0x00, 
        0x1e, 0x00, 0x1b, 0x00, 0x39, 0x80, 0x78, 0xc0, 0xd9, 0x80, 0x1b, 0x00, 0x1e, 0x00, 0x1c, 0x00, 
        0x18, 0x00
    };

    static constexpr uint8_t usbConnectIco[] = {
        0x3f, 0x00, 0x2d, 0x00, 0x2d, 0x00, 0x2d, 0x00, 0x7f, 0x80, 0xc0, 0xc0, 0x88, 0x40, 0x9c, 0x40, 
        0x89, 0x40, 0xa9, 0x40, 0xaa, 0x40, 0xac, 0x40, 0x98, 0x40, 0x88, 0x40, 0x80, 0x40, 0xff, 0xc0, 
        0x7f, 0x80
    };

    #define GAMEPAD_WIDTH 18
    #define GAMEPAD_HEIGHT 12

    static constexpr uint8_t gamepadIco[] = {
        0x38, 0x07, 0x00, 0x7d, 0xef, 0x80, 0x42, 0x10, 0x80, 0x80, 0x00, 0x40, 0x91, 0x22, 0x40, 0xb8, 
        0x05, 0x40, 0x93, 0x32, 0x40, 0x84, 0xc8, 0x40, 0x84, 0xc8, 0x40, 0x87, 0x38, 0x40, 0x44, 0x08, 
        0x80, 0x38, 0x07, 0x00
    };

    #define MOUSE_WIDTH 16
    #define MOUSE_HEIGHT 13

    static constexpr uint8_t mouseIco[] = {
        0x06, 0x00, 0x09, 0x03, 0x10, 0x84, 0x7c, 0x48, 0x92, 0x48, 0x92, 0x30, 0x92, 0x00, 0xfe, 0x00, 
        0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x44, 0x00, 0x38, 0x00
    };

    #define MAMEHOOK_WIDTH 21
    #define MAMEHOOK_HEIGHT 16

    static constexpr uint8_t mamehookIco[] = {
        0x00, 0x02, 0x08, 0x00, 0x06, 0x18, 0x00, 0x0e, 0x38, 0x00, 0x1a, 0x68, 0x00, 0x32, 0xc8, 0x00, 
        0x63, 0x88, 0x00, 0xc3, 0x08, 0x01, 0x82, 0x08, 0x03, 0x00, 0x08, 0x06, 0x10, 0x08, 0x0c, 0x30, 
        0x88, 0x18, 0x71, 0x88, 0x30, 0xb3, 0x88, 0x61, 0x35, 0x88, 0xc2, 0x39, 0x88, 0xfc, 0x31, 0xf8
    };

    #define CUSTSPLASHBANN_WIDTH 80
    #define CUSTSPLASHBANN_HEIGHT 16
    static constexpr uint8_t customSplashBanner[] = {
        0x07, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xef, 0xf1, 0xff, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x03, 
        0xfe, 0xef, 0xf9, 0xff, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xef, 0xf9, 0xc0, 0x7a, 0xf0, 
        0x00, 0x00, 0x00, 0x03, 0x80, 0xee, 0x1d, 0xc0, 0x72, 0x73, 0xfc, 0x1f, 0x8f, 0xf3, 0x80, 0xee, 
        0x0d, 0xc0, 0xe2, 0x3b, 0xfe, 0x3f, 0xcf, 0xfb, 0xfe, 0xee, 0x1d, 0xff, 0xff, 0xfb, 0x8f, 0x78, 
        0xef, 0xfb, 0xfe, 0xef, 0xf9, 0xff, 0xe2, 0x3b, 0x87, 0x70, 0x6e, 0x7b, 0xfe, 0xef, 0xf9, 0xc0, 
        0x72, 0x73, 0x8f, 0x7f, 0xee, 0x3b, 0x80, 0xef, 0xe1, 0xc0, 0x7a, 0xf3, 0xfe, 0x70, 0x0e, 0x3b, 
        0x80, 0xee, 0xf1, 0xc0, 0x3f, 0xe3, 0xfc, 0x78, 0xee, 0x3b, 0x80, 0xee, 0x79, 0xff, 0x1f, 0xc3, 
        0x80, 0x3f, 0xce, 0x3b, 0x80, 0xee, 0x3d, 0xff, 0x07, 0x03, 0x80, 0x1f, 0x8e, 0x3b, 0x80, 0xee, 
        0x1d, 0xff, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    #define CUSTSPLASH_WIDTH  48
    #define CUSTSPLASH_HEIGHT 48
    static constexpr uint8_t customSplash[] = {
        0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 
        0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 
        0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xf8, 0x1f, 0xf0, 0x00, 0x00, 0x1f, 0xc0, 0x03, 0xf8, 0x00, 
        0x00, 0x3f, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x7e, 0x07, 0xe0, 0x7e, 0x00, 0x00, 0xfc, 0x1f, 0xf8, 
        0x3f, 0x00, 0x00, 0xf8, 0x7f, 0xfe, 0x1f, 0x00, 0x01, 0xf0, 0xfc, 0x3f, 0x0f, 0x80, 0x01, 0xe1, 
        0xf0, 0x0f, 0x87, 0x80, 0x03, 0xe3, 0xe0, 0x07, 0xc7, 0xc0, 0x03, 0xc3, 0xc0, 0x03, 0x03, 0xc0, 
        0x07, 0xc7, 0x80, 0x00, 0x03, 0xe0, 0x07, 0x87, 0x80, 0x00, 0x01, 0xe0, 0x07, 0x8f, 0x00, 0x00, 
        0x01, 0xe0, 0x0f, 0x8f, 0x00, 0x00, 0x01, 0xf0, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x7b, 0xff, 0xff, 0xff, 
        0xff, 0x0f, 0x7b, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x7b, 0xc0, 0x00, 0xf0, 0x0f, 0x0f, 0x7b, 0xc0, 
        0x00, 0xf0, 0x0f, 0x80, 0x03, 0xc0, 0x01, 0xf0, 0x07, 0x8f, 0x7b, 0xc0, 0x01, 0xe0, 0x07, 0x8f, 
        0x7b, 0xc0, 0x01, 0xe0, 0x07, 0xcf, 0x7b, 0xff, 0x83, 0xe0, 0x03, 0xcf, 0x7b, 0xff, 0x83, 0xc0, 
        0x03, 0xe0, 0x03, 0xff, 0x87, 0xc0, 0x01, 0xe0, 0x03, 0xc0, 0x07, 0x80, 0x01, 0xf0, 0x03, 0xc0, 
        0x0f, 0x80, 0x00, 0xf8, 0x03, 0xc0, 0x1f, 0x00, 0x00, 0x7c, 0x03, 0xc0, 0x3e, 0x00, 0x00, 0x7e, 
        0x03, 0xc0, 0x7e, 0x00, 0x00, 0x3f, 0x03, 0xc0, 0xfc, 0x00, 0x00, 0x1f, 0xc3, 0xc3, 0xf8, 0x00, 
        0x00, 0x07, 0xfb, 0xdf, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 
        0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 
        0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00
    };
};
/*
/// NECESSARIO PER COMPILATORE C++ 11 - DAL COMOPILATORE C++17 O SUCCESSIVO NON SERVE
constexpr uint8_t ExtDisplay::number_0[];
constexpr uint8_t ExtDisplay::number_1[];
constexpr uint8_t ExtDisplay::number_2[];
constexpr uint8_t ExtDisplay::number_3[];
constexpr uint8_t ExtDisplay::number_4[];
constexpr uint8_t ExtDisplay::number_5[];
constexpr uint8_t ExtDisplay::number_6[];
constexpr uint8_t ExtDisplay::number_7[];
constexpr uint8_t ExtDisplay::number_8[];
constexpr uint8_t ExtDisplay::number_9[];
constexpr uint8_t ExtDisplay::lifeIcoSmall[];
constexpr uint8_t ExtDisplay::lifeIcoLarge[];
constexpr uint8_t ExtDisplay::lifeBarBanner[];
constexpr uint8_t ExtDisplay::lifeBarSmall[];
constexpr uint8_t ExtDisplay::lifeBarLarge[];
//constexpr uint8_t ExtDisplay::reloadGlyph[];
constexpr uint8_t ExtDisplay::dividerLine[];
constexpr uint8_t ExtDisplay::upArrowGlyph[];
constexpr uint8_t ExtDisplay::downArrowGlyph[];
constexpr uint8_t ExtDisplay::btConnectIco[];
constexpr uint8_t ExtDisplay::usbConnectIco[];
constexpr uint8_t ExtDisplay::gamepadIco[];
constexpr uint8_t ExtDisplay::mouseIco[];
constexpr uint8_t ExtDisplay::customSplashBanner[];
constexpr uint8_t ExtDisplay::customSplash[];
/// FINE -NECESSARIO PER COMPILATORE C++ 11 - DAL COMOPILATORE C++17 O SUCCESSIVO NON SERVE
*/

#endif // _SAMCODISPLAY_H_
