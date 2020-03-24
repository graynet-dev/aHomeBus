//#ifndef AHB_NODE_C
//#define AHB_NODE_C
    
    #include "ahb_node.h"
    #include <Wire.h>
    #include <SPI.h>
    

AHB_NODE::AHB_NODE(uint8_t id){
  //bme = new Adafruit_BME280();
} 

byte AHB_NODE::begin() {
  
  //bme.begin(0x76, &Wire);
  //bme.begin(0x76, &Wire);
  //bme_temp->printSensorDetails();
  //bme_pressure->printSensorDetails();
  //bme_humidity->printSensorDetails();
}

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

void AHB_NODE::bme280_print(void) {
/**
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Humidity = "));
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");
*/
    if (! bme.begin(0x76, &Wire))
    {
    Serial.println("BME ERROR");
    }
    else
    {
    Serial.println("BME OK");
    }
    
    
    
    bme.takeForcedMeasurement();
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(round((bme.readPressure() / 100.0F)* 0.75006375541921));
    Serial.println(" mmHg");

    //Serial.print("Approx. Altitude = ");
    //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
  
}

bool AHB_NODE::nodeBusAttach(AHB *node_bus) {
        _ahb = node_bus;
        //Serial.println(F("nodeBusAttach"));
        #ifdef debug
          timeoutsConfigControl();
          parametersConfigControl();
        #endif //debug
        pinmode();                  // настройка портов (в зависимости от конфигурации массива device)
        return true;
}


byte AHB_NODE::CommandExecuting (const byte &dev_Type, const byte &Command_Type, const byte &Command_Value, const byte &cell, const bool &EXE, const bool &AlreadyEx, const byte &Command_Value_2, const byte &Command_Value_3, const byte &Command_Value_4, const byte &Sensor_num) {
        // ТУТ НЕПОСРЕДСТВЕННО ВЫПОЛНЯЕМ КОМАНДУ
        bool Long=0;
        if (Command_Type!= PARAMETER_WRITE){ // определим долго ли будет девайс выполнять команду или мгновенно
          Long = (device [device_addr(dev_Type)][DEVICE_VID] & B10000000)>>7;  
        }  
        if ((Long && !AlreadyEx) || (!Long && EXE)){
          if (Command_Type !=  PARAMETER_WRITE){
            if (((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE) || ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE_INVERT)){    
              if  (Command_Type == DIGITAL_REMOTE &&  (device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE) digitalWrite (device [device_addr(dev_Type)][DEVICE_PIN],  Command_Value);
              if  (Command_Type == DIGITAL_REMOTE &&  (device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE_INVERT) digitalWrite (device [device_addr(dev_Type)][DEVICE_PIN],  !Command_Value);
              if  (Command_Type == DIGITAL_INVERT) digitalWrite (device [device_addr(dev_Type)][DEVICE_PIN],!digitalRead(device [device_addr(dev_Type)][DEVICE_PIN]));
            }                                                 
            else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == PWMWRITE){
              if (Command_Type == PWM_SETTING)  device [device_addr(dev_Type)][DEVICE_VALUE] = Command_Value; 
              else if (Command_Type == PWM_TURN_ON)  {
                if (device [device_addr(dev_Type)][DEVICE_VALUE] + Command_Value <= 255 ) device [device_addr(dev_Type)][DEVICE_VALUE]+=Command_Value; 
                else device [device_addr(dev_Type)][DEVICE_VALUE] = 255;
                }
              else if (Command_Type == PWM_TURN_OFF) {
                if (device [device_addr(dev_Type)][DEVICE_VALUE] - Command_Value >= 0 )   device [device_addr(dev_Type)][DEVICE_VALUE]-=Command_Value; 
                else device [device_addr(dev_Type)][DEVICE_VALUE] = 0;
              }
              analogWrite (device [device_addr(dev_Type)][DEVICE_PIN],  device [device_addr(dev_Type)][DEVICE_VALUE]);
            }
            else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == PROCENTWRITE){
            }
            
            else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_GND || (device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_POWER){
              byte ok = 0;
              if (Command_Type == IMPULSE_INVERT) ok = 2;
              else if (Command_Type == IMPULSE_ON  && device [device_addr(dev_Type)][DEVICE_VALUE]==0) { ok = 1;  }
              else if (Command_Type == IMPULSE_OFF && device [device_addr(dev_Type)][DEVICE_VALUE]!=0) ok = 1; 
              for (int i = 0 ; i< IMPULSEDEVICE_QUANTITY; i++) { 
                if (TimerImpulse_ON[i]==device [device_addr(dev_Type)][DEVICE_PIN]) ok = 0;
              }
              if (ok == 1){   
                for (int i = 0; i< IMPULSEDEVICE_QUANTITY; i++){
                  if (TimerImpulse_ON[i]==0) {
                    TimerImpulse_ON[i] = device [device_addr(dev_Type)][DEVICE_PIN]; timerImpulse[i] = curMillis; ok = 2; break;
                  } 
                }
                #ifdef debug
                if (ok!=2){ 
                  Serial.println(); 
                  Serial.println (F("Пытаемся управлять неколькими импульсными девайсами одновременно! Превышенно максимальное значение девайсов одновременного управления"));
                }
                #endif   
                if (ok == 2){ 
                  if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_GND)   digitalWrite (device [DEVICE_PIN][dev_Type],  LOW);
                  else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_POWER) digitalWrite (device [DEVICE_PIN][dev_Type],  HIGH);  
                }
              }
            }
          } // НИЖЕ ЕСЛИ КОМАНДА - "УПРАВЛЕНИЕ ИЗМЕНЕНИЕМ ПАРАМЕТРА"  ТО В РАЗРЯДЕ dev-type ЗАГОЛОВКА ID  БУДЕТ ТИП ПАРАМЕТРА, А НЕ ИСПОЛНИТЕЛЬНОГО МЕХАНИЗМА
          else{
            parameter[param_addr(dev_Type, Sensor_num)][PARAMETER_VALUE] = Command_Value;
            if ((parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) != BYTE_1 && (parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) != BYTE_1_SIGNED)  parameter[param_addr(dev_Type, Sensor_num)+1][PARAMETER_VALUE] = Command_Value_2;                         
            if ((parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) == BYTE_4 || (parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) == BYTE_4_SIGNED ||(parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) == BYTE_4_FLOAT){
              parameter[param_addr(dev_Type, Sensor_num)+2][PARAMETER_VALUE] = Command_Value_3; parameter[param_addr(dev_Type, Sensor_num)+3][PARAMETER_VALUE] = Command_Value_4;
            }                                                                         
          }
          #ifdef debug    
          if (!AlreadyEx && Long){
            Serial.println(); Serial.print (F("Началось выполнение долгой команды \" "));
            PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
            PrinT (dev_Type, TypeDEV); Serial.println();
          }
          if (EXE && !Long) {
            Serial.println(); Serial.print (F("Произведено выполенение быстрой команды \" "));
            PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
            if (Command_Type!=PARAMETER_WRITE)   PrinT (dev_Type, TypeDEV);
            else PrinT (dev_Type, PAR);
            Serial.println(); 
          }
          if (!EXE && !Long){
            Serial.println();
            Serial.println (F("Получена команда в течение 1 секунды с глоб.счетчиком как в предыдущей команде. Игнорируем выполнение команды, но отчёт отправляем."));
          }
          #endif  
        }

        if (Long) 
        return EXECUTING;
        else      
        return COMPLETE;                             
}

