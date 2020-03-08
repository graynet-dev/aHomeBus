#ifndef AHB_ETH_C
#define AHB_ETH_C

#include "ahb_eth.h"

AHB_ETH::AHB_ETH(uint8_t _cspin) {  
   _interface_eth.init(_cspin);
}

void AHB_ETH::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet){

   _interface_eth.begin(mac_address, local_ip, dns_server, gateway, subnet); // );
   //Serial.println("Ethernet is OK");
   Serial.print("Node IP - ");
   Serial.println(Ethernet.localIP());
   //server.begin();
   Serial.println("Ethernet is OK"); 
}

bool AHB_ETH::EthAttach(AHB *ahb) {
  _ahb = ahb;
  return true;
}


#endif //AHB_ETH_C


/**#include "ahb_eth.h"
AHB_ETH::AHB_ETH (uint8_t _cspin ) { //,uint16_t server_port){ //= 10  = 0
_interface.init(_cspin);
  //if (server_port>0){
  //  EthernetServer server(server_port);
  //  server.begin(server_port);
  //}
}
*/