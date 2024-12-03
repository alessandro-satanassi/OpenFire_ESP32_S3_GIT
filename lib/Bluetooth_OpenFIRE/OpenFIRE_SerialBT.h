/*
    Serial-over-Bluetooth for the Raspberry Pi Pico RP2040

    Copyright (c) 2023 Earle F. Philhower, III <earlephilhower@yahoo.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <_needsbt.h>
#include <Arduino.h>
#include <api/HardwareSerial.h>
#include <stdarg.h>
#include <queue>
#include <pico/cyw43_arch.h>
#include <CoreMutex.h>
// ========== spostato .. guarda sotto ========== #include <btstack.h>

// The BTStack has redefinitions of this USB enum (same values, just redefined), so hide it to allow easy compilation
#define HID_REPORT_TYPE_INPUT HID_REPORT_TYPE_INPUT_BT
#define HID_REPORT_TYPE_OUTPUT HID_REPORT_TYPE_OUTPUT_BT
#define HID_REPORT_TYPE_FEATURE HID_REPORT_TYPE_FEATURE_BT
#define hid_report_type_t hid_report_type_t_bt

#include <btstack.h>

#undef hid_report_type_t
#undef HID_REPORT_TYPE_FEATURE
#undef HID_REPORT_TYPE_OUTPUT
#undef HID_REPORT_TYPE_INPUT






// ================================================================


class OpenFIRE_SerialBT_ : public HardwareSerial {
public:
    OpenFIRE_SerialBT_();

    bool setFIFOSize(size_t size);
    bool setName(const char *name) {
        if (_running) {
            return false;
        }
        free(_name);
        _name = strdup(name);
        return true;
    }

    void begin(unsigned long baud = 115200) override {
        begin(baud, SERIAL_8N1);
    };
    void begin(unsigned long baud, uint16_t config) override;
    void end() override;

    virtual int peek() override;
    virtual int read() override;
    virtual int available() override;
    virtual int availableForWrite() override;
    virtual void flush() override;
    virtual size_t write(uint8_t c) override;
    virtual size_t write(const uint8_t *p, size_t len) override;
    using Print::write;
    bool overflow();
    operator bool() override;

    // ESP8266 compat
    void setDebugOutput(bool unused) {
        (void) unused;
    }

private:
    bool _running = false;
    mutex_t _mutex;
    bool _overflow = false;
    volatile bool _connected = false;

    void packetHandler(uint8_t type, uint16_t channel, uint8_t *packet, uint16_t size);
    uint8_t send_data(uint16_t rfcomm_cid);

    // Lockless, IRQ-handled circular queue
    uint32_t _writer;
    uint32_t _reader;
    size_t   _fifoSize = 512; // l'originale era 32
    uint8_t *_queue;

    const int RFCOMM_SERVER_CHANNEL = 1;

    uint16_t _channelID;
    uint8_t  _spp_service_buffer[150]; // l'originale era 150
    btstack_packet_callback_registration_t _hci_event_callback_registration;

    volatile int _writeLen = 0;
    const void *_writeBuff;

    char *_name = nullptr;

    //uint8_t _device_id_sdp_service_buffer[1000];
    //const uint8_t * service_uuid128 = new uint8_t[18];
/*
static constexpr const uint8_t service_uuid128[16] = {
    0x56, 0x49, 0x44, 0x31, 0x38, 0x26, 0x50, 0x49, 0x44, 0x32, 0x34, 0x26, 
    0x52, 0x45, 0x56, 0x2e
};
*/


    /*
    
    de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_SERIAL_PORT );
    
    // 0x0201 "VendorID"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VENDOR_ID);
	de_add_number(service,  DE_UINT, DE_SIZE_16, vendor_id);

	// 0x0202 "ProductID"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PRODUCT_ID);
	de_add_number(service,  DE_UINT, DE_SIZE_16, product_id);
    */

    #define BUFFER_SIZE 1024 //buffer scrittura
    uint8_t buffer[BUFFER_SIZE];
    volatile size_t writeIndex = 0;
    volatile size_t readIndex = 0;

    /*
    #define VENDOR_ID 0x1234 // Esempio VID #define PRODUCT_ID 0x5678 // Esempio PID
    #define PRODUCT_ID 0x5678 // Esempio PID
    // Configura VID e PID 
    uint16_t vid = VENDOR_ID; 
    uint16_t pid = PRODUCT_ID; 
    btstack_set_vendor_id(vid); 
    btstack_set_product_id(pid);
    */

};
