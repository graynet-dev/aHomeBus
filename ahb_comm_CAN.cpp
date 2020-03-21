/**
  aHomeBus CAN interface

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

  This interface depends on the coryjfowler MCP_CAN_lib library.
    Download: https://github.com/coryjfowler/MCP_CAN_lib

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

#ifndef AHB_CAN__C
#define AHB_CAN__C
    #include "ahb_comm_CAN.h"
    #include "ahb_proto.h"
    #include <mcp_can_coryjfowler.h>
    #include <SPI.h>

    volatile bool ahb_CANIntReq=false;

void ahb_CANInt() {
      ahb_CANIntReq=true;
}

    /*
     * @TODO communication without INT
     */

     
AHB_CAN::AHB_CAN(byte cs, byte speed, byte clockspd) : 
_interface(cs){
        _speed = speed;
        _clockspd = clockspd;
}

AHB_CAN::AHB_CAN(byte cs, byte speed, byte clockspd, byte interrupt, uint8_t *route_table, uint8_t netsize) :
_interface(cs) {

        all_network_address = new uint8_t[netsize];

        for (int i=0; i<netsize; i++){
            this->all_network_address[i]=route_table[i];
        } 

        net_size = netsize; 
        
        pinMode(interrupt, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(interrupt), ahb_CANInt, FALLING);
        _intPin = interrupt;
        _speed = speed;
        _clockspd = clockspd;        
}

void AHB_CAN::SetNodeId(uint8_t nodeId) {}

void AHB_CAN::mcp2515_reset(void){
        Serial.println("mcp2515_reset()");
        _interface.mcp2515_reset();
}

uint8_t AHB_CAN::checkError(void){
  return _interface.checkError();
}
            
uint8_t AHB_CAN::errorCountRX(void){
  return _interface.errorCountRX();
}
            
uint8_t AHB_CAN::errorCountTX(void){
  return _interface.errorCountTX();
}

uint8_t AHB_CAN::getError(void){
  return _interface.getError();
}

byte AHB_CAN::begin() {
        //mcp2515_reset();
        Serial.println("AHB_CAN::begin()");
        delay(100);
        lastErr = _interface.begin(MCP_STDEXT, _speed, _clockspd);
        Serial.println("_interface.begin(MCP_STDEXT, _speed, _clockspd);");
        delay(100);
        if(lastErr == CAN_OK) {_interface.setMode(MCP_NORMAL); Serial.println("_interface.setMode(MCP_NORMAL);");}
 
      
//_interface.init_Mask(0,1,0x000FF000);                       // Init first mask...
//_interface.init_Filt(0,1,0x00000000);   // Init first filter...
//_interface.init_Filt(1,1,0x00013000);                       // Init second filter...
//_interface.init_Mask(1,1,0x000FF000);                       // Init second mask...   
//_interface.init_Filt(2,1,0x00000000);                       // Init 3 filter...
//_interface.init_Filt(3,1,0x00013000);                       // Init 4 filter...
//_interface.init_Filt(4,1,0x00000000);  //Кому 0 узлу бродкаст                  // Init 5 filter...
//_interface.init_Filt(5,1,0x00013000);  //Кому 19 узлу                     // Init 6 filter...

/*
void initCAN() {
    // TODO: make baud rate variable
  while(CAN.begin(CAN_500KBPS)!=0) delay(1000);

  CAN.init_Mask(0,0, 0x007E); // allow two consecutive ids
  CAN.init_Filt(0,0, getNodeId()); // RxPDO and SDO
  CAN.init_Filt(1,0, getNodeId()); // same

  CAN.init_Mask(1,0, 0x07FF);
  CAN.init_Filt(2,0, 0x0000); // NMT
  CAN.init_Filt(3,0, 0x0080); // sync
  CAN.init_Filt(4,0, 0x0100); // time stamp
  CAN.init_Filt(5,0, 0x0000); // dummy
}        
*/
        

        
        #ifdef AHB_DEBUG
        if(lastErr == CAN_OK) {Serial.println(F("CAN OK"));}
        else {
          Serial.print(F("CAN FAIL Error - "));
          Serial.print(lastErr);
        }

        for (int i=0; i<net_size; i++){
          Serial.print(F("Node ")); Serial.println(all_network_address[i]);
        }  
        #endif //AHB_DEBUG
        return lastErr;
}

