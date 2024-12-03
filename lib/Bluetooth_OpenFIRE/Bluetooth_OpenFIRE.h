#ifndef _Bluetooth_OpenFIRE_h_
#define _Bluetooth_OpenFIRE_h_

#pragma once

#include "OpenFIRE_SerialBT.h"

#ifdef ENABLE_CLASSIC
    //#include "OpenFIRE_PicoBluetoothHID.h"
#endif


#if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
    #include "OpenFIRE_PicoBluetoothBLEHID.h"
#else
    #include "OpenFIRE_PicoBluetoothHID.h"
#endif

#define SERVICE_RECORD_STATE_BT 71

// ===================================================================

class BT_OpenFIRE_ {
public:    
    BT_OpenFIRE_() {}
    ~BT_OpenFIRE_() {}

    
    #if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
        OpenFIRE_PicoBluetoothBLEHID_ OpenFIRE_PicoBluetoothBLEHID;
    #else
        OpenFIRE_PicoBluetoothHID_ OpenFIRE_PicoBluetoothHID;
    #endif
    
    OpenFIRE_SerialBT_ OpenFIRE_SerialBT;
    bool startHID(const char *localName, const char *hidName, uint16_t appearance, uint16_t hidClass, uint8_t hidSubclass, const uint8_t *hidDescriptor, uint16_t hidDescriptorSize, int battery, unsigned long baud, uint16_t config);

        
    /*
    #if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
    OpenFIRE_PicoBluetoothBLEHID_* getBLEHID() {
        return &OpenFIRE_PicoBluetoothBLEHID;
    }
    #else
    OpenFIRE_PicoBluetoothHID_* getHID() {
        return &OpenFIRE_PicoBluetoothHID;
    }
    #endif
    */

    bool send(int id, void *rpt, int len) {
        #if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
            //void *rptBLE = malloc(sizeof(int) + len);
            //memcpy(rptBLE, &id, sizeof(int));
            //memcpy((uint8_t *)rptBLE + sizeof(int), rpt, len);  // al posto di (char *) si potrebbe mettere (uint8_t *) che è più chiaro ???
    

            /*
            void *rptBLE = malloc(sizeof(uint8_t) + len);
            *((uint8_t *)rptBLE) = id;
            memcpy((uint8_t *)rptBLE + sizeof(uint8_t), rpt, len);
            */

            //static uint8_t rptBLE[len+1];
            /////////////////////////////////uint8_t *rptBLE = (uint8_t*)malloc(len+1);  // gestire gli eventi diversamente

            rptBLE[0]=(uint8_t)id;
            memcpy(&rptBLE[1], (uint8_t*)rpt,len);



            //reportID = ((const uint8_t *)_sendReport)[0];
            //&(((const uint8_t *)_sendReport)[1])



            return OpenFIRE_PicoBluetoothBLEHID.send(rptBLE,len+1);
        #else
            return OpenFIRE_PicoBluetoothHID.send(id,rpt,len);
        #endif    
    }

void end() {
    if (!_running) {
        return;
    }
    _running = false;

    #if defined(ENABLE_BLE) && defined(OPENFIRE_BLE_ENABLE)
        OpenFIRE_PicoBluetoothBLEHID.end();
    #else
        OpenFIRE_PicoBluetoothHID.end();
    #endif
        
    OpenFIRE_SerialBT.end();
    
    
    hci_power_control(HCI_POWER_OFF);
    __lockBluetooth();
    __unlockBluetooth();
}



operator bool() {
    return _running;
}

void crea_sdp(uint8_t *service, uint32_t service_record_handle, int rfcomm_channel, const char *name, uint16_t vendor_id_source, uint16_t vendor_id, uint16_t product_id, uint16_t version, const hid_sdp_record_t * params) {
    //////////////// PARTE PER SPP ///////////////////////////////
//static void spp_create_sdp_record_internal(uint8_t *service, uint32_t service_record_handle, const uint8_t * service_uuid128, int rfcomm_channel, const char *name){
	
	//uint32_t service_record_handle = sdp_create_service_record_handle();
    
    uint8_t* attribute;
	de_create_sequence(service);
    
    // 0x0000 "Service Record Handle"
	de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_HANDLE);
	de_add_number(service, DE_UINT, DE_SIZE_32, service_record_handle);
    
