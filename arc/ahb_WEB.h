#ifndef AHB_WEB_H
#define AHB_WEB_H

#include "ahb.h"
#include <SPI.h>  
#include <Ethernet2.h>
#include <EthernetUdp2.h>

class AHB_WEB {
        private:
         AHB *_ahb;

         EthernetClient client;
         
         EthernetServer server = EthernetServer(80);
             
         char node_name[22][30] = {"0","node_1_Net_center_PC", "node_2_Net_center_oraPi1","node_3_Net_center_oraPi2","node_4_Net_center_Due1","node_5_Net_center_Due2","node_6_Hallway_net_center","node_7_Hallway_main","node_8_Hallway_light","node_9_Kitchen_net_center","node_10_Kitchen_main","node_11_Kitchen_light","node_12_WC_main","node_13_WC_waterleak","node_14_Bathroom_main","node_15_Boxroom_main","node_16_Balcony_meteo","node_17_Loggia_main","node_18_Loggia_recuoerator","node_19_Livingroom_main","node_20_Bedroom_main","node_21_Cabinet_main"};                  
      
        public:
        
         AHB_WEB(uint8_t _cspin);
        
         void begin(void);
        
         bool webAttach(AHB *ahb);
         
         String HTTP_req;
         
         boolean gflag=false;
         String parm;
         int dout[8]={0,0,0,0,0,0,0,0};
        
         void loop_web();
         
         void loop_web2();
         
         void loop_web3();
         
         void GetSwitchState(EthernetClient cl);
};

#endif