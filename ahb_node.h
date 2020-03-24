#ifndef AHB_NODE_H
#define AHB_NODE_H

    #include "ahb.h"
    #include "ahb_proto.h"
    #ifdef debug
      #include <avr/pgmspace.h>
    #endif
    #include "Adafruit_BME280.h"
    
class AHB;
    
class AHB_NODE { 
private:
            AHB *_ahb;
public:            
            Adafruit_BME280 bme;
            
            //Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
            //Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
            //Adafruit_Sensor *bme_humidity = bme.getHumiditySensor(); 
            
            #define SEALEVELPRESSURE_HPA (1013.25)
            
            AHB_NODE(uint8_t id); 
            
            byte begin(void);
            
            void bme280_print(void);
            
            void WTD_node(unsigned long millis_);
            
            unsigned long prevTimeHeartbeatNode = 0; //для таймера Heartbeat
            
            uint8_t heartbeat_count = 10; //количество циклов не приема Heartbeat
            
            bool nodeBusAttach(AHB *node_bus);

//Конфигурирование из прошивки
//Конфигурирование из EEPROM
//Конфигурирование по сети
//Запись в EEPROM            

//Конфигурирование интерфейсов CAN/232/485/
//Конфигурирование адресов
//Конфигурирование маршрутизации 

//Конфигурирование pin GPIO DIGITAL INPUT bool
//Конфигурирование pin GPIO DIGITAL OUTPUT bool
//Конфигурирование pin GPIO DIGITAL INPUT ШИМ
//Конфигурирование pin GPIO DIGITAL OUTPUT ШИМ
//Конфигурирование pin GPIO ANALOG INPUT 
//Конфигурирование pin GPIO ANALOG OUTPUT 
//Конфигурирование RS-232/485
//Конфигурирование I2C
//Конфигурирование SPI
//Конфигурирование 1 wire

//Запрос параметра
//Передача параметра по запросу
//Передача параметра по таймеру
//Передача команды
//Выполнение команды
//Отчет о выполнении команды
//Выполнение длинной команды
//Отчет о выполнении длинной команды

//Опрос массивов параметров по запросу параметра

//порос датчиков и исполнительных устройств
//Заполнение массивов параметров датчиками и исполнительными устройствами
           
            //const byte node_address = node_1_Net_center_PC;                 //1                   
            //const byte node_address = node_2_Net_center_oraPi1;             //2                    
            //const byte node_address = node_3_Net_center_oraPi2;             //3                    
            //const byte node_address = node_4_Net_center_Due1;               //4                   
            //const byte node_address = node_5_Net_center_Due2;               //5                   
            //const byte node_address = node_6_Hallway_net_center;            //6                    
            //const byte node_address = node_7_Hallway_main;                  //7                    
            //const byte node_address = node_8_Hallway_light;                 //8                    
            //const byte node_address = node_9_Kitchen_net_center             //9                    
            //const byte node_address = node_10_Kitchen_main;                 //10                   
            //const byte node_address = node_11_Kitchen_light;                //11                   
            //const byte node_address = node_12_WC_main;                      //12                  
            //const byte node_address = node_13_WC_waterleak;                 //13                  
            //const byte node_address = node_14_Bathroom_main;                //14                  
            //const byte node_address = node_15_Boxroom_main;                 //15                 
            //const byte node_address = node_16_Balcony_meteo;                //16                        
            //const byte node_address = node_17_Loggia_main;                  //17                          
            //const byte node_address = node_18_Loggia_recuperator;           //18                   
            //const byte node_address = node_19_Livingroom_main;              //19                              
            //const byte node_address = node_20_Bedroom_main;                 //20                            
            //const byte node_address = node_21_Cabinet_main;                 //21                       

            // Выбираем адрес мастера (из списка выше)------------------//
            //                                                          //
            byte SendAddrStatus_master = node_4_Net_center_Due1;        //
            //                                                         //
            // Выбираем адрес узла мониторинга (из списка выше)---------//
            //                                                          //
            byte MonitoringAddr = node_1_Net_center_PC;                 //
            //----------------------------------------------------------//
            bool TimerStatus = 0;
            bool sendparameters = 0;

            // настраиваем отладку-------------------------------------------------------------------------------// 
                                                                                                                 //
            #define debug                   //отладка в сериал_монитор. Закоментировать строку, если не нужна    //
            #ifdef debug                                                                                         //
            #define debugStatus             //флаг вывода таблицы статусов узлов CAN сети (для МАСТЕРА)          //
            const bool statusprint =     0; //флаг распечатки принятых/отправленных статусов.                    //
            const bool RXparametrprint = 0; //флаг распечатки принятых параметров.                               //
            const bool TXparametrprint = 0; //флаг распечатки отправленных параметров.                           //
            const bool errors =          0; //флаг распечатки ошибок MCP2515.                                    //                                                                                //
            #endif                                                                                               //
            //---------------------------------------------------------------------------------------------------//

// ЗАДАЁМ СПИСОК НАЗВАНИЙ (АДРЕСОВ) ВСЕХ УЗЛОВ, ПАРАМЕТРОВ И ДЕВАЙСОВ СИСТЕМЫ  SmartHomeCAN
            
            //ЗДЕСЬ ЗАДАЁМ СПИСОК НАЗВАНИЙ (АДРЕСОВ) ВСЕХ ВОЗМОЖНЫХ УЗЛОВ, ИСПОЛЬЗУЕМЫХ В СИСТЕМЕ  SmartHomeCAN
            enum Node_addr_enum{
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
              NODS_QUANTITY                          //22 size    
            };   
            
