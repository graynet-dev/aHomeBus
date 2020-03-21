/**
  aHomeBus bus interface

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



#ifndef AHB__H
    #define AHB__H
    #include "Wire.h"  
    #include <SPI.h>        
    #include "ahb_master.h" 
    #include "ahb_slave.h" 
    #include "ahb_node.h"     
    #include "ahb_comm_template.h"   
    #include "ahb_proto.h"
    #include "ahb_hook.h"

       
    #include "ahb_comm_CAN.h"
    #include "ahb_comm_UART.h"
    #include "ahb_comm_MODBUS_RTU.h"
    #include "ahb_comm_MODBUS_TCP.h"

    #include "ahb_io.h"
    #include "ahb_io_din.h"
    #include "ahb_io_dout.h"
    
    #include "ahb_eth.h"
    #include "ahb_WEB.h"
    
    #include "ahb_gw_can_to_udp.h"
    #include "ahb_gw_can_to_okbit.h"
    
    
    //#include "ahb_comm_okbit_udp.h"
    
    
   // #include "ahb_timers.h"
    

    
    #if defined  (__AVR__)
       #include <avr/wdt.h>
    #endif  
    
    //#include <stdint.h>  

    #if defined  (__AVR__)
        #define AHB_EEPROM_USE //Arduino DUE not have EEPROM, set manual or use end sectors flash
    #endif    

    #ifdef AHB_EEPROM_USE
      #include<EEPROM.h>
    #endif    

    
    
    /**
     * Maximum number of parallel communication interfaces
     *
     * AHB_BUSNUM sets the maximum number of active communication interfaces
     * in one instance. You can set it to a integer between 1 and 120. Keep
     * in mind higher numbers will increase RAM and CPU usage. Default is 6.
     */
     
    #ifndef AHB_BUSNUM
        #define AHB_BUSNUM 6 //<120!
    #endif

    /**
     * Activate debugging on Serial
     *
     */
    #ifndef AHB_DEBUG
        //#define AHB_DEBUG
    #endif

    /**
     * Maximum number of active hooks
     *
     * AHB_HOOKNUM sets the maximum number of active hooks in one instance.
     * You can set it to a integer between 1 and 120. Keep
     * in mind higher numbers will increase RAM and CPU usage. Default is 16.
     */
    #ifndef AHB_HOOKNUM
        #define AHB_HOOKNUM 16 //<120!
    #endif

    /**
     * Maximum number of active modules
     *
     * AHB_MODNUM sets the maximum number of active modules in one instance.
     * You can set it to a integer between 1 and 120. Keep in mind higher
     * numbers will increase RAM and CPU usage. Default is 16.
     */
    #ifndef AHB_MODNUM
        #define AHB_MODNUM 16 //<120!
    #endif
    
    /**
    * Set parametr for HW Clock DS3231
    */   
    #ifndef DS3231_I2C_ADDRESS
        #define DS3231_I2C_ADDRESS          (0x68)
        #define DS3231_REG_TIME             (0x00)
        #define DS3231_REG_ALARM_1          (0x07)
        #define DS3231_REG_ALARM_2          (0x0B)
        #define DS3231_REG_CONTROL          (0x0E)
        #define DS3231_REG_STATUS           (0x0F)
        #define DS3231_REG_TEMPERATURE      (0x11)
    #endif

typedef enum enum_busType {
                type_null_bus,
                type_GW_CAN_TO_UDP,
                type_GW_CAN_TO_OKBIT,
                type_CAN,
                type_UART,
                type_MQTT,
                type_MODBUS_RTU,
                type_MODBUS_TCP,
                type_TCP
                
} enum_busTYPE;
    
     /**
      AHB Node type ENUM
     */   
typedef enum enum_nodeType {
                null_node,
                Master,
                Slave,
                Node
} enum_nodeTYPE;
    
    /**
     * AHB main controller class
     *
     * This class stores all node parameters, handles packet routing between
     * different communication interfaces, implements basic functions like
     * sending boot messages, answering to PING and allows for attaching your
     * own functions to various messages
     */    

//void ahb_RXInt(void);
class AHB_NODE;
class AHB_NODE2;
//class AHB_SLAVE;
//class AHB_MASTER;

class AHB {
        private:    

            AHB_MASTER *_master; 
            AHB_SLAVE *_slave;
            AHB_NODE *_node;
            AHB_NODE2 *_node2;  //Непонятная хрень, без этого указателя на пустой класс идет сбой nodeAttach          

            /**
             * Array of pointers to active communication interfaces
             */
            AHB_COMM *_busAddr[AHB_BUSNUM];


              

