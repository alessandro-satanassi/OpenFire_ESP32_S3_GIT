#include "Bluetooth_OpenFIRE.h"

// ===========================================================

bool BT_OpenFIRE_::startHID(const char *localName, const char *hidName, uint16_t appearance, uint16_t hidClass, uint8_t hidSubclass, const uint8_t *hidDescriptor, uint16_t hidDescriptorSize, int battery, unsigned long baud, uint16_t config) {
if (_running) {
        return false;
    }
    _running = true;
     

    const bool hid_boot_device = 0;
    const uint8_t hid_virtual_cable = 0;
    const uint8_t hid_remote_wake = 1;
    const uint8_t hid_reconnect_initiate = 1;
    const bool hid_normally_connectable = 1;
    // When not set to 0xffff, sniff and sniff subrating are enabled
    const uint16_t host_max_latency = 1600; // più basso è, più volece è .. più alto è diminuisce il consumo
    const uint16_t host_min_timeout = 3200;
    const uint16_t hid_supervision_timeout = 3200;

    hid_sdp_record_t hid_params = {
        hidClass,                           // uint16_t        hid_device_subclass;
        hidSubclass,                        // uint8_t         hid_country_code;
        hid_virtual_cable,                  // uint8_t         hid_virtual_cable;
        hid_remote_wake,                    // uint8_t         hid_remote_wake;
        hid_reconnect_initiate,             // uint8_t         hid_reconnect_initiate;
        hid_normally_connectable,           // bool            hid_normally_connectable;
        hid_boot_device,                    // bool            hid_boot_device;
        host_max_latency,                   // uint16_t        hid_ssr_host_max_latency; 
        host_min_timeout,                   // uint16_t        hid_ssr_host_min_timeout;
        hid_supervision_timeout,            // uint16_t        hid_supervision_timeout;
        hidDescriptor,                      // const uint8_t * hid_descriptor;
        hidDescriptorSize,                  // uint16_t        hid_descriptor_size;
        hidName                             // const char *    device_name;

    };

    /*
    uint16_t        hid_device_subclass;
    uint8_t         hid_country_code;
    uint8_t         hid_virtual_cable;
    uint8_t         hid_remote_wake;
    uint8_t         hid_reconnect_initiate;
    bool            hid_normally_connectable;
    bool            hid_boot_device;
    uint16_t        hid_ssr_host_max_latency; 
    uint16_t        hid_ssr_host_min_timeout;
    uint16_t        hid_supervision_timeout;
    const uint8_t * hid_descriptor;
    uint16_t        hid_descriptor_size;
    const char *    device_name;
    */

    // Setup L2CAP
    l2cap_init();

#ifdef ENABLE_BLE
    // Initialize LE Security Manager. Needed for cross-transport key derivation
    sm_init();
#endif

#ifdef RFCOMM_ENABLE
    sdp_init();
#endif

    //OpenFIRE_SerialBT.begin(baud,config);

    //SERVIZIO HID

#if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
    OpenFIRE_PicoBluetoothBLEHID.startHID(localName,hidName,appearance,hidDescriptor,hidDescriptorSize,battery);
#else
    OpenFIRE_PicoBluetoothHID.startHID(hid_boot_device, localName,hidName,hidClass,hidSubclass,hidDescriptor,hidDescriptorSize);
#endif

    // SERVIZIO SPP - RFCOM

    #ifdef RFCOMM_ENABLE
    OpenFIRE_SerialBT.begin(baud,config);
    #endif

    //// impostazione del SDP /////////////////////

    //memset(_sdp_service_buffer, 0, sizeof(_sdp_service_buffer));
    
    #ifdef RFCOMM_ENABLE
    memset(_sdp_service_buffer_spp, 0, sizeof(_sdp_service_buffer_spp));
    memset(_sdp_service_buffer_hid, 0, sizeof(_sdp_service_buffer_hid));
    memset(_sdp_service_buffer_id, 0, sizeof(_sdp_service_buffer_id));
    #endif
    ///////crea_sdp(_sdp_service_buffer, sdp_create_service_record_handle(), RFCOMM_SERVER_CHANNEL, localName, (uint16_t) DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, (uint16_t) 1, &hid_params); 
    ///////sdp_register_service(_sdp_service_buffer);
    
    #ifdef RFCOMM_ENABLE
    //crea_sdp_id(_sdp_service_buffer_id, sdp_create_service_record_handle(), RFCOMM_SERVER_CHANNEL, localName, (uint16_t) DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, (uint16_t) 1, &hid_params); 
    crea_sdp_id(_sdp_service_buffer_id, sdp_create_service_record_handle(), RFCOMM_SERVER_CHANNEL, localName, (uint16_t) DEVICE_ID_VENDOR_ID_SOURCE_USB, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, (uint16_t) 1, &hid_params); 
    sdp_register_service(_sdp_service_buffer_id);
    #endif

    #if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
        // NON CREA IL RECORD SDP IN QUANTO STA SUL GATT
    #else
        crea_sdp_hid(_sdp_service_buffer_hid, sdp_create_service_record_handle(), RFCOMM_SERVER_CHANNEL, "HID" /*localName*/, (uint16_t) DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, (uint16_t) 1, &hid_params); 
        sdp_register_service(_sdp_service_buffer_hid);
    #endif

    #ifdef RFCOMM_ENABLE
    //spp_create_sdp_record(_sdp_service_buffer_spp, sdp_create_service_record_handle(), RFCOMM_SERVER_CHANNEL, "PicoW Serial");
    crea_sdp_spp(_sdp_service_buffer_spp, sdp_create_service_record_handle(), RFCOMM_SERVER_CHANNEL, "SPP" /*localName*/, (uint16_t) DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, (uint16_t) 1, &hid_params); 
    sdp_register_service(_sdp_service_buffer_spp);
    #endif
        
    // ========= impostazione del GAP ==================
    #ifdef RFCOMM_ENABLE
    gap_discoverable_control(1);  
    gap_set_class_of_device(hidClass);
    #if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
        gap_set_local_name("PicoW 69 Serial 00:00:00:00:00:00"); //localName); // "PicoW Serial 00:00:00:00:00:00"
    #else
        gap_set_local_name(localName); //localName); // "PicoW Serial 00:00:00:00:00:00"
    #endif
    gap_set_default_link_policy_settings(LM_LINK_POLICY_ENABLE_ROLE_SWITCH | LM_LINK_POLICY_ENABLE_SNIFF_MODE);
    gap_set_allow_role_switch(true);
    
    //gap_ssp_set_io_capability(SSP_IO_CAPABILITY_DISPLAY_YES_NO); // POI rimuovere
    #endif
    
    
    
    hci_power_control(HCI_POWER_ON);
    return true;
}