void AHB_NODE::LongCommandExecuting_buffer(){
        // ===========================Извлечение из буфера долгих команд и ожидание выполнения
        for (int i = 0; i<bufferEXElongCOM_size; i++) {
          if (bufferEXElongCOM_cell[i][cellState]==1)  {                       // если нашли занятую ячейку в очереди        
            if (bufferEXElongCOM_cell[i][flagTimerCommand]==1 && curMillis - bufferEXElongCOM_Timer[i] > ((uint32_t)timeout(bufferEXElongCOM_cell[i][DEVTYPE])*1000)-500) {      //если таймер был включен и кончился отправляем отчёт
              byte DATa[5] = {0};
              DATa[3]=bufferEXElongCOM_cell[i][CommandType];
              DATa[4]=bufferEXElongCOM_cell[i][CommandValue];
              DATa[2]= ExecutionResult ( bufferEXElongCOM_cell[i][DEVTYPE], DATa[3], DATa[4]); 
              for (int j = 0; j<3; j++) {
                if (j==0) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER];
                else if (j==1) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER_2];
                else if (j==2) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER_3];
                if (bufferEXElongCOM_cell[i][queuelongCOM_size_columns-4+j]!=0)  TX_COMMAND(1, COMMAND_REPORT, bufferEXElongCOM_cell[i][queuelongCOM_size_columns-4+j],  bufferEXElongCOM_cell[i][DEVTYPE], EXTENDED,  DATA_CANFRAME, sizeof(DATa), DATa) ; 
              } // отправляем отчёт в CAN    
              #ifdef debug
                Serial.println();        
                Serial.println (F("удалил из буфера отчетов"));
              #endif 
              for (int j = 0; j<queuelongCOM_size_columns; j++)  bufferEXElongCOM_cell[i][j] = 0; //чистим ячейку так как отчет отправлен
            }
          }
        }
}

byte AHB_NODE::ExecutionResult (byte dev_Type, byte Command_Type, byte Command_Value) {
        if (digitalRead (device [device_addr(dev_Type)][DEVICE_VALUE]) == Command_Type) {
          #ifdef debug
            Serial.println(); 
            Serial.print (F("Выполнение долгой команды \" "));
            PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
            PrinT (dev_Type, TypeDEV); Serial.println(F("  завершилось успешно!"));
          #endif   
          return COMPLETE;
        }
        else{
          #ifdef debug
            Serial.println(); 
            Serial.print (F("Таймаут на выполнение долгой команды \" "));
            PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
            PrinT (dev_Type, TypeDEV); Serial.println(F("  истёк! Команда НЕ выполнена! Результат отрицательный!"));
          #endif 
          return NEGATIVE_RESULT; 
        } 
}

void AHB_NODE::RX_PROC_NODE_COMMAND(ahbPacket &pkg){
        //#ifdef debug
        Serial.println("-----------------------------------");
        Serial.print("pkg.meta.priority - ");Serial.println(pkg.meta.priority);
        Serial.print("pkg.meta.msg_type - ");Serial.println(pkg.meta.msg_type);
        Serial.print("pkg.meta.source - ");Serial.println(pkg.meta.source);
        Serial.print("pkg.meta.target - ");Serial.println(pkg.meta.target);
        Serial.print("pkg.meta.dev_type - ");Serial.println(pkg.meta.dev_type);
        Serial.print("pkg.meta.cmd - ");Serial.println(pkg.meta.cmd);
        Serial.print("pkg.meta.type - ");Serial.println(pkg.meta.type);
        Serial.print("pkg.meta.port - ");Serial.println(pkg.meta.port);
        Serial.print("pkg.meta.busType - ");Serial.println(pkg.meta.busType);
        Serial.print("pkg.meta.busId - ");Serial.println(pkg.meta.busId);
        Serial.print("pkg.len - ");Serial.println(sizeof(pkg.data));
        Serial.print("pkg.data[0] - ");Serial.println(pkg.data[0]);
        Serial.print("pkg.data[1] - ");Serial.println(pkg.data[1]);
        Serial.print("pkg.data[2] - ");Serial.println(pkg.data[2]);
        Serial.print("pkg.data[3] - ");Serial.println(pkg.data[3]);
        Serial.print("pkg.data[4] - ");Serial.println(pkg.data[4]);
        Serial.print("pkg.data[5] - ");Serial.println(pkg.data[5]);
        Serial.print("pkg.data[6] - ");Serial.println(pkg.data[6]);
        Serial.print("pkg.data[7] - ");Serial.println(pkg.data[7]);
        Serial.println("-----------------------------------");
        //#endif //debug
        if (pkg.meta.msg_type>0){
          RX_MSG_TYPE(pkg);
        }
}

