/*
  aHomeBus CAN to UDP Gateway

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AHB_GW_CAN_TO_OKBIT_H
#define AHB_GW_CAN_TO_OKBIT_H
    #include "ahb.h"
    #include <SPI.h>  
    #include <Ethernet2.h>
    #include <EthernetUdp2.h>     
    #include "ahb_proto.h"  
    //#include "OkbitUDP_ESP8266.h"  
    #if defined  (__AVR__)
        #define AHB_EEPROM_USE 
    #endif  
    #ifdef AHB_EEPROM_USE
      #include<EEPROM.h>
    #endif 


class AHB_GW_CAN_TO_OKBIT : public AHB_COMM {
        private:
            /**
             * Modbus 
             * @see 
             * @see https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
             */
            EthernetUDP _interface;
            
            unsigned int localPort_okbit = 6400;
            
            unsigned int   remPort_okbit = 6500;
            
            IPAddress dest_gw_can_to_okbit; 
            
            char buffer[50];
            
            #define UDP_TX_PACKET_MAX_SIZE  10
            
            char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

            char incomingPacket[100];          // буфер для входящих пакетов
            char replyPacekt[100];  // буфер для исходящих пакетов
            
            //OKBIT
            int _vol[10]; // массив вспомогательный
            int _holdingRegs[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};// массив регистров для сравнения
            int _sub_id;
            int _id;
            int _device;
            int _firmware[2];
            char _replyPacekt[100];
            
            
        public:
            // OKBIT
            void parsing(char inPacket[255], int len, uint32_t geteIP); //Парсинг полученного пакета
            void build(int b_sub_id = 0, int b_id = 0, int b_device = 0, int b_cmd = 0 , int b_subto_id = 0, int b_to_id = 0, unsigned int b_vol1 = 0, unsigned int b_vol2 = 0, unsigned int b_vol3 = 0, unsigned int b_vol4 = 0); //Сборка пакета на отправку
            //void send_event();
            #ifdef AHB_EEPROM_USE
            void eeprom_read();
            void eeprom_write(uint32_t geteIP);
            #endif
            void holding_update();
            int status_err;
            int in_cmd;
            int holdingRegs[10];
            char replyGate[100];
            int gateIP[4];
            void debounce(void);
            
            
            /**
            * Pointer to ARRAY from user main app. Array send to consructor route_table
            * {1,2,10,22} etc.
            */         
           uint8_t *all_network_address; 
           
           /**
            * Size route table from constructor. Get from user main app, from constructor.
            * Used as all_network_address [net_size]
            */           
           uint8_t net_size;
           
            /**
            * Return to main class net_size for routing logic 
            */            
           uint8_t net_size_get(void);
           
            /**
            * Return to main class data from array all_network_address[] for routing logic
            */            
           uint8_t address_network_get(uint8_t i);
           
            /**
            * Return to main class busType for communication logic from typedef enum enum_busType
            */           
           uint8_t busType(); 
           
            /**
            * @brief
            * Full constructor for a Master/Slave through USB/RS232C/RS485
            * It needs a pin for flow control only for RS485 mode
            *
            * @param u8id   node address 0=master, 1..247=slave
            * @param u8serno  serial port used 0..3
            * @param u8txenpin pin for txen RS-485 (=0 means USB/RS232C mode)
            * @ingroup setup
            * @overload Modbus::Modbus(uint8_t u8id, uint8_t u8serno, uint8_t u8txenpin)
            * @overload Modbus::Modbus(uint8_t u8id)
            * @overload Modbus::Modbus()
            */
            AHB_GW_CAN_TO_OKBIT(uint32_t localport, uint32_t remoteport, IPAddress gw_ip,int sub_id, int id, int device, int firmware1, int firmware2);

            /**
             * Initialize Modbus controller
             * @return byte error code
             * @see 
             */
            byte begin(void);
            
            uint8_t checkError(void);
            uint8_t errorCountRX(void);
            uint8_t errorCountTX(void); 
            uint8_t getError(void);


            bool ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]);
            /**
             * Receive a message from the CAN-bus
             *
             * This polls the CAN-Controller for new messages.
             * The received message will be passed to &pkg, if no message
             * is available the function will return false.
             *
             * @param pkg ahbPacket-Reference to store received packet
             * @return true if a message was received
             */
            bool ahbReceive_V(ahbPacket &pkg);
};

#endif //AHB_GW_CAN_TO_OKBIT_H