            // ЗДЕСЬ ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ НАЗВАНИЯ (АДРЕСА) ПАРАМЕТРОВ, ПРИМЕНЯЕМЫХ В УМНОМ ДОМЕ
            enum Parameter_addr_enum{
              null_param,                     //0 
              air_t                       ,   //1 //датчик температуры
              air_h                       ,   //2 //датчик влажности
              air_p                       ,   //3 //датчик давления
              mot_pir                     ,   //4 // 
              moti_mw                     ,   //5  //датчик движения
              vibrat                      ,   //6 //датчик вибрации
              noise                       ,   //7   //датчик шума
              Illum                       ,   //8 //датчик освещенности
              gas_co                      ,   //9  //датчик СО
              gas_co2                     ,   //10 //датчик СО2
              gas_met                     ,   //11 //датчик метана
              smoke                       ,   //12  //датчик курения
              fire                        ,   //13  //датчик Огня
              body_temp                   ,   //14  //Температура тела
              jal_stat                    ,   //15  //Концевик Жалюзи
              steam                       ,   //16 //Датчик Пара
              microwave                   ,   //17 //Излучение микроволновки
              fridge_temp                 ,   //18 //Холодильник
              freezer_temp                ,   //19 //Морозилка
              oven_temp                   ,   //20 //Духовка
              elec_board                  ,   //21 //Электрощит концевик
              door_lock                   ,   //22 //Замок двери
              door_close                  ,   //23 //Концевик двери
              mot_door1                   ,   //24 //Датчик прохода 1
              mot_door2                   ,   //25 //Датчик прохода 2
              wind_close                  ,   //26 //Закрыто
              water_hot_temp              ,   //27 //Температура горячей воды
              water_cold_temp             ,   //28 //Температура холодной воды
              water_hot_pres              ,   //29 //Давление горячей воды
              water_cold_pres             ,   //30 //Давление холодной воды
              leakage                     ,   //31 //Протечка
              aquar_lamp                  ,   //32 //Лампа аквариум
              aquar_temp                  ,   //33 //Температура аквариум
              aquar_pump                  ,   //34 //Насос аквариум
              aquar_clearing              ,   //35 //Очистка аквариум
              cupboard_door               ,   //36 //Дверь шкафа
              safe_door                   ,   //37 //Дверь сейфа
              balcony_door                ,   //38 //Дверь балкона
              loggia_door                 ,   //39 //Дверь лоджии
              radiator_temp_S             ,   //40 //Температура батареи
              lamp_ceiling                ,   //41 //Лампа на потолке
              lamp_wall                   ,   //42 //Ланпа настенная
              lamp_Illum                  ,   //43 //Лампа подсветки
              lamp_accident               ,   //44 //Лампа аварийная
              earth_humidity              ,   //45 //Влажность земли
              counter_gas_2               ,   //46 //Счетчик газа
              counter_gas_2_SB2           ,   //47                 сервисный байт
              counter_water_hot_2         ,   //48 //Счетчик горячей воды
              counter_water_hot_2_SB2     ,   //49                 сервисный байт
              counter_water_cold_2        ,   //50 //Счетчик холодной воды
              counter_water_cold_2_SB2    ,   //51                 сервисный байт      
              counter_elect_4             ,   //52 //Счетчик электричества
              counter_elect_4_SB2         ,   //53                 сервисный байт      
              counter_elect_4_SB3         ,   //54                 сервисный байт      
              counter_elect_4_SB4         ,   //55                 сервисный байт        
              ups_voltage_4F              ,   //56 //напряжение АКБ бесперебойника
              ups_voltage_4F_SB2          ,   //57                 сервисный байт      
              ups_voltage_4F_SB3          ,   //58                 сервисный байт      
              ups_voltage_4F_SB4          ,   //59                 сервисный байт    
              SIZE_PARAM_ARRAY                //60 size  // общее количество парамтеров, применяемых в умном доме
            }; 


            //ЗДЕСЬ ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ ИСПОЛНИТЕЛЬНЫЕ УСТРОЙСТВА, КОТОРЫЕ МОГУТ ПРИСУТСТВОВАТЬ В СИСТЕМЕ УМНЫЙ ДОМ 
            enum Device_addr_enum{
              null_device           ,// 0 
              lamp_ceil_onoff_d     ,// 1 //Лампа на потолке вкл/выкл
              lamp_wall_onoff_d     ,// 2 //Ланпа настенная вкл/выкл
              lamp_Illum_onoff_d    ,// 3 //Лампа подсветки вкл/выкл
              lamp_accident_onoff_d ,// 4 //Лампа аварийная вкл/выкл
              lamp_ceiling_dimm_d   ,// 5 //Лампа на потолке диммируемая
              lamp_wall_dimm_d      ,// 6 //Ланпа настенная диммируемая
              lamp_Illum_dimm_d     ,// 7 //Лампа подсветки диммируемая
              lamp_accid_dimm_d     ,// 8 //Лампа аварийная диммируемая
              fan_d                 ,// 9 
              recup_d               ,// 10
              air_cond_d            ,// 11
              valve_radiator_d      ,// 12
              jalousie_d            ,// 13
              valve_gas_d           ,// 14
              valve_hot_water_d     ,// 15
              valve_cold_water_d    ,// 16
              irda_d                ,// 17
              freshener_d           ,// 18 //Освежитель
              humidifier_d          ,// 19 //Увлажнитель
              door_lock_magnet_d    ,// 20
              door_lock_elmagnet_d  ,// 21
              door_lock_elmech_d    ,// 22
              door_lock_blink_d     ,// 23
              siren_d               ,// 24
              blinker_d             ,// 25
              music_d               ,// 26
              tv_d                  ,// 27
              radio_d               ,// 28
              power_socket_d        ,// 29 //Розетки  
              SIZE_DEVICE_ARRAY        // 30;  // КОЛИЧЕСТВО ИСПОЛНИТЕЛЬНЫХ МЕХАНИЗМОВ
            };

//CAN NETWORK

            #define BIT_29 1
            #define BIT_11 0

            #define EXTENDED 1
            #define STANDART 0

            #define HIGH_PRIORITY 0
            #define LOW_PRIORITY  1

            #define REMOTE_CANFRAME 1
            #define DATA_CANFRAME   0

            #define END_ MULTIFRAME      1
            #define STREAM_ MULTIFRAME   0
            
            // НИЖЕ ТИП ОТЧЕТОВ НА ПОЛУЧЕННЫЕ КОМАНДЫ

            #define COMPLETE           0x01   // команда выполнена успешно 
            #define EXECUTING          0x02   // команда принята, началось выполнение 
            #define FAIL_UNKNOWN       0x03   // ошибка выполнения команды, причины неизвестны
            #define UNKNOWN_COMMAND    0x04   // ошибка выполнения команды, неизвестная команда
            #define UNKNOWN_TYPEDEV    0x05   // ошибка выполнения команды, неизвестный тип устройства, посланный в команде. 
            #define NOT_INCLUDED       0x06   // ошибка выполнения команды, тип устройства, посланный в команде, отсутствует на узле
            #define NOT_SUITABLE       0x07   // ошибка выполнения команды, команда не подходит для данного вида устройства.
            #define NEGATIVE_RESULT    0x08   // ошибка выполнения команды, желаемый результат воздействия на устройство не достигнут (желаемое состояние устройства после выполнения команды не соответствует значению , полученному при контроле состояния устройства)

            // НИЖЕ ПРИЧИНА ОСТЫЛКИ В CAN ПАРАМЕТРА

            #define PERIODICALLY       0x09   // параметр отправлен периодически
            #define CHANGEVALUE        0x0A   // параметр отправлен по факту изменения его значения на узле
            #define ON_REQUEST         0x0B   // параметр отправлен по запросу от другого узла