void AHB_NODE::RX_MSG_TYPE(ahbPacket &pkg){
          //=------------------------------------Начинаем разбор принятого       P A R S I N G ----------------------------------
          switch (pkg.meta.msg_type){    
          
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            case COMMAND:{ //выполняется, когда мне прислали команду от удаленного контроллера
              byte Command_NOActual = 0;
              if ((pkg.data[CAN_CommandType]!=PARAMETER_WRITE && pkg.meta.dev_type>=SIZE_DEVICE_ARRAY) || (pkg.data[CAN_CommandType]==PARAMETER_WRITE && pkg.meta.dev_type>=SIZE_PARAM_ARRAY)) {
                #ifdef debug
                  Serial.println(); 
                  Serial.println (F("Ошибка выполнения команды. Неизвестный тип устройства, посланный в команде!"));
                #endif 
                pkg.data[CAN_REPORT] = UNKNOWN_TYPEDEV ;
                Command_NOActual = 1;
              }
              else if ((pkg.data[CAN_CommandType]!=PARAMETER_WRITE && device_addr(pkg.meta.dev_type)==0xFF) || (pkg.data[CAN_CommandType]==PARAMETER_WRITE && param_addr(pkg.meta.dev_type, pkg.data[CAN_SENSOR_NUM])==0xFF) ) {
                #ifdef debug
                  Serial.println(); 
                  Serial.println (F("Ошибка выполнения команды. Устройство или параметр, которым пытаемся управлять данной командой, не укомплектованы на узле!"));
                #endif 
                pkg.data[CAN_REPORT] = NOT_INCLUDED ;
                Command_NOActual = 1;
              }
              else if ( pkg.data[CAN_CommandType]>=Command_enum_SIZE) {
                #ifdef debug
                  Serial.println(); 
                  Serial.println (F("Ошибка выполнения команды. Неизвестная команда!"));
                #endif 
                pkg.data[CAN_REPORT] = UNKNOWN_COMMAND ;
                Command_NOActual = 1;
              }
              if (Command_NOActual == 0 && pkg.data[CAN_CommandType]!=PARAMETER_WRITE){
                switch (device [device_addr(pkg.meta.dev_type)][DEVICE_VID] & 0xF){
                  case DIGITALWRITE:
                    if (pkg.data[CAN_CommandType]>DIGITAL_INVERT) Command_NOActual = 2; break;
                  case DIGITALWRITE_INVERT:
                    if (pkg.data[CAN_CommandType]>DIGITAL_INVERT) Command_NOActual = 2; break;
                  case PWMWRITE:
                    if (pkg.data[CAN_CommandType]<PWM_SETTING || pkg.data[CAN_CommandType]>PWM_TURN_ON) Command_NOActual = 2; break;
                  case PROCENTWRITE:
                    if (pkg.data[CAN_CommandType]<DIMMER_SETTING || pkg.data[CAN_CommandType]>DIMMER_TURN_ON) Command_NOActual = 2; break;
                  case IMPULSE_GND:
                    if (pkg.data[CAN_CommandType]<IMPULSE_ON || pkg.data[CAN_CommandType]>IMPULSE_INVERT) Command_NOActual = 2; break;
                  case PARAMETER_WRITE:
                  break;  
                }
              }  
              if (Command_NOActual == 2) { 
                #ifdef debug
                  Serial.println(); 
                  Serial.println (F("Ошибка выполнения команды. Команда не подходит для данного вида устройства!"));
                #endif 
                pkg.data[CAN_REPORT] = NOT_SUITABLE ;
              }
              if (Command_NOActual==0){
                bool AlreadyExecuting = 0; 
                byte Cell;
                if ((device [device_addr(pkg.meta.dev_type)][DEVICE_VID] & B10000000)>>7) {  //если вид устройства долгий (LONG) значит это долгая команда
                  //-----------------------------------------Проверка. нет ли уже в исполнении тайкой же команды (даже если глобальный счетчик отличается)
                  for (int i = 0; i<bufferEXElongCOM_size; i++){ 
                    if (bufferEXElongCOM_cell[i][CommandType] == pkg.data[CAN_CommandType] &&
                    bufferEXElongCOM_cell[i][CommandValue] == pkg.data[CAN_CommandValue] &&
                    bufferEXElongCOM_cell[i][DEVTYPE] == pkg.meta.dev_type){
                      AlreadyExecuting = 1;
                      // если команда уже выполняется и прилетела такая же команда, но с другого МК, запишем в очередь отчёт ему:
                      if (bufferEXElongCOM_cell[i][target_Address]!= pkg.meta.source){     
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("В момент выполнения команды получена точно такая же команда, но от ДРУГОГО Узла. Команда продолжает выполняться. Этому узлу отправлен отчёт EXECUTING. "));
                        #endif 
                        if (bufferEXElongCOM_cell[i][target_Address_2]==0) {
                          bufferEXElongCOM_cell[i][target_Address_2] = pkg.meta.source; 
                          bufferEXElongCOM_cell[i][Command_TX_COUNTER_2] = pkg.data[CAN_COUNTER];
                          #ifdef debug
                          Serial.println (F("Также, по завершении выполнения команды, будет отправлен отчёт о результате выполнения!"));
                          #endif   
                        }
                        else if (bufferEXElongCOM_cell[i][target_Address_3]==0) {
                          bufferEXElongCOM_cell[i][target_Address_3] = pkg.meta.source; 
                          bufferEXElongCOM_cell[i][Command_TX_COUNTER_3] = pkg.data[CAN_COUNTER];
                          #ifdef debug
                            Serial.println (F("Также, по завершении выполнения команды, будет отправлен отчёт о результате выполнения!"));
                          #endif   
                        }
                        else  
                        {
                          #ifdef debug
                          Serial.println (F("Не хватает места в буфере. Отчёт о результатах выполнения НЕ будет отправлен этому узлу!"));
                          #endif
                        }
                      }
                      else {
                        #ifdef debug
                          Serial.println(); 
                          Serial.println (F("В момент выполнения команды получена точно такая же команда от ЭТОГО ЖЕ узла. Команда продолжает выполняться. Узлу отправлен отчёт EXECUTING"));
                        #endif
                        if (bufferEXElongCOM_cell[i][Command_TX_COUNTER]!= pkg.data[CAN_COUNTER]){ 
                          if (bufferEXElongCOM_cell[i][target_Address_2]==0) {
                            bufferEXElongCOM_cell[i][target_Address_2] = pkg.meta.source; 
                            bufferEXElongCOM_cell[i][Command_TX_COUNTER_2] = pkg.data[CAN_COUNTER];
                            #ifdef debug
                            Serial.println (F("Счётчик команды НЕ СОВПАДАЕТ с предыдущим. По завершении выполнения команды, на НОВЫЙ счётчик ТАКЖЕ будет отправлен отчёт о результате выполнения команды!"));
                            #endif   
                          }
                          else if (bufferEXElongCOM_cell[i][target_Address_3]==0) {
                            bufferEXElongCOM_cell[i][target_Address_3] = pkg.meta.source; 
                            bufferEXElongCOM_cell[i][Command_TX_COUNTER_3] = pkg.data[CAN_COUNTER];
                            #ifdef debug
                              Serial.println (F("Счётчик команды НЕ СОВПАДАЕТ с предыдущим. По завершении выполнения команды, на НОВЫЙ счётчик ТАКЖЕ будет отправлен отчёт о результате выполнения команды!"));
                            #endif   
                          }
                        }
                        else {
                          #ifdef debug
                            Serial.println (F("Счётчик команды СОВПАДАЕТ с предыдущим. По завершении выполнения команды, будет отправлен отчёт о результате выполнения команды на ЭТОТ один счётчик!"));
                          #endif   
                        }
                      } 
                    }
                  }
                  //------------------------------------------Конец проверки - нет ли уже в исполнении тайкой же команды. 
                  if  (!AlreadyExecuting){         // если такой команды нет в буфере выполнения, то запишем в буфер
                    bool empty_cells = 0; // наличие свободной ячейки в буфере 
                    //-----------------------------------------запись выполнения долгой команды в буфер
                    for (int i = 0; i<bufferEXElongCOM_size; i++) {
                      if (bufferEXElongCOM_cell[i][cellState]==0) {
                        empty_cells = 1;
                        bufferEXElongCOM_cell[i][Command_TX_COUNTER] = pkg.data[CAN_COUNTER]; // запись значения глобального счетчика в ячейку буфера
                        bufferEXElongCOM_cell[i][CommandType] =  pkg.data[CAN_CommandType];    // запись типа команды в ячейку буфера
                        bufferEXElongCOM_cell[i][CommandValue] = pkg.data[CAN_CommandValue];   // запись значения (байт1) команды в ячейку буфера
                        bufferEXElongCOM_cell[i][target_Address]   = pkg.meta.source;       // запись адреса отправителя команды в ячейку буфера
                        bufferEXElongCOM_cell[i][cellState] = 1;                     // запись состояния ячейки  (1 - команда выполняется)
                        bufferEXElongCOM_cell[i][DEVTYPE] = pkg.meta.dev_type;                // запись типа устройства  в ячейку буфера
                        bufferEXElongCOM_Timer[i] = curMillis;                       // нулим таймер
                        bufferEXElongCOM_cell[i][flagTimerCommand] = 1;              // включаем таймер
                        Cell = i;
                        #ifdef debug
                          Serial.println(); 
                          Serial.println (F("записал в буфер отчетов"));
                        #endif 
                        break;
                      }
                    }
                    #ifdef debug //если при постановке в буфер нет места, пожалуемся в отладку
                      if (!empty_cells){ 
                        Serial.println(); 
                        Serial.println (F("Внимание!!! отсутствует свободная ячейка в буфере выполнения долгих команд. Отчёт о выполнении команды не будет доставлен!!!"));
                      }
                    #endif 
                  }
                  pkg.data[CAN_REPORT] = EXECUTING;    // тип отчета будет - EXECUTING 
                }
                bool exe;
                if (timercountRepeat_ON && countRepeat!=pkg.data[CAN_COUNTER] || !timercountRepeat_ON) exe = 1; //если в течение 1,2 сек глобальный счетчик одинаковый, то игнорируем выполнение команды
                else  {pkg.data[CAN_REPORT] = COMPLETE; exe = 0;}
                pkg.data[CAN_REPORT] = CommandExecuting (pkg.meta.dev_type, pkg.data[CAN_CommandType], pkg.data[CAN_CommandValue], Cell, exe, AlreadyExecuting, pkg.data[CAN_CommandValue_2], pkg.data[CAN_CommandValue_3],pkg.data[CAN_CommandValue_4], pkg.data[CAN_SENSOR_NUM]); 
              }
              TX_COMMAND(LOW_PRIORITY, COMMAND_REPORT,        pkg.meta.source,       pkg.meta.dev_type,   EXTENDED,  DATA_CANFRAME     , sizeof(pkg.data),        pkg.data) ; // отправляем отчёт в CAN    
              // (priority, тип сообщения, адрес получателя, тип устройства,  тип ID,   тип FRAME,  длина поля данных кадра, DATa)
              timercountRepeat = curMillis; 
              timercountRepeat_ON = 1; 
              countRepeat = pkg.data[CAN_COUNTER];        // взводим таймер (1,2 сек) определения команд с одинаковым глоб счетчиком)            
            }
            break;
            
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            case PARAMETER_REQUEST:{  //выполняется когда удаленный контроллер у меня запросил параметр датчика или еще чего то
              bool not_includ = 1;
              for (byte p ; p<SIZE_PARAM_ARRAY; p++){
                if (parameter[p][PARAMETER_TYPE]==pkg.meta.dev_type) not_includ = 0;
              }  // выясняем присутствует ли запрашиваемый тип параметра на узле
              if (not_includ) { 
                byte DataP[8]; 
                Parameter_COUNTER++;  
                DataP[2] = NOT_INCLUDED; 
                DataP[0] = Parameter_COUNTER; DataP[1] = 0;
                TX_COMMAND(1,PARAMETER, pkg.meta.source, pkg.meta.dev_type, EXTENDED, DATA_CANFRAME, 3, DataP);
                #ifdef debug 
                  Serial.println();  
                  Serial.println(); 
                  Serial.println(F("Принят запрос на тип параметра, отсутствующего на данном узле!"));
                #endif   
                break;
              }   
              ///// был )>              
              byte sEnsoR;
              byte siZe; 
              byte iter;
              bool fullSens; 
              if (pkg.data[1]==0) { 
                siZe = parameters_quantity; 
                iter=0; 
                fullSens = 1;
              }
              else {
                siZe = sizeof(pkg.data); 
                iter=1; 
                fullSens = 0;
              }
              not_includ=1;
              for (iter ; iter<siZe; iter++){
                byte DlC = 3; byte DataP[8]; bool ok = 0;
                if (fullSens){
                  if (parameter[iter][PARAMETER_TYPE]==pkg.meta.dev_type) {
                    ok = true; sEnsoR = parameter[iter][PARAMETER_SENS];
                  }
                }
                else {
                  ok = true; 
                  sEnsoR = pkg.data[iter]; 
                }            
        
                if (ok){  
                  Parameter_COUNTER++;       
                  if (param_addr(pkg.meta.dev_type, sEnsoR)!=0xFF) {
                    if ((parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR] & 0xF)!= SERVICE_BYTE){
                      if ((parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR] & 0xF)<=BYTE_4)  DlC = (parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR] & 0xF) + 4;
                      else if ((parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_4_SIGNED ||(parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_4_FLOAT) DlC = 8;
                      else if ((parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_2_SIGNED) DlC = 6;              
                      else if ((parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_1_SIGNED) DlC = 5;              
                      DataP[0]= Parameter_COUNTER;   DataP[1]= 0; DataP[2] = ON_REQUEST; DataP[3] = parameter [param_addr(pkg.meta.dev_type,sEnsoR)][TYPE_VAR]; DataP[4] = parameter [param_addr(pkg.meta.dev_type,sEnsoR)][PARAMETER_VALUE];
                      DataP[5] = parameter [param_addr(pkg.meta.dev_type,sEnsoR)+1][PARAMETER_VALUE]; DataP[6] = parameter [param_addr(pkg.meta.dev_type,sEnsoR)+2][PARAMETER_VALUE]; DataP[7] =  parameter [param_addr(pkg.meta.dev_type,sEnsoR)+3][PARAMETER_VALUE];    
                      DataP[1] = sEnsoR;
                    }
                    else   { 
                      DataP[2] = NOT_INCLUDED; 
                      DataP[0] = Parameter_COUNTER; 
                      DataP[1] = sEnsoR;
                      #ifdef debug 
                        Serial.println();  
                        Serial.println(); 
                        Serial.println(F("Принят запрос на сервисный байт!"));
                      #endif             
                    }
                  }
                  else { 
                    DataP[2] = NOT_INCLUDED; 
                    DataP[0] = Parameter_COUNTER; 
                    DataP[1] = sEnsoR;
                    #ifdef debug 
                      Serial.println();  
                      Serial.println(); 
                      Serial.println(F("Принят запрос на № датчика, который отсутствует на данном типе параметра у данного узла!")); 
                    #endif             
                  }
                  TX_COMMAND(LOW_PRIORITY, PARAMETER, pkg.meta.source, pkg.meta.dev_type, EXTENDED, DATA_CANFRAME, DlC, DataP);
                  delay (5);
                } // end if(ok)
              } // end for
            }
            break;
                          
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~             
            case STATUS_REQUEST: { //выполняется когда удаленный контроллер у меня запрашивает состояние
              StatusTX(pkg.meta.source);  
            }
            break;       
              
            // выполняется когда мне отвечают на мой запрос     
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            case COMMAND_REPORT: { //выполняется когда удаленный контроллер отвечает на посланную команду
              for (int i = 0; i<queueTX_size; i++) {
                if ( Command_TX_cell[i][cellState] != 0 ){
                  if (pkg.data[CAN_CommandType]== Command_TX_cell[i][CommandType] && 
                    pkg.data[CAN_CommandValue]== Command_TX_cell[i][CommandValue] && 
                    pkg.meta.source    == Command_TX_cell[i][target_Address]   &&
                    pkg.meta.dev_type    == Command_TX_cell[i][DEVTYPE]          &&
                    pkg.data[CAN_COUNTER]== Command_TX_cell[i][Command_TX_COUNTER]){       //если отчет совпадает с командой в ячейке очереди
                      //----------и статус отчета "команда выполнена" , то вычёркиваем команду из очереди
                      if (pkg.data[CAN_REPORT]==COMPLETE) {   
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("Команда \" "));  
                          PrinT (Command_TX_cell[i][CommandType], COM); 
                          Serial.print (F(" \""));
                          Serial.print (F(" на тип устройства ")); 
                          if (Command_TX_cell[i][CommandType]!=PARAMETER_WRITE) PrinT(pkg.meta.dev_type, TypeDEV);  
                          else PrinT(pkg.meta.dev_type, PAR);  
                          Serial.print (F(" оппонентом ")); PrinT (pkg.meta.source, ADDR); 
                          Serial.println (F(" выполнена успешно !"));
                        #endif 
                        for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0;  
                      } //-----------и статус отчета "команда выполняется", то:    
                      else if (pkg.data[CAN_REPORT]==EXECUTING){                                                                                                                             
                        Command_TX_cell[i][Command_TX_count] = 0;     // сбросим счетчик количества отправления команды повторно, из-за отсутсвтия отчёта
                        queueTX_Timer [i] = curMillis;                // обнулим таймер задержки отправки команды повторно
                        Command_TX_cell[i][flagTimerCommand] = 1;     // включим таймер задержки отправки команды повторно
                        Command_TX_cell[i][cellState] = EXECUTING;    // статус ячейки в очереди на отправку делаем  "команда выполняется"
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("Оппонент ")); 
                          PrinT(pkg.meta.source, ADDR); 
                          Serial.print (F("команду \" ")); 
                          PrinT (Command_TX_cell[i][CommandType], COM);   
                          Serial.print (F(" \""));  
                          Serial.print (F(" на тип устройства ")); 
                          PrinT(pkg.meta.dev_type, TypeDEV); 
                          Serial.println (F(" принял. Происходит выполнение команды!"));
                        #endif 
                        return;
                      } //-----------и статус отчета "Ошибка выполнения команды оппонетом" , то вычёркиваем команду из очереди
                      else if (pkg.data[CAN_REPORT]==FAIL_UNKNOWN) { 
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("Ошибка выполнения команды \" ")); 
                          PrinT (Command_TX_cell[i][CommandType], COM); 
                          Serial.print (F(" \""));
                          Serial.print (F(" оппонентом!  на тип устройства ")); 
                          PrinT(pkg.meta.dev_type, TypeDEV);  
                          Serial.println (F(" Получен отчёт от оппонента * Выполнение команды не завершено по неизвестным причинам! *"));
                        #endif 
                      }  
                      else if (pkg.data[CAN_REPORT]==UNKNOWN_COMMAND)     { 
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("Ошибка выполнения команды \" ")); 
                          PrinT (Command_TX_cell[i][CommandType], COM); 
                          Serial.print (F(" \""));
                          Serial.print (F(" оппонентом!  на тип устройства ")); 
                          PrinT(pkg.meta.dev_type, TypeDEV); 
                          Serial.println (F(" Получен отчёт от оппонента * Неизвестная команда! *"));
                        #endif 
                      }  
                      else if (pkg.data[CAN_REPORT]==UNKNOWN_TYPEDEV)     { 
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("Ошибка выполнения команды \" ")); 
                          PrinT (Command_TX_cell[i][CommandType], COM); 
                          Serial.print (F(" \""));
                          Serial.print (F(" оппонентом!  на тип устройства ")); 
                          PrinT(pkg.meta.dev_type, TypeDEV); 
                          Serial.println (F(" Получен отчёт от оппонента * Неизвестный тип устройства, посланный в команде! *"));
                        #endif 
                      }  
                      else if (pkg.data[CAN_REPORT]==NOT_INCLUDED)     { 
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("Ошибка выполнения команды \" ")); 
                          PrinT (Command_TX_cell[i][CommandType], COM); 
                          Serial.print (F(" \""));
                          Serial.print (F(" оппонентом!  на тип устройства ")); 
                          PrinT(pkg.meta.dev_type, TypeDEV); 
                          Serial.println (F(" Получен отчёт от оппонента * Узел не укомплектован таким устройством! *"));
                        #endif 
                      }  
                      else if (pkg.data[CAN_REPORT]==NOT_SUITABLE)     { 
                        #ifdef debug 
                          Serial.println(); 
                          Serial.print (F("Ошибка выполнения команды \" ")); 
                          PrinT (Command_TX_cell[i][CommandType], COM); 
                          Serial.print (F(" \""));
                          Serial.print (F(" оппонентом!  на тип устройства ")); PrinT(pkg.meta.dev_type, TypeDEV); Serial.println (F(" Получен отчёт от оппонента * Команда не поддерживается видом подключения устройства! *"));
                        #endif 
                      }  
                      else if (pkg.data[CAN_REPORT]==NEGATIVE_RESULT)     { 
                        #ifdef debug
                          Serial.println(); 
                          Serial.print (F("Ошибка выполнения команды \" ")); 
                          PrinT (Command_TX_cell[i][CommandType], COM); 
                          Serial.print (F(" \""));
                          Serial.print (F(" оппонентом!  на тип устройства ")); 
                          PrinT(pkg.meta.dev_type, TypeDEV); 
                          Serial.println (F(" Получен отчёт от оппонента * По контролю устройства определено, что результат выполнения команды не достигнут! *"));
                        #endif 
                      }  
                      if (pkg.data[CAN_REPORT]==COMPLETE || pkg.data[CAN_REPORT] == FAIL_UNKNOWN || pkg.data[CAN_REPORT] == UNKNOWN_COMMAND || 
                      pkg.data[CAN_REPORT] == UNKNOWN_TYPEDEV || pkg.data[CAN_REPORT] == NOT_INCLUDED || pkg.data[CAN_REPORT] == NOT_SUITABLE || pkg.data[CAN_REPORT] == NEGATIVE_RESULT) {
                        for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0; 
                      } //вычеркиваем команду из очереди
                      break;
                    }
                }
              }
            }
            break;
                        
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~             
            case PARAMETER: { //выполняется когда удаленный контроллер мне отвечают на мой запрос параметра датчика        
            // Функция обработки ответа на мой запрос параметра датчика   
              
            }
            break;
                
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~           
            case STATUS: { //выполняется когда удаленный контроллер мне отвечает на мой запрос состояния   
              #ifdef type_node_master
                countNoTAnswerNode[pkg.meta.source] = 0; //если получили ответ от узла, сбрасываем счетчик неответов в ноль  
                StatusNode_OK[pkg.meta.source] = true;   //и возвращаем статус "ЖИВ"
                alarmAccident_send[pkg.meta.source]=0;             //и флаг "авария отправлена" сбрасываем                                          
              #endif 
              //#if defined (type_node_slave) or defined (type_node_mk)    
                //ss = pkg.data[CAN_SEC]; mm = pkg.data[CAN_MIN]; hh = pkg.data[CAN_HOURS]; dd = pkg.data[CAN_DAYS]; 
                if (pkg.meta.source == SendAddrStatus_master) {
                  prevtimeStatus = curMillis; 
                  TimerStatus = 1;
                }
              //#endif
            }           
            break;  
            
            default: 
             // MCP2515_Init ();
            break;  
          }    
}