uint8_t AHB_CAN::address_network_get(uint8_t i){
    return all_network_address[i];   
}

uint8_t AHB_CAN::net_size_get(void){
//Serial.print("Net Size - "); Serial.println(net_size);
return net_size;
}

uint8_t AHB_CAN::busType(){
        return type_CAN;
}



// For convenient frame visualization we use nibble.
//ID structure   x.y.z.aa.bb
//     E                DD             CC              B            AA             
// pocket type     command&ansver  target addres  target port  source address 
//  1^2=2             2^8=256       2^8=256         2^4=16        2^8=256
/* E DD CC B AA
E (0-1) 2 values - 0 multicast & 1 unicast //I do not understand why we also need Broadcast, 
                                             especially since the address zero is Broadcast. 
                                             In principle, the multicast is not needed, 
                                             although it is possible if the multicast flag 
                                             is set during transmission, then the recipient's 
                                             address is read as the multicast group's address, 
                                             if the multicast flag is not set, the recipient's 
                                             address is the node address. It can also be used 
                                             to address neighboring networks. 
DD (0-F/0-F) 256 values from struct - type message //Service message, command and more
CC (0-F/0-F) 256 values from struct - target address
B (0-F) 16 values from struct - port
AA (0-F/0-F) 256 values from struct - source address

*/
uint32_t AHB_CAN::ahbCanAddrAssemble(ahbMeta meta) {
        return ahbCanAddrAssemble(meta.type, 0,meta.target, meta.port, meta.source);
}

uint32_t AHB_CAN::ahbCanAddrAssemble(uint8_t type, uint8_t target, uint8_t source) {
        return ahbCanAddrAssemble(type, 0,target, -1, source);
}

/**
uint32_t AHB_CAN::ahbCanAddrAssemble(uint8_t f_type, uint8_t f_cmd, uint8_t f_target, uint8_t f_port, uint8_t f_source) {
          uint32_t addr = 0x80000000;

          //Проверка на корректность передаваемых данных
          if(f_type == AHB_PKGTYPE_UNICAST) {
            if(f_target > 255) {
              //Serial.println("f_target > 255");
              return 0;
            }
            if(f_port < 0 || f_port > 15) {
              //Serial.println("f_port < 0 || f_port > 15");
              return 0;
            }
          }
          else{
            if(f_target > 255) {
              //Serial.println("f_target > 255");
              return 0;
            }
          }

          if(f_source > 255) {
            //Serial.println("f_source > 255");
            return 0;
          }
          
          addr = (f_type & 0xFFFFFFFF)<<28 | (f_cmd & 0xFFFFFFFF)<<20 | (f_target & 0xFFFFFFFF)<<12 | (f_port & 0xFFFFFFFF)<<8 | f_source ;

          return addr;
}
*/

//uint32_t AHB_CAN::ahbCanAddrAssemble(uint8_t f_type, uint8_t f_cmd, uint8_t f_target, uint8_t f_port, uint8_t f_source) {
uint32_t AHB_CAN::ahbCanAddrAssemble(bool priority, uint8_t dev_type, uint8_t f_target, uint8_t msg_type, uint8_t f_source) {
          uint32_t addr = 0x80000000;

          //Проверка на корректность передаваемых данных
          if(priority == AHB_PKGTYPE_UNICAST) {
            if(f_target > 255) {
              //Serial.println("f_target > 255");
              return 0;
            }
            if(msg_type < 0 || msg_type > 15) {
              //Serial.println("f_port < 0 || f_port > 15");
              return 0;
            }
          }
          else{
            if(f_target > 255) {
              //Serial.println("f_target > 255");
              return 0;
            }
          }

          if(f_source > 255) {
            //Serial.println("f_source > 255");
            return 0;
          }
          
          //addr = (f_type & 0xFFFFFFFF)<<28 | (f_cmd & 0xFFFFFFFF)<<20 | (f_target & 0xFFFFFFFF)<<12 | (f_port & 0xFFFFFFFF)<<8 | f_source ;
          //                                   раньше  port                                                                             раньше cmd
          addr = (priority & 0xFFFFFFFF)<<28 | (msg_type & 0xFFFFFFFF)<<24 | (f_source & 0xFFFFFFFF)<<16 | (f_target & 0xFFFFFFFF)<<8 | dev_type ;           
          //addr = (priority & 0xFFFFFFFF)<<28 | (msg_type & 0xFFFFFFFF)<<24 | (node_address & 0xFFFFFFFF)<<16 | (target_addr & 0xFFFFFFFF)<<8 | dev_type ; 
          //TX(bool priority, uint8_t msg_type, uint8_t target_addr, uint8_t dev_type, bool can_ID_type, bool can_frame_type, uint8_t DLC,  byte data[8])
          return addr;
}

