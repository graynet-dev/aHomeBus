#include <EEPROM.h>
//#include <SerialCommands.h>

typedef enum Node_addr_enum
{
    broadcast,                            //0
    node_1_Net_center_PC,                 //1
    node_2_Net_center_oraPi1,             //2
    node_3_Net_center_oraPi2,             //3 
    node_4_Net_center_Due1,               //4
    node_5_Net_center_Due2,               //5
    node_6_Hallway_net_center,            //6
    node_7_Hallway_main,                  //7
    node_8_Hallway_light,                 //8
    node_9_Kitchen_net_center,            //9
    node_10_Kitchen_main,                  //10
    node_11_Kitchen_light,                 //11
    node_12_WC_main,                       //12
    node_13_WC_waterleak,                  //13
    node_14_Bathroom_main,                 //14
    node_15_Boxroom_main,                  //15
    node_16_Balcony_meteo,                 //16                          
    node_17_Loggia_main,                   //17                             
    node_18_Loggia_recuperator,            //18                             
    node_19_Livingroom_main,               //19                             
    node_20_Bedroom_main,                  //20                             
    node_21_Cabinet_main,                  //21                             
    NODS_NUMBER                            //22 size    
}Node_addr_ENUM; 

const char stringADDR_0[]  PROGMEM = "ШИРОКОВЕЩАТЕЛЬНО           ";
const char stringADDR_1[]  PROGMEM = "node_1_Net_center_PC       ";
const char stringADDR_2[]  PROGMEM = "node_2_Net_center_oraPi1   ";  
const char stringADDR_3[]  PROGMEM = "node_3_Net_center_oraPi2   "; 
const char stringADDR_4[]  PROGMEM = "node_4_Net_center_Due1     ";
const char stringADDR_5[]  PROGMEM = "node_5_Net_center_Due2     ";    
const char stringADDR_6[]  PROGMEM = "node_6_Hallway_net_center  ";
const char stringADDR_7[]  PROGMEM = "node_7_Hallway_main        ";  
const char stringADDR_8[]  PROGMEM = "node_8_Hallway_light       ";
const char stringADDR_9[]  PROGMEM = "node_9_Kitchen_net_center  ";
const char stringADDR_10[] PROGMEM = "node_10_Kitchen_main       ";
const char stringADDR_11[] PROGMEM = "node_11_Kitchen_light      ";
const char stringADDR_12[] PROGMEM = "node_12_WC_main            ";
const char stringADDR_13[] PROGMEM = "node_13_WC_waterleak       ";         
const char stringADDR_14[] PROGMEM = "node_14_Bathroom_main      ";  
const char stringADDR_15[] PROGMEM = "node_15_Boxroom_main       ";
const char stringADDR_16[] PROGMEM = "node_16_Balcony_meteo      ";
const char stringADDR_17[] PROGMEM = "node_17_Loggia_main        ";
const char stringADDR_18[] PROGMEM = "node_18_Loggia_recuperator ";              
const char stringADDR_19[] PROGMEM = "node_19_Livingroom_main    ";               
const char stringADDR_20[] PROGMEM = "node_20_Bedroom_main       ";                 
const char stringADDR_21[] PROGMEM = "node_21_Cabinet_main       ";               


const char* const string_tableADDR[] PROGMEM = {stringADDR_0, stringADDR_1, stringADDR_2, stringADDR_3, stringADDR_4,   // здесь массив строк отчетов
stringADDR_5,  stringADDR_6,  stringADDR_7,  stringADDR_8,  stringADDR_9, stringADDR_10,stringADDR_11,stringADDR_12,stringADDR_13, 
stringADDR_14, stringADDR_15, stringADDR_16, stringADDR_17, stringADDR_18,stringADDR_19,stringADDR_20,stringADDR_21};