            /**
             * Array of structs containing hooks
             */
            ahbHook _hooks[AHB_HOOKNUM];

            /**
             * Array of pointers to active modules
             */
            AHB_IO *_module[AHB_MODNUM];

            /**
             * First EEPROM address to use
             */
            unsigned int _cfgAddrStart=0;

            /**
             * Last EEPROM address to use
             */
            unsigned int _cfgAddrStop=511;           

            /**
             Массив ячеек очереди сообщений, в которой хранятся сообщения на отправку
             10 сообщений по 15 параметров
             */             
            byte Message_TX_cell[30][15] = {{ 0 }}; 
            
            
            unsigned long prevTimeNodeGuard = 0; //для таймера NodeGuard
            
            unsigned long prevTimeHeartbeat = 0; //для таймера Heartbeat
            
            unsigned long prevTimeHeartbeatMaster = 0; //для таймера Heartbeat
            
            unsigned long prevTimeNodeGuard_print = 0; //для таймера NodeGuard
            
            unsigned long curMillis = 0; //снимок машинного времени
            
            unsigned long curMicros = 0;
            
            const byte intervalNodeGuard = 10; // интервал таймера NodeGuard
            
            const byte intervalHeartbeat = 1; // интервал таймера Heartbeat
            
            const byte intervalNodeGuard_print = 10; // интервал таймера NodeGuard
            
            unsigned long prevTimeSndMsgQueue = 10; //для таймера NodeGuard
            
            int num_node_loop=0;
            
            /**
            * Convert normal decimal numbers to binary coded decimal
            */
            byte decToBcd(byte val);
            
            /**
            * Convert binary coded decimal to normal decimal numbers
            */           
            byte bcdToDec(byte val);
            
            /**
            * Read temp DS3231 chip 
            */             
            float readDS3231temp();
            