void AHB_NODE::TX_COMMAND(bool priority, uint8_t msg_type, uint8_t target_addr, uint8_t dev_type, bool can_ID_type, bool can_frame_type, uint8_t DLC,  byte data[8]){
        //Serial.println ("TX COMMAND >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        _ahb->ahbSend(priority, dev_type, target_addr, msg_type, _ahb->_nodeId, sizeof(data), data);
        // вызов TX (priority, тип сообщения, адрес получателя, тип устройства, тип ID, тип FRAME, длина поля данных кадра, data)
        // тип ID     0 - 11 bit;   1 - 29 bit
        // тип FRAME  0 - DATA;     1 - REMOTE FRAME
} 


void AHB_NODE::SendCommand(bool Priority, byte Command_Type, byte Command_Value, byte Command_Value_2, byte Command_Value_3, byte Command_Value_4, byte Target_Address,  byte Device_Type, byte Sensor_numb){
        bool empty_cells = 0; // наличие свободной ячейки в очереди
        Command_COUNTER++;
        //-----------------------------------------запись в  очередь 
        for (int i = 0; i<queueTX_size; i++) {
          if (Command_TX_cell[i][cellState]==0) {
            empty_cells = 1;
            Command_TX_cell[i][Command_TX_COUNTER] = Command_COUNTER; // запись значения глобального счетчика 
            Command_TX_cell[i][CommandType] =        Command_Type;
            Command_TX_cell[i][CommandValue] =       Command_Value;
            Command_TX_cell[i][CommandValue_2] =     Command_Value_2;
            Command_TX_cell[i][CommandValue_3] =     Command_Value_3;
            Command_TX_cell[i][CommandValue_4] =     Command_Value_4;
            Command_TX_cell[i][target_Address] =     Target_Address;
            Command_TX_cell[i][cellState] = 1; 
            Command_TX_cell[i][PrioritY] =           Priority;
            Command_TX_cell[i][DEVTYPE] =            Device_Type; 
            Command_TX_cell[i][SensNumber] =         Sensor_numb; 
            break;
          }
        }
        #ifdef debug //если при постановке в очередь нет места, пожалуемся в отладку
          if (!empty_cells) Serial.println (F("Внимание!!! отсутствует свободная ячейка в очереди на отправку CAN сообщения. Сообщение НЕ отправлено!!!"));
        #endif  
        //------------------------------------------  
}

