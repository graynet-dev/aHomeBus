#ifndef AHB_SLAVE_H
#define AHB_SLAVE_H
    //#include "ahb.h"
    #include "ahb_proto.h"
    
class AHB_SLAVE { 
public:            
            /*
            Массив со статусами удаленных узлов. Заполняется на основе ответов
            */
            bool NodeGuard_OK[255][3] = {{ 0 }}; //[2]
            
            //Конструктор
            AHB_SLAVE(uint8_t id);
  
            // Up Time remote node
            uint8_t ut_rn[256][7];  
};
    
#endif /* AHB_SLAVE_H */