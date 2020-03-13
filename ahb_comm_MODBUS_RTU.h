/*
  aHomeBus MODBUS_RTU (over RS-485) interface

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

#ifndef AHB_MODBUS_RTU__H
#define AHB_MODBUS_RTU__H
    #include "ahb.h"
    #include "ModbusRtu.h"        


class AHB_MODBUS_RTU : public AHB_COMM {
        private:
            /**
             * Modbus RTU object
             * @see MCP_CAN
             * @see https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
             */
            MODBUS_RTU _interface;
            
            uint16_t au16data[16]; //!< data array for modbus network sharing
            uint8_t u8state; //!< machine state
            uint8_t u8query; //!< pointer to message query
            modbus_t telegram[2];
            bool send_modbus_rtu = false;
            uint8_t modbus_rtu_request_sender;
            
        public:
           void SetNodeId(uint8_t nodeId);
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
            * Default Constructor for Master through Serial
            *
            * @ingroup setup
            */          
            AHB_MODBUS_RTU();   
 
            /**
            * @brief
            * Constructor for a Master/Slave through USB/RS232C via software serial
            * This constructor only specifies u8id (node address) and should be only
            * used if you want to use software serial instead of hardware serial.
            * If you use this constructor you have to begin ModBus object by
            * using "void Modbus::begin(SoftwareSerial *softPort, long u32speed)".
            *
            * @param u8id   node address 0=master, 1..247=slave
            * @ingroup setup
            * @overload Modbus::Modbus(uint8_t u8id, uint8_t u8serno, uint8_t u8txenpin)
            * @overload Modbus::Modbus(uint8_t u8id, uint8_t u8serno)
            * @overload Modbus::Modbus()
            */
            AHB_MODBUS_RTU(uint8_t u8id);
            
            /**
            * @brief
            * Full constructor for a Master/Slave through USB/RS232C
            *
            * @param u8id   node address 0=master, 1..247=slave
            * @param u8serno  serial port used 0..3
            * @ingroup setup
            * @overload Modbus::Modbus(uint8_t u8id, uint8_t u8serno)
            * @overload Modbus::Modbus(uint8_t u8id)
            * @overload Modbus::Modbus()
            */
            AHB_MODBUS_RTU(uint8_t u8id, uint8_t u8serno);

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
            AHB_MODBUS_RTU(uint8_t u8id, uint8_t u8serno, uint8_t u8txenpin, uint8_t *route_table, uint8_t netsize);

            /**
             * Initialize Modbus controller
             * @return byte error code
             * @see 
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
            type
            cmd
            target
            port
            source
            len
            data[8]
             */

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

#endif /* AHB_MODBUS_RTU__H */