            // ЕСЛИ ПРИХОДИТ ЗАПРОС НА ПАРАМЕТР, НЕ УКОМПЛЕКТОВАННЫЙ НА УЗЛЕ - ОТЧЁТ БУДЕТ ТАКОЙ:    #define NOT_INCLUDED       0x06

            // НИЖЕ ДЛЯ МАССИВА "ПАРАМЕТРЫ"
            #define PARAMETER_TYPE    0
            #define PARAMETER_SENS    1
            #define TYPE_VAR          2
            #define PARAMETER_VALUE   3
            #define PARAMETER_LAST    4
            #define PERIODIC_CANADDR  5

            #define BYTE_1        1
            #define BYTE_2        2
            #define BYTE_4        4
            #define SERVICE_BYTE  5
            #define BYTE_4_SIGNED 6
            #define BYTE_2_SIGNED 7
            #define BYTE_1_SIGNED 8
            #define BYTE_4_FLOAT  9

            #define NOT_SENT 0xFF

            // НИЖЕ ДЛЯ МАССИВА "ИСПОЛНИТЕЛЬНЫЕ МЕХАНИЗМЫ"

            #define DEVICE_TYPE           0
            #define DEVICE_VALUE          1
            #define DEVICE_PIN            2
            #define DEVICE_STATUS_PIN     3
            #define DEVICE_VID            4


            // НИЖЕ ДЛЯ МАССИВА ТАЙМАУТОВ

            #define TIMEOUTS_NUMBER sizeof(timeoutsArray)/2 
            #define DEV_TYPE 0
            #define _TIMEOUT  1


            // НИЖЕ ВИД УСТРОЙСТВА 

            #define LONG                  |0x80
            #define DIGITALWRITE           1   // булевое устройство, имеющее два состояния вкл и выкл, подключено к цифровому выходу ардуино
            #define DIGITALWRITE_INVERT    2   // булевое устройство, имеющее два состояния вкл и выкл, подключено к цифровому выходу ардуино, у которого инвертирована логика (например, как у релейных модулей)
            #define PWMWRITE               3   // устройство управляемое ШИМ сигналом, подключено к PWM выходу ардуино
            #define PROCENTWRITE           4   // устройство управляемое градацией от 0 до 100%, подключено к ардуино может быть по разному. 
            #define IMPULSE_GND            5   // устройство, подавая на которое импульс LOW,  меняет своё состояние на противоположное
            #define IMPULSE_POWER          6   // устройство, подавая на которое импульс HIGH, меняет своё состояние на противоположное


            // Типы команд (CommandType)

            enum Command_enum{
             RESERVE,              //  0x00   // Резерв
             DIGITAL_REMOTE,       //  0x01   // Управление (вкл/выкл) булевыми устройствами
             DIGITAL_INVERT,       //  0x02   // Инвертирование состояния булевых устройств 
             DIMMER_SETTING,       //  0x03   // Установка процента включения диммируемых устройств (значение в CommandValue)
             DIMMER_TURN_OFF,      //  0x04   // Увеличене  включенности PWM устройств (значение увеличения в CommandValue)
             DIMMER_TURN_ON,       //  0x05   // Уменьшение включенности PWM устройств (значение уменьшения в CommandValue)
             IMPULSE_ON,           //  0x06   // Включение устройств, запуск которых осуществляется 1 сек импульсом GND 
             IMPULSE_OFF,          //  0x07   // Включение устройств, запуск которых осуществляется 1 сек импульсом GND 
             IMPULSE_INVERT,       //  0x08   // Инвертирование состояния устройств, запуск которых осуществляется 1 сек импульсом GND 
             PWM_SETTING,          //  0x09   // Установка величины включения диммируемых устройств управляемых PWM (значение в CommandValue)
             PWM_TURN_OFF,         //  0x0A   // Увеличене  включенности PWM устройств (значение увеличения в CommandValue)
             PWM_TURN_ON,          //  0x0B   // Уменьшение включенности PWM устройств (значение уменьшения в CommandValue)
             PARAMETER_WRITE,      //  0x0C    // Изменение значения выбранного параметра (массив parameter)
             Command_enum_SIZE     // размер списка команд 
            } ;


            // структуры полей данных CAN фреймов (за что отвечает какой байт)

            #define CAN_COUNTER             0  // для всех типов сообщений

            // структура поля данных типа сообщения "COMMAND" и "COMMAND_REPORT"
            #define CAN_SENSOR_NUM          1
            #define CAN_REPORT              2
            #define CAN_CommandType         3
            #define CAN_CommandValue        4
            #define CAN_CommandValue_2      5
            #define CAN_CommandValue_3      6
            #define CAN_CommandValue_4      7

            // структура поля данных типа сообщения "STATUS"

            #define CAN_SEC                 1
            #define CAN_MIN                 2
            #define CAN_HOURS               3
            #define CAN_DAYS                4


            // Типы сообщений в системе SmartHomeCAN
            enum Message_enum{
                NULL_C,             //0
                COMMAND,            //1
                COMMAND_REPORT,     //2
                PARAMETER_REQUEST,  //3 
                PARAMETER,          //4
                STATUS_REQUEST,     //5
                STATUS,             //6
                MULTIFRAME_REQUEST, //7
                MULTIFRAME,         //8
                MULTIFRAME_END,     //9  
                ACCIDENT,           //10
                ACCIDENT_REPORT,    //11
                size_Message_ENUM   //12 size
            } ;   

            //ЗАДАЁМ ВСЕ СЛУЖЕБНЫЕ СИСТЕМНЫЕ ПЕРЕМЕННЫЕ CAN СЕТИ
            // ~~~~~~~~~~~~~~~~~~ниже для организации COMMAND 

            const byte TX_repeatPeriod = 200; // периодичность повторений TX сообщений, мс,  если не приходит  отчет, но более 5 раз. 
            static const byte queueTX_size =     10; // максимальный размер очереди TX сообщений (количество ячеек)
            const byte maxNumberRepeat_TX  = 5; // максимальное количество повторений команды, если не получен отчет о приянтии команды оппонентом