bool AHB_CAN::ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]) {
        uint32_t addr = ahbCanAddrAssemble(type, cmd, target, port, source);
        if(addr == 0){ 
          //Serial.println(F("TX Message send addr=0"));
          return false;
        }
        
        lastErr = _interface.sendMsgBuf(addr, 1, len, data);
        if(lastErr != CAN_OK) {
          //Serial.println("CAN ahbSend lastErr = CAN_FAIL");
          return false;
        }
        else{
            //Serial.println("CAN ahbSend lastErr = CAN_OK");
            #ifdef AHB_DEBUG
            //Serial.print(F("TX <--- ADDR - ")); Serial.println(addr, HEX);
              Serial.print(F("TX <---  "));
              Serial.print(F("   "));
              Serial.print(F(" - Type: 0x"));
              Serial.print(type);
              Serial.print(F(" - CMD: 0x"));
              Serial.print(cmd);
              Serial.print(F("  Target: 0x"));
              Serial.print(target);
              Serial.print(F("  Port: 0x"));
              Serial.print(port);
              Serial.print(F("  Source: 0x"));
              Serial.print(source);
              Serial.print(F("  Length: 0x"));
              Serial.print(len);
              for(byte i=0; i<len; i++) {
                Serial.print(F(" 0x"));
                Serial.print(data[i]);
              }
              Serial.println();
            #endif //AHB_DEBUG
        }
        return true;
}

/**
ahbMeta AHB_CAN::ahbCanAddrParse(uint32_t rxId_can) {

        ahbMeta temp;
        temp.target = 0;
        temp.source = 0;
        temp.port = 0;
      
        //length, buf = data byte(s)
        uint8_t  f_cmd;
        bool can_frame_type, can_ID_type, RTR_11BIT;
        can_ID_type    = (rxId_can & 0x80000000)>>31;  //извлекаем тип ID        (0 - 11bit, 1 - 29 bit)
        can_frame_type = (rxId_can & 0x40000000)>>30;  //извлекаем тип СAN FRAME (0 - Data,  1 - Remote)   
        if (can_frame_type && ((rxId_can & 0x20000000)>>29)) RTR_11BIT = 1;
        else RTR_11BIT = 0;                            //если 29 бит единичка, то это RTR 11 бит
        temp.type  = (rxId_can & 0x10000000)>>28; //  извлекаем 1-битный идентификатор запроса-ответа из ID       
        temp.cmd = (rxId_can &   0xFF00000)>>20;  //  извлекаем 4-битный идентификатор сообщения из ID         
        temp.target  = (rxId_can &  0xFF000)>>12; //  извлекаем 8-битный адрес отправителя из ID - от кого
        temp.port = (rxId_can &   0xF00)>>8;      //  извлекаем 8-битный адрес получателя из ID - кому
        temp.source  = (rxId_can &   0xFF);       //  извлекаем 8-битный тип устройства у получателя из ID
        
        #ifdef AHB_DEBUG
          Serial.println(F(" ----------------------------------------------------------")); 
          //Serial.print(F("RX ---> ADDR - ")); Serial.println(rxId_can, HEX);
          Serial.print(F("RX --->  "));Serial.print("   ");
          Serial.print(F(" - Type: 0x"));Serial.print(temp.type);
          Serial.print(F(" - CMD: 0x"));Serial.print(temp.cmd);
          Serial.print(F("  Target: 0x"));Serial.print(temp.target);
          Serial.print(F("  Port: 0x"));Serial.print(temp.port);
          Serial.print(F("  Source: 0x"));Serial.print(temp.source);               
        #endif

        return temp;
}
*/

