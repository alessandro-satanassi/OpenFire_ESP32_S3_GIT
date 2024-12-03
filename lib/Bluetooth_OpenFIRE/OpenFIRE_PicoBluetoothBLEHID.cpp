/*
    PicoBluetoothBLEHID.h - Simple wrapper for BT-HID objects like
    keyboards, mice, gamepads.
    Copyright (c) 2023 Earle F. Philhower, III.  All rights reserved.

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

#include "OpenFIRE_PicoBluetoothBLEHID.h"

#define CCALLBACKNAME _BLEHIDCB
#include <ctocppcallback.h>

#define PACKETHANDLERCB(class, cbFcn) \
  (CCALLBACKNAME<void(uint8_t, uint16_t, uint8_t*, uint16_t), __COUNTER__>::func = std::bind(&class::cbFcn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), \
   static_cast<btstack_packet_handler_t>(CCALLBACKNAME<void(uint8_t, uint16_t, uint8_t*, uint16_t), __COUNTER__ - 1>::callback))

// ================================================================


bool OpenFIRE_PicoBluetoothBLEHID_::startHID(const char *localName, const char *hidName, uint16_t appearance, const uint8_t *hidDescriptor, uint16_t hidDescriptorSize, int battery) {
    if (_running) {
        return false;
    }
    _running = true;

    _buildAdvData(localName, appearance);
    _buildAttdb(hidName);

    _battery = battery;

    /* ===========RIMOSSO ========================
    // Setup L2CAP
    l2cap_init();

    // Setup SM
    sm_init();
    =========================================== */
    
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    sm_set_authentication_requirements(SM_AUTHREQ_SECURE_CONNECTION | SM_AUTHREQ_BONDING);

    // Setup ATT server
    att_server_init(_attdb, NULL, NULL);

    // Setup battery service
    battery_service_server_init(battery);

    // Setup device information service
    device_information_service_server_init();

    /*
    device_information_service_server_set_manufacturer_name(const char * manufacturer_name)
    device_information_service_server_set_model_number(const char * model_number)
    device_information_service_server_set_serial_number(const char * serial_number)
    device_information_service_server_set_hardware_revision(const char * hardware_revision)
    device_information_service_server_set_firmware_revision(const char * firmware_revision)
    device_information_service_server_set_software_revision(const char * software_revision)
    device_information_service_server_set_system_id(uint64_t manufacturer_identifier, uint32_t organizationally_unique_identifier)
    device_information_service_server_set_ieee_regulatory_certification(uint16_t value_a, uint16_t value_b)
    device_information_service_server_set_pnp_id(uint8_t vendor_source_id, uint16_t vendor_id, uint16_t product_id, uint16_t product_version)
    */
    ///////////////////////device_information_service_server_set_pnp_id(uint8_t vendor_source_id, uint16_t vendor_id, uint16_t product_id, uint16_t product_version);

    // Setup HID Device service, depending on activated reports
    uint8_t numreports = 5; //start with 1 (feature report) + 2 key (keyborad + consumer key) + 1 mouse + 1 joypad
    
    /* ==================== RIMOSSO ====================
    // Setup HID Device service, depending on activated reports
    uint8_t numreports = 1; //start with 1 (feature report)
    if (__BLEInstallKeyboard) {
        numreports += 2; //add keycodes + consumer keys
    }
    if (__BLEInstallMouse) {
        numreports += 1;
    }
    if (__BLEInstallJoystick) {
        numreports += 1;
    }
    ================================================= */
    
    //allocate memory for hid reports
    _reportStorage = (hids_device_report_t *) malloc(sizeof(hids_device_report_t) * numreports);
    hids_device_init_with_storage(0/*hid_country_code*/, hidDescriptor, hidDescriptorSize, numreports, _reportStorage);

    // Setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(_advDataLen, _advData);
    gap_advertisements_enable(1);

    
    /*
    // Register for HCI events
    _hci_event_callback_registration.callback = PACKETHANDLERCB(OpenFIRE_PicoBluetoothBLEHID_, packetHandler);
    hci_add_event_handler(&_hci_event_callback_registration);
    */


    // Register for SM events
    _sm_event_callback_registration.callback = PACKETHANDLERCB(OpenFIRE_PicoBluetoothBLEHID_, packetHandler);
    sm_add_event_handler(&_sm_event_callback_registration);

    // Register for HIDS events
    hids_device_register_packet_handler(PACKETHANDLERCB(OpenFIRE_PicoBluetoothBLEHID_, packetHandler));

    
    /* ===========RIMOSSO ============
    // GO!
    hci_power_control(HCI_POWER_ON);
    =============================== */
    
    return true;
}