            enum Name_Cells_TX_Queue{
              Command_TX_COUNTER   ,//0   // глобальный счетчик команды
              Command_TX_COUNTER_2 ,//1   // глобальный счетчик команды 
              Command_TX_count     ,//2   // счетчик отправленных комманд БЕЗ получения ответа от получателя для каждой ячейки очереди
              CommandType          ,//3   // тип команды 
              CommandValue         ,//4   // значение (байт1) команды
              DEVTYPE              ,//5   // тип устройства
              flagTimerCommand     ,//6   // флаг таймера ячеек  
              target_Address       ,//7   // адрес получателя (отправителя) команды 
              target_Address_2     ,//8   // адрес получателя (отправителя) команды 
              target_Address_3     ,//9   // адрес получателя (отправителя) команды 
              cellState            ,//10  // состояние занята или нет ячейка очереди
              CommandValue_2       ,//11   // значение (байт2) команды 
              CommandValue_3       ,//12   // значение (байт3) команды
              CommandValue_4       ,//13   // значение (байт4) команды
              SensNumber           ,//14   // номер девайса данного типа девайса (номер датчика данного типа параметра)
              PrioritY             ,//15   // CAN приоритет команды (низкий или высокий)
              queue_size_columns
            };            
            
            #define Command_TX_COUNTER_3 2   // глобальный счетчик команды 

            byte Command_TX_cell  [queueTX_size][queue_size_columns] = {{0}}; // массив ячеек очереди, в которой хранятся команды на отправку
            byte Command_COUNTER = 0;
            uint32_t queueTX_Timer [queueTX_size] = {0};   // таймер задержки между отправкой команд повторно в случае, если не приходит отчет, но не балее 5 раз. 

            static const byte IMPULSEDEVICE_QUANTITY = 5;// количество одновременно управляемых девайсов импульсом
            uint32_t timerImpulse[IMPULSEDEVICE_QUANTITY] = {0};            // для таймера создания импульса управления импульсными девайсами
            byte TimerImpulse_ON[IMPULSEDEVICE_QUANTITY]  = {0};            // для таймера создания импульса управления импульсными девайсами                                                         
            const byte IMPULSE_DURATION = 8;   // длина импульса управления импульсными девайсами *100мс

            // ~~~~~~~~~~~~~~~~~~ниже для организации COMMAND_REPORT

            static const byte bufferEXElongCOM_size =     10; // максимальный размер буфера исполнения долгих команд

            static const byte queuelongCOM_size_columns = 11;
            byte bufferEXElongCOM_cell  [bufferEXElongCOM_size][queuelongCOM_size_columns] = {{0}}; // массив ячеек буфера, в котором хранятся долго исполняемые команды 
            uint32_t bufferEXElongCOM_Timer [bufferEXElongCOM_size] = {0}; // таймер максимального ожидания выполннения долгой команды, по истечение которого нужно полюбому отправлять отчёт инициатору команды.  

            byte countRepeat;
            uint32_t timercountRepeat = 0;
            bool timercountRepeat_ON = 0; 

            #define Sensors(...)  [](void)->size_t{ byte _[] = { __VA_ARGS__}; return sizeof(_)/sizeof(_[0]); }(),[](void)->byte *{ static byte _[] = { __VA_ARGS__}; return _; }()
  
            
//ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ ИСПОЛНИТЕЛЬНЫЕ УСТРОЙСТВА, КОТОРЫЕ МОГУТ ПРИСУТСТВОВАТЬ В СИСТЕМЕ УМНЫЙ ДОМ 

            // ЗАДАЁМ ТАБЛИЦУ (МАССИВ) ИСПОЛНИТЕЛЬНЫХ МЕХАНИЗМОВ (девайсы, которые присутствуют на данном узле) ,т.е. это уже конфигурация - эта информация на каждом узле будет своя
            // названия (типы)девайсов берём из файла name_addresses.h
            #define device_quantity sizeof(device)/5
            
            byte device [4][5] = {
              {255,6,6,6,6}, // этy строку не трогаем
              //     девайс          состояние  пин ардуино, куда  пин ардуино             вид         
              //                     девайса    подключен девайс  считывания статуса     устройства 
              {   lamp_ceil_onoff_d,    0,           5,              14,                PWMWRITE        } ,
              {   valve_gas_d      ,    0,           6,              15,                DIGITALWRITE_INVERT  LONG  },
              {   siren_d          ,    0,           7,              9,                 DIGITALWRITE}
            };

            // возможные виды устройств:
            //      DIGITALWRITE           1    // булевое устройство, имеющее два состояния вкл и выкл, подключено к цифровому выходу ардуино
            //      DIGITALWRITE_INVERT    2    // булевое устройство, имеющее два состояния вкл и выкл, подключено к цифровому выходу ардуино, у которого инвертирована логика (например, как у релейных модулей)
            //      PWMWRITE               3    // устройство управляемое ШИМ сигналом, подключено к PWM выходу ардуино
            //      PROCENTWRITE           4    // устройство управляемое градацией от 0 до 100%, подключено к ардуино может быть по разному. 
            //      IMPULSE_GND            5    // устройство, подавая на которое импульс LOW,  меняет своё состояние на противоположное
            //      IMPULSE_POWER          6    // устройство, подавая на которое импульс HIGH, меняет своё состояние на противоположное
            //      LONG                  |0x80 // устройство, команда на которое выполняется не мгновнно, например 2 и более секунд. Приставку LONG добавляем после вида устройства через пробел 

            //ниже задаём значения таймаутов на выполнение долгих команд для "долгих" устройств, сек
              //тип устр.     время задержки,сек
            const byte timeoutsArray [4][2]= {
              {valve_radiator_d,      60},
              {valve_cold_water_d,    30},
              {valve_hot_water_d,     30}, 
              {valve_gas_d      ,      5}
            };            

// ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ НАЗВАНИЯ (АДРЕСА) ПАРАМЕТРОВ, ПРИМЕНЯЕМЫХ В УМНОМ ДОМЕ
            
            // ЗАДАЁМ ТАБЛИЦУ (МАССИВ) ПАРАМЕТРОВ, КОТОРЫЕ БУДУТ ПРИСУТСТВОВАТЬ НА ДАННОМ УЗЛЕ ,т.е. это уже конфигурация - эта информация на каждом узле будет своя
            // названия (типы) параметров берём из файла name_addresses.h
            #define AUTOSENT_NODES_QUANTITY  2    // тут указываем максимальное количество узлов, на которые автоматически отсылаться один и тот же параметр
            
            static const byte second_dimension = 5 + AUTOSENT_NODES_QUANTITY ;

            #define parameters_quantity sizeof(parameter)/second_dimension
            
