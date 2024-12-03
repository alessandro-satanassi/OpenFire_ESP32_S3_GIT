#include <Arduino.h>

// operating modes
enum GunMode_e {
    GunMode_Init = -1,
    GunMode_Run = 0,
    GunMode_Calibration,
    GunMode_Verification,
    GunMode_Pause,
    GunMode_Docked
};


enum RunMode_e {
    RunMode_Normal = 0,         ///< Normal gun mode, no averaging
    RunMode_Average = 1,        ///< 2 frame moving average
    RunMode_Average2 = 2,       ///< weighted average with 3 frames
    RunMode_ProfileMax = 2,     ///< maximum mode allowed for profiles
    RunMode_Processing = 3,     ///< Processing test mode
    RunMode_Count
};

//----------------------------------------------------------------------------------------------------
// DICHIARAZIONE DELLE FUNZIONI ==============================================



void LoadPreferences();
void VerifyPreferences();
void GetPosition();
void CameraSet();
void SetMode(GunMode_e newMode);
void UpdateBindings(bool offscreenEnable);
void IncreaseIrSensitivity();
void startIrCamTimer(int frequencyHz);
void SetProfileSelection(bool isIncrement);
void SerialProcessingDocked();
void SetIrSensitivity(uint8_t sensitivity);
void FeedbackSet();
void LedInit();
void ExecGunModeDocked();
void LedOff();
bool SelectCalProfile(unsigned int profile);
void PrintRunMode();
void SetProfileSelection(bool isIncrement);
void DecreaseIrSensitivity();
void LedUpdate(byte r, byte g, byte b);
void SetIrSensitivity(uint8_t sensitivity);
void PrintIrSensitivity();
void SetLedColorFromMode();
void PrintNVStorage();
void PrintPreferences();
void ExecRunMode();
void SolenoidToggle();
void CaliMousePosMove(uint8_t caseNumber);
void RumbleToggle();
void SerialProcessing();
void SendEscapeKey();
void AutofireSpeedToggle(byte setting);
void SetPauseModeSelection(bool isIncrement);
void UpdateLastSeen();
void SetLedPackedColor(uint32_t color);
void AnalogStickPoll();
void TriggerFire();
void SavePreferences();
void SetRunMode(RunMode_e newMode);
void PrintSelectedProfile();
void PrintExtras();
void PrintNVPrefsError();
void SerialHandling();
void TriggerNotFire();
void TriggerFireSimple();
void TriggerNotFireSimple();
void OffscreenToggle();
void SelectCalProfileFromBtnMask(uint32_t mask);
void ExecCalMode();
void TinyUSBInit();
void PrintResults();
void rp2040EnablePWMTimer(unsigned int slice_num, unsigned int frequency);
void PrintDebugSerial();

