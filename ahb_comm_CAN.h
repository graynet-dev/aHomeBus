/*
  aHomeBus CAN interface

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

#ifndef AHB_CAN__H
#define AHB_CAN__H
    #include "ahb.h"
    #include <mcp_can_coryjfowler.h>
    #include <SPI.h>

    /**
     * Global variable indicating a CAN-bus got a message
     * This is currently shared amongst all CAN-interfaces!
     */
extern volatile bool ahb_CANIntReq;

    /**
     * Global interrupt function called on CAN interrupts
     * This is currently shared amongst all CAN-interfaces!
     */
void ahb_CANInt(void);

    /**
     * CAN Communication Interface
     * @see AHB_COMM
     */
     

  
class AHB_CAN : public AHB_COMM { // <net_size>
        private:
            /**
             * CAN-Bus object
             * @see MCP_CAN
             * @see https://github.com/coryjfowler/MCP_CAN_lib
             */
            MCP_CAN _interface;

            /**
             * Interrupt pin to be used
             */
            byte _intPin = 0;

            /**
             * CAN Bus Speed
             */
            byte _speed = 0;

            /**
             * CAN Crystal Frequency
             */
            byte _clockspd = 0;
                       
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
             * Constructor for NonInterrupt operation
             * @TODO This is not implemented yet…
             * @param cs pin used for CHIP_SELECT
             * @param speed CAN bus speed definition from mcp_can_dfs.h (bottom)
             * @param clockspd MCP crystal frequency from mcp_can_dfs.h (bootom)
             */
            AHB_CAN(byte cs, byte speed, byte clockspd);

            /**
             * Constructor for Interrupt based operation
             * @param cs pin used for CHIP_SELECT
             * @param speed CAN bus speed definition from mcp_can_dfs.h (bottom)
             * @param clockspd MCP crystal frequency from mcp_can_dfs.h (bootom)
             * @param interrupt pin used for interrupt. Must have hardware INT support
             * @see 
             */
            AHB_CAN(byte cs, byte speed, byte clockspd, byte interrupt, uint8_t *route_table, uint8_t netsize);

            /**
             * Initialize CAN controller
             * @return byte error code
             * @see 
             */
            byte begin(void);
            
            void mcp2515_reset(void);
            
            uint8_t checkError(void);
            
            uint8_t errorCountRX(void);
            
            uint8_t errorCountTX(void); 
            
            uint8_t getError(void);

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
            //uint32_t ahbCanAddrAssemble(uint8_t f_type, uint8_t f_cmd, uint8_t f_target, uint8_t f_port, uint8_t f_source);
            uint32_t ahbCanAddrAssemble(bool priority, uint8_t dev_type, uint8_t f_target, uint8_t msg_type, uint8_t f_source);
            
            /**
             * Send message to CAN-bus
             * @param type 2 bit message type (AHB_PKGTYPE_*)
             * @param target address between 0x0001 and 0x07FF/0xFFFF
             * @param source source address between 0x0001 and 0x07FF
             * @param port port address between 0x00 and 0x1F, Unicast only
             * @param len number of bytes to send (0-8)
             * @param data array of bytes to send
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

#endif /* AHB_CAN__H */

            /* typedef enum enum_nodeState {
                 Initialisation  = 0x00,  // 0:   'INITIALISING',    - +
                 Disconnected    = 0x01,  // 1:   ''
                 Connecting      = 0x02,  // 2:   ''
                 Preparing       = 0x02,  // 3:   ''
                 Stopped         = 0x04,  // 4:   'STOPPED',         - +
                 Operational     = 0x05,  // 5:   'OPERATIONAL',     - + полный функционал
                                          // 80:  'SLEEP',          -
                                          // 96:  'STANDBY',        -
                 Pre_operational = 0x7F,  // 127: 'PRE-OPERATIONAL'- + конфигурирование по SDO, PDO отключен???
                 Unknown_state   = 0x0F   //
            }e_nodeState;         

            typedef struct {
                 INTEGER8 csBoot_Up;
                 INTEGER8 csSDO;
                 INTEGER8 csEmergency;
                 INTEGER8 csSYNC;
                 INTEGER8 csLifeGuard;
                 INTEGER8 csPDO;
                 INTEGER8 csLSS;
            } s_state_communication;
            */ 
            
            /**
             * Bus address of this node
             */