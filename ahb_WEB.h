#ifndef AHB_WEB_H
#define AHB_WEB_H

#include "ahb.h"
#include <SPI.h>  
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <ICMPPing.h>

class AHB_WEB {
        private:
        AHB *_ahb;
         EthernetClass _interface_eth;
         EthernetClient client;
         EthernetServer server = EthernetServer(80);
        
         unsigned long prevTimePingNode = 0;
         unsigned long prevTimePing = 0;
         byte countOnline;
         ICMPPing _ping;
         
         #define TOTAL_NET_DEVICES 12

         //#define PING_REQUEST_TIMEOUT_MS     2500

         char netDevicesNames[TOTAL_NET_DEVICES][8] = {"node_6", "node_7", "node_8", "node_9", "node_10", "node_11", "node_12", "node_13", "node_14", "node_19", "node_20", "node_21"}; // "node_18",
         byte ips[TOTAL_NET_DEVICES][4] = { //{192, 168, 2, 101},
                                  //{192, 168, 2, 102},
                                  //{192, 168, 2, 103},
                                  //{192, 168, 2, 104},
                                  //{192, 168, 2, 105},
                                  {192, 168, 2, 106}, // 
                                  {192, 168, 2, 107},  // 
                                  {192, 168, 2, 108}, // 
                                  {192, 168, 2, 109},  // 
                                  {192, 168, 2, 110}, // 
                                  {192, 168, 2, 111}, // 
                                  {192, 168, 2, 112}, // 
                                  {192, 168, 2, 113}, // 
                                  {192, 168, 2, 114}, // 
                                  //{192, 168, 2, 118}, // 
                                  {192, 168, 2, 119}, //
                                  {192, 168, 2, 120}, // 
                                  {192, 168, 2, 121}};//
        byte online[TOTAL_NET_DEVICES] = {0};
        SOCKET pingSocket = 0;                        
        char pingMessage[64]; // 256 original
        char buffer [256];
        bool lastPingSucceeded = false;
        
        public:
        
        AHB_WEB(uint8_t _cspin);
        
        void Ping_node(void);
        
        void Ping_node_async(uint8_t i);
        
        void someCriticalStuffThatCantWait(void);
        
        void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);
        
        char webAttach(AHB *ahb);
        
        
        void loop_web();
};

#endif