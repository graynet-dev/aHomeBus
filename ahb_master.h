#ifndef AHB_MASTER_H
#define AHB_MASTER_H
    //#include "ahb.h"
    #include "ahb_proto.h"
    
class AHB_MASTER { 
public:            
            /*
            Массив со статусами удаленных узлов. Заполняется на основе ответов
            */
            bool NodeGuard_OK[255][3] = {{ 0 }}; //[2]
            
            //Конструктор
            AHB_MASTER(uint8_t id); 
             
            // Up Time remote node 
            uint8_t ut_rn[256][7];    
};
    
#endif /* AHB_MASTER_H */