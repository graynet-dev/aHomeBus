/**
  aHomeBus Modbus TCP interface

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

  This interface depends on the  Modbus-Master-Slave-for-Arduino library.
    Download: https://github.com/goddland16/Modbus-TCP

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

#ifndef AHB_MODBUS_TCP__C
#define AHB_MODBUS_TCP__C

    #include "ahb_comm_MODBUS_RTU.h"
    #include "ahb_proto.h"
    #include "ModbusTCP.h" 
    
AHB_MODBUS_TCP::AHB_MODBUS_TCP(void): //Добавить передачу ModbusDeviceIP
_interface(){

}    

AHB_MODBUS_TCP::AHB_MODBUS_TCP(uint8_t u8MBUnitID): //Добавить передачу ModbusDeviceIP
_interface(u8MBUnitID){

} 

byte AHB_MODBUS_TCP::begin() {
//ModbusTCP node(1); 
//node.setServerIPAddress(ModbusDeviceIP);
//Привязать к ETH
  lastErr = 0; //Херня!!!! Обработать наличие ошибок. Лезть в класс чужой либы.
  return lastErr;
}
    
void AHB_MODBUS_TCP::address_network_set(uint8_t *addr_net , int len_addr_net){

      for (int i=0; i<len_addr_net; i++)
     {
       all_network_address[i]=addr_net[i];
       #ifdef AHB_DEBUG
       Serial.print(F("Address network CAN - "));Serial.println(all_network_address[i]);
       #endif //AHB_DEBUG
     }  
}

uint8_t AHB_MODBUS_TCP::address_network_get(uint8_t i){
     return all_network_address[i];   
}

uint8_t AHB_MODBUS_TCP::busType(){
        return type_MODBUS_TCP;
}

bool AHB_MODBUS_TCP::ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]){

}

bool AHB_MODBUS_TCP::ahbReceive_V(ahbPacket &pkg){

}
    
#endif //AHB_MODBUS_TCP__C