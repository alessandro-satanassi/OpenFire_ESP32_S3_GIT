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

#include "OpenFIRE_SerialBT.h"
#include <CoreMutex.h>
#define CCALLBACKNAME _SERIALBTCB
#include <ctocppcallback.h>

#define PACKETHANDLERCB(class, cbFcn) \
  (CCALLBACKNAME<void(uint8_t, uint16_t, uint8_t*, uint16_t), __COUNTER__>::func = std::bind(&class::cbFcn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), \
   static_cast<btstack_packet_handler_t>(CCALLBACKNAME<void(uint8_t, uint16_t, uint8_t*, uint16_t), __COUNTER__ - 1>::callback))


// ================================================================


bool OpenFIRE_SerialBT_::setFIFOSize(size_t size) {
    if (!size || _running) {
        return false;
    }
    _fifoSize = size + 1; // Always 1 unused entry
    return true;
}

OpenFIRE_SerialBT_::OpenFIRE_SerialBT_() {
    mutex_init(&_mutex);
}

void OpenFIRE_SerialBT_::begin(unsigned long baud, uint16_t config) {
    if (_running) {
        end();
    }

    (void) baud;
    (void) config;

    _overflow = false;

    _queue = new uint8_t[_fifoSize];
    _writer = 0;
    _reader = 0;

    /* ============= RIMOSSO ==================
    // register for HCI events
    */
    //_hci_event_callback_registration.callback = PACKETHANDLERCB(OpenFIRE_SerialBT_, packetHandler);
    //hci_add_event_handler(&_hci_event_callback_registration);
    //======================================== */


/* ============= RIMOSSO ==================
    l2cap_init();

#ifdef ENABLE_BLE
    // Initialize LE Security Manager. Needed for cross-transport key derivation
    sm_init();
#endif
======================================== */

    rfcomm_init();
    rfcomm_register_service(PACKETHANDLERCB(OpenFIRE_SerialBT_, packetHandler), RFCOMM_SERVER_CHANNEL, 0xffff);  // reserved channel, mtu limited by l2cap

    //_hci_event_callback_registration.callback = PACKETHANDLERCB(OpenFIRE_SerialBT_, packetHandler);
    //hci_add_event_handler(&_hci_event_callback_registration);


    /* ========= RIMOSSO =================
    // init SDP, create record for SPP and register with SDP
    sdp_init();
    ====================================*/
    
    //bzero(_spp_service_buffer, sizeof(_spp_service_buffer));
    ///////////////////////////////////////////////////////////////memset(_spp_service_buffer, 0, sizeof(_spp_service_buffer));
    //bzero(_device_id_sdp_service_buffer, sizeof(_device_id_sdp_service_buffer));
    //memset(_device_id_sdp_service_buffer, 0, sizeof(_device_id_sdp_service_buffer));
    //* ========= RIMOSSO =================
    //spp_create_sdp_record(_spp_service_buffer, 0x10001, RFCOMM_SERVER_CHANNEL, "PicoW Serial");
    //====================================*/
    // SOSTITUITO COL SEGUENTE
    ///////////////////////////////////////////////spp_create_sdp_record(_spp_service_buffer, sdp_create_service_record_handle(), RFCOMM_SERVER_CHANNEL, "PicoW 69 Serial");
    //spp_create_custom_sdp_record(_spp_service_buffer, sdp_create_service_record_handle(), service_uuid128, RFCOMM_SERVER_CHANNEL, "PicoW Serial");
    
    
    
 
    //device_id_create_sdp_record(_spp_service_buffer, sdp_create_service_record_handle(), DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, 1);







    
    /////////////////////////////////////////////////////////////////////////////sdp_register_service(_spp_service_buffer);

    //device_id_create_sdp_record(_device_id_sdp_service_buffer, sdp_create_service_record_handle(), DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, 1); 
    //device_id_create_sdp_record(_device_id_sdp_service_buffer, sdp_create_service_record_handle(), DEVICE_ID_VENDOR_ID_SOURCE_USB, (uint16_t) DEVICE_VID, (uint16_t) DEVICE_PID, 1); 
   
    //sdp_register_service(_device_id_sdp_service_buffer);

    
    //gap_discoverable_control(1);
    
    // ========= RIMOSSO PER NON CHIEDERE CONFERMA PIN =================
    //gap_ssp_set_io_capability(SSP_IO_CAPABILITY_DISPLAY_YES_NO);
    //================================================================= */
    /*
    if (!_name) {
        setName("PicoW Serial 00:00:00:00:00:00");
    }
    gap_set_local_name(_name);
    */
    /* =========== RIMOSSO ===============
    // Turn on!
    hci_power_control(HCI_POWER_ON);
    =================================== */
    /*
    #define VENDOR_ID 0x1234 // Esempio VID #define PRODUCT_ID 0x5678 // Esempio PID
    #define PRODUCT_ID 0x5678 // Esempio PID
    // Configura VID e PID 
    uint16_t vid = VENDOR_ID; 
    uint16_t pid = PRODUCT_ID; 
    btstack_set_vendor_id(vid); 
    btstack_set_product_id(pid);
    */

    //gap_vendor_specific_command(VENDOR_CMD_SET_VID_PID, vid, pid);


    _running = true;
}

