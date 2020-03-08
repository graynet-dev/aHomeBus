#ifndef AHB_SLAVE_H
#define AHB_SLAVE_H
    #include "ahb.h"
    #include "ahb_proto.h"
    
class AHB_SLAVE { 
public:            

            bool NodeGuard_OK[255][3] = {{ 0 }}; //[2]
            
            AHB_SLAVE(uint8_t id);  
          
            uint8_t ut_rn[256][6];  
};
    
#endif /* AHB_SLAVE_H */