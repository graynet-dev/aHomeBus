/**
  aHomeBus CAN interface

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

  This interface depends on the coryjfowler MCP_CAN_lib library.
    Download: https://github.com/coryjfowler/MCP_CAN_lib

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AHB_CAN__C
#define AHB_CAN__C
    #include "ahb_comm_CAN.h"
    #include "ahb_proto.h"
    #include <mcp_can_coryjfowler.h>
    #include <SPI.h>

    volatile bool ahb_CANIntReq=false;

void ahb_CANInt() {
      ahb_CANIntReq=true;
}

    /*
     * @TODO communication without INT
     */
AHB_CAN::AHB_CAN(byte cs, byte speed, byte clockspd) : 
_interface(cs){
        _speed = speed;
        _clockspd = clockspd;
}

AHB_CAN::AHB_CAN(byte cs, byte speed, byte clockspd, byte interrupt, uint8_t *route_table, uint8_t netsize) :
_interface(cs) {

        all_network_address = new uint8_t[netsize];

        for (int i=0; i<netsize; i++){
            this->all_network_address[i]=route_table[i];
        } 

        net_size = netsize; 
        
        pinMode(interrupt, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(interrupt), ahb_CANInt, FALLING);
        _intPin = interrupt;
        _speed = speed;
        _clockspd = clockspd;        
}

void AHB_CAN::mcp2515_reset(void){
        Serial.println("mcp2515_reset()");
        _interface.mcp2515_reset();
}

byte AHB_CAN::begin() {
        //mcp2515_reset();
        Serial.println("AHB_CAN::begin()");
        delay(100);
        lastErr = _interface.begin(MCP_STDEXT, _speed, _clockspd);
        Serial.println("_interface.begin(MCP_STDEXT, _speed, _clockspd);");
        delay(100);
        if(lastErr == CAN_OK) {_interface.setMode(MCP_NORMAL); Serial.println("_interface.setMode(MCP_NORMAL);");}
       
/* _interface.init_Mask(0,1,0x00000000);                       // Init first mask...
_interface.init_Filt(0,1,0x00000000);   // Init first filter...
_interface.init_Filt(1,1,0x00000000);                       // Init second filter...
_interface.init_Mask(1,1,0x00000000);                       // Init second mask...   
_interface.init_Filt(2,1,0x00000000);                       // Init 3 filter...
_interface.init_Filt(3,1,0x00000000);                       // Init 4 filter...
_interface.init_Filt(4,1,0x00000000);                       // Init 5 filter...
_interface.init_Filt(5,1,0x00000000);                       // Init 6 filter...

void initCAN() {
    // TODO: make baud rate variable
  while(CAN.begin(CAN_500KBPS)!=0) delay(1000);

  CAN.init_Mask(0,0, 0x007E); // allow two consecutive ids
  CAN.init_Filt(0,0, getNodeId()); // RxPDO and SDO
  CAN.init_Filt(1,0, getNodeId()); // same

  CAN.init_Mask(1,0, 0x07FF);
  CAN.init_Filt(2,0, 0x0000); // NMT
  CAN.init_Filt(3,0, 0x0080); // sync
  CAN.init_Filt(4,0, 0x0100); // time stamp
  CAN.init_Filt(5,0, 0x0000); // dummy
}        
*/
        

        
        #ifdef AHB_DEBUG
        if(lastErr == CAN_OK) {Serial.println(F("CAN OK"));}
        else {
          Serial.print(F("CAN FAIL Error - "));
          Serial.print(lastErr);
        }

        for (int i=0; i<net_size; i++){
          Serial.print(F("Node ")); Serial.println(all_network_address[i]);
        }  
        #endif //AHB_DEBUG
        return lastErr;
}

uint8_t AHB_CAN::address_network_get(uint8_t i){
    return all_network_address[i];   
}

uint8_t AHB_CAN::net_size_get(void){
//Serial.print("Net Size - "); Serial.println(net_size);
return net_size;
}

uint8_t AHB_CAN::busType(){
        return type_CAN;
}



// For convenient frame visualization we use nibble.
//ID structure   x.y.z.aa.bb
//     E                DD             CC              B            AA             
// pocket type     command&ansver  target addres  target port  source address 
//  1^2=2             2^8=256       2^8=256         2^4=16        2^8=256
/* E DD CC B AA
E (0-1) 2 values - 0 multicast & 1 unicast //I do not understand why we also need Broadcast, 
                                             especially since the address zero is Broadcast. 
                                             In principle, the multicast is not needed, 
                                             although it is possible if the multicast flag 
                                             is set during transmission, then the recipient's 
                                             address is read as the multicast group's address, 
                                             if the multicast flag is not set, the recipient's 
                                             address is the node address. It can also be used 
                                             to address neighboring networks. 
DD (0-F/0-F) 256 values from struct - type message //Service message, command and more
CC (0-F/0-F) 256 values from struct - target address
B (0-F) 16 values from struct - port
AA (0-F/0-F) 256 values from struct - source address

*/
uint32_t AHB_CAN::ahbCanAddrAssemble(ahbMeta meta) {
        return ahbCanAddrAssemble(meta.type, 0,meta.target, meta.port, meta.source);
}