void AHB_NODE::test (){
        if (Serial.available()>0){
          byte inByte;
          inByte = Serial.read () - '0';
            if      (inByte == 1) SendCommand (1, PWM_TURN_ON,     20, 0,0,0, node_19_Livingroom_main, lamp_wall_onoff_d , 1);
            else if (inByte == 0) SendCommand (1, PWM_TURN_OFF,    20, 0,0,0, node_19_Livingroom_main, lamp_wall_onoff_d , 1);
            else if (inByte == 2) SendCommand (1, DIGITAL_REMOTE,   1, 0,0,0, node_19_Livingroom_main, lamp_ceil_onoff_d , 1);
            else if (inByte == 3) SendCommand (1, DIGITAL_REMOTE,   0, 0,0,0, node_19_Livingroom_main, lamp_ceil_onoff_d , 1), Serial.println("TRI");
            else if (inByte == 4) device [device_addr (lamp_ceiling_dimm_d)][DEVICE_VALUE] = 0;
            else if (inByte == 5) SendCommand (1, PARAMETER_WRITE, 45, 0,0,0, node_19_Livingroom_main, air_t, 1);
            //else if (inByte == 9) MCP2515_Init ();
        }
}

void AHB_NODE::SendCommand_queue(){
        // ======================================Извлечение из очереди отправки сообщений TX
        for (int i = 0; i<queueTX_size; i++) {
          if (Command_TX_cell[i][cellState]==1)  {                       // если нашли занятую ячейку в очереди и статус, что ещё НЕ получили отчёт "команда принята"       
            if (Command_TX_cell[i][flagTimerCommand]==0) {      //если таймер не включен отправляем TX сообщение
              byte DATa[8] = {0};
              DATa[CAN_COUNTER]=Command_TX_cell[i][Command_TX_COUNTER];
              DATa[CAN_CommandType]   = Command_TX_cell[i][CommandType];
              DATa[CAN_CommandValue]  = Command_TX_cell[i][CommandValue];
              DATa[CAN_CommandValue_2]= Command_TX_cell[i][CommandValue_2];
              DATa[CAN_CommandValue_3]= Command_TX_cell[i][CommandValue_3];
              DATa[CAN_CommandValue_4]= Command_TX_cell[i][CommandValue_4];
              DATa[CAN_SENSOR_NUM]    = Command_TX_cell[i][SensNumber];
              TX_COMMAND(Command_TX_cell[i][PrioritY], COMMAND, Command_TX_cell[i][target_Address],  Command_TX_cell[i][DEVTYPE], BIT_29, DATA_CANFRAME, sizeof(DATa), DATa ) ; // отправляем команду в CAN    
              //  вызов TX (priority, тип сообщения, адрес получателя, тип устройства, тип ID, тип FRAME, длина поля данных кадра, DATa)
              Command_TX_cell[i][flagTimerCommand] = 1;     // вклчаем таймер
              queueTX_Timer [i] = curMillis; // обнуляем таймер
              Command_TX_cell[i][Command_TX_count]++; // прибавляем счетчик отправленных сообщений
            } // конец скобки "если таймер не включен отправляем TX сообщение"
            else if (Command_TX_cell[i][flagTimerCommand]==1 && (curMillis - queueTX_Timer[i]) > TX_repeatPeriod){  // если таймер былл включен и мы все ещё не получили отчет
              byte DATa[5] = {0};
              DATa[0]=Command_TX_cell[i][Command_TX_COUNTER];
              DATa[3]=Command_TX_cell[i][CommandType];
              DATa[4]=Command_TX_cell[i][CommandValue];  
              TX_COMMAND(1, COMMAND, Command_TX_cell[i][target_Address], Command_TX_cell[i][DEVTYPE], BIT_29, DATA_CANFRAME, sizeof(DATa), DATa ) ;  // отправляем команду в CAN
              Command_TX_cell[i][Command_TX_count]++;   // прибавляем счетчик отправленных сообщений
              // если через 5 попыток отправки команды так не пришёл отчет, бросаем это дело -  освобождаем ячейку в очереди
              if (Command_TX_cell[i][Command_TX_count]>=maxNumberRepeat_TX) {
                // и пожалуемся в отладчик
                #ifdef debug
                  Serial.println(); 
                  Serial.print (F("Ошибка команды  \" ")); 
                  PrinT (Command_TX_cell[i][CommandType], COM);
                  Serial.print (F(" \"")); 
                  Serial.print (F(" на тип устройства ")); 
                  PrinT(Command_TX_cell[i][DEVTYPE], TypeDEV);
                  Serial.println (F(" Отчёт о принятии команды от оппонента не получен!!!"));
                #endif 
                for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0; // освобождаем ячейку в очереди
              }
              else    queueTX_Timer [i] = curMillis;              // иначе обнуляем таймер
            }   
            //    delay (4); // эта задержка нужна чтобы сообщения очереди не подряд сыпались, но можно и без неё попробовать          
          }
          else if (Command_TX_cell[i][cellState]==0x02)  {                       // если нашли занятую ячейку в очереди и статус, что получили отчёт "команда принята, выполняется"
            if (Command_TX_cell[i][flagTimerCommand]==0) { // если таймаут ожидания выполнения команды кончился то:
              // ТУТ ЧТО-НИБУДЬ ДЕЛАЕМ, ЕСЛИ ОППОНЕНТ КОМАНДУ ПРИНЯЛ, НО ОТЧЁТА ОБ УСПЕЛНОМ ВЫПОЛНЕНИИ КОМАНДЫ ОТ ОППОНЕНТА МЫ ТАК И НЕ ДОЖДАЛИСЬ
              #ifdef debug
                Serial.println(); 
                Serial.print (F("Ошибка выполнения команды \" ")); 
                PrinT (Command_TX_cell[i][CommandType], COM);
                Serial.print (F(" \"")); 
                Serial.print (F(" на тип устройства ")); 
                PrinT(Command_TX_cell[i][DEVTYPE], TypeDEV); 
                Serial.println (F(" Закончился таймаут, отведённый на выполнение команды. Отчёт об успешном выполнении не получен!!! ")); 
              #endif
              for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0;
            }                  
            else if (Command_TX_cell[i][flagTimerCommand]==1 && curMillis - queueTX_Timer[i] > (uint32_t)timeout(Command_TX_cell[i][DEVTYPE])*1000) { // если таймер таймаута был включен и он кончился
              Command_TX_cell[i][flagTimerCommand]=0; // выключаем таймер, значит таймаут кончился 
            }
          }
        }
}


