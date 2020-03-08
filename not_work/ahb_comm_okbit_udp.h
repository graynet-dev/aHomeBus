#ifndef AHB_OKBIT_H
#define AHB_OKBIT_H

  #include <OkbitUDP_ESP8266.h>
  #include "ahb.h"

class AHB_OKBIT_UDP : public AHB_COMM { 
        private:
            /**
             * OkbitUDP-Bus object
             * @see 
             */
            OKBIT_UDP _interface;

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
             * Constructor for NonInterrupt operation
             * @TODO This is not implemented yet…
             * @param cs pin used for CHIP_SELECT
             * @param speed CAN bus speed definition from mcp_can_dfs.h (bottom)
             * @param clockspd MCP crystal frequency from mcp_can_dfs.h (bootom)
             */
            AHB_OKBIT_UDP(uint8_t cs);
            //OKBIT_UDP HandlerUDP(sub_id, id, device, firmware[0], firmware[1]);

            /**
             * Constructor for Interrupt based operation
             * @param cs pin used for CHIP_SELECT
             * @param speed CAN bus speed definition from mcp_can_dfs.h (bottom)
             * @param clockspd MCP crystal frequency from mcp_can_dfs.h (bootom)
             * @param interrupt pin used for interrupt. Must have hardware INT support
             * @see 
             */
            AHB_OKBIT_UDP(byte cs, byte speed, byte clockspd, byte interrupt, uint8_t *route_table, uint8_t netsize);

            /**
             * Initialize CAN controller
             * @return byte error code
             * @see 
             */
            byte begin(void);
            
            bool ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]);

            bool ahbReceive_V(ahbPacket &pkg);
            
};


#endif //AHB_OKBIT_H
