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

#ifndef AHB_GW_CAN_TO_H
#define AHB_GW_CAN_TO_H
    #include "ahb.h"
    #include <SPI.h>  
    #include <Ethernet2.h>
    #include <EthernetUdp2.h>     
    #include "ahb_proto.h"    


class AHB_GW_CAN_TO_UDP : public AHB_COMM {
        private:
            /**
             * Modbus 
             * @see 
             * @see https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
             */
            EthernetUDP _interface;
            
            unsigned int localPort = 8888;
            
            unsigned int   remPort = 54321;
            
            IPAddress dest_gw_can_to_udp; 
            
            char buffer[50];
            
            #define UDP_TX_PACKET_MAX_SIZE  10
            
            char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

            
        public:
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
            AHB_GW_CAN_TO_UDP(uint32_t localport, uint32_t remoteport, IPAddress gw_ip);

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

#endif //AHB_GW_CAN_TO_H