uint32_t AHB_CAN::ahbCanAddrAssemble(uint8_t type, uint8_t target, uint8_t source) {
        return ahbCanAddrAssemble(type, 0,target, -1, source);
}

uint32_t AHB_CAN::ahbCanAddrAssemble(uint8_t f_type, uint8_t f_cmd, uint8_t f_target, uint8_t f_port, uint8_t f_source) {
          uint32_t addr = 0x80000000;
 
          if(f_type == AHB_PKGTYPE_UNICAST) {
            if(f_target > 255) {
              //Serial.println("f_target > 255");
              return 0;}
            if(f_port < 0 || f_port > 15) {
              //Serial.println("f_port < 0 || f_port > 15");
              return 0;
            }
          }
          else{
            if(f_target > 255) {
              //Serial.println("f_target > 255");
              return 0;
            }
          }

          if(f_source > 255) {
            //Serial.println("f_source > 255");
            return 0;
          }
          
          addr = (f_type & 0xFFFFFFFF)<<28 | (f_cmd & 0xFFFFFFFF)<<20 | (f_target & 0xFFFFFFFF)<<12 | (f_port & 0xFFFFFFFF)<<8 | f_source ;

          return addr;
}

bool AHB_CAN::ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]) {
        uint32_t addr = ahbCanAddrAssemble(type, cmd, target, port, source);
        if(addr == 0){ 
          return false;
        }
        
        lastErr = _interface.sendMsgBuf(addr, 1, len, data);
        if(lastErr != CAN_OK) {
          //Serial.println("CAN ahbSend lastErr != CAN_OK");
          return false;
        }
        else{
            #ifdef AHB_DEBUG
            //Serial.print(F("TX <--- ADDR - ")); Serial.println(addr, HEX);
            Serial.print(F("TX <---  "));
            Serial.print(F("   "));
            Serial.print(F(" - Type: 0x"));
            Serial.print(type);
            Serial.print(F(" - CMD: 0x"));
            Serial.print(cmd);
            Serial.print(F("  Target: 0x"));
            Serial.print(target);
            Serial.print(F("  Port: 0x"));
            Serial.print(port);
            Serial.print(F("  Source: 0x"));
            Serial.print(source);
            Serial.print(F("  Length: 0x"));
            Serial.print(len);
            for(byte i=0; i<len; i++) {
              Serial.print(F(" 0x"));
              Serial.print(data[i]);
            }
            Serial.println();
            #endif //AHB_DEBUG
        }
        return true;
}

ahbMeta AHB_CAN::ahbCanAddrParse(uint32_t rxId_can) {

        ahbMeta temp;
        temp.target = 0;
        temp.source = 0;
        temp.port = 0;
      
        //length, buf = data byte(s)
        uint8_t  f_cmd;
        bool can_frame_type, can_ID_type, RTR_11BIT;
        can_ID_type    = (rxId_can & 0x80000000)>>31;  //извлекаем тип ID        (0 - 11bit, 1 - 29 bit)
        can_frame_type = (rxId_can & 0x40000000)>>30;  //извлекаем тип СAN FRAME (0 - Data,  1 - Remote)   
        if (can_frame_type && ((rxId_can & 0x20000000)>>29)) RTR_11BIT = 1;
        else RTR_11BIT = 0;                            //если 29 бит единичка, то это RTR 11 бит
        temp.type  = (rxId_can & 0x10000000)>>28; //  извлекаем 1-битный идентификатор запроса-ответа из ID       
        temp.cmd = (rxId_can &   0xFF00000)>>20;  //  извлекаем 4-битный идентификатор сообщения из ID         
        temp.target  = (rxId_can &  0xFF000)>>12;   //  извлекаем 8-битный адрес отправителя из ID - от кого
        temp.port = (rxId_can &   0xF00)>>8;      //  извлекаем 8-битный адрес получателя из ID - кому
        temp.source  = (rxId_can &   0xFF);                      //  извлекаем 8-битный тип устройства у получателя из ID
        
        #ifdef AHB_DEBUG
          Serial.println(F(" ----------------------------------------------------------")); 
          //Serial.print(F("RX ---> ADDR - ")); Serial.println(rxId_can, HEX);
          Serial.print(F("RX --->  "));Serial.print("   ");
          Serial.print(F(" - Type: 0x"));Serial.print(temp.type);
          Serial.print(F(" - CMD: 0x"));Serial.print(temp.cmd);
          Serial.print(F("  Target: 0x"));Serial.print(temp.target);
          Serial.print(F("  Port: 0x"));Serial.print(temp.port);
          Serial.print(F("  Source: 0x"));Serial.print(temp.source);               
        #endif

        return temp;
}