	// 0x0001 "Service Class ID List"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_CLASS_ID_LIST);
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_PNP_INFORMATION);
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE);
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_SERIAL_PORT);
	}
	de_pop_sequence(service, attribute);
	
	// 0x0004 "Protocol Descriptor List"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST);
	attribute = de_push_sequence(service);
	{
		uint8_t* l2cpProtocol = de_push_sequence(attribute);
		{
			de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
            de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, BLUETOOTH_PSM_HID_CONTROL);
		}
		de_pop_sequence(attribute, l2cpProtocol);
		
        uint8_t * hidProtocol = de_push_sequence(attribute);
        {
            de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_HIDP); 
        }
        de_pop_sequence(attribute, hidProtocol);

		uint8_t* rfcomm = de_push_sequence(attribute);
		{
			de_add_number(rfcomm,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_RFCOMM);  // rfcomm_service
			de_add_number(rfcomm,  DE_UINT, DE_SIZE_8,  rfcomm_channel);  // rfcomm channel
		}
		de_pop_sequence(attribute, rfcomm);
	}
	de_pop_sequence(service, attribute);
	
	// 0x0005 "Public Browse Group"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BROWSE_GROUP_LIST); // public browse group
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PUBLIC_BROWSE_ROOT );
	}
	de_pop_sequence(service, attribute);
	
	// 0x0006 BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x656e);
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x006a);
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x0100);
	}
	de_pop_sequence(service, attribute);
	
	// 0x0009 "Bluetooth Attribute Profile Descriptor List"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST);
	attribute = de_push_sequence(service);
	{
		uint8_t *sppProfile = de_push_sequence(attribute);
		{
			// 0x1101 BLUETOOTH_SERVICE_CLASS_SERIAL_PORT
            de_add_number(sppProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_SERIAL_PORT);
			de_add_number(sppProfile,  DE_UINT, DE_SIZE_16, 0x0102);

            
            if (name == NULL){
                name = "SPP";
            }
            if (strlen(name) > 0){
                de_add_number(sppProfile,  DE_UINT, DE_SIZE_16, 0x0100);
                de_add_data(sppProfile,  DE_STRING, (uint16_t) strlen(name), (uint8_t *) name);
            }
            
        }
		de_pop_sequence(attribute, sppProfile);

        uint8_t * hidProfile = de_push_sequence(attribute);
        {
            // 0x1124  BLUETOOTH_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE
            de_add_number(hidProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, 0x0101);    // Version 1.1
            
            // 0x0201
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_PARSER_VERSION);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, 0x0111);  // v1.1.1

            // 0x0202
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_DEVICE_SUBCLASS);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_8,  params->hid_device_subclass);
            
            // 0x203
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_COUNTRY_CODE);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_8,  params->hid_country_code);

            // 0x204
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_VIRTUAL_CABLE);
            de_add_number(hidProfile,  DE_BOOL, DE_SIZE_8,  params->hid_virtual_cable);

            // 0x205
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_RECONNECT_INITIATE); 
            de_add_number(hidProfile,  DE_BOOL, DE_SIZE_8,  params->hid_reconnect_initiate); 
            
            // 0x0206 BLUETOOTH_ATTRIBUTE_HID_DESCRIPTOR_LIST
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_DESCRIPTOR_LIST);
            uint8_t* hidDescriptor = de_push_sequence(hidProfile);
            {
                de_add_number(hidDescriptor,  DE_UINT, DE_SIZE_8, 0x22);    // Report Descriptor
                de_add_data(hidDescriptor,  DE_STRING, params->hid_descriptor_size, (uint8_t *) params->hid_descriptor);
            }
            de_pop_sequence(hidProfile, hidDescriptor);  
            
            // 0x0207 BLUETOOTH_ATTRIBUTE_HIDLANGID_BASE_LIST
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HIDLANGID_BASE_LIST);
            uint8_t* hig_lang_base = de_push_sequence(hidProfile);
            {
                // see: http://www.usb.org/developers/docs/USB_LANGIDs.pdf
                de_add_number(hig_lang_base,  DE_UINT, DE_SIZE_16, 0x0409);    // HIDLANGID = English (US)
                de_add_number(hig_lang_base,  DE_UINT, DE_SIZE_16, 0x0100);    // HIDLanguageBase = 0x0100 default
            }
            de_pop_sequence(hidProfile, hig_lang_base);

            // battery power 
            // 0x020A
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_REMOTE_WAKE); 
            de_add_number(hidProfile,  DE_BOOL, DE_SIZE_8,  params->hid_remote_wake ? 1 : 0);

            // supervision timeout
            // 0x020C
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_SUPERVISION_TIMEOUT); 
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, params->hid_supervision_timeout);

            // 0x020D
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_NORMALLY_CONNECTABLE); 
            de_add_number(hidProfile,  DE_BOOL, DE_SIZE_8,  params->hid_normally_connectable); 

            // 0x020E
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_BOOT_DEVICE); 
            de_add_number(hidProfile,  DE_BOOL, DE_SIZE_8,  params->hid_boot_device ? 1 : 0);

            // 0x020F
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HIDSSR_HOST_MAX_LATENCY); 
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, params->hid_ssr_host_max_latency);

            // 0x0210
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HIDSSR_HOST_MIN_TIMEOUT); 
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, params->hid_ssr_host_min_timeout);

            // 0x0100 "ServiceName"
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, 0x0100);
            de_add_data(hidProfile,  DE_STRING, (uint16_t) strlen(params->device_name), (uint8_t *) params->device_name);
            
        }
        de_pop_sequence(attribute, hidProfile);
	    
		/*
        uint8_t *didProfile = de_push_sequence(attribute);
		{
			de_add_number(didProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_PNP_INFORMATION);
			de_add_number(didProfile,  DE_UINT, DE_SIZE_16, 0x0102); // ?????

            // 0x0200 "SpecificationID"
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SPECIFICATION_ID);
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, 0x0103);	// v1.3

            // 0x0201 "VendorID"
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VENDOR_ID);
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, vendor_id);

            // 0x0202 "ProductID"
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PRODUCT_ID);
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, product_id);

            // 0x0203 "Version"
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VERSION);
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, version);

            // 0x0204 "PrimaryRecord"
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PRIMARY_RECORD);
            de_add_number(didProfile,  DE_BOOL, DE_SIZE_8,  1);	// yes, this is the primary record - there are no others

            // 0x0205 "VendorIDSource"
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VENDOR_ID_SOURCE);
            de_add_number(didProfile,  DE_UINT, DE_SIZE_16, vendor_id_source);
   
        }
		de_pop_sequence(attribute, didProfile);
        */
    }
	de_pop_sequence(service, attribute);
	
    
    // 0x000D BLUETOOTH_ATTRIBUTE_ADDITIONAL_PROTOCOL_DESCRIPTOR_LISTS
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_ADDITIONAL_PROTOCOL_DESCRIPTOR_LISTS);
    attribute = de_push_sequence(service);
    {
        uint8_t * additionalDescriptorAttribute = de_push_sequence(attribute); 
        {
            uint8_t * l2cpProtocol = de_push_sequence(additionalDescriptorAttribute);
            {
                de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
                de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, BLUETOOTH_PSM_HID_INTERRUPT);
            }
            de_pop_sequence(additionalDescriptorAttribute, l2cpProtocol);          
            
            uint8_t * hidProtocol = de_push_sequence(additionalDescriptorAttribute);
            {
                de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_HIDP); 
            }
            de_pop_sequence(additionalDescriptorAttribute, hidProtocol);

           uint8_t* rfcomm = de_push_sequence(attribute);
		    {
			    de_add_number(rfcomm,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_RFCOMM);  // rfcomm_service
			    de_add_number(rfcomm,  DE_UINT, DE_SIZE_8,  rfcomm_channel);  // rfcomm channel
		    }
		    de_pop_sequence(attribute, rfcomm);
        }
        de_pop_sequence(attribute, additionalDescriptorAttribute);
    }
    de_pop_sequence(service, attribute); 
    

    /*
	// 0x0100 "ServiceName"  /// SISTEMARE
    if (name == NULL){
        name = "SPP";
    }
    if (strlen(name) > 0){
        de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0100);
        de_add_data(service,  DE_STRING, (uint16_t) strlen(name), (uint8_t *) name);
        //de_add_data(service,  DE_STRING, (uint16_t) strlen(params->device_name), (uint8_t *) params->device_name);
    }
    */
}