            //тип данных это :
               // SIGNED ЭТО ЗНАЧИТ ПЕРЕМЕННАЯ ИМЕЕТ ЗНАК. 
               // SIGNED ЭТО ЗНАЧИТ ПЕРЕМЕННАЯ ИМЕЕТ ЗНАК.    // ЕСЛИ ПЕРЕМЕННАЯ BYTE_2 ТО ЗА НЕЙ ДОЛЖЕН СЛЕДОВАТЬ ОДИН SERVICE_BYTE
               // SIGNED ЭТО ЗНАЧИТ ПЕРЕМЕННАЯ ИМЕЕТ ЗНАК.    // ЕСЛИ ПЕРЕМЕННАЯ BYTE_4 ТО ПОСЛЕ НЕЁ ДОЛЖНЫ СЛЕДОВАТЬ ТРИ SERVICE_BYTE
               // SIGNED ЭТО ЗНАЧИТ ПЕРЕМЕННАЯ ИМЕЕТ ЗНАК.    // ПЕРЕМЕННАЯ FLOAT МОЖЕТ БЫТЬ ВЫБРАНА ТОЛЬКО ДЛЯ BYTE_4 БЕЗ SIGNED И С ПОСЛЕДУЮЩИМИ ТРЕМЯ SERVICE_BYTE

            //возможные варианты типов данных 

            //   BYTE_1                   беззнаковый 1байтный (0...255)
            //   BYTE_2                   беззнаковый 2байтный (0...65535)
            //   BYTE_4                   беззнаковый 4байтный (0...4294967295)
            //   SERVICE_BYTE             сервис байт (добавляется, если тип данных более одного байта)- Соответсвенно для 2 байтных добавляется один сервисбайт,для 4 байтных - три сервисбайта
            //   BYTE_1_SIGNED            знаковый 1байтный (-128...+127)
            //   BYTE_2_SIGNED            знаковый 2байтный (-32768...+23767)
            //   BYTE_4_SIGNED            знаковый 4байтный (-2147483648...2147483647)
            //   BYTE_4_FLOAT             4байтный флоат , с плавающей точкой                   
            byte parameter [2][second_dimension] =  { 
              {255,1,1,4,4,255,255}, // этy строку не трогаем                                                                   
              //       тип             № датчика     тип данных            значение       посл.значение   таргет адрес 1-го узла          таргет адрес 2-го узла 
              //    параметра                                              параметра        параметра     автоматической отправки         автоматической отправки
              {       Illum,              1,         BYTE_1_SIGNED,            0xBB,            0,              broadcast,                NOT_SENT,}, 
            };

            /////////////////////////////СИСТМЕМНЫЕ ПЕРЕМЕННЫЕ/////////////////////////////////////////

            bool Setup = 0;
            byte Parameter_COUNTER = 0;  // глобальный счетчик сообщения параметров
            byte Status_COUNTER = 0;
 
            //~~~~~~~~~~~~~~~~~~ниже для периодической отправки массива параметров 

            uint32_t timerPeriodSendParameters = 0; // для таймера периодической отправки параметров в кан
            const byte interval_sendParam = (1.5*NODS_QUANTITY) + 5;     // интервал периодической отправки в кан всех параметров

            //~~~~~~~~~~~~~~~~~~ниже для организации StatusConrol

            #ifdef type_node_master
              byte Accident_COUNTER = 0;  // глобальный счетчик сообщения параметров
              bool flag_alarmAccident = 0;                // флаг что уже можно оценивать живы ли узлы (включется через 30 сек после старта МК)
              bool alarmAccident_send[NODS_QUANTITY] = {0};   // флаг что сообщение об аварии отправлено. 
              bool StatusNode_OK[NODS_QUANTITY] = {0};      // массив состояние живы или нет узлы
              byte countNoTAnswerNode[NODS_QUANTITY] = {0}; // массив счетчиков неответов от узлов
                                            // номер элемента массива это АДРЕС УЗЛА В СЕТИ!!!
              const byte intervalStatus = 1;              // интервал таймера прибавления счетчика неответов, сек

              #define MAX_NoTAnswerCOUNT 5       //число счетчика неответов, превысив которое, делаем статус узла "Мёртв"
              bool printstatus = 0;
            #endif 

            #if defined (type_node_slave) or defined (type_node_mk) 
              //byte MonitoringAddr      = node_1_Net_center_PC;
              byte SendAddrStatus_master = node_4_Net_center_Due1; //адрес мастера по умолчанию, на который будет периодически отправляться статус узла
              byte SendAddrStatus_slave = node_5_Net_center_Due2;  //адрес слейва по умолчанию, на который будет периодически отправляться статус узла

              bool TimerStatus = 0;
              bool sendparameters = 0;
            #endif

            unsigned long prevtimeStatus = 0; //для таймера статус
            unsigned long curMillis = 0;      //снимок машинного времени

//Для DEBUG NODE
#ifdef debug

            #define TypeDEV 1
            #define TypeMES 2
            #define ADDR    3
            #define PAR     4
            #define COM     5

            #define  BUF_LENGTH 100
            char buf[BUF_LENGTH]; // буффер для работы прогмем, если нужны строки более 100 символов, тут ставим бОльшее значение

            const char stringMES_0[1] PROGMEM = "NULL_C"; 
            const char stringMES_1[1] PROGMEM = "КОМАНДА";                 //= "COMMAND";    
            const char stringMES_2[1] PROGMEM = "ОТЧЁТ О КОМАНДЕ";         //= "COMMAND_REPORT";  
            const char stringMES_3[1] PROGMEM = "ЗАПРОС ПАРАМЕТРА";        //= "PARAMETER_REQUEST";  
            const char stringMES_4[1] PROGMEM = "ПАРАМЕТР";                //= "PARAMETER"; 
            const char stringMES_5[1] PROGMEM = "ЗАПРОС СТАТУСА";          //= "STATUS_REQUEST";    
            const char stringMES_6[1] PROGMEM = "СТАТУС";                  //= "STATUS";   
            const char stringMES_7[1] PROGMEM = "ЗАПРОС МУЛЬТИКАДРА";      //= "MULTIFRAME_REQUEST";
            const char stringMES_8[1] PROGMEM = "МУЛЬТИКАДР";              //= "MULTIFRAME";   
            const char stringMES_9[1] PROGMEM = "КОНЕЦ МУЛЬТИКАДРА";       //= "MULTIFRAME_END";
            const char stringMES_10[1] PROGMEM = "АВАРИЯ";                 //= "ACCIDENT";  
            const char stringMES_11[1] PROGMEM = "АВАРИЯ ПРИНЯТА";         //= "ACCIDENT_REPORT";
 
            const char* const string_tableMES[12] PROGMEM = {stringMES_0, stringMES_1, 
            stringMES_2, stringMES_3, stringMES_4,  stringMES_5, stringMES_6,
            stringMES_7, stringMES_8, stringMES_9, stringMES_10, stringMES_11};

