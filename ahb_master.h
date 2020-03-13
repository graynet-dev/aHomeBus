#ifndef AHB_MASTER_H
#define AHB_MASTER_H
    //#include "ahb.h"
    #include "ahb_proto.h"
    
class AHB_MASTER { //: public AHB_MCOMM
public:            
            bool NodeGuard_OK[255][3] = {{ 0 }}; //[2]
            
            AHB_MASTER(uint8_t id); 
             
            // Up Time remote node 
            uint8_t ut_rn[256][7];    
};
    
#endif /* AHB_MASTER_H */