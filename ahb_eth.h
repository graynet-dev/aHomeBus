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
#ifndef AHB_ETH_H
#define AHB_ETH_H
/**
  #include <Ethernet2.h>
  #include <EthernetUdp2.h>

  #define ETH_SS 10
  #define SD_SS 4

class AHB_ETH : public EthernetClass{
        private:
        
        
        public:
        EthernetClass _interface;
        //EthernetServer server;
        EthernetClient client;
        AHB_ETH(uint8_t _cspin = 10);//, uint16_t server_port = 0);

//   int begin(uint8_t *mac_address);
//  void begin(uint8_t *mac_address, IPAddress local_ip);
//  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server);
//  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway);
//  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);
};
*/
#endif