void OpenFIRE_SerialBT_::end() {
    if (!_running) {
        return;
    }
    _running = false;

    //hci_power_control(HCI_POWER_OFF);
    __lockBluetooth();
    delete[] _queue; /// va rimossa ?
    /*
    _overflow = false;
    _writer = 0;
    _reader = 0;
    writeIndex = 0;
    readIndex = 0;
    _writeLen = 0;
    //_running = false;
    //_overflow = false;
    _connected = false;
    */
    __unlockBluetooth();
}

int OpenFIRE_SerialBT_::peek() {
    CoreMutex m(&_mutex);
    if (!_running || !m) {
        return -1;
    }
    if (_writer != _reader) {
        return _queue[_reader];
    }
    return -1;
}
/*
int OpenFIRE_SerialBT_::read() {
    CoreMutex m(&_mutex);
    if (!_running || !m) {
        return -1;
    }
    // aggiungere un timeout non bloccante con millis per attendere il carattere, altrimenti ritorna subito se ancora non è arrivato il dato
    
    if (_writer != _reader) {
        auto ret = _queue[_reader];
        asm volatile("" ::: "memory"); // Ensure the value is read before advancing
        auto next_reader = (_reader + 1) % _fifoSize;
        asm volatile("" ::: "memory"); // Ensure the reader value is only written once, correctly
        _reader = next_reader;
        return ret;
    }
    return -1;
}
*/

int OpenFIRE_SerialBT_::read() {
    CoreMutex m(&_mutex);
    if (!_running || !m) {
        return -1;
    }

    unsigned long startMillis = millis();
    #define TIMEOUTMILLIS  1000
    //unsigned long timeoutMillis = 1000; // Timeout di esempio, aggiustabile secondo necessità

    
    //while ((millis () - lastMillis <= MILLIS_TIMEOUT) && (!USBDevice.mounted())) { yield(); }
    
    while ((_writer == _reader)&&(millis() - startMillis < TIMEOUTMILLIS)) {yield();} 
        if (_writer != _reader) {
            auto ret = _queue[_reader];
            asm volatile("" ::: "memory"); // Ensure the value is read before advancing
            
            // Aggiorna _reader senza l'uso dell'operatore %
            _reader += 1;
            if (_reader >= _fifoSize) {
                _reader -= _fifoSize;
            }
            
            asm volatile("" ::: "memory"); // Ensure the reader value is only written once, correctly
            return ret;
        }
    return -1;
}


bool OpenFIRE_SerialBT_::overflow() {
    if (!_running) {
        return false;
    }

    __lockBluetooth();
    bool ovf = _overflow;
    _overflow = false;
    __unlockBluetooth();

    return ovf;
}

int OpenFIRE_SerialBT_::available() {
    CoreMutex m(&_mutex);
    if (!_running || !m) {
        return 0;
    }
    /////////////return (_fifoSize + _writer - _reader) % _fifoSize;

    int result = _fifoSize + _writer - _reader;  /// alternativa per non usare l'operatore %
    if (result >= _fifoSize) { result -= _fifoSize; } 
    return result;
}

int OpenFIRE_SerialBT_::availableForWrite() {
    CoreMutex m(&_mutex);
    if (!_running || !m) {
        return 0;
    }
    return _connected ? 1 : 0;
}

void OpenFIRE_SerialBT_::flush() {
    // We always send blocking
}

size_t OpenFIRE_SerialBT_::write(uint8_t c) {
    return write(&c, 1);
}