void AHB_NODE::StatusTX (byte addr) {
        byte Data[8] = {0};
        //#ifdef type_node_master
          Data[0] = Status_COUNTER;
        //  Data[1] = ss;
        //  Data[2] = mm;
        //  Data[3] = hh;
        //  Data[4] = dd;
        //#endif 
        TX_COMMAND(1,STATUS,addr,00,EXTENDED, DATA_CANFRAME, sizeof(Data), Data);
        // вызов TX (priority, тип сообщения, адрес получателя, тип устройства, тип ID, тип FRAME, длина поля данных кадра, data)
        Status_COUNTER++;
}

/**
#ifdef  type_node_master
void AHB_NODE::SendAccident (byte AlarmAddr) {
        if (NodeCANpresence [AlarmAddr] == 0 || AlarmAddr == _ahb->_nodeId) return;
        Accident_COUNTER++;
        byte Data[8] = {Accident_COUNTER,0,0,0,0,0,0,0};
        if (flag_alarmAccident && !alarmAccident_send[AlarmAddr]) { 
          TX_COMMAND(1  , ACCIDENT , MonitoringAddr  , AlarmAddr , EXTENDED ,DATA_CANFRAME , 1, Data); 
          alarmAccident_send[AlarmAddr]=1;
        }
}
#endif
*/