ahbMeta AHB_CAN::ahbCanAddrParse(uint32_t rxId_can) {

        ahbMeta temp;
        temp.target = 0;
        temp.source = 0;
        temp.port = 0;
      
        //length, buf = data byte(s)
        uint8_t  f_cmd;
        bool can_frame_type, can_ID_type, RTR_11BIT;
        can_ID_type    = (rxId_can & 0x80000000)>>31;  //извлекаем тип ID        (0 - 11bit, 1 - 29 bit)
        can_frame_type = (rxId_can & 0x40000000)>>30;  //извлекаем тип СAN FRAME (0 - Data,  1 - Remote)   
        if (can_frame_type && ((rxId_can & 0x20000000)>>29)) RTR_11BIT = 1;
        else RTR_11BIT = 0;                            //если 29 бит единичка, то это RTR 11 бит
        //OLD //temp.type //temp.cmd //temp.port
        //NEW //priority  //msg_type //dev_type
        temp.priority = (rxId_can & 0x10000000)>>28;     //  извлекаем 1-битный идентификатор запроса-ответа из ID       
        temp.msg_type = (rxId_can &  0xF000000)>>24;     //  извлекаем 4-битный идентификатор сообщения из ID         
        temp.source   = (rxId_can &   0xFF0000)>>16;     //  извлекаем 8-битный адрес отправителя из ID - от кого
        temp.target   = (rxId_can &     0xFF00)>>8;      //  извлекаем 8-битный адрес получателя из ID - кому
        temp.dev_type = (rxId_can &       0xFF);         //  извлекаем 8-битный тип устройства у получателя из ID
        
        if (temp.dev_type > 127) {
          temp.cmd = temp.dev_type;
        }
        else{
          temp.cmd = 0;
        }
        
        if (temp.target==0) {
            temp.type=AHB_PKGTYPE_MULTICAST;
        }
        //port заменить на команды 230-255 (25 функций)
        temp.port = 0;
        
        #ifdef AHB_DEBUG
          Serial.println(F(" ----------------------------------------------------------")); 
          //Serial.print(F("RX ---> ADDR - ")); Serial.println(rxId_can, HEX);
          Serial.print(F("RX --->  "));Serial.print("   ");
          Serial.print(F(" - Type: 0x"));Serial.print(temp.type);
          Serial.print(F(" - CMD: 0x"));Serial.print(temp.cmd);
          Serial.print(F("  Target: 0x"));Serial.print(temp.target);
          Serial.print(F("  Port: 0x"));Serial.print(temp.port);
          Serial.print(F("  Source: 0x"));Serial.print(temp.source); 
          Serial.print(F("  Priority: 0x"));Serial.print(temp.priority); 
          Serial.print(F("  Msg_type: 0x"));Serial.print(temp.msg_type); 
          Serial.print(F("  Dev_type: 0x"));Serial.println(temp.dev_type); 
        #endif

        return temp;
}

bool AHB_CAN::ahbReceive_V(ahbPacket &pkg) {
        uint32_t rxId;
        byte len = 0;
        byte rxBuf[8];

        //if(_interface.checkReceive() != CAN_MSGAVAIL) return false; //if not use interrapt
        if (!ahb_CANIntReq) return false; //if use interrapt

        byte state = _interface.readMsgBuf(&rxId, &len, rxBuf); 

        if(state != CAN_OK) return false;

        pkg.meta = ahbCanAddrParse(rxId);
       
        pkg.len = len;

        #ifdef AHB_DEBUG  
        Serial.print(F("  Length: 0x"));
        Serial.print(len);
        #endif //AHB_DEBUG 
        
        for(byte i=0; i<len; i++){ 
          pkg.data[i] = rxBuf[i];
          #ifdef AHB_DEBUG  
          Serial.print(F(" 0x"));
          Serial.print(pkg.data[i]);
          //Serial.print(F(" | "));
         #endif //AHB_DEBUG 
        }
          
        #ifdef AHB_DEBUG 
          Serial.println();  
        #endif //AHB_DEBUG
        
        return true;
}


#endif /* AHB_CAN__C */