size_t OpenFIRE_SerialBT_::write(const uint8_t *p, size_t len) {
    CoreMutex m(&_mutex);
    if (!_running || !m || !_connected || !len)  {
        return 0;
    }
    _writeLen = len;
/*
    _writeBuff = p;
    _writeLen = len;
    __lockBluetooth();
    rfcomm_request_can_send_now_event(_channelID);
    __unlockBluetooth();
    while (_connected && _writeLen) {
        // noop busy wait 
    }
    return len;
*/

//////////////////////////////////////////////////

    size_t sentBytes = 0;

    while (sentBytes < len) {
        // Calcola lo spazio disponibile nel buffer circolare
        size_t spaceAvailable = (writeIndex >= readIndex) ? (BUFFER_SIZE - writeIndex + readIndex) : (readIndex - writeIndex - 1);
    
        // Controlla se c'è spazio disponibile
        if (spaceAvailable > 0) {
            // Calcola quanti byte possono essere scritti
            size_t bytesToWrite = min(spaceAvailable, len - sentBytes);
            
            /*
            // Scrive i dati nel buffer circolare
            for (size_t i = 0; i < bytesToWrite; i++) {
                buffer[writeIndex] = p[sentBytes + i];
                writeIndex = (writeIndex + 1) % BUFFER_SIZE;
            }

            sentBytes += bytesToWrite;
            */
        
            // Scrive i dati nel buffer circolare
            size_t bytesToWriteEnd = min(bytesToWrite, BUFFER_SIZE - writeIndex);
            memcpy(&buffer[writeIndex], &p[sentBytes], bytesToWriteEnd);
            if (bytesToWrite > bytesToWriteEnd) {
                memcpy(&buffer[0], &p[sentBytes + bytesToWriteEnd], bytesToWrite - bytesToWriteEnd);
            }
            //////////////////////////// writeIndex = (writeIndex + bytesToWrite) % BUFFER_SIZE;
            writeIndex += bytesToWrite; 
            if (writeIndex >= BUFFER_SIZE) { writeIndex -= BUFFER_SIZE; } // metodo forse più performante di quello sopra con l'operatore %
            
            sentBytes += bytesToWrite;      
        }
        else
        {
            __lockBluetooth();
            rfcomm_request_can_send_now_event(_channelID);
            __unlockBluetooth();
            while (_connected && _writeLen) { 
                yield(); //// ?????
                // noop busy wait 
            }
        }
         
    }
    __lockBluetooth();
    rfcomm_request_can_send_now_event(_channelID);
    __unlockBluetooth();
    
    return sentBytes; 
  }


OpenFIRE_SerialBT_::operator bool() {
    return _running;
}


uint8_t OpenFIRE_SerialBT_::send_data(uint16_t rfcomm_cid) {
    //rfcomm_channel_t * channel = rfcomm_channel_for_rfcomm_cid(rfcomm_cid);
    
    // Controlla se ci sono dati da inviare
    
    size_t dataAvailable = (readIndex <= writeIndex) ? (writeIndex - readIndex) : (BUFFER_SIZE - readIndex + writeIndex);

    if (dataAvailable > 0) {
      // Riserva il buffer di pacchetti
      rfcomm_reserve_packet_buffer();
      uint8_t* outgoingBuffer = rfcomm_get_outgoing_buffer();
      int max_frame_size = rfcomm_get_max_frame_size(rfcomm_cid);

      // Calcola quanti byte possono essere inviati in questo pacchetto
      size_t bytesToSend = min(max_frame_size, dataAvailable);

      // Copia i dati nel buffer di uscita
      size_t bytesToSendEnd = min(bytesToSend, BUFFER_SIZE - readIndex);
      memcpy(outgoingBuffer, &buffer[readIndex], bytesToSendEnd);
      if (bytesToSend > bytesToSendEnd) {
        memcpy(&outgoingBuffer[bytesToSendEnd], &buffer[0], bytesToSend - bytesToSendEnd);
      }

      // Invia i dati 
      uint8_t status = rfcomm_send_prepared(rfcomm_cid, bytesToSend);

      // Aggiorna l'indice di lettura
      //////////////////////////// readIndex = (readIndex + bytesToSend) % BUFFER_SIZE;
      readIndex += bytesToSend; 
      if (readIndex >= BUFFER_SIZE) { readIndex -= BUFFER_SIZE; } // dovrebbe essere più efficente di quella sopra con %
    
        if (status != ERROR_CODE_SUCCESS) {
            rfcomm_release_packet_buffer();  
        }
    
        if (bytesToSend<dataAvailable){
            rfcomm_request_can_send_now_event(_channelID); // da rivedere
        }
        
    } 
    _writeLen = 0;
    return ERROR_CODE_SUCCESS;
}



void OpenFIRE_SerialBT_::packetHandler(uint8_t type, uint16_t channel, uint8_t *packet, uint16_t size) {
    UNUSED(channel);
    bd_addr_t event_addr;
    //uint8_t   rfcomm_channel_nr;
    //uint16_t  mtu;
    int i;

    switch (type) {
    case HCI_EVENT_PACKET:
        switch (hci_event_packet_get_type(packet)) {
        case HCI_EVENT_PIN_CODE_REQUEST:
            //Serial.printf("Pin code request - using '0000'\n");
            hci_event_pin_code_request_get_bd_addr(packet, event_addr);
            gap_pin_code_response(event_addr, "0000");
            break;

        case HCI_EVENT_USER_CONFIRMATION_REQUEST:
            // ssp: inform about user confirmation request
            //Serial.printf("SSP User Confirmation Request with numeric value '%06" PRIu32 "'\n", little_endian_read_32(packet, 8));
            //Serial.printf("SSP User Confirmation Auto accept\n");
            break;

        case RFCOMM_EVENT_INCOMING_CONNECTION:
            rfcomm_event_incoming_connection_get_bd_addr(packet, event_addr);
            //rfcomm_channel_nr = rfcomm_event_incoming_connection_get_server_channel(packet);
            _channelID = rfcomm_event_incoming_connection_get_rfcomm_cid(packet);
            //Serial.printf("RFCOMM channel %u requested for %s\n", rfcomm_channel_nr, bd_addr_to_str(event_addr));
            rfcomm_accept_connection(_channelID);
            break;

        case RFCOMM_EVENT_CHANNEL_OPENED:
            if (rfcomm_event_channel_opened_get_status(packet)) {
                //Serial.printf("RFCOMM channel open failed, status 0x%02x\n", rfcomm_event_channel_opened_get_status(packet));
            } else {
                _channelID = rfcomm_event_channel_opened_get_rfcomm_cid(packet);
                //mtu = rfcomm_event_channel_opened_get_max_frame_size(packet);
                //Serial.printf("RFCOMM channel open succeeded. New RFCOMM Channel ID %u, max frame size %u\n", rfcomm_channel_id, mtu);
                _connected = true;
            }
            break;
        case RFCOMM_EVENT_CAN_SEND_NOW:
            _channelID = rfcomm_event_can_send_now_get_rfcomm_cid(packet); // non serve provare a togliere
            send_data(_channelID);


            //rfcomm_send(_channelID, (uint8_t *)_writeBuff, _writeLen);
            //_writeLen = 0;
            break;
        case RFCOMM_EVENT_CHANNEL_CLOSED:
            //Serial.printf("RFCOMM channel closed\n");
            _channelID = 0;
            _connected = false;
            break;

        default:
            break;
        }
        break;
/*
    case RFCOMM_DATA_PACKET:
        for (i = 0; i < size; i++) {
            auto next_writer = _writer + 1;
            if (next_writer == _fifoSize) {
                next_writer = 0;
            }
            if (next_writer != _reader) {
                _queue[_writer] = packet[i];
                asm volatile("" ::: "memory"); // Ensure the queue is written before the written count advances
                // Avoid using division or mod because the HW divider could be in use
                _writer = next_writer;
            } else {
                _overflow = true;
            }
        }
        break;
*/

    case RFCOMM_DATA_PACKET:
        if (size > 0) {
            // Calcola lo spazio disponibile nel buffer circolare
            size_t spaceAvailable = (_reader <= _writer) ? (_fifoSize - _writer + _reader) : (_reader - _writer - 1);
            if (spaceAvailable >= size) {
                size_t firstChunk = _fifoSize - _writer;
                if (firstChunk > size) {
                    firstChunk = size;
                }
                // Copia il primo blocco di dati nel buffer circolare
                memcpy(&_queue[_writer], packet, firstChunk);
                _writer += firstChunk;
                if (_writer == _fifoSize) {
                    _writer = 0;
                }
                size_t remaining = size - firstChunk;
                if (remaining > 0) {
                    memcpy(&_queue[_writer], packet + firstChunk, remaining);
                    _writer += remaining;
                    if (_writer == _fifoSize) {
                        _writer = 0;
                    }
                }
                asm volatile("" ::: "memory"); // Assicura che la coda venga scritta prima di avanzare il conteggio scritto
            } else {
                _overflow = true;
            }
        }
        break;

    default:
        break;
    }
}