void AHB_NODE::DataStreamSend(){
        bool Periodical_timer = 0;  
        // if (curMillis - timerPeriodSendParameters > (uint32_t)interval_sendParam *1500ul) {Periodical_timer = 1; timerPeriodSendParameters = curMillis; }
        if (_ahb->print_ss_hw() == _ahb->_nodeId && !sendparameters) {sendparameters = 1; Periodical_timer = 1;}
        if (_ahb->print_ss_hw() != _ahb->_nodeId) sendparameters = 0;
        
        for (int i = 0; i < parameters_quantity; i++) {
          for (byte p = PERIODIC_CANADDR; p<(PERIODIC_CANADDR+second_dimension-5); p++){
            if ( (parameter [i][TYPE_VAR]& 0xF)!=SERVICE_BYTE && parameter [i][p]!=0xFF){  // если  это не сервисный байт
              byte DLc; byte ok = 0;
              if ((parameter [i][TYPE_VAR] & 0xF)== BYTE_1||(parameter [i][TYPE_VAR] & 0xF)== BYTE_1_SIGNED) {
                if (ok==0) {ok = 1;} DLc = 5;
              }
              else if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_2||(parameter [i][TYPE_VAR]& 0xF) == BYTE_2_SIGNED) {
                if (ok==0) {ok = 2;} 
                DLc = 6;
              }
              else if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_4||(parameter [i][TYPE_VAR]& 0xF) == BYTE_4_SIGNED || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_FLOAT) {
                if (ok==0) {ok = 4;} 
                DLc = 8;
              }
              // ниже если параметр поменял своё значение
              if (parameter [i][PARAMETER_VALUE]!= parameter [i][PARAMETER_LAST] && ok == 1) ok = CHANGEVALUE;          
              else if ((parameter [i][PARAMETER_VALUE]!= parameter [i][PARAMETER_LAST] || parameter [i+1][PARAMETER_VALUE]!= parameter [i+1][PARAMETER_LAST]) && ok == 2) ok = CHANGEVALUE;          
              else if ((parameter [i][PARAMETER_VALUE]!= parameter [i][PARAMETER_LAST] || parameter [i+1][PARAMETER_VALUE]!= parameter [i+1][PARAMETER_LAST] || parameter [i+2][PARAMETER_VALUE]!= parameter [i+2][PARAMETER_LAST]||
              parameter [i+3][PARAMETER_VALUE]!= parameter [i+3][PARAMETER_LAST])&& ok == 4) ok = CHANGEVALUE;                  
              if (Periodical_timer) ok = PERIODICALLY;
              if (ok == PERIODICALLY || ok == CHANGEVALUE){   
                Parameter_COUNTER++;     
                byte Data[8]= {Parameter_COUNTER, parameter [i][PARAMETER_SENS], ok, parameter [i][TYPE_VAR], parameter [i][PARAMETER_VALUE], parameter [i+1][PARAMETER_VALUE], parameter [i+2][PARAMETER_VALUE], parameter [i+3][PARAMETER_VALUE]};          
                TX_COMMAND(1,PARAMETER, parameter[i][p], parameter [i][PARAMETER_TYPE], EXTENDED, DATA_CANFRAME, DLc, Data);
                delay (5);      
              }
            }
          }  
          //ниже  обновляем прошлое значение параметра
          parameter [i][PARAMETER_LAST]= parameter [i][PARAMETER_VALUE];  
          if ((parameter [i][TYPE_VAR]& 0xF) != BYTE_1&&(parameter [i][TYPE_VAR]& 0xF) != BYTE_1_SIGNED) parameter [i+1][PARAMETER_LAST]= parameter [i+1][PARAMETER_VALUE]; 
          if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_4 || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_SIGNED || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_FLOAT) {
            parameter [i+2][PARAMETER_LAST]= parameter [i+2][PARAMETER_VALUE]; 
            parameter [i+3][PARAMETER_LAST]= parameter [i+3][PARAMETER_VALUE]; 
          } 
        } 
}

/**
//Пока не используется
void AHB_NODE::SendRequestParam (bool Priority, byte Target_Address,  byte Param_Type, const size_t siZE, byte *Sensor_numb) {
        byte sensors_quantity;
        if (siZE<8) sensors_quantity = siZE; else sensors_quantity = 7;
        if (Sensor_numb[0]==0) sensors_quantity = 1;
        byte daTa[sensors_quantity+1]; 
        Parameter_COUNTER++; daTa[0]=Parameter_COUNTER;
        for (byte i = 1 ; i<sensors_quantity+1; i++) daTa [i] = Sensor_numb[i-1];
        TX_COMMAND(Priority, PARAMETER_REQUEST, Target_Address, Param_Type, BIT_29, DATA_CANFRAME, sizeof(daTa), daTa);
}
*/

