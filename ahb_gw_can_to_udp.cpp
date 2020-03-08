/**
  aHomeBus CAN to UDP gateway

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

  This interface depends on the  Modbus-Master-Slave-for-Arduino library.
    Download: https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino

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

#ifndef AHB_GW_CAN_TO_UDP_C
#define AHB_GW_CAN_TO_UDP_C

    #include "ahb_gw_can_to_udp.h"
    



AHB_GW_CAN_TO_UDP::AHB_GW_CAN_TO_UDP(uint32_t localport, uint32_t remoteport, IPAddress gw_ip): //, 
_interface(){

 dest_gw_can_to_udp = gw_ip;
 localPort= localport;
 remPort=remoteport;

}



byte AHB_GW_CAN_TO_UDP::begin() {
     _interface.begin(localPort);
  
    Serial.print("GW_CAN_TO_UDP address ");
    Serial.print(dest_gw_can_to_udp);
    Serial.print(" on Remote port ");
    Serial.print(remPort);  
    Serial.print(" on local port ");
    Serial.println(localPort);
 
     lastErr = 0; //Херня!!!! Обработать наличие ошибок. Лезть в класс чужой либы.
    
        return lastErr;
}

uint8_t AHB_GW_CAN_TO_UDP::checkError(void){}
uint8_t AHB_GW_CAN_TO_UDP::errorCountRX(void){}
uint8_t AHB_GW_CAN_TO_UDP::errorCountTX(void){}
uint8_t AHB_GW_CAN_TO_UDP::getError(void){}

uint8_t AHB_GW_CAN_TO_UDP::net_size_get(void){
        return net_size;
}

uint8_t AHB_GW_CAN_TO_UDP::address_network_get(uint8_t i){
        return all_network_address[i];   
}

uint8_t AHB_GW_CAN_TO_UDP::busType(){
        return type_GW_CAN_TO_UDP;
}


bool AHB_GW_CAN_TO_UDP::ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]){
       uint32_t rxId = 0x80000000;
       rxId = (type & 0xFFFFFFFF)<<28 | (cmd & 0xFFFFFFFF)<<20 | (target & 0xFFFFFFFF)<<12 | (port & 0xFFFFFFFF)<<8 | source ;
       sprintf(buffer, "ID: %.8lX  Data: %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n\r",
              rxId, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
       //Serial.print("UDP ");Serial.print(dest_gw_can_to_udp);Serial.print(":");Serial.println(remPort);       
      _interface.beginPacket(dest_gw_can_to_udp, remPort);
      _interface.write(buffer); //
      _interface.endPacket();
      
}

bool AHB_GW_CAN_TO_UDP::ahbReceive_V(ahbPacket &pkg){

    // _interface.read();
    // _interface.read(packetBuffer, MaxSize);


  int packetSize = _interface.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = _interface.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(_interface.remotePort());

    // read the packet into packetBufffer
    _interface.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
}

  
#endif //AHB_GW_CAN_TO_UDP_C