void crea_sdp_id(uint8_t *service, uint32_t service_record_handle, int rfcomm_channel, const char *name, uint16_t vendor_id_source, uint16_t vendor_id, uint16_t product_id, uint16_t version, const hid_sdp_record_t * params) {
uint8_t* attribute;
	de_create_sequence(service);
    
    // 0x0000 "Service Record Handle"
	de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_HANDLE);
	de_add_number(service, DE_UINT, DE_SIZE_32, service_record_handle);
    
	// 0x0001 "Service Class ID List"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_CLASS_ID_LIST);
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_PNP_INFORMATION );
	}
	de_pop_sequence(service, attribute);
	
	// 0x0002 "Service Record State"
	de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_STATE);
	de_add_number(service, DE_UINT, DE_SIZE_32, (uint32_t) SERVICE_RECORD_STATE_BT);
	
    
    // 0x0005 "Public Browse Group"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BROWSE_GROUP_LIST); // public browse group
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PUBLIC_BROWSE_ROOT );
	}
	de_pop_sequence(service, attribute);
	
    // 0x0006  BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x656e);
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x006a);
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x0100);
	}
	de_pop_sequence(service, attribute);
      
    // 0x0100 "ServiceName"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0100);
    de_add_data(service,  DE_STRING, (uint16_t) strlen(params->device_name), (uint8_t *) params->device_name);

    // 0x0101 "ServiceDescription"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0101);
    de_add_data(service,  DE_STRING, (uint16_t) strlen("TOPO GIGIO-id"), (uint8_t *) "TOPO GIGIO-id" ); //params->device_name);

    // 0x0102 "ProviderName"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0102);
    de_add_data(service,  DE_STRING, (uint16_t) strlen("SATAN7-id"), (uint8_t *) "SATAN7-id"); // params->device_name);

	// 0x0200 "SpecificationID"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SPECIFICATION_ID);
	de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0103);	// v1.3 0x0103   // v5.0 0x0500
    
	// 0x0201 "VendorID"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VENDOR_ID);
	de_add_number(service,  DE_UINT, DE_SIZE_16, vendor_id);

	// 0x0202 "ProductID"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PRODUCT_ID);
	de_add_number(service,  DE_UINT, DE_SIZE_16, product_id);

	// 0x0203 "Version"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VERSION);
	de_add_number(service,  DE_UINT, DE_SIZE_16, version);

	// 0x0204 "PrimaryRecord"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PRIMARY_RECORD);
	de_add_number(service,  DE_BOOL, DE_SIZE_8,  1);	// yes, this is the primary record - there are no others

	// 0x0205 "VendorIDSource"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VENDOR_ID_SOURCE);
	de_add_number(service,  DE_UINT, DE_SIZE_16, vendor_id_source);
}