bool AHB_CAN::ahbReceive_V(ahbPacket &pkg) {
        uint32_t rxId;
        byte len = 0;
        byte rxBuf[8];

        if(_interface.checkReceive() != CAN_MSGAVAIL) return false; //if not use interrapt
        //if (!ahb_CANIntReq) return false; //if use interrapt

        byte state = _interface.readMsgBuf(&rxId, &len, rxBuf); 

        if(state != CAN_OK) return false;

        pkg.meta = ahbCanAddrParse(rxId);
       
        pkg.len = len;

        #ifdef AHB_DEBUG  
        Serial.print(F("  Length: 0x"));
        Serial.print(len);
        #endif //AHB_DEBUG 
        
        for(byte i=0; i<len; i++){ 
          pkg.data[i] = rxBuf[i];
          #ifdef AHB_DEBUG  
          Serial.print(F(" 0x"));
          Serial.print(pkg.data[i]);
          #endif //AHB_DEBUG 
        }
          
        #ifdef AHB_DEBUG 
          Serial.println();  
        #endif //AHB_DEBUG
        
        return true;
}

/**
Примеры вызова
ahbSendCommand (PWM_TURN_ON,     20,  node_10_Kitchen_main,  lamp_wall_onoff_d);
ahbSendCommand (PWM_TURN_OFF,    20,  node_10_Kitchen_main,  lamp_wall_onoff_d);
ahbSendCommand (DIGITAL_ON,       0,  node_10_Kitchen_main,  lamp_ceil_onoff_d);
ahbSendCommand (DIGITAL_OFF,      0,  node_10_Kitchen_main,  lamp_ceil_onoff_d);
ahbSendCommand (PARAMETER_WRITE, 45,  node_10_Kitchen_main,  air_t            );
*/
/**
void AHB_CAN::ahbSendCommand(byte Command_Razryad_1, byte Command_Razryad_2, byte Target_Address,  byte Device_Type){
        bool empty_cells = 0; // наличие свободной ячейки в очереди
        Command_COUNTER++;
        //-----------------------------------------запись в  очередь 
        for (int i = 0; i<queueTX_size; i++) {
            if  (Command_TX_cell[i][cellState]==0) {
                empty_cells = 1;
                Command_TX_cell[i][Command_TX_COUNTER] = Command_COUNTER;   // запись значения глобального счетчика 
                Command_TX_cell[i][CommandRazryad_1]   = Command_Razryad_1; //
                Command_TX_cell[i][CommandRazryad_2]   = Command_Razryad_2; //
                Command_TX_cell[i][target_Address]     = Target_Address;    //
                Command_TX_cell[i][cellState]          = 1;                 //
                Command_TX_cell[i][DEVTYPE]            = Device_Type;       //
                break;
            }
        }
        #ifdef AHB_DEBUG //Вывести сообщение если очередь переполнена
        if (!empty_cells) Serial.println (F("Внимание!!! Очередь команд переполнена. Сообщение НЕ отправлено!!!"));
        #endif //AHB_DEBUG
        //------------------------------------------  
}

void AHB_CAN::ahbSendCommand_queue(){
        // =========Извлечение из очереди отправки сообщений TX
        for (int i = 0; i<queueTX_size; i++) {
            if (Command_TX_cell[i][cellState]==1)  {              // если нашли занятую ячейку в очереди и статус, что ещё НЕ получили отчёт "команда принята"
               if (Command_TX_cell[i][flagTimerCommand]==0) {      //если таймер не включен отправляем TX сообщение
                   byte DATa[5] = {0};
                   DATa[0]=Command_TX_cell[i][Command_TX_COUNTER];
                   DATa[3]=Command_TX_cell[i][CommandRazryad_1];
                   DATa[4]=Command_TX_cell[i][CommandRazryad_2];
        ////CAN_TX  (1, COMMAND_SEND, Command_TX_cell[i][target_Address],  Command_TX_cell[i][DEVTYPE], BIT_29, DATA_CANFRAME, sizeof(DATa), DATa ) ; // отправляем команду в CAN    
                   //  вызов CAN_TX (direction, тип сообщения, адрес получателя, тип устройства, тип ID, тип FRAME, длина поля данных кадра, DATa)
                   Command_TX_cell[i][flagTimerCommand] = 1;     // вклчаем таймер
                   queueTX_Timer [i] = millis(); // обнуляем таймер
                   Command_TX_cell[i][Command_TX_count]++; // прибавляем счетчик отправленных сообщений
               } // конец скобки "если таймер не включен отправляем TX сообщение"
               else if (Command_TX_cell[i][flagTimerCommand]==1 && (millis() - queueTX_Timer[i]) > TX_repeatPeriod){  // если таймер былл включен и мы все ещё не получили отчет
                   byte DATa[5] = {0};
                   DATa[0]=Command_TX_cell[i][Command_TX_COUNTER];
                   DATa[3]=Command_TX_cell[i][CommandRazryad_1];
                   DATa[4]=Command_TX_cell[i][CommandRazryad_2];
        ////CAN_TX  (1, COMMAND_SEND, Command_TX_cell[i][target_Address], Command_TX_cell[i][DEVTYPE], BIT_29, DATA_CANFRAME, sizeof(DATa), DATa ) ;  // отправляем команду в CAN
                   Command_TX_cell[i][Command_TX_count]++;   // прибавляем счетчик отправленных сообщений
                   // если через 5 попыток отправки команды так не пришёл отчет, бросаем это дело -  освобождаем ячейку в очереди
                   if (Command_TX_cell[i][Command_TX_count]>=maxNumberRepeat_TX) {
                       // и пожалуемся в отладчик
                       #ifdef AHB_DEBUG
                       Serial.println(); 
                       Serial.print (F("Ошибка команды  \" ")); 
                       CAN_print_message (Command_TX_cell[i][CommandRazryad_1], COM);
                       Serial.print (F(" \"")); 
                       Serial.print (F(" на тип устройства ")); 
                       CAN_print_message(Command_TX_cell[i][DEVTYPE], TypeDEV);
                       Serial.println (F(" Отчёт о принятии команды от оппонента не получен!!!"));
                       #endif //AHB_DEBUG
                       for (int j = 0; j<11; j++)  Command_TX_cell[i][j] = 0; // освобождаем ячейку в очереди
                   }
                   else    queueTX_Timer [i] = millis();              // иначе обнуляем таймер
               }   
               //delay (4); // эта задержка нужна чтобы сообщения очереди не подряд сыпались, но можно и без неё попробовать          
            }
            else if (Command_TX_cell[i][cellState]==0x02)  {                       // если нашли занятую ячейку в очереди и статус, что получили отчёт "команда принята, выполняется"
               if (Command_TX_cell[i][flagTimerCommand]==0) { // если таймаут ожидания выполнения команды кончился то:
                   //ТУТ ЧТО-НИБУДЬ ДЕЛАЕМ, ЕСЛИ ОППОНЕНТ КОМАНДУ ПРИНЯЛ, НО ОТЧЁТА ОБ УСПЕЛНОМ ВЫПОЛНЕНИИ КОМАНДЫ ОТ ОППОНЕНТА МЫ ТАК И НЕ ДОЖДАЛИСЬ
                   #ifdef AHB_DEBUG
                   Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); CAN_print_message (Command_TX_cell[i][CommandRazryad_1], COM);
                   Serial.print (F(" \"")); Serial.print (F(" на тип устройства ")); CAN_print_message(Command_TX_cell[i][DEVTYPE], TypeDEV); 
                   Serial.println (F(" Закончился таймаут, отведённый на выполнение команды. Отчёт об успешном выполнении не получен!!! ")); 
                   #endif //AHB_DEBUG
                   for (int j = 0; j<11; j++)  Command_TX_cell[i][j] = 0;
               }                  
               else if (Command_TX_cell[i][flagTimerCommand]==1 && millis() - queueTX_Timer[i] > (uint32_t)ahbTimeout(Command_TX_cell[i][DEVTYPE])*1000) { // если таймер таймаута был включен и он кончился
                   Command_TX_cell[i][flagTimerCommand]=0; // выключаем таймер, значит таймаут кончился 
               }
            }
        } 
}

//ниже значения таймаутов на выполнение команды , сек
byte AHB_CAN::ahbTimeout(byte dev_Type) {
        for (int i=0; i<TIMEOUTS_NUMBER; i++){ 
          if (dev_Type == timeoutsArray[i][DEV_TYPE]) return timeoutsArray[i][TIMEOUT_];
        }
        return 15; // если таймаут не найден в баз,е делаем его по умолчанию 15 секунд
        #ifdef AHB_DEBUG
        Serial.println(); Serial.println(F("При попытке воспользоваться таймаутом ожидания выполнения долгой команды, он НЕ НАЙДЕН в базе (в массиве)!!! Таймаут установлен по умолчанию 15 секунд"));
        #endif //AHB_DEBUG
}


byte AHB_CAN::ahbCommandExecuting (byte dev_Type, byte Command_Razryad_1, byte Command_Razryad_2, byte cell, bool EXE, bool AlreadyEx, byte Command_Razryad_3, byte Command_Razryad_4, byte Command_Razryad_5, byte Sensor_num) {
        // ТУТ НЕПОСРЕДСТВЕННО ВЫПОЛНЯЕМ КОМАНДУ
        bool Long=0;
        if (Command_Razryad_1!= PARAMETER_WRITE){
            Long = (device [DEVICE_PRESENCE][dev_Type] & B10000000)>>7;  
        } 
        
        // определим долго ли будет девайс выполнять команду или мгновенно 
        if ((Long && !AlreadyEx) || (!Long && EXE)){
            if (Command_Razryad_1 !=  PARAMETER_WRITE){
                if ((device [DEVICE_PRESENCE][dev_Type] & 0xF) == DIGITALWRITE){                                
                    if (Command_Razryad_1!=DIGITAL_INVERT) digitalWrite (device [DEVICE_PIN][dev_Type],  Command_Razryad_1);
                    else {
                      if (!digitalRead (device [DEVICE_PIN][dev_Type])) digitalWrite (device [DEVICE_PIN][dev_Type],  HIGH);
                      else if  (digitalRead (device [DEVICE_PIN][dev_Type])) digitalWrite (device [DEVICE_PIN][dev_Type],  LOW);
                    }
                }
                
                else if ((device [DEVICE_PRESENCE][dev_Type] & 0xF) == DIGITALWRITE_INVERT){                                                        
                    if  (Command_Razryad_1!=DIGITAL_INVERT){
                        if (Command_Razryad_1 == DIGITAL_ON)  digitalWrite (device [DEVICE_PIN][dev_Type],  LOW);
                        else if (Command_Razryad_1 == DIGITAL_OFF) digitalWrite (device [DEVICE_PIN][dev_Type],  HIGH);
                    }
                    else {
                        if (!digitalRead (device [DEVICE_PIN][dev_Type])) digitalWrite (device [DEVICE_PIN][dev_Type],  HIGH);
                        else if  (digitalRead (device [DEVICE_PIN][dev_Type])) digitalWrite (device [DEVICE_PIN][dev_Type],  LOW);
                    }
                }
                
                else if ((device [DEVICE_PRESENCE][dev_Type] & 0xF) == PWM_WRITE){                          
                    if (Command_Razryad_1 == PWM_SETTING)  device [DEVICE_STATUS][dev_Type] = Command_Razryad_2; 
                    else if (Command_Razryad_1 == PWM_TURN_ON)  {
                        if (device [DEVICE_STATUS][dev_Type] + Command_Razryad_2 <= 255 ) device [DEVICE_STATUS][dev_Type]+=Command_Razryad_2; 
                        else device [DEVICE_STATUS][dev_Type] = 255;
                    }
                    else if (Command_Razryad_1 == PWM_TURN_OFF) {
                        if (device [DEVICE_STATUS][dev_Type] - Command_Razryad_2 >= 0 ) device [DEVICE_STATUS][dev_Type]-=Command_Razryad_2; 
                        else device [DEVICE_STATUS][dev_Type] = 0;
                    }
      
                    analogWrite (device [DEVICE_PIN][dev_Type],  device [DEVICE_STATUS][dev_Type]);
      
                }
                
                else if ((device [DEVICE_PRESENCE][dev_Type] & 0xF) == PROCENT_WRITE){
                }
                
                else if ((device [DEVICE_PRESENCE][dev_Type] & 0xF) == IMPULSE_GND || (device [DEVICE_PRESENCE][dev_Type] & 0xF) == IMPULSE_POWER){
                    byte ok = 0;
                    
                    if (Command_Razryad_1 == IMPULSE_INVERT) ok = 2;
                    
                    else if (Command_Razryad_1 == IMPULSE_ON  && device [DEVICE_STATUS][dev_Type]==0) { 
                        ok = 1;  
                    }
                    
                    else if (Command_Razryad_1 == IMPULSE_OFF && device [DEVICE_STATUS][dev_Type]!=0) ok = 1; 
                    
                    for (int i = 0 ; i< IMPULSEDEVICE_QUANTITY; i++) { 
                        if (TimerImpulse_ON[i]==device[DEVICE_PIN][dev_Type]) ok = 0;
                    }
                    
                    if (ok == 1){   
                        for (int i = 0; i< IMPULSEDEVICE_QUANTITY; i++){
                            if (TimerImpulse_ON[i]==0) {
                                TimerImpulse_ON[i] = device[DEVICE_PIN][dev_Type]; 
                                timerImpulse[i] = curMillis; 
                                ok = 2; 
                                break;
                            } 
                        }
                        
                        #ifdef AHB_DEBUG
                        if (ok!=2){ 
                            Serial.println(); 
                            Serial.println (F("Пытаемся управлять неколькими импульсными девайсами одновременно! Превышенно максимальное значение девайсов одновременного управления"));
                        }
                        #endif //AHB_DEBUG 
                        
                        if (ok == 2){ 
                            if ((device [DEVICE_PRESENCE][dev_Type] & 0xF)==IMPULSE_GND)   digitalWrite (device [DEVICE_PIN][dev_Type],  LOW);
                            else if ((device [DEVICE_PRESENCE][dev_Type] & 0xF)==IMPULSE_POWER) digitalWrite (device [DEVICE_PIN][dev_Type],  HIGH);  
                        }
                    }
                }
            }
            // НИЖЕ ЕСЛИ КОМАНДА - "УПРАВЛЕНИЕ ИЗМЕНЕНИЕМ ПАРАМЕТРА"  ТО В РАЗРЯДЕ dev-type ЗАГОЛОВКА ID  БУДЕТ ТИП ПАРАМЕТРА, А НЕ ИСПОЛНИТЕЛЬНОГО МЕХАНИЗМА
            else {
                parameter[param_addr(dev_Type, Sensor_num)][PARAMETER] = Command_Razryad_2;
                if ((parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) != BYTE_1)  parameter[param_addr(dev_Type, Sensor_num)+1][PARAMETER] = Command_Razryad_3;                         
                if ((parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) == BYTE_4) {
                    parameter[param_addr(dev_Type, Sensor_num)+2][PARAMETER] = Command_Razryad_4; parameter[param_addr(dev_Type, Sensor_num)+3][PARAMETER] = Command_Razryad_5;
                }
            }  
        
            #ifdef AHB_DEBUG    
            if (!AlreadyEx && Long){
                Serial.println(); Serial.print (F("Началось выполнение долгой команды \" "));
                CAN_print_message (Command_Razryad_1, COM);  Serial.print (F(" \" типа устройства: "));
                CAN_print_message (dev_Type, TypeDEV); Serial.println();
            }
        
            if (EXE && !Long) {
                Serial.println(); 
                Serial.print (F("Произведено выполенение быстрой команды \" "));
                CAN_print_message (Command_Razryad_1, COM);  
                Serial.print (F(" \" типа устройства: "));
                if (Command_Razryad_1!=PARAMETER_WRITE)   CAN_print_message (dev_Type, TypeDEV);
                else CAN_print_message (dev_Type, PAR);
                Serial.println(); 
            }
        
            if (!EXE && !Long){
                Serial.println();
                Serial.println (F("Получена команда в течение 1 секунды с глоб.счетчиком как в предыдущей команде. Игнорируем выполнение команды, но отчёт отправляем."));
            }
        
            #endif //AHB_DEBUG   
        }
        
        if (Long) return EXECUTING;
        else      return COMPLETE;
}
  
void CAN_LongCommandExecuting_buffer(){
  // ===========================Извлечение из буфера долгих команд и ожидание выполнения
  for (int i = 0; i<bufferEXElongCOM_size; i++) {
    if (bufferEXElongCOM_cell[i][cellState]==1)  {                       // если нашли занятую ячейку в очереди
      if (bufferEXElongCOM_cell[i][flagTimerCommand]==1 && curMillis - bufferEXElongCOM_Timer[i] > ((uint32_t)timeout(bufferEXElongCOM_cell[i][DEVTYPE])*1000)-500) {      //если таймер был включен и кончился отправляем отчёт
        byte DATa[5] = {0};
        DATa[3]=bufferEXElongCOM_cell[i][CommandRazryad_1];
        DATa[4]=bufferEXElongCOM_cell[i][CommandRazryad_2];
        DATa[2]= CAN_ExecutionResult (bufferEXElongCOM_cell[i][DEVTYPE], DATa[3], DATa[4]); 
        for (int j = 0; j<3; j++) {
          if (j==0) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER];
          else if (j==1) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER_2];
          else if (j==2) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER_3];
          if (bufferEXElongCOM_cell[i][7+j]!=0)  CAN_TX (1, COMMAND_ANSWER, bufferEXElongCOM_cell[i][7+j],  bufferEXElongCOM_cell[i][DEVTYPE], EXTENDED,  DATA_CANFRAME, sizeof(DATa), DATa) ; 
        }
        // отправляем отчёт в CAN    
        #ifdef debug
        Serial.println();        
        Serial.println (F("удалил из буфера отчетов"));
        #endif //end debug
        for (int j = 0; j<11; j++)  bufferEXElongCOM_cell[i][j] = 0; //чистим ячейку так как отчет отправлен
      }
    }
  }
}

byte CAN_ExecutionResult (byte dev_Type, byte Command_Razryad_1, byte Command_Razryad_2) {
  if (digitalRead (device[DEVICE_PIN][dev_Type]) == Command_Razryad_1) {
    #ifdef debug
    Serial.println(); 
    Serial.print (F("Выполнение долгой команды \" "));
    CAN_print_message (Command_Razryad_1, COM);  
    Serial.print (F(" \" типа устройства: "));
    CAN_print_message (dev_Type, TypeDEV); 
    Serial.println(F("  завершилось успешно!"));
    #endif //end debug  
    return COMPLETE;
  }
  else {
    #ifdef debug
    Serial.println(); 
    Serial.print (F("Таймаут на выполнение долгой команды \" "));
    CAN_print_message (Command_Razryad_1, COM);  
    Serial.print (F(" \" типа устройства: "));
    CAN_print_message (dev_Type, TypeDEV); 
    Serial.println(F("  истёк! Команда НЕ выполнена! Результат отрицательный!"));
    #endif //end debug
    return NEGATIVE_RESULT; 
  }  
}


//ниже функция конфигурирования портов ардуино , в случае укомплетованности узла тем или иным устройством и факта подсоединения к пину ардуино
void device_pin_config() {
  timerPeriodSendParameters = 0-((uint32_t)interval_sendParam-node_address)*1500ul; // для таймера периодической отправки параметров в кан
  pinMode(CAN0_INT, INPUT);               
  for (int i = 0 ; i < SIZE_DEVICE_ARRAY; i++) { 
    if (device[DEVICE_PRESENCE][i]>0) pinMode (device[DEVICE_PIN][i], OUTPUT);
    if ((device[DEVICE_PRESENCE][i] & 0xF) == IMPULSE_GND || (device[DEVICE_PRESENCE][i]& 0xF) == IMPULSE_POWER) {
      #ifdef ARD_UNO
      pinMode ( ((device[DEVICE_PRESENCE][i] & 0xF0)>>4)+13 , INPUT_PULLUP); 
      #endif 
      #ifdef ARD_MEGA
      pinMode ( ((device[DEVICE_PRESENCE][i] & 0xF0)>>4)+21 , INPUT_PULLUP); 
      #endif 
   }
  }
}

void CAN_timers() {
  #ifdef type_node_master
  if (curMillis - prevSystemtime > 1000) {
    ss++; if (ss >=60) {
      ss = 0; mm++; if (mm >=60) {
        mm = 0; hh++; if (hh>=24) {
          dd++; hh=0;
        }
      }
    } 
    prevSystemtime = curMillis;
  } 
  // это тикание системных часов на миллис 
  if (curMillis >= 30000 && !flag_alarmAccident) {
    flag_alarmAccident = 1;
    for (int i = 0; i<NODS_NUMBER; i++) if (!StatusNode_OK[i] && i!= node_address && NodeCANpresence [i]!=0) CAN_SendAccident(i);
  }
  #ifdef debug
  if ((ss == 30 || ss == 0) && !printstatus) {
    PrintSystemTime(); 
    if (errors){
      readErrorFlags_MCP2515 ();
    } 

    #ifdef debugStatus
    Print_CAN_Status(); 
    
    #endif
    #ifdef PING_FEATURE
    Ping_node();
    #endif //PING_FEATURE
    printstatus = 1;
  }
  if (ss != 30 && ss != 0) printstatus = 0;
  #endif
  #endif 
  #if defined (type_node_slave) or defined (type_node_mk) 
  //ниже если это подчинённый , то шлём наш статус мастеру, через 50мс*адрес_узла после того как получили статус от мастера 
  if  (TimerStatus && curMillis - prevtimeStatus > node_address*30) {
    CAN_status_TX(SendAddrStatus_master,0); //если ответ то должен выть ANS если запрос то SEND  
    CAN_status_TX(SendAddrStatus_slave,0);
    TimerStatus = 0;
  }
  #endif 
  // ниже таймер, в течение которого происходит игнорирование поступающих одинаковых команд с одинаковым глобальным счетчиком
  if (timercountRepeat_ON && curMillis - timercountRepeat > TX_repeatPeriod*(maxNumberRepeat_TX+1)) timercountRepeat_ON = 0; 
  // ниже таймер создания импульса управления импульсными девайсами
  for (int i = 0; i< IMPULSEDEVICE_QUANTITY; i++){
    if (TimerImpulse_ON[i]>0 && (millis() - timerImpulse[i] > IMPULSE_DURATION * 100)){
      digitalWrite(TimerImpulse_ON[i], !digitalRead(TimerImpulse_ON[i]));
      TimerImpulse_ON[i] = 0;
    }
  }
  //ниже для функционирования управления импульсными устройствами. Это мониторинг статуса устройства при помощи входов: ВКЛ оно или ВЫКЛ
  for (int i = 0; i < SIZE_DEVICE_ARRAY; i++) { 
    if ((device[DEVICE_PRESENCE][i] & 0xF) == IMPULSE_GND || (device[DEVICE_PRESENCE][i]& 0xF) == IMPULSE_POWER) {
      #ifdef ARD_UNO
      device [DEVICE_STATUS][i] = !digitalRead ( ((device[DEVICE_PRESENCE][i] & 0xF0)>>4)+13 );
      #endif
      #ifdef ARD_MEGA
      device [DEVICE_STATUS][i] = !digitalRead ( ((device[DEVICE_PRESENCE][i] & 0xF0)>>4)+21 );
      #endif
    }
  }
}



// ниже функция по адресу параметра находит его порядковый номер в массиве
byte param_addr (byte AdDr, byte SensNum){
  for (int h = 0 ; h < parameters_quantity; h++) {
    if (AdDr == parameter [h][PARAMETER_TYPE]&& SensNum == parameter [h][PARAMETER_SENS]) return h; 
  } 
  return 0xFF;
}

#if defined (type_node_slave) or defined (type_node_mk) 
void CAN_DataStreamSend(){
  bool Periodical_timer = 0;  
  // if (curMillis - timerPeriodSendParameters > (uint32_t)interval_sendParam *1500ul) {Periodical_timer = 1; timerPeriodSendParameters = curMillis; }
  if (ss == node_address && !sendparameters) {sendparameters = 1; Periodical_timer = 1;}
  if (ss!=node_address) sendparameters = 0;
  #ifdef debug
  if (Periodical_timer) PrintSystemTime();
  #endif
  for (int i = 0; i < parameters_quantity; i++) {
    for (byte p = PERIODIC_CANADDR; p<(PERIODIC_CANADDR+second_dimension-5); p++){
      if ( (parameter [i][TYPE_VAR]& 0xF)!=SERVICE_BYTE && parameter [i][p]!=0xFF){  // если  это не сервисный байт
        byte DLc; byte ok = 0;
        if ((parameter [i][TYPE_VAR] & 0xF)== BYTE_1||(parameter [i][TYPE_VAR] & 0xF)== BYTE_1_SIGNED) {
          if (ok==0) {ok = 1;} 
          DLc = 5;}
        else if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_2||(parameter [i][TYPE_VAR]& 0xF) == BYTE_2_SIGNED) {
          if (ok==0) {ok = 2;} 
          DLc = 6;
        }
        else if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_4||(parameter [i][TYPE_VAR]& 0xF) == BYTE_4_SIGNED || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_FLOAT) {
          if (ok==0) {ok = 4;} 
          DLc = 8;
        }
        // ниже если параметр поменял своё значение
        if (parameter [i][PARAMETER]!= parameter [i][PARAMETER_LAST] && ok == 1) ok = CHANGEVALUE;          
        else if ((parameter [i][PARAMETER]!= parameter [i][PARAMETER_LAST] || parameter [i+1][PARAMETER]!= parameter [i+1][PARAMETER_LAST]) && ok == 2) ok = CHANGEVALUE;          
        else if ((parameter [i][PARAMETER]!= parameter [i][PARAMETER_LAST] || parameter [i+1][PARAMETER]!= parameter [i+1][PARAMETER_LAST] || parameter [i+2][PARAMETER]!= parameter [i+2][PARAMETER_LAST] || parameter [i+3][PARAMETER]!= parameter [i+3][PARAMETER_LAST])&& ok == 4) ok = CHANGEVALUE;           
        if (Periodical_timer) ok = PERIODICALLY;
        if (ok == PERIODICALLY || ok == CHANGEVALUE){   
          Parameter_COUNTER++;  
          byte Data[8]= {Parameter_COUNTER, parameter [i][PARAMETER_SENS], ok, parameter [i][TYPE_VAR], parameter [i][PARAMETER], parameter [i+1][PARAMETER], parameter [i+2][PARAMETER], parameter [i+3][PARAMETER]};    
          CAN_TX(1,REQUEST_ANSWER, parameter[i][p], parameter [i][PARAMETER_TYPE], EXTENDED, DATA_CANFRAME, DLc, Data);
          delay (5);
        }
      }    
    }
    //ниже  обновляем прошлое значение параметра
    parameter [i][PARAMETER_LAST]= parameter [i][PARAMETER];  
    if ((parameter [i][TYPE_VAR]& 0xF) != BYTE_1&&(parameter [i][TYPE_VAR]& 0xF) != BYTE_1_SIGNED) parameter [i+1][PARAMETER_LAST]= parameter [i+1][PARAMETER]; 
    if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_4 || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_SIGNED || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_FLOAT) {
      parameter [i+2][PARAMETER_LAST]= parameter [i+2][PARAMETER]; parameter [i+3][PARAMETER_LAST]= parameter [i+3][PARAMETER]; 
    } 
    
  }
}
#endif //defined (type_node_slave) or defined (type_node_mk) 

*/



#endif /* AHB_CAN__C */
