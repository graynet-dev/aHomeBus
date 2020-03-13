#ifndef AHB_NODE_H
#define AHB_NODE_H

    //#include "ahb.h"
    #include "ahb_proto.h"
    
class AHB_NODE { 

public:            
            
            AHB_NODE(uint8_t id); 
            
            void WTD_node(unsigned long millis_);
            
            unsigned long prevTimeHeartbeatNode = 0; //для таймера Heartbeat
            
            uint8_t heartbeat_count = 10; //количество циклов не приема Heartbeat
  
};

class AHB_NODE2 {};
    
#endif /* AHB_NODE_H */