void crea_sdp_hid(uint8_t *service, uint32_t service_record_handle, int rfcomm_channel, const char *name, uint16_t vendor_id_source, uint16_t vendor_id, uint16_t product_id, uint16_t version, const hid_sdp_record_t * params) {
   uint8_t * attribute;
    de_create_sequence(service);
    
    // 0x0000
    de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_HANDLE);
    de_add_number(service, DE_UINT, DE_SIZE_32, service_record_handle);        
    
    // 0x0001
    de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_CLASS_ID_LIST);
    attribute = de_push_sequence(service);
    {
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE);
    }
    de_pop_sequence(service, attribute);
    
    // 0x0002 "Service Record State"
	de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_STATE);
	de_add_number(service, DE_UINT, DE_SIZE_32, (uint32_t) SERVICE_RECORD_STATE_BT);
    
    // 0x0004
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t * l2cpProtocol = de_push_sequence(attribute);
        {
            de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
            de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, BLUETOOTH_PSM_HID_CONTROL);
        }
        de_pop_sequence(attribute, l2cpProtocol);

        uint8_t * hidProtocol = de_push_sequence(attribute);
        {
            de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_HIDP); 
        }
        de_pop_sequence(attribute, hidProtocol);
    }
    de_pop_sequence(service, attribute);
    
    // 0x0006
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
    attribute = de_push_sequence(service);
    {
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x656e);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x006a);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x0100); // e' l'indirizzo del "ServiceName" si può impostare tra 0x0100 e 0x01FF
    }
    de_pop_sequence(service, attribute);
    
    // 0x0009    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t * hidProfile = de_push_sequence(attribute);
        {
            de_add_number(hidProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, 0x0101);    // Version 1.1
        }
        de_pop_sequence(attribute, hidProfile);
    }
    de_pop_sequence(service, attribute);

    // 0x000D
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_ADDITIONAL_PROTOCOL_DESCRIPTOR_LISTS);
    attribute = de_push_sequence(service);
    {
        uint8_t * additionalDescriptorAttribute = de_push_sequence(attribute); 
        {
            uint8_t * l2cpProtocol = de_push_sequence(additionalDescriptorAttribute);
            {
                de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
                de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, BLUETOOTH_PSM_HID_INTERRUPT);
            }
            de_pop_sequence(additionalDescriptorAttribute, l2cpProtocol);

            uint8_t * hidProtocol = de_push_sequence(additionalDescriptorAttribute);
            {
                de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_HIDP); 
            }
            de_pop_sequence(additionalDescriptorAttribute, hidProtocol);
        }
        de_pop_sequence(attribute, additionalDescriptorAttribute);
    }
    de_pop_sequence(service, attribute);   

    // 0x0100 "ServiceName"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0100);
    de_add_data(service,  DE_STRING, (uint16_t) strlen(params->device_name), (uint8_t *) params->device_name);

    // 0x0101 "ServiceDescription"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0101);
    de_add_data(service,  DE_STRING, (uint16_t) strlen("TOPO GIGIO-hid"), (uint8_t *) "TOPO GIGIO-hid" ); //params->device_name);

    // 0x0102 "ProviderName"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0102);
    de_add_data(service,  DE_STRING, (uint16_t) strlen("SATAN7-hid"), (uint8_t *) "SATAN7-hid"); // params->device_name);

    // 0x0200    ->    Deprecated in v1.1.1
    // de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_DEVICE_RELEASE_NUMBER);
    // de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0101);

    // 0x0201
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_PARSER_VERSION);
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0111);  // v1.1.1

    // 0x0202
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_DEVICE_SUBCLASS);
    de_add_number(service,  DE_UINT, DE_SIZE_8,  params->hid_device_subclass); ////////////////////////////////////
    
    // 0x0203
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_COUNTRY_CODE);
    de_add_number(service,  DE_UINT, DE_SIZE_8,  params->hid_country_code);

    // 0x0204
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_VIRTUAL_CABLE);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  params->hid_virtual_cable ? 1 : 0);

    // 0x0205
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_RECONNECT_INITIATE); 
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  params->hid_reconnect_initiate ? 1 : 0); 

    // 0x0206
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t* hidDescriptor = de_push_sequence(attribute);
        {
            de_add_number(hidDescriptor,  DE_UINT, DE_SIZE_8, 0x22);    // Report Descriptor
            de_add_data(hidDescriptor,  DE_STRING, params->hid_descriptor_size, (uint8_t *) params->hid_descriptor);
        }
        de_pop_sequence(attribute, hidDescriptor);
    }        
    de_pop_sequence(service, attribute);  
    
    
    // 0x0207
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HIDLANGID_BASE_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t* hig_lang_base = de_push_sequence(attribute);
        {
            // see: http://www.usb.org/developers/docs/USB_LANGIDs.pdf
            de_add_number(hig_lang_base,  DE_UINT, DE_SIZE_16, 0x0409);    // HIDLANGID = English (US)
            de_add_number(hig_lang_base,  DE_UINT, DE_SIZE_16, 0x0100);    // HIDLanguageBase = 0x0100 default
        }
        de_pop_sequence(attribute, hig_lang_base);
    }
    de_pop_sequence(service, attribute);

    // battery power 
    /*
    // 0x0209
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_BATTERY_POWER); //// PROVARE
    de_add_number(service,  DE_UINT, DE_SIZE_16,  100);  
    */

    // 0x020A
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_REMOTE_WAKE); 
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  params->hid_remote_wake ? 1 : 0); ////////////////////////////////////

    /*
    // 0x020B
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_PROFILE_VERSION); //// PROVARE
    de_add_number(service,  DE_UINT, DE_SIZE_16,  0x0101);  
    */

    // supervision timeout
    // 0x020C
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_SUPERVISION_TIMEOUT); 
    de_add_number(service,  DE_UINT, DE_SIZE_16, params->hid_supervision_timeout);

    // 0x020D
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_NORMALLY_CONNECTABLE); 
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  params->hid_normally_connectable ? 1 : 0); 

    // 0x020E
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_BOOT_DEVICE); 
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  params->hid_boot_device ? 1 : 0);

    // 0x020F
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HIDSSR_HOST_MAX_LATENCY); 
    de_add_number(service,  DE_UINT, DE_SIZE_16, params->hid_ssr_host_max_latency);

    // 0x0210
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HIDSSR_HOST_MIN_TIMEOUT); 
    de_add_number(service,  DE_UINT, DE_SIZE_16, params->hid_ssr_host_min_timeout);
}

