/*
  aHomeBus communication template (virtual function for AHB comminication classes, uses in AHB class, 
  for access to function to pointer)
  
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

#ifndef AHB_COMM__H
#define AHB_COMM__H
    #include "ahb_proto.h"
    #include "ahb_comm_template.h"

    /**
     * Class defining the base functions for any communication interface
     * This is a template and can not be used by itself
     */
//template <int net_size> 
class AHB_COMM {
        public:
            /**
             * Return to main class net_size for routing logic
             * @return net_size 
             */          
            virtual uint8_t net_size_get(void);
            
            /**
             * Return to main class data from array all_network_address[] for routing logic
             * @param i is number element of array route_table //
             * @return busType 
             */  
            virtual uint8_t address_network_get(uint8_t i);
 
            /**
             * Return bus_type to main class
             * @return busType 
             */           
            virtual uint8_t busType();
            
            /**
             * Error code of the last action - 0 = no error
             */
            byte lastErr = 0;

            /**
             * Initialize Interface
             * @return error code, 0=OK
             */
            virtual byte begin(void) = 0;

/**
Заполнение очереди команд
*/
//virtual void ahbSendCommand(byte Command_Razryad_1, byte Command_Razryad_2, byte Target_Address,  byte Device_Type);
            
/**
Отправка команд из очереди команд
*/
//virtual void ahbSendCommand_queue();
            
            /**
             * Send message to the interface
             * @param type 2 bit message type (AHB_PKGTYPE_*)
             * @param target address between 0x0001 and 0x07FF/0xFFFF
             * @param source source address between 0x0001 and 0x07FF
             * @param port port address between 0x00 and 0x1F, Unicast only
             * @param len number of bytes to send (0-8)
             * @param data array of bytes to send
             */
            //virtual bool ahbSend(byte type, unsigned int target, unsigned int source, char port, byte len, byte data[8]) = 0;
            virtual bool ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t source, uint8_t port, uint8_t len, byte data[8]) = 0;
            
            /**
             * Receive a message from the interface
             *
             * The received message will be passed to &pkg, if no message
             * is available the function will return false.
             *
             * @param pkg ahbPacket-Reference to store received packet
             * @return true if a message was received
             */
            virtual bool ahbReceive_V(ahbPacket &pkg)=0;
};

#endif /* AHB_COMM__H */