            const char stringADDR_0[1]  PROGMEM = "ШИРОКОВЕЩАТЕЛЬНО           ";
            const char stringADDR_1[1]  PROGMEM = "node_1_Net_center_PC       ";
            const char stringADDR_2[1]  PROGMEM = "node_2_Net_center_oraPi1   ";  
            const char stringADDR_3[1]  PROGMEM = "node_3_Net_center_oraPi2   "; 
            const char stringADDR_4[1]  PROGMEM = "node_4_Net_center_Due1     ";
            const char stringADDR_5[1]  PROGMEM = "node_5_Net_center_Due2     ";    
            const char stringADDR_6[1]  PROGMEM = "node_6_Hallway_net_center  ";
            const char stringADDR_7[1]  PROGMEM = "node_7_Hallway_main        ";  
            const char stringADDR_8[1]  PROGMEM = "node_8_Hallway_light       ";
            const char stringADDR_9[1]  PROGMEM = "node_9_Kitchen_net_center  ";
            const char stringADDR_10[1] PROGMEM = "node_10_Kitchen_main       ";
            const char stringADDR_11[1] PROGMEM = "node_11_Kitchen_light      ";
            const char stringADDR_12[1] PROGMEM = "node_12_WC_main            ";
            const char stringADDR_13[1] PROGMEM = "node_13_WC_waterleak       ";         
            const char stringADDR_14[1] PROGMEM = "node_14_Bathroom_main      ";  
            const char stringADDR_15[1] PROGMEM = "node_15_Boxroom_main       ";
            const char stringADDR_16[1] PROGMEM = "node_16_Balcony_meteo      ";
            const char stringADDR_17[1] PROGMEM = "node_17_Loggia_main        ";
            const char stringADDR_18[1] PROGMEM = "node_18_Loggia_recuperator ";              
            const char stringADDR_19[1] PROGMEM = "node_19_Livingroom_main    ";               
            const char stringADDR_20[1] PROGMEM = "node_20_Bedroom_main       ";                 
            const char stringADDR_21[1] PROGMEM = "node_21_Cabinet_main       ";               


            const char* const string_tableADDR[22] PROGMEM = {stringADDR_0, stringADDR_1, stringADDR_2, stringADDR_3, stringADDR_4,   // здесь массив строк отчетов
            stringADDR_5,  stringADDR_6,  stringADDR_7,  stringADDR_8,  stringADDR_9, stringADDR_10,stringADDR_11,stringADDR_12,stringADDR_13, 
            stringADDR_14, stringADDR_15, stringADDR_16, stringADDR_17, stringADDR_18,stringADDR_19,stringADDR_20,stringADDR_21};