            /**
            * Set date time DS3231 chip 
            */               
            void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);

            /**
            * Read date time DS3231 chip 
            */             
            void readDS3231time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year);

            void system_uptime(void);
           
            void system_hwtime(void);
           
            void PrintSystemUpTime (void);
           
            void PrintSystemHwTime (void);
           
            void system_WTD(void);
            
            
            #define WDT_KEY (0xA5)
        
            //void watchdogSetup (void);
            
            uint32_t status_wdt;
            
            /**
            * Watch dog timer
            */
            unsigned long prevtime_wtd = 0;

            /**
            * Watch dog loop
            */            
            void WTD(void);
            
            /**
            * Watch dog 1 enable/ 0 disable
            */            
            bool WTD_set(bool set_wdt); 
            
            signed char busId_GW_CAN_TO_UDP = -1;
            
            signed char busId_GW_CAN_TO_OKBIT = -1;

            
        public:
            
            bool NodeGuard_OK_check (uint8_t a, uint8_t b);

            /*
            Массив со статусами удаленных узлов. Заполняется на основе ответов
            */
            //bool NodeGuard_OK[255][3] = {{ 0 }}; //[2]
 
            /**
             * Node Type 
             * Master       master node
             * Slave        slave node
             * Node         node
             */                
           uint8_t _nodeType; //лучше в приваты и выдавать через функцию, хотя может стоит 
                              //и на лету менять тип узла из приложения????

            /**
             * Bus address of this node
             */
           uint8_t _nodeId=0;      
           
           /* У мастера и слейва есть        
           1. Аппаратные часы храним xx_hw их значение передаем в PING
           2. Up_time часы храним в xx_ut для получения своего время жизни
           3. Up_time удаленных узлов их храним в xx_ut_rn
           */

            /* У узлов нет аппаратных часов но их мы заменяем рассылкой в PING от мастера и слейва
            1. PING от аппаратных часов мастера их храним в xx_hw, !!!!!! не забыть их крутить пока не пришел новый PING
            2. Up_time часы храним в xx_ut для получения своего время жизни
            3. Up_time удаленных узлов !!!!!!!!!!!!!НУЖНО очистить память они не нужны
            */
            // Системные часы если есть аппаратный контроллер
            uint8_t ss_hw = 0;
            uint8_t mm_hw = 0;
            uint8_t hh_hw = 0;
            uint8_t dd_hw = 0;
            uint8_t dayOfWeek_hw = 0;
            uint8_t month_hw = 0;
            byte    year_hw = 0;
            byte    temp_hw = 0;
        
           // Up Time my node - системные часы на миллис 
           uint8_t ss_ut = 0;
           uint8_t mm_ut = 0;
           uint8_t hh_ut = 0;
           uint8_t dd_ut = 0;
           uint32_t prevSystemtime_ut = 0;

           // Up Time remote node 
           //uint8_t ut_rn[256][6];           

           uint8_t print_ss_ut(void);
           uint8_t print_mm_ut(void);
           uint8_t print_hh_ut(void);
           uint8_t print_dd_ut(void);

           uint8_t print_ss_hw(void);
           uint8_t print_mm_hw(void);
           uint8_t print_hh_hw(void);
           uint8_t print_dd_hw(void);
           uint8_t print_month_hw(void);
           uint8_t print_year_hw(void);
           uint8_t print_dayofweak_hw(void);

           uint8_t print_tx_error_rn(uint8_t i);
           uint8_t print_rx_error_rn(uint8_t i);  
          
           uint8_t print_reboot(uint8_t i); 
           
           uint8_t print_ss_rn(uint8_t i);
           uint8_t print_mm_rn(uint8_t i);
           uint8_t print_hh_rn(uint8_t i);
           uint8_t print_dd_rn(uint8_t i);         
        
            /**
             * Constructor reads configuration
             * @param EEPROM start address, usually 0
             * @param EEPROM stop address, usually 511
             */
            AHB(unsigned int start, unsigned int stop, uint8_t ndType);
            
            /**
             * "Light" Constructor without EEPROM-read
             * @param Node-ID between 0x0001 and 0x07FF
             */
            AHB(uint8_t id, uint8_t ndType);

            /**
             * Initialize AHB
             * @return byte error code
             * @see 
             */         
            byte begin(void);
            
            /**
             * Initialize node if no configuration found in EEPROM
             * @param function to execute
             * @return true if initialized
             */
            bool firstboot(void (*function)(), bool manual);

            /**
             * Change Node-ID
             * @param id Node-ID between 0x0001 and 0x07FF
             */
            bool setNodeId(unsigned int id);

            /**
             * Attach a bus-object to this controller
             * @param bus Bus object, derived from AHB_COMM
             * @return bus-ID, -1 on errors
             * @see busDetach()
             */
            char busAttach(AHB_COMM *bus);
            
            bool masterAttach(AHB_MASTER *mas);              
            bool slaveAttach(AHB_SLAVE *sla);
            bool nodeAttach(AHB_NODE *node);


            
            /**
             * Detach a bus-object from this controller
             * @param busId ID of the bus-object as given by busAttach
             * @return true if successful
             * @see busAttach()
             */
            bool busDetach(signed char busId);

            /**
             * First EEPROM address to use
             */
            unsigned int cfsAddrStart=0;
            
            /**
             * Last EEPROM address to use
             */
            unsigned int cfsAddrStop=511;
            
            /**
             * Find next free config block with >=size bytes
             * @param lenth of requested block in bytes
             * @param id configuration ID of requesting module
             * @return unsigned int address, 0 for error
             */
            unsigned int cfgFindFreeblock(byte bytes, byte id);
            
            /**
             * Send a message to the bus
             * @param target target address between 0 and 255             
             * @param len number of data bytes to send
             * @param data array of data bytes to send
             * @return number of errors
             * @see busAttach()
             */
            byte ahbSend(uint8_t target, byte len, byte data[8]); //3 Моя логика

            /**
             * Send a message to the bus
             * @param type 1 bit message type (AHB_PKGTYPE_*)
             * @param target target address between 0 and 255
             * @param source source address between 1-255             
             * @param len number of data bytes to send
             * @param data array of data bytes to send
             * @return number of errors
             * @see busAttach()
             */
            byte ahbSend(uint8_t type, uint8_t target, byte len, byte data[8]); //4 Моя логика

            /**
             * Send a message to the bus
             * @param type 1 bit message type (AHB_PKGTYPE_*)
             * @param cmd
             * @param target target address between 0 and 255
             * @param source source address between 1-255             
             * @param len number of data bytes to send
             * @param data array of data bytes to send
             * @return number of errors
             * @see busAttach()
             */
            byte ahbSend(uint8_t type, uint8_t cmd, uint8_t target, byte len, byte data[8]); //5 Моя логика

            /**
             * Send a message to the bus
             * @param type 1 bit message type (AHB_PKGTYPE_*)
             * @param cmd
             * @param target target address between 0 and 255
             * @param port port address between 0x0 and 0xF, Unicast only ??????            
             * @param len number of data bytes to send
             * @param data array of data bytes to send
             * @return number of errors
             * @see busAttach()
             */
            byte ahbSend(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, byte len, byte data[8]); //6 Моя логика

            /**
             * Send a message to the bus
             * @param type 1 bit message type (AHB_PKGTYPE_*)
             * @param cmd
             * @param target target address between 0 and 255
             * @param port port address between 0x0 and 0xF, Unicast only ??????
             * @param source source address between 1-255             
             * @param len number of data bytes to send
             * @param data array of data bytes to send
             * @return number of errors
             * @see busAttach()
             */
            byte ahbSend(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source, byte len, byte data[8]); //7 Моя логика

            
            /**
             * Receive a message from the bus
             *
             * This polls all attached communication interfaces for new messages
             * The first received message will be passed to &pkg, if no messages
             * are available the function will return false. If a message is
             * received you are adviced to call the function again until no
             * more messages are availabe on any interface. All received packets
             * will be redistributed to all other attached interfaces
             *
             * @param pkg ahbPacket-Reference to store received packet
             * @return true if a message was received
             */
            bool ahbReceive(ahbPacket &pkg);

            /**
             * Receive a message from the bus
             *
             * Same as above but doesn't return anything. Useful if you just use hooks
             *
             * @see ahbReceive(ahbPacket &pkg)
             */
            void ahbReceive(void);

            /**
             * Receive a message from the bus
             *
             * This polls all attached communication interfaces for new messages
             * The first received message will be passed to &pkg, if no messages
             * are available the function will return false. If a message is
             * received you are adviced to call the function again until no
             * more messages are availabe on any interface. All received packets
             * will be redistributed to all other attached interfaces if routing
             * is true
             *
             * @param pkg ahbPacket-Reference to store received packet
             * @param routing If false packet will not be redistributed
             * @return true if a message was received
             */
            bool ahbReceive(ahbPacket &pkg, bool routing);
            
            
            bool ahbReceiveRouting(ahbPacket &pkg, uint8_t packet_state, signed char busId_route);
            

            /**
             * Process incoming packet
             * @param pkg Packet struct
             */
            void ahbProcess(ahbPacket &pkg);

            /**
             * Attach a hook to a set of metadata
             *
             * If a received packet matches the type, target and port the supplied function
             * will be called. This allows for custom actors. Note: There is no detach (yet?).
             *
             * @param type 2 bit message type (AHB_PKGTYPE_*), 0xFF = everything
             * @param target target address between 0x0001 and 0x07FF/0xFFFF, 0x0 = everything
             * @param port port address between 0x00 and 0x1F, Unicast only, -1 = everything
             * @param First data byte (usually AHB_CMD_*), 0xFF = everything
             * @param function to call when matched
             * @return true if successfully added
             */
            bool hookAttach(byte type, unsigned int target, char port, byte firstByte, void (*function)(ahbPacket&));

            /**
             * Attach a module to this controller
             *
             * Attached modules will get initialized and later called for loop and received Packets
             *
             * @param AHB_IO based module
             * @return true if successfully added
             */
            bool hookAttachModule(AHB_IO *module);

            /**
             * Detach a module from this controller
             *
             * @param AHB_IO based module-ID
             * @return true if successfully added
             */
            bool hookDetachModule(byte id);

            /**
             * Main processing loop
             *
             * Receives and routes packets, checks inputs, etc
             *
             * @return ahbPacket last received packet
             */
            ahbPacket loop(void);
            
            

            /**
            Добавление сообщения в TX очередь сообщений
            Вызов аналогичен ahbSend
            */
            bool ahbAddSendMsgQueue(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source, byte len, byte data[8], signed char busId); 
            
            /**
            Отправка сообщений из TX очереди сообщений
            */
            byte ahbSendMsgQueue();
            
            /**
            Получение сообщений в RX очередь сообщений
            */           
            bool ahbAddRcvMsgQueue(ahbPacket &pkg); 
            
            /**
            Опрос узлов сети PING
            */  
            void ahbNodeGuard(uint8_t bus_Type);
 
            /**
            Отправка своего состояния (Master выдает аппаратное время, Node - UpTime)
            */  
            void ahbHeartbeat(uint8_t bus_Type);
            
            /**
            Печать ошибок CAN шины, занесение в таблицу Up time для отправки
            */              
            void ahbCAN_pintError(signed char busId);
            
            /**
            Вывод состояния ответов узлов сети на PING
            */              
            void ahbNodeGuard_print(void);

            /**
            Прием ответов на PING -> PONG и заполнение массива NodeGuard_OK[][]
            */              
            void ahbRxProcessingNode(ahbPacket &pkg);
            
            /**
            Таймер?????
            */
            //void ahbTimers();
};

#endif /* AHB__H */
