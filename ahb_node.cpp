//#ifndef AHB_NODE_C
//#define AHB_NODE_C

    #include "ahb_node.h"

AHB_NODE::AHB_NODE(uint8_t id){} 

void AHB_NODE::WTD_node(unsigned long millis_){

        if (millis_ - prevTimeHeartbeatNode > 10000) {
          Serial.print(heartbeat_count);Serial.println(F(" - heartbeat_count"));
          //Serial.println(F("+++++++++++++++++++++++++++++++++++++++"));
          heartbeat_count--; 
          prevTimeHeartbeatNode = millis_;
        }
        
        if (heartbeat_count > 10) {
          heartbeat_count=10;
        }
        
        if (heartbeat_count < 1) {
          Serial.println(F("No receive Heartbeat from all, reboot node"));
           while(1){}
        }

}
  
    
//#endif /* AHB_NODE_C */