            const char stringPAR_0[1] PROGMEM = "NULL PARAMETER"; //0 
            const char stringPAR_1[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ВОЗДУХА"; //1 air_temperature_censor
            const char stringPAR_2[1] PROGMEM = "ДАТЧИК ВЛАЖНОСТИ ВОЗДУХА"; //2 air_humidity_censor
            const char stringPAR_3[1] PROGMEM = "ДАТЧИК ДАВЛЕНИЯ ВОДУХА"; //3 air_pressure_censor
            const char stringPAR_4[1] PROGMEM = "ДАТЧИК ДВИЖЕНИЯ ПИРОЭЛЕКТРИЧЕСКИЙ (ИК)"; //4 motion_pir_censor
            const char stringPAR_5[1] PROGMEM = "ДАТЧИК ДВИЖЕНИЯ МИКРОВОЛНОВОЙ"; //5  motion_mw_censor
            const char stringPAR_6[1] PROGMEM = "ДАТЧИК ВИБРАЦИИ"; //6 vibration_censor
            const char stringPAR_7[1] PROGMEM = "ДАТЧИК ШУМА"; //7  noise_censor
            const char stringPAR_8[1] PROGMEM = "ДАТЧИК ОСВЕЩЁННОСТИ"; // Illumination_censor
            const char stringPAR_9[1] PROGMEM = "ДАТЧИК ГАЗА CO"; //9  gas_co_censor
            const char stringPAR_10[1] PROGMEM = "ДАТЧИК ГАЗА CO2"; //10 gas_co2_censor
            const char stringPAR_11[1] PROGMEM = "ДАТЧИК МЕТАНА"; //11 gas_metan_censor 
            const char stringPAR_12[1] PROGMEM = "ДАТЧИК КУРЕНИЯ"; //12  smoke_censor
            const char stringPAR_13[1] PROGMEM = "ДАТЧИК ОГНЯ"; //13  fire_censor
            const char stringPAR_14[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ТЕЛА"; //14  body_temperature_censor
            const char stringPAR_15[1] PROGMEM = "ДАТЧИК ОТРКРЫТИЯ ЖАЛЮЗИ"; //15  jalousie_status_censor 
            const char stringPAR_16[1] PROGMEM = "ДАТЧИК ПАРА"; //16 steam_censor
            const char stringPAR_17[1] PROGMEM = "ДАТЧИК ИЗЛУЧЕНИЯ МИКРОВОЛНОВОЙ"; //17 microwave_censor
            const char stringPAR_18[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ХОЛОДИЛЬНИКА"; //18 fridge_temperature_censor
            const char stringPAR_19[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ МОРОЗИЛКА"; // 19 freezer_temperature_censor
            const char stringPAR_20[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ДУХОВКА"; // 20 oven_temperature_censor
            const char stringPAR_21[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ЭЛЕКТРОЩИТА"; // 21 electrical_board_censor
            const char stringPAR_22[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ЗАМКА ДВЕРИ"; // 22 door_lock_censor
            const char stringPAR_23[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ"; // 23 door_close_censor
            const char stringPAR_24[1] PROGMEM = "ДАТЧИК ПРОХОДА 1"; // 24 motion_door1_censor
            const char stringPAR_25[1] PROGMEM = "ДАТЧИК ПРОХОДА 2"; //25 motion_door2_censor
            const char stringPAR_26[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ОКНА"; // 26 window_close_censor
            const char stringPAR_27[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ГОРЯЧЕЙ ВОДЫ"; // 27 water_hot_temperature_censor 
            const char stringPAR_28[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ХОЛОДНОЙ ВОДЫ"; // 28 water_cold_temperature_censor
            const char stringPAR_29[1] PROGMEM = "ДАТЧИК ДАВЛЕНИЯ ГОРЯЧЕЙ ВОДЫ"; // 29 water_hot_pressure_censor
            const char stringPAR_30[1] PROGMEM = "ДАТЧИК ДАВЛЕНИЯ ХОЛОДНОЙ ВОДЫ"; // 3 0water_cold_pressure_censor
            const char stringPAR_31[1] PROGMEM = "ДАТЧИК ПРОТЕЧКИ"; // 31 leakage_censor
            const char stringPAR_32[1] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ЛАМПЫ АКВАРИУМА"; //46 lamp_aquarium_censor
            const char stringPAR_33[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ АКВАРИУМА"; //47 aquarium_temperature_censor
            const char stringPAR_34[1] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ НАСОСА АКВАРИУМА"; //48 aquarium_pump_censor
            const char stringPAR_35[1] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ФИЛЬТРА ОЧИСТКИ АКВАРИУМА"; //49 aquarium_clearing_censor
            const char stringPAR_36[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ ШКАФА";  //36 cupboard_door_censor
            const char stringPAR_37[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ СЕЙФА"; //37 safe_door_censor
            const char stringPAR_38[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ БАЛКОНА"; //38 balcony_door_censor
            const char stringPAR_39[1] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ ЛОДЖИИ"; // 39 loggia_door_censor
            const char stringPAR_40[1] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ БАТАРЕИ"; //40 radiator_temperature_censor
            const char stringPAR_41[1] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ЛАМПЫ НА ПОТОЛКЕ"; //41 lamp_ceiling_censor
            const char stringPAR_42[1] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ НАСТЕННОЙ ЛАМПЫ"; //42 lamp_wall_censor
            const char stringPAR_43[1] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ЛАМПЫ ПОДСВЕТКИ"; //43 lamp_Illumination_censor
            const char stringPAR_44[1] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ АВАРИЙНАЯ ЛАМПЫ"; //44 lamp_accident_censor
            const char stringPAR_45[1] PROGMEM = "ДАТЧИК ВЛАЖНОСТИ ЗЕМЛИ"; //45 earth_humidity_censor
            const char stringPAR_46[1] PROGMEM = "СЧЕТЧИК ГАЗА"; //32 counter_gas_censor
            const char stringPAR_47[1] PROGMEM = "";                    //47 сервис байт
            const char stringPAR_48[1] PROGMEM = "СЧЕТЧИК ГОРЯЧЕЙ ВОДЫ"; // 33 counter_water_hot_censor
            const char stringPAR_49[1] PROGMEM = "";                     //49 сервис байт
            const char stringPAR_50[1] PROGMEM = "СЧЕТЧИК ХОЛОДНОЙ ВОДЫ"; // 34 counter_water_hot_censor
            const char stringPAR_51[1] PROGMEM = "";                    //51 сервис байт
            const char stringPAR_52[1] PROGMEM = "СЧЕТЧИК ЭЛЕКТРОЭНЕРГИИ"; // 35 counter_electricity_censor
            const char stringPAR_53[1] PROGMEM = "";                    //53 сервис байт
            const char stringPAR_54[1] PROGMEM = "";                    //54 сервис байт
            const char stringPAR_55[1] PROGMEM = "";                    //55 сервис байт

            const char stringPAR_56[1] PROGMEM = "НАПРЯЖЕНИЕ АКБ ИБП"; // 35 counter_electricity_censor
            const char stringPAR_57[1] PROGMEM = "";                    //57 сервис байт
            const char stringPAR_58[1] PROGMEM = "";                    //58 сервис байт
            const char stringPAR_59[1] PROGMEM = "";                    //59 сервис байт


            const char* const string_tablePAR[60] PROGMEM = {stringPAR_0, stringPAR_1, stringPAR_2, stringPAR_3, stringPAR_4,   // здесь массив строк отчетов
            stringPAR_5,  stringPAR_6,  stringPAR_7,  stringPAR_8,  stringPAR_9, stringPAR_10,stringPAR_11,stringPAR_12,stringPAR_13, 
            stringPAR_14, stringPAR_15, stringPAR_16, stringPAR_17, stringPAR_18,stringPAR_19,stringPAR_20,stringPAR_21,stringPAR_22, 
            stringPAR_23, stringPAR_24, stringPAR_25, stringPAR_26, stringPAR_27,stringPAR_28,stringPAR_29,stringPAR_30,stringPAR_31,
            stringPAR_32, stringPAR_33, stringPAR_34, stringPAR_35, stringPAR_36,stringPAR_37,stringPAR_38,stringPAR_39,stringPAR_40,
            stringPAR_41, stringPAR_42, stringPAR_43, stringPAR_44, stringPAR_45,stringPAR_46,stringPAR_47,stringPAR_48,stringPAR_49,
            stringPAR_50, stringPAR_51, stringPAR_52, stringPAR_53, stringPAR_54,stringPAR_55,stringPAR_56,stringPAR_57,stringPAR_58,
            stringPAR_59};

            const char stringDEV_0[1] PROGMEM = "NULL DEVICE";
            const char stringDEV_1[1] PROGMEM = "ЛАМПА НА ПОТОЛКЕ БУЛЕВАЯ"; //64 lamp_ceiling_onoff_device
            const char stringDEV_2[1] PROGMEM = "ЛАМПА НАСТЕННАЯ БУЛЕВАЯ"; //65 lamp_wall_onoff_device
            const char stringDEV_3[1] PROGMEM = "ЛАМПА ПОДСВЕТКИ БУЛЕВАЯ"; //66 lamp_Illumination_onoff_PARice
            const char stringDEV_4[1] PROGMEM = "ЛАМПА АВАРИЙНАЯ БУЛЕВАЯ"; //67 lamp_accident_onoff_device
            const char stringDEV_5[1] PROGMEM = "ЛАМПА НА ПОТОЛКЕ ДИММЕР"; //68 lamp_ceiling_dimm_device
            const char stringDEV_6[1] PROGMEM = "ЛАМПА НА СТЕНЕ ДИММЕР"; //69 lamp_wall_dimm_device 
            const char stringDEV_7[1] PROGMEM = "ЛАМПА ПОДСВЕТКИ ДИММЕР"; //70 lamp_Illumination_dimm_device
            const char stringDEV_8[1] PROGMEM = "ЛАМПА АВАРИЙНАЯ ДИММЕР"; //71 lamp_accident_dimm_device
            const char stringDEV_9[1] PROGMEM = "ВЕНТИЛЯТОР"; // 72 fan 
            const char stringDEV_10[1] PROGMEM = "РЕКУПЕРАТОР";//73 recuperator
            const char stringDEV_11[1] PROGMEM = "КОНДИЦИОНЕР"; // 74 air_conditioning 
            const char stringDEV_12[1] PROGMEM = "КЛАПАН РАДИАТОРА ОТОПЛЕНИЯ"; // 75 valve_radiator
            const char stringDEV_13[1] PROGMEM = "ЖАЛЮЗИ"        ; // 76 jalousie
            const char stringDEV_14[1] PROGMEM = "КРАН ГАЗА"      ; // 77 valve_gas 
            const char stringDEV_15[1] PROGMEM = "КРАН ГОРЯЧЕЙ ВОДЫ"  ; // 78 valve_hot_water
            const char stringDEV_16[1] PROGMEM = "КРАН ХОЛОДНОЙ ВОДЫ"   ; //79 valve_cold_water
            const char stringDEV_17[1] PROGMEM = "ИК ПРИЕМОПЕРЕДАТЧИК IRDA"               ; // 80 irda
            const char stringDEV_18[1] PROGMEM = "ОСВЕЖИТЕЛЬ ВОЗДУХА"            ; // 81 freshener
            const char stringDEV_19[1] PROGMEM = "УВЛАЖНИТЕЛЬ ВОЗДУХА"           ; // 82 humidifier 
            const char stringDEV_20[1] PROGMEM = "ЗАМОК ДВЕРИ МАГНИТНЫЙ"      ; // 83 door_lock_magnet
            const char stringDEV_21[1] PROGMEM = "ЗАМОК ДВЕРИ ЭЛЕКТРОМАГНИТНЫЙ"     ; //84 door_lock_electromagnet 
            const char stringDEV_22[1] PROGMEM = "ЗАМОК ДВЕРИ ЭЛЕКТРОМЕХАНИЧЕСКИЙ"     ;  //85 door_lock_electromechanical
            const char stringDEV_23[1] PROGMEM = "ЗАМОК ДВЕРИ BLINK"        ; // 86  door_lock_blink 
            const char stringDEV_24[1] PROGMEM = "СИРЕНА"                  ; // 87 siren
            const char stringDEV_25[1] PROGMEM = "СТРОБОСКОП МИГАЛКА";  // 88 blinker 
            const char stringDEV_26[1] PROGMEM = "МУЗЫКА";  // 89 music
            const char stringDEV_27[1] PROGMEM = "ТВ"; // 90 tv
            const char stringDEV_28[1] PROGMEM = "РАДИО"; // 91 radio 
            const char stringDEV_29[1] PROGMEM = "РОЗЕТКА";  // 92 power_socket 

            const char* const string_tableDEV[30] PROGMEM = {stringDEV_0, stringDEV_1, stringDEV_2, stringDEV_3, stringDEV_4,   // здесь массив строк отчетов
            stringDEV_5,  stringDEV_6,  stringDEV_7,  stringDEV_8,  stringDEV_9, stringDEV_10,stringDEV_11,stringDEV_12,stringDEV_13, 
            stringDEV_14, stringDEV_15, stringDEV_16, stringDEV_17, stringDEV_18,stringDEV_19,stringDEV_20,stringDEV_21,stringDEV_22, 
            stringDEV_23, stringDEV_24, stringDEV_25, stringDEV_26, stringDEV_27,stringDEV_28,stringDEV_29};


            const char stringCOM_0[1]  PROGMEM = "БУЛЕВОЕ ВЫКЛ";
            const char stringCOM_1[1]  PROGMEM = "БУЛЕВОЕ ВКЛ";
            const char stringCOM_2[1]  PROGMEM = "БУЛЕВОЕ ИНВЕРТИРОВАТЬ";  
            const char stringCOM_3[1]  PROGMEM = "ЗНАЧЕНИЕ ДИММЕРА УСТАНОВИТЬ"; 
            const char stringCOM_4[1]  PROGMEM = "ЗНАЧЕНИЕ ДИММЕРА УМЕНЬШИТЬ";
            const char stringCOM_5[1]  PROGMEM = "ЗНАЧЕНИЕ ДИММЕРА УВЕЛИЧИТЬ";    
            const char stringCOM_6[1]  PROGMEM = "ИМПУЛЬСНОЕ ВКЛ";
            const char stringCOM_7[1]  PROGMEM = "ИМПУЛЬСНОЕ ВЫКЛ"; 
            const char stringCOM_8[1]  PROGMEM = "ИМПУЛЬСНОЕ ИНВЕРТИРОВАТЬ"; 
            const char stringCOM_9[1]  PROGMEM = "ЗНАЧЕНИЕ PWM УСТАНОВИТЬ"; 
            const char stringCOM_10[1]  PROGMEM = "ЗНАЧЕНИЕ PWM УМЕНЬШИТЬ";
            const char stringCOM_11[1]  PROGMEM = "ЗНАЧЕНИЕ PWM УВЕЛИЧИТЬ"; 
            const char stringCOM_12[1]  PROGMEM = "УСТАНОВИТЬ ЗНАЧЕНИЕ ПАРАМЕТРА"; 


            const char* const string_tableCOM[13] PROGMEM = {stringCOM_0, stringCOM_1, 
            stringCOM_2, stringCOM_3, stringCOM_4,  stringCOM_5, stringCOM_6,
            stringCOM_7, stringCOM_8, stringCOM_9, stringCOM_10, stringCOM_11, stringCOM_12};            
            
#endif //debug
           
//ФУНКЦИИ ВЫПОЛНЕНИЯ КОМАНД     
            byte CommandExecuting (const byte &dev_Type, const byte &Command_Type, const byte &Command_Value, const byte &cell, const bool &EXE, const bool &AlreadyEx, const byte &Command_Value_2, const byte &Command_Value_3, const byte &Command_Value_4, const byte &Sensor_num);
            void LongCommandExecuting_buffer();
            byte ExecutionResult (byte dev_Type, byte Command_Type, byte Command_Value);

//ФУНКЦИИ RX TX
            void RX_PROC_NODE_COMMAND(ahbPacket &pkg);
            void RX_MSG_TYPE(ahbPacket &pkg);
            void TX_COMMAND(bool priority, uint8_t msg_type, uint8_t target_addr, uint8_t dev_type, bool can_ID_type, bool can_frame_type, uint8_t DLC,  byte data[8]);

//ФУНКЦИИ SEND
            void SendCommand(bool Priority, byte Command_Type, byte Command_Value, byte Command_Value_2, byte Command_Value_3, byte Command_Value_4, byte Target_Address,  byte Device_Type, byte Sensor_numb);
            void test ();
            void SendCommand_queue();
            void StatusTX (byte addr);
            //void SendAccident (byte AlarmAddr);
            void DataStreamSend();
            //void SendRequestParam (bool Priority, byte Target_Address,  byte Param_Type, const size_t siZE, byte *Sensor_numb);         

//ФУНКЦИИ SYSTEM
            void pinmode(); // настройка портов (в зависимости от конфигурации массива device)
            byte timeout(byte dev_Type);
            byte param_addr (byte AdDr, byte SensNum);
            byte device_addr (byte AdDr);
      
            
//Для DEBUG NODE
#ifdef debug
            void PrinT (byte Str_nomer,  byte subject);
            void timeoutsConfigControl ();   // функция контроля настройки таймаутов для долгих команд
            void parametersConfigControl (); // функция контроля настройки параметров
#endif
            
};

class AHB_NODE2 {};
    
#endif /* AHB_NODE_H */