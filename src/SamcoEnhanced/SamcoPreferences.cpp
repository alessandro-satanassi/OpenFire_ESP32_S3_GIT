/*!
 * @file SamcoPreferences.cpp
 * @brief Samco Prow Enhanced light gun preferences to save in non-volatile memory.
 *
 * @copyright Mike Lynch, 2021
 * @copyright GNU Lesser General Public License
 *
 * @author Mike Lynch
 * @author [That One Seong](SeongsSeongs@gmail.com)
 * @version V1.1
 * @date 2023
 */

#include "SamcoPreferences.h"
#include <Arduino.h>

#ifdef SAMCO_EEPROM_ENABLE
#include <EEPROM.h>   // obsoleta per ESP32, SOSTITUIRE IN FUTURO CON <Preferences.h>
#endif // SAMCO_EEPROM_ENABLE

// 4 byte header ID
/*
 * The latter two characters of the header correlate to the Save Table Version (see SamcoPreferences.h).
 * If the save table is ever changed (values added/removed or changed order/size),
 * the version number MUST be incremented in tandem so that the firmware will
 * appropriately update previously saved tables (TODO) or reset current NVRAM.
 * Failure to do this might cause save corruption or other undefined behavior.
 */
const SamcoPreferences::HeaderId_t SamcoPreferences::HeaderId = {'O', 'F', '0', '1'};

#ifdef SAMCO_EEPROM_ENABLE
void SamcoPreferences::WriteHeader()
{
    EEPROM.put(0, HeaderId.u32);
}

int SamcoPreferences::CheckHeader()
{
    uint32_t u32;
    EEPROM.get(0, u32);
    if(u32 != HeaderId.u32) {
        return Error_NoData;
    } else {
        return Error_Success;
    }
}

int SamcoPreferences::LoadProfiles()
{
    int status = CheckHeader();
    if(status == Error_Success) {
        profiles.selectedProfile = EEPROM.read(4);
        uint8_t* p = ((uint8_t*)profiles.pProfileData);
        for(unsigned int i = 0; i < sizeof(ProfileData_t) * profiles.profileCount; ++i) {
            p[i] = EEPROM.read(5 + i);
        }
        return Error_Success;
    } else {
        return status;
    }
}

int SamcoPreferences::SaveProfiles()
{
    WriteHeader();
    
    #ifdef ARDUINO_ESP32S3_DEV  //esp32
        //EEPROM.write(4, profiles.selectedProfile);
        EEPROM.put(4, profiles.selectedProfile);
    #else //rp2040
        EEPROM.update(4, profiles.selectedProfile);
    #endif
    
    uint8_t* p = ((uint8_t*)profiles.pProfileData);
    for(unsigned int i = 0; i < sizeof(ProfileData_t) * profiles.profileCount; ++i) {
        EEPROM.write(5 + i, p[i]);
    }

    // Remember that we need to commit changes to the virtual EEPROM on RP2040!
    EEPROM.commit();
    return Error_Success;
}

int SamcoPreferences::LoadToggles()
{
    int status = CheckHeader();
    if(status == Error_Success) {
        EEPROM.get(300, toggles);
        return Error_Success;
    } else {
        return status;
    }
}

int SamcoPreferences::SaveToggles()
{
    WriteHeader();
    EEPROM.put(300, toggles);
    EEPROM.commit();
    return Error_Success;
}

int SamcoPreferences::LoadPins()
{
    int status = CheckHeader();
    if(status == Error_Success) {
        EEPROM.get(350, pins);
        return Error_Success;
    } else {
        return status;
    }
}

int SamcoPreferences::SavePins()
{
    WriteHeader();
    EEPROM.put(350, pins);
    EEPROM.commit();
    return Error_Success;
}

int SamcoPreferences::LoadSettings()
{
    int status = CheckHeader();
    if(status == Error_Success) {
        EEPROM.get(400, settings);
        return Error_Success;
    } else {
        return status;
    }
}

int SamcoPreferences::SaveSettings()
{
    WriteHeader();
    EEPROM.put(400, settings);
    EEPROM.commit();
    return Error_Success;
}

int SamcoPreferences::LoadUSBID()
{
    int status = CheckHeader();
    if(status == Error_Success) {
        EEPROM.get(900, usb);
        return Error_Success;
    } else {
        return status;
    }
}

int SamcoPreferences::SaveUSBID()
{
    WriteHeader();
    EEPROM.put(900, usb);
    EEPROM.commit();
    return Error_Success;
}

void SamcoPreferences::ResetPreferences()
{
    for(uint16_t i = 0; i < EEPROM.length(); ++i) {
        #ifdef ARDUINO_ESP32S3_DEV  //esp32
            //EEPROM.write(i, 0);
            EEPROM.put(i, 0);
        #else //rp2040
            EEPROM.update(i, 0);
        #endif        
    }

    EEPROM.commit();
}

void SamcoPreferences::LoadPresets()
{
// For the Adafruit ItsyBitsy RP2040 - optimized for SAMCO boards
#ifdef ARDUINO_ADAFRUIT_ITSYBITSY_RP2040

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = -1;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 24;
    pins.oSolenoid = 25;
    pins.bTrigger = 6;
    pins.bGunA = 27;
    pins.bGunB = 26;
    pins.bGunC = 11;
    pins.bStart = 28;
    pins.bSelect = 29;
    pins.bGunUp = 9;
    pins.bGunDown = 7;
    pins.bGunLeft = 8;
    pins.bGunRight = 10;
    pins.bPedal = 4;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = -1;

// For the Adafruit KB2040 - optimized for GUN4IR boards
#elif defined(ARDUINO_ADAFRUIT_KB2040_RP2040)

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A0;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 5;
    pins.oSolenoid = 7;
    pins.bTrigger = A2;
    pins.bGunA = A3;
    pins.bGunB = 4;
    pins.bGunC = 6;
    pins.bStart = 9;
    pins.bSelect = 8;
    pins.bGunUp = 18;
    pins.bGunDown = 20;
    pins.bGunLeft = 19;
    pins.bGunRight = 10;
    pins.bPedal = -1;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = A1;

// For the Arduino Nano RP2040 Connect - because it was requested
#elif defined(ARDUINO_NANO_RP2040_CONNECT)

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A2;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

      // Button Pins setup
    pins.oRumble = 17;
    pins.oSolenoid = 16;
    pins.bTrigger = 15;
    pins.bGunA = 0;
    pins.bGunB = 1;
    pins.bGunC = 18;
    pins.bStart = 19;
    pins.bSelect = 20;
    pins.bGunUp = -1;
    pins.bGunDown = -1;
    pins.bGunLeft = -1;
    pins.bGunRight = -1;
    pins.bPedal = -1;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = -1;

// For the Waveshare RP2040 Zero - smallest/cheapest board
#elif defined(ARDUINO_WAVESHARE_RP2040_ZERO)

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A3;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 17;
    pins.oSolenoid = 16;
    pins.bTrigger = 0;
    pins.bGunA = 1;
    pins.bGunB = 2;
    pins.bGunC = 3;
    pins.bStart = 4;
    pins.bSelect = 5;
    pins.bGunUp = -1;
    pins.bGunDown = -1;
    pins.bGunLeft = -1;
    pins.bGunRight = -1;
    pins.bPedal = -1;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = -1;

// For the Raspberry Pi Pico - first party baybeeee
#elif defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_W)

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A2;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = 10;
        pins.oLedG = 11;
        pins.oLedB = 12;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 17;
    pins.oSolenoid = 16;
    pins.bTrigger = 15;
    pins.bGunA = 0;
    pins.bGunB = 1;
    pins.bGunC = 2;
    pins.bStart = 3;
    pins.bSelect = 4;
    pins.bGunUp = 6;
    pins.bGunDown = 7;
    pins.bGunLeft = 8;
    pins.bGunRight = 9;
    pins.bPedal = 14;
    pins.bPedal2 = -1;
    pins.bPump = 13;
    pins.bHome = 5;

// For ESP32_S3 - DevKitC  // IMPOSTARE
#elif defined(ARDUINO_ESP32_S3_WROOM1_DevKitC_1_N16R8) || defined(ARDUINO_ESP32_S3_WROOM1_DevKitC_1_N16R8)

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A2; // IMPOSTARE
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;  // IMPOSTARE
        pins.oLedG = -1;  // IMPOSTARE
        pins.oLedB = -1;  // IMPOSTARE
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1; // IMPOSTARE
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = -1; // IMPOSTARE
    pins.oSolenoid = -1;  // IMPOSTARE
    pins.bTrigger = -1;  // IMPOSTARE
    pins.bGunA = -1;  // IMPOSTARE
    pins.bGunB = -1;  // IMPOSTARE
    pins.bGunC = -1;  // IMPOSTARE
    pins.bStart = -1;  // IMPOSTARE
    pins.bSelect = -1;  // IMPOSTARE
    pins.bGunUp = -1;  // IMPOSTARE
    pins.bGunDown = -1;  // IMPOSTARE
    pins.bGunLeft = -1;  // IMPOSTARE
    pins.bGunRight = -1;  // IMPOSTARE
    pins.bPedal = -1;  // IMPOSTARE
    pins.bPedal2 = -1;  // IMPOSTARE
    pins.bPump = -1;  // IMPOSTARE
    pins.bHome = -1;  // IMPOSTARE

    #endif // ARDUINO_BOARD

    PresetCam();
}

void SamcoPreferences::PresetCam()
{
#if defined(ARDUINO_ADAFRUIT_ITSYBITSY_RP2040) || defined(ARDUINO_ADAFRUIT_KB2040_RP2040)
    pins.pCamSCL = 3;
    pins.pCamSDA = 2;
#elif defined(ARDUINO_NANO_RP2040_CONNECT)
    pins.pCamSCL = 13;
    pins.pCamSDA = 12;
#elif defined(ARDUINO_WAVESHARE_RP2040_ZERO)
    pins.pCamSCL = 15;
    pins.pCamSDA = 14;
#elif defined(ARDUINO_ESP32_S3_WROOM1_DevKitC_1_N16R8) || defined(ARDUINO_ESP32_S3_WROOM1_DevKitC_1_N16R8)
    pins.pCamSCL = 9; // IMPOSTARE
    pins.pCamSDA = 8; // IMPOSTARE
#else // RASPBERRY_PI_PICO - RASPBERRY_PI_PICO_W -ecc 
    pins.pCamSCL = 21; //cam
    pins.pCamSDA = 20; //cam
    pins.pPeriphSCL = 19; //oled - altrimenti si configura solo da App
    pins.pPeriphSDA = 18; //oled - altrimenti si configura solo da App
#endif // ARDUINO_BOARD
}

#else

int SamcoPreferences::Load()
{
    return Error_NoStorage;
}

int SamcoPreferences::Save()
{
    return Error_NoStorage;
}

#endif // SAMCO_EEPROM_ENABLE