//ниже функция конфигурирования портов ардуино , в случае укомплетованности узла тем или иным устройством и факта подсоединения к пину ардуино
void AHB_NODE::pinmode() {
        timerPeriodSendParameters = 0-((uint32_t)interval_sendParam-_ahb->_nodeId)*1500ul; // для таймера периодической отправки параметров в кан
        //pinMode(CAN0_INT, INPUT);         
        for (int i = 0 ; i < device_quantity; i++) { 
          pinMode (device[i][DEVICE_PIN], OUTPUT);
          if ((device[i][DEVICE_VID] & 0xF) == IMPULSE_GND || (device[i][DEVICE_VID]& 0xF) == IMPULSE_POWER) {
            pinMode ( device[i][DEVICE_STATUS_PIN], INPUT_PULLUP); 
          }
        }
        #ifdef debug
          Serial.println(); 
          Serial.println(F("Pin устройств сконфигугированы"));
        #endif
}

//ниже значения таймаутов на выполнение команды , сек
byte AHB_NODE::timeout(byte dev_Type) {
        for (int i=0; i<TIMEOUTS_NUMBER; i++){ 
          if (dev_Type == timeoutsArray[i][DEV_TYPE]) return timeoutsArray[i][_TIMEOUT];
        }
        return 15; // если таймаут не найден в базе, делаем его по умолчанию 15 секунд
        #ifdef debug
          Serial.println(); 
          Serial.println(F("При попытке воспользоваться таймаутом ожидания выполнения долгой команды, он НЕ НАЙДЕН в базе (в массиве)!!! Таймаут установлен по умолчанию 15 секунд"));
        #endif
}

// ниже функция по адресу параметра находит его порядковый номер в массиве
byte AHB_NODE::param_addr (byte AdDr, byte SensNum){
        for (int h = 0 ; h < parameters_quantity; h++) {
          if (AdDr == parameter [h][PARAMETER_TYPE]&& SensNum == parameter [h][PARAMETER_SENS]) return h; 
        } 
        return 0xFF;
}

// ниже функция по адресу девайса находит его порядковый номер в массиве
byte AHB_NODE::device_addr (byte AdDr){
        for (int h = 0 ; h < device_quantity; h++) {
          if (AdDr == device [h][DEVICE_TYPE]) return h; 
        } 
        return 0xFF;
}

#ifdef debug
void AHB_NODE::PrinT (byte Str_nomer,  byte subject) {
        #ifdef ARD_DUE
          if (subject == TypeDEV) {if (Str_nomer<SIZE_DEVICE_ARRAY) Serial.print (string_tableDEV[Str_nomer]);  else Serial.print (F("Неизвестное устройство  "));}
          else if (subject == TypeMES) {if (Str_nomer<size_Message_ENUM) Serial.print (string_tableMES[Str_nomer]);   else Serial.print (F("Неизвестное сообщение  "));}
          else if (subject == ADDR)    {if (Str_nomer<NODS_QUANTITY)       Serial.print (string_tableADDR[Str_nomer]);  else Serial.print (F("Неизвестный адрес  "));}
          else if (subject == PAR)     {if (Str_nomer<SIZE_PARAM_ARRAY)  Serial.print (string_tablePAR[Str_nomer]);   else Serial.print (F("Неизвестный параметр  "));}
          else if (subject == COM)     {if (Str_nomer<Command_enum_SIZE) Serial.print (string_tableCOM[Str_nomer]);   else Serial.print (F("Неизвестная команда  "));}
        #else
          bool ok = 1;
          if (subject == TypeDEV) {if (Str_nomer<SIZE_DEVICE_ARRAY) strcpy_P(buf, (char*)pgm_read_word(&(string_tableDEV[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестное устройство  "));}}
          else if (subject == TypeMES) {if (Str_nomer<size_Message_ENUM) strcpy_P(buf, (char*)pgm_read_word(&(string_tableMES[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестное сообщение  "));}}
          else if (subject == ADDR)    {if (Str_nomer<NODS_QUANTITY)       strcpy_P(buf, (char*)pgm_read_word(&(string_tableADDR[Str_nomer])));  else {ok = 0; Serial.print (F("Неизвестный адрес  "));}}
          else if (subject == PAR)     {if (Str_nomer<SIZE_PARAM_ARRAY)  strcpy_P(buf, (char*)pgm_read_word(&(string_tablePAR[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестный параметр  "));}}
          else if (subject == COM)     {if (Str_nomer<Command_enum_SIZE) strcpy_P(buf, (char*)pgm_read_word(&(string_tableCOM[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестная команда  "));}}
          if (ok) Serial.print (buf);
        #endif
}
      
// ниже функция контроля настройки таймаутов ддя долгих команд

void AHB_NODE::timeoutsConfigControl(){
        bool alarmTimeout = 0;
        bool alarmDEV = 0;
        bool alarm = 0;
        for (int i = 0; i<device_quantity; i++) {   
          if ((device [i][DEVICE_VID] & B10000000)>>7){
            alarmTimeout = 1; alarmDEV = 1; 
            for (int j = 0 ; j < TIMEOUTS_NUMBER; j++) {
              if (timeoutsArray [j][DEV_TYPE] == device [i][0]) {
                alarmTimeout = 0; 
              } 
            }
            if (alarmDEV && alarmTimeout) alarm = 1;
            alarmTimeout = 0; alarmDEV = 0;
          }
        }
        Serial.println("timeoutsConfigControl");
        if  (!alarm) Serial.println (F("Таймауты для долгих устройств сконфигурированы успешно!"));
        else Serial.println (F("Неправильно сконфигурированы таймауты для долгих устройств!!!!!!"));
}

void AHB_NODE::parametersConfigControl (){
        bool alarm = 0;
        bool alarm2 = 0;
        for (int i = 0; i < parameters_quantity; i++ ){
          byte compare_type = parameter[i][PARAMETER_TYPE];
          byte compare_sens = parameter[i][PARAMETER_SENS];  
          for (int k = 0 ; k < parameters_quantity; k++){
            if (k!=i && parameter[k][PARAMETER_TYPE]==compare_type && parameter[k][PARAMETER_SENS]==compare_sens) alarm = 1; 
          }
          if   (compare_sens==0) alarm2 = 1;
        }    
        Serial.println("parametersConfigControl");
        if (!alarm && !alarm2) {
          Serial.print (F("Массив параметров сконфигурирован успешно! Количество параметров: ")); 
          Serial.println(parameters_quantity);
        }
        if (alarm)  Serial.println (F("Неправильно сконфигурирован массив параметров!!!!!!  На одном типе параметров найдены датчики с одинаковым номером!"));
        if (alarm2 )Serial.println (F("Неправильно сконфигурирован массив параметров!!!!!!  Найден датчик с номером 0, номера датчиков должны начинаться с 1"));
}

#endif
    
//#endif /* AHB_NODE_C */