/*
 * This module simulates the standard Arduino "Mouse.h" and
 * "Keyboard.h" API for use with the TinyUSB HID API. Instead of doing
 *  #include <HID.h>
 *  #include <Mouse.h>
 *  #include <Keyboard.h>
 *  
 *  Simply do
 *  
 *  #include <TinyUSB_Mouse_Keyboard.h>
 *  
 *  and this module will automatically select whether or not to use the
 *  standard Arduino mouse and keyboard API or the TinyUSB API. We had to
 *  combine them into a single library because of the way TinyUSB handles
 *  descriptors.
 *  
 *  For details on Arduino Mouse.h see
 *   https://www.arduino.cc/reference/en/language/functions/usb/mouse/
 *  For details on Arduino Keyboard.h see
 *   https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
 *
 *  NOTE: This code is derived from the standard Arduino Mouse.h, Mouse.cpp,
 *    Keyboard.h, and Keyboard.cpp code. The copyright on that original code
 *    is as follows.
 *   
 *  Copyright (c) 2015, Arduino LLC
 *  Original code (pre-library): Copyright (c) 2011, Peter Barrett
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Arduino.h>

/*****************************
 *   GLOBAL SECTION
 *****************************/

class TinyUSBDevices_ {
public:
  TinyUSBDevices_(void);
  void begin(byte polRate);
  void beginBT(const char *localName, const char *hidName);
  bool onBattery = false;
};
extern TinyUSBDevices_ TinyUSBDevices;

/*****************************
 *   MOUSE SECTION
 *****************************/ 
#ifndef _ABSMOUSE5_H_
#define _ABSMOUSE5_H_

#include <stdint.h>

#define MOUSE_LEFT 0x01
#define MOUSE_RIGHT 0x02
#define MOUSE_MIDDLE 0x04
#define MOUSE_BUTTON4 0x08
#define MOUSE_BUTTON5 0x10

#if defined(USE_TINYUSB)
#define TUD_HID_REPORT_DESC_ABSMOUSE5(...) \
	0x05, 0x01, \
	0x09, 0x02, \
	0xA1, 0x01, \
	__VA_ARGS__ \
	0x09, 0x01, \
	0xA1, 0x00, \
	0x05, 0x09, \
	0x19, 0x01, \
	0x29, 0x05, \
	0x15, 0x00, \
	0x25, 0x01, \
	0x95, 0x05, \
	0x75, 0x01, \
	0x81, 0x02, \
	0x95, 0x01, \
	0x75, 0x03, \
	0x81, 0x03, \
	0x05, 0x01, \
	0x09, 0x30, \
	0x09, 0x31, \
	0x16, 0x00, 0x00, \
	0x26, 0xFF, 0x7F, \
	0x36, 0x00, 0x00, \
	0x46, 0xFF, 0x7F, \
	0x75, 0x10, \
	0x95, 0x02, \
	0x81, 0x02, \
	0xC0, \
	0xC0
#endif // USE_TINYUSB

// 5 button absolute mouse
class AbsMouse5_
{
private:
	const uint8_t _reportId;
	uint8_t _buttons;
	uint16_t _x;
	uint16_t _y;
	bool _autoReport;

public:
	AbsMouse5_(uint8_t reportId = 1);
	void init(bool autoReport = true);
	void report(void);
	void move(uint16_t x, uint16_t y);
	void press(uint8_t b = MOUSE_LEFT);
	void release(uint8_t b = MOUSE_LEFT);
	void releaseAll() { release(0x1f); }
};

// global singleton
extern AbsMouse5_ AbsMouse5;

#endif // _ABSMOUSE5_H_

/******************************
 *    KEYBOARD SECTION
 ******************************/
  //  Keyboard codes
  //  Note these are different in some respects to the TinyUSB codes but 
  //  are compatible with Arduino Keyboard.h API
  
  #define KEY_LEFT_CTRL   0x80
  #define KEY_LEFT_SHIFT    0x81
  #define KEY_LEFT_ALT    0x82
  #define KEY_LEFT_GUI    0x83
  #define KEY_RIGHT_CTRL    0x84
  #define KEY_RIGHT_SHIFT   0x85
  #define KEY_RIGHT_ALT   0x86
  #define KEY_RIGHT_GUI   0x87
  
  #define KEY_UP_ARROW    0xDA
  #define KEY_DOWN_ARROW    0xD9
  #define KEY_LEFT_ARROW    0xD8
  #define KEY_RIGHT_ARROW   0xD7
  #define KEY_BACKSPACE   0xB2
  #define KEY_TAB       0xB3
  #define KEY_RETURN      0xB0
  #define KEY_ESC       0xB1
  #define KEY_INSERT      0xD1
  #define KEY_DELETE      0xD4
  #define KEY_PAGE_UP     0xD3
  #define KEY_PAGE_DOWN   0xD6
  #define KEY_HOME      0xD2
  #define KEY_END       0xD5
  #define KEY_CAPS_LOCK   0xC1
  #define KEY_F1        0xC2
  #define KEY_F2        0xC3
  #define KEY_F3        0xC4
  #define KEY_F4        0xC5
  #define KEY_F5        0xC6
  #define KEY_F6        0xC7
  #define KEY_F7        0xC8
  #define KEY_F8        0xC9
  #define KEY_F9        0xCA
  #define KEY_F10       0xCB
  #define KEY_F11       0xCC
  #define KEY_F12       0xCD
  #define KEY_F13       0xF0
  #define KEY_F14       0xF1
  #define KEY_F15       0xF2
  #define KEY_F16       0xF3
  #define KEY_F17       0xF4
  #define KEY_F18       0xF5
  #define KEY_F19       0xF6
  #define KEY_F20       0xF7
  #define KEY_F21       0xF8
  #define KEY_F22       0xF9
  #define KEY_F23       0xFA
  #define KEY_F24       0xFB
  
  //  Low level key report: up to 6 keys and shift, ctrl etc at once
  typedef struct
  {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
  } KeyReport;
  
  /*
   * This class contains the exact same methods as the Arduino Keyboard.h class.
   */
  
  class Keyboard_ : public Print
  {
  private:
    KeyReport _keyReport;
    void sendReport(KeyReport* keys);
  public:
    Keyboard_(void);
    size_t write(uint8_t k);
    size_t write(const uint8_t *buffer, size_t size);
    size_t press(uint8_t k);
    size_t release(uint8_t k);
    void releaseAll(void);
  };
extern Keyboard_ Keyboard;

/*****************************
 *   GAMEPAD SECTION
 *****************************/

#ifdef USE_TINYUSB
#define TUD_HID_REPORT_DESC_GAMEPAD16(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_GAMEPAD  )                 ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    /* 16 bit X, Y, Rx, Ry (min -32767, max 32767 ) */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_X                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RX                   ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RY                   ) ,\
    HID_LOGICAL_MIN_N  ( -32767, 2                              ) ,\
    HID_LOGICAL_MAX_N  ( 32767, 2                               ) ,\
    HID_REPORT_COUNT   ( 4                                      ) ,\
    HID_REPORT_SIZE    ( 16                                     ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* 8 bit DPad/Hat Button Map  */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_HAT_SWITCH           ) ,\
    HID_LOGICAL_MIN    ( 1                                      ) ,\
    HID_LOGICAL_MAX    ( 8                                      ) ,\
    HID_PHYSICAL_MIN   ( 0                                      ) ,\
    HID_PHYSICAL_MAX_N ( 315, 2                                 ) ,\
    HID_REPORT_COUNT   ( 1                                      ) ,\
    HID_REPORT_SIZE    ( 8                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* 16 bit Button Map */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
    HID_USAGE_MIN      ( 1                                      ) ,\
    HID_USAGE_MAX      ( 15                                     ) ,\
    HID_LOGICAL_MIN    ( 0                                      ) ,\
    HID_LOGICAL_MAX    ( 1                                      ) ,\
    HID_REPORT_COUNT   ( 16                                     ) ,\
    HID_REPORT_SIZE    ( 1                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END
#endif // USE_TINYUSB
// Using the Adafruit Gamepad desc as the basis, but with 4 16-bit axis and a 16-bit 15 button report

#define PAD_A      0
#define PAD_B      1
#define PAD_C      2
#define PAD_X      3
#define PAD_Y      4
#define PAD_Z      5
#define PAD_LB     6
#define PAD_RB     7
#define PAD_LT     8
#define PAD_RT     9
#define PAD_SELECT 10
#define PAD_START  11
#define PAD_HOME   12
#define PAD_LS     13
#define PAD_RS     14
#define PAD_UP     15
#define PAD_DOWN   16
#define PAD_LEFT   17
#define PAD_RIGHT  18

#define GAMEPAD_HAT_CENTERED 0
#define GAMEPAD_HAT_UP 1
#define GAMEPAD_HAT_UP_RIGHT 2
#define GAMEPAD_HAT_RIGHT 3
#define GAMEPAD_HAT_DOWN_RIGHT 4
#define GAMEPAD_HAT_DOWN 5
#define GAMEPAD_HAT_DOWN_LEFT 6
#define GAMEPAD_HAT_LEFT 7
#define GAMEPAD_HAT_UP_LEFT 8

typedef struct {
        int16_t X = 0;
        int16_t Y = 0;
        int16_t Rx = 0;
        int16_t Ry = 0;
        uint8_t hat;
        uint16_t buttons;     // button bitmask
} __attribute__ ((packed)) gamepad16Report_s;

class Gamepad16_ {
private:
  gamepad16Report_s gamepad16Report;
  uint16_t _x = 2048;
  uint16_t _y = 2048;
  bool _autoReport = true;
public:
  Gamepad16_(void);
  void moveCam(uint16_t origX, uint16_t origY);
  void moveStick(uint16_t origX, uint16_t origY);
  void press(uint8_t buttonNum);
  void release(uint8_t buttonNum);
  void padUpdate(uint8_t padMask);
  void report(void);
  void releaseAll(void);
  void setAutoreport(bool state) { _autoReport = state; }
  bool stickRight;
};
extern Gamepad16_ Gamepad16;