void crea_sdp_spp(uint8_t *service, uint32_t service_record_handle, int rfcomm_channel, const char *name, uint16_t vendor_id_source, uint16_t vendor_id, uint16_t product_id, uint16_t version, const hid_sdp_record_t * params) {
	
	uint8_t* attribute;
	de_create_sequence(service);
    
    // 0x0000 "Service Record Handle"
	de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_HANDLE);
	de_add_number(service, DE_UINT, DE_SIZE_32, service_record_handle);
    
	// 0x0001 "Service Class ID List"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_CLASS_ID_LIST);
	attribute = de_push_sequence(service);
	{
	    de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_SERIAL_PORT );	 
	}
	de_pop_sequence(service, attribute);
	
	// 0x0002 "Service Record State"
	de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_STATE);
	de_add_number(service, DE_UINT, DE_SIZE_32, (uint32_t) SERVICE_RECORD_STATE_BT);
    
    // 0x0004 "Protocol Descriptor List"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST);
	attribute = de_push_sequence(service);
	{
		uint8_t* l2cpProtocol = de_push_sequence(attribute);
		{
			de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
            de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, BLUETOOTH_PSM_RFCOMM); // aggiunto da me
		}
		de_pop_sequence(attribute, l2cpProtocol);
		
		uint8_t* rfcomm = de_push_sequence(attribute);
		{
			de_add_number(rfcomm,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_RFCOMM);  // rfcomm_service
			de_add_number(rfcomm,  DE_UINT, DE_SIZE_8,  rfcomm_channel);  // rfcomm channel
		}
		de_pop_sequence(attribute, rfcomm);
	}
	de_pop_sequence(service, attribute);
	
	// 0x0005 "Public Browse Group"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BROWSE_GROUP_LIST); // public browse group
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PUBLIC_BROWSE_ROOT );
	}
	de_pop_sequence(service, attribute);
	
	// 0x0006  BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
	attribute = de_push_sequence(service);
	{
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x656e);
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x006a);
		de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x0100);
	}
	de_pop_sequence(service, attribute);
	
	// 0x0009 "Bluetooth Profile Descriptor List"
	de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST);
	attribute = de_push_sequence(service);
	{
		uint8_t *sppProfile = de_push_sequence(attribute);
		{
			de_add_number(sppProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_SERIAL_PORT);
			de_add_number(sppProfile,  DE_UINT, DE_SIZE_16, 0x0102);
		}
		de_pop_sequence(attribute, sppProfile);
	}
	de_pop_sequence(service, attribute);
	
	
    // 0x0100 "ServiceName"
    if (name == NULL){
        name = "SPP";
    }
    if (strlen(name) > 0){
        de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0100);
        de_add_data(service,  DE_STRING, (uint16_t) strlen(name), (uint8_t *) name);
    }
    
    // 0x0101 "ServiceDescription"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0101);
    de_add_data(service,  DE_STRING, (uint16_t) strlen("TOPO GIGIO-spp"), (uint8_t *) "TOPO GIGIO-spp" ); //params->device_name);

    // 0x0102 "ProviderName"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0102);
    de_add_data(service,  DE_STRING, (uint16_t) strlen("SATAN7-spp"), (uint8_t *) "SATAN7-spp"); // params->device_name);
    
}


private:
    bool _running = false;
    uint8_t  _sdp_service_buffer[2000];
    uint8_t  _sdp_service_buffer_spp[200];
    uint8_t  _sdp_service_buffer_hid[1600];
    uint8_t  _sdp_service_buffer_id[200];
    const int RFCOMM_SERVER_CHANNEL = 1;
    //uint8_t *rptBLE = (uint8_t*)malloc(6);  // gestire gli eventi diversamente
    uint8_t rptBLE[30]; // mettere la dimensione massima del buffer che viene passato +1 .. esempio, per il muose è (5), tastiera (sizeof(KeyReport)) e joystick sizeof(gamepad16Report),ma messo quella più grande +1 
};



#endif