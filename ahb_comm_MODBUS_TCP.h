/*
  aHomeBus MODBUS_TCP (over TCP/IP) interface

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

#ifndef AHB_MODBUS_TCP__H
#define AHB_MODBUS_TCP__H
    #include "ahb.h"
    #include "ModbusTCP.h"

class AHB_MODBUS_TCP : public AHB_COMM {
        private:
            /**
             * Modbus TCP object
             * @see ModbusTCP
             * @see https://github.com/goddland16/Modbus-TCP
             */
            ModbusTCP _interface;



        public:
           uint8_t all_network_address[255]; //256
           
           uint8_t address_network_get(uint8_t i);
           
           void address_network_set(uint8_t *addr_net , int len_addr_net);
           /**
            Return to main class busType for communication logic from typedef enum enum_busType
            */           
           uint8_t busType(); 
           
            /**
            Constructor.    _u8MBUnitID = 1; _u16MBTransactionID = 1;
            */
            AHB_MODBUS_TCP();

            /**
            Constructor. _u8MBUnitID = u8MBUnitID;

            Creates class object using Modbus Unit ID. This is for unique identification
            of individual client by the server. 

            @overload void ModbusTCP::ModbusTCP(uint8_t u8MBUnitID)
            @param u8MBUnitID Modbus Unit ID (1..255)
            */
            AHB_MODBUS_TCP(uint8_t u8MBUnitID);

            /**
             * Initialize CAN controller
             * @return byte error code
             * @see https://github.com/Seeed-Studio/CAN_BUS_Shield/blob/master/mcp_can_dfs.h
             */
            byte begin(void);

            /**
             * Parse CAN-address into our metadata format
             * @param canAddr CAN-address
             * @return ahbMeta object containing decoded metadata, targst/source==0x00 on errors
             */
            ahbMeta ahbCanAddrParse(uint32_t rxId_can);

            /**
             * Assemble a CAN-address based on our adressing format
             * @param meta ahbMeta object
             * @return unsigned long CAN-address
             */
            uint32_t ahbCanAddrAssemble(ahbMeta meta);
            /**
             * Assemble a CAN-address based on our adressing format
             * @param type 2 bit message type (AHB_PKGTYPE_*)
             * @param target address between 0x0001 and 0x07FF/0xFFFF
             * @param source source address between 0x0001 and 0x07FF
             * @return unsigned long CAN-address
             */
            //unsigned long ahbCanAddrAssemble(byte type, unsigned int target, unsigned int source);
            uint32_t ahbCanAddrAssemble(uint8_t type, uint8_t target, uint8_t source);
            /**
             * Assemble a CAN-address based on our adressing format
             * @param type 2 bit message type (AHB_PKGTYPE_*)
             * @param target address between 0x0001 and 0x07FF/0xFFFF
             * @param source source address between 0x0001 and 0x07FF
             * @param port port address between 0x00 and 0x1F, Unicast only
             * @return unsigned long CAN-address
             */
            //unsigned long ahbCanAddrAssemble(byte type, unsigned int target, unsigned int source, char port);
            uint32_t ahbCanAddrAssemble(uint8_t f_type, uint8_t f_cmd, uint8_t f_target, uint8_t f_port, uint8_t f_source);
            /**
             * Send message to CAN-bus
             * @param type 2 bit message type (AHB_PKGTYPE_*)
             * @param target address between 0x0001 and 0x07FF/0xFFFF
             * @param source source address between 0x0001 and 0x07FF
             * @param port port address between 0x00 and 0x1F, Unicast only
             * @param len number of bytes to send (0-8)
             * @param data array of bytes to send
             */
            //bool ahbSend(byte type, unsigned int target, unsigned int source, char port, byte len, byte data[8]);
            //bool ahbSend(uint8_t type, uint8_t target, uint8_t source, uint8_t port, uint8_t len, byte data[8]);
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

#endif /* AHB_MODBUS_TCP_H */
