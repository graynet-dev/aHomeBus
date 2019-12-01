/**
  aHomeBus bus interface

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

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

#ifndef AHB__C
#define AHB__C
    #include "ahb.h"
    #include "ahb_proto.h"
    #include <stdio.h> 
    #ifdef AHB_EEPROM_USE
      #include<EEPROM.h>
    #endif

    
AHB::AHB(unsigned int start, unsigned int stop, uint8_t ndType)  {

        if(_cfgAddrStop < _cfgAddrStart+2) _cfgAddrStop = _cfgAddrStart;
        _nodeType=ndType;
        //Read NodeID from EEPROM
        uint8_t cfg = 0;
        #ifdef AHB_EEPROM_USE
        EEPROM.get(_cfgAddrStart, cfg);
        #endif
        setNodeId(cfg);
        _cfgAddrStop = stop;
        _cfgAddrStart = start;

        #ifdef AHB_DEBUG
        Serial.println("                       ");
        Serial.print(F("Node type - ")); 
        #endif //AHB_DEBUG
        
        switch(_nodeType) {
              case Master: {
                   #ifdef AHB_DEBUG
                      Serial.println(F("Master"));
                   #endif //AHB_DEBUG
                   
                  
                   break;
              }
              case Slave:{
                   #ifdef AHB_DEBUG
                      Serial.println(F("Slave"));
                   #endif //AHB_DEBUG 
                
                   break;

              }  
              case Node:{
                   #ifdef AHB_DEBUG
                      Serial.println(F("Node"));
                   #endif //AHB_DEBUG
                
                   break;
              }
              default: {
                    #ifdef AHB_DEBUG
                    Serial.println(F("UNCNOWN_Node")); 
                    #endif //AHB_DEBUG
                    
                    break; 
              }      
              
        }
        
              
}

AHB::AHB(uint8_t id, uint8_t ndType) {      
        setNodeId(id);
        _nodeType=ndType;
        _cfgAddrStop = 0;
        _cfgAddrStart = 0;
        
        #ifdef AHB_DEBUG
        Serial.println("                       ");
        Serial.print(F("Node type - ")); 
        #endif //AHB_DEBUG
        
        switch(_nodeType) {
              case Master: {
                   #ifdef AHB_DEBUG
                      Serial.println(F("Master"));
                   #endif //AHB_DEBUG
                   
                   
                   break;
              }
              case Slave:{
                   #ifdef AHB_DEBUG
                      Serial.println(F("Slave"));
                   #endif //AHB_DEBUG 
                
                   break;

              }  
              case Node:{
                   #ifdef AHB_DEBUG
                      Serial.println(F("Node"));
                   #endif //AHB_DEBUG
                
                   break;
              }
              default: {
                    #ifdef AHB_DEBUG
                    Serial.println(F("UNCNOWN_Node")); 
                    #endif //AHB_DEBUG
                    
                    break; 
              }      
              
        }
}

void watchdogSetup(void) {
  /*** watchdogDisable (); ***/
}

byte AHB::begin() {
        Wire.begin();
        #if defined  (__AVR__)
          wdt_enable(WDTO_8S); // разрешение работы сторожевого таймера с тайм-аутом 8 с
          Serial.println(F("Set WTD AVR"));
          //WTD_set(1);
        #else
          watchdogSetup();
          WDT->WDT_MR = WDT_MR_WDD(0xFFF)| WDT_MR_WDRPROC| WDT_MR_WDRSTEN | WDT_MR_WDV(256 * 10); 
          // Watchdog triggers a reset after 2 seconds if underflow
          // 2 seconds equal 84000000 * 2 = 168000000 clock cycles
          /* Slow clock is running at 32.768 kHz
          watchdog frequency is therefore 32768 / 128 = 256 Hz
          WDV holds the periode in 256 th of seconds  */   
          status_wdt = (RSTC->RSTC_SR & RSTC_SR_RSTTYP_Msk) >> RSTC_SR_RSTTYP_Pos; // Get status from the last Reset      
          Serial.print(F("Set WTD DUE ")); Serial.print(" RSTTYP = 0b "); Serial.println(status_wdt, BIN);
        #endif
}

void AHB::WTD(void){
        //Serial.println (F(" WTD loop "));
        if  (millis() - prevtime_wtd > 7500) { ///WDTO_8S -1000) 
          //Serial.println(F("Reset WTD"));
          #if defined  (__AVR__)
            wdt_reset();  // сброс сторожевого таймера
            prevtime_wtd = millis();
          #else
            WDT->WDT_CR = WDT_CR_KEY(WDT_KEY)| WDT_CR_WDRSTT;
            GPBR->SYS_GPBR[0] += 1;
            //Serial.print("GPBR = "); Serial.println(GPBR->SYS_GPBR[0]);
            prevtime_wtd = millis();
          #endif
        }
}

//void AHB::watchdogSetup (void) __attribute__ ((weak, alias("_watchdogDefaultSetup")));

bool AHB::WTD_set(bool set_wdt){
       Serial.print(F("WTD!"));
        //WDTO_15MS      // 15 мс 
        //WDTO_30MS      // 30 мс 
        //WDTO_60MS      // 60 мс 
        //WDTO_120MS    // 120 мс 
        //WDTO_250MS    // 250 мс 
        //WDTO_500MS    // 500 мс 
        //WDTO_1S            // 1 сек 
        //WDTO_2S            // 2 сек 
        //WDTO_4S           // 4 сек 
        //WDTO_8S          // 8 сек
        if (set_wdt){
          #if defined  (__AVR__)         
            wdt_enable(WDTO_8S);
            Serial.print(F("Enable WTD"));
            return 1;
          #else
            watchdogEnable(1000);
            return 1;
          #endif
        }
        else{
          #if defined  (__AVR__)
            wdt_disable();
            Serial.print(F("Disable WTD"));
            return 0;
          #else
            watchdogDisable ();
            return 0;
          #endif
        }

}



bool AHB::firstboot(void (*function)()) {
        if(function == NULL) return false;
        if(_nodeId < 1 || _nodeId > 255) {
            #ifdef AHB_DEBUG
                Serial.print(F("Running initial configuration")); Serial.println(); Serial.flush();
            #endif
            function();
            return true;
        }else{
            return false;
        }
}

bool AHB::setNodeId(unsigned int id) {
        if(id < 1 || id > 255) return false;
        _nodeId = id;
        #ifdef AHB_EEPROM_USE
          EEPROM.put(_cfgAddrStart, id);
        #endif
        return true;
}

char AHB::busAttach(AHB_COMM *bus) {
        for(signed char busId=0; busId<AHB_BUSNUM; busId++) {
            if(_busAddr[busId] == 0x00) {
                _busAddr[busId] = bus;
                               
                byte err = bus->begin();
                //#ifdef AHB_DEBUG
                Serial.println();
                Serial.print(F("Attach Bus type - ")); Serial.println(bus->busType()); 
                //#endif
                if(err == 0) {
                    //Boot message
                    byte data[1] = {AHB_CMD_BOOT};
                    bus->ahbSend_V(AHB_PKGTYPE_MULTICAST, 0, 0, 0, _nodeId,  sizeof(data), data);
                    // Нужно вызвать ahbNMT_on_boot(on_boot, *bus);
                    return busId;
                }else{
                    _busAddr[busId] = 0x00;
                }
                return -2;
            }
        }
        return -1;
}

bool AHB::busDetach(signed char busId) {
        if(busId < 0 || busId >= AHB_BUSNUM) return false;
        if(_busAddr[busId] == 0x00) return false;
        _busAddr[busId] = 0x00;
        return true;
}

unsigned int AHB::cfgFindFreeblock(byte bytes, byte id) {
        if(_cfgAddrStart == _cfgAddrStop)  {
            #ifdef AHB_DEBUG
                Serial.print(F("EEPROM space 0")); Serial.println(); Serial.flush();
            #endif
            return 0;
        }
        
        bytes++; //Header
        
        #ifdef AHB_DEBUG
            Serial.print(F("ID is ")); Serial.println(id); Serial.flush();
            Serial.print(F("size is ")); Serial.println(bytes, HEX); Serial.flush();
        #endif

        unsigned int address = _cfgAddrStart+2; //bytes 1+2 are our ID
        byte check,len;

        if(((int)_cfgAddrStop-_cfgAddrStart-bytes) < 0) {
            #ifdef AHB_DEBUG
                Serial.print(F("Space < length")); Serial.println(); Serial.flush();
            #endif
            return 0;
        }

        do {
            #ifdef AHB_EEPROM_USE
            check = EEPROM.read(address);
            #endif
            #ifdef AHB_DEBUG
                Serial.print(F("check addr ")); Serial.println(address); Serial.flush();
                Serial.print(F(" = ")); Serial.println(check, HEX); Serial.flush();
            #endif
            if(check == 0xFF || check == 0) { //Nothing saved yet
                len = 0;
                while(((1 << len) + 5) < bytes) {
                    #ifdef AHB_DEBUG
                        Serial.print(F("Testing length")); Serial.println(len); Serial.flush();
                    #endif
                    len++;
                    if(len > 0x0F) {
                        #ifdef AHB_DEBUG
                            Serial.print(F("length exceeded")); Serial.println(); Serial.flush();
                        #endif
                        return 0;
                    }
                }
                #ifdef AHB_DEBUG
                    Serial.print(F("final length")); Serial.println(len); Serial.flush();
                #endif

                check  = (id << 4) | len;
                
                #ifdef AHB_DEBUG
                    Serial.print(F("ID is now ")); Serial.println(check, HEX); Serial.flush();
                #endif
                
                #ifdef AHB_EEPROM_USE
                EEPROM.write(address, check);
                #endif
                
                return address;
            }

            len = ((1 << (check & 0x0F)) + 5);
            if((check & 0xF0) == 0x00 && len >= bytes) { //block is marked as free and has enough free space
                //@TODO try to split into multiple blocks
                return address;
            //@TODO check if adjacent block is also free
            }else{
                address += len;
            }
        }while(address+bytes < _cfgAddrStop);
        return 0;
}

bool AHB::hookAttach(byte type, unsigned int target, char port, byte firstByte, void (*function)(ahbPacket&)) {
        for(byte i=0; i<AHB_HOOKNUM; i++) {
            if(_hooks[i].execute == 0) {
                _hooks[i].type = type;
                _hooks[i].target = target;
                _hooks[i].port = port;
                _hooks[i].firstByte = firstByte;
                _hooks[i].execute = function;
                return true;
            }
        }
        return false;
}

bool AHB::hookAttachModule(AHB_IO *module) {
        for(byte i=0; i<AHB_MODNUM; i++) {
            if(_module[i] == NULL) {
                module->_control = this;
                _module[i] = (AHB_IO *)module;

                byte id = module->_cfgId;

                //Read configuration
                if(_cfgAddrStart < _cfgAddrStop && id < 128) {
                    id <<= 4;
                    unsigned int address = _cfgAddrStart+2; //bytes 1+2 are our ID
                    byte check,len,num=0;

                    //Round 1 - count objects
                    do {
                        #ifdef AHB_EEPROM_USE
                        check = EEPROM.read(address);
                        #endif
                        len = ((1 << (check & 0x0F)) + 5);
                        if((check & 0xF0) == id) { //this is probably related to our module
                            num++;
                        }
                        address += len;
                    }while(address < _cfgAddrStop && check != 0xFF && check != 0x00);

                    if(
                        num > 0 && 
                        module->cfgReset() && 
                        module->cfgReserve(num)
                    ) {
                        //Round 2 - read objects
                        address = _cfgAddrStart+2;
                        do {
                            #ifdef AHB_EEPROM_USE
                            check = EEPROM.read(address);
                            #endif
                            len = ((1 << (check & 0x0F)) + 5);
                            if((check & 0xF0) == id) { //this is probably related to our module
                                module->cfgRead(address);
                            }
                            address += len;
                        }while(address < _cfgAddrStop && check != 0xFF && check != 0x00);
                        return true;
                    }else{
                        #ifdef AHB_DEBUG
                            Serial.print(F("ERR RES ")); 
                        #endif
                        return false;
                    }
                }
            }
        }
        #ifdef AHB_DEBUG
            Serial.print(F("No free module slot to attach")); Serial.println(); Serial.flush();
        #endif
        return false;
}

bool AHB::hookDetachModule(byte id) {
        for(byte i=0; i<AHB_MODNUM; i++) {
            if(_module[i] != NULL && _module[i]->_cfgId == id && _module[i]->cfgReset()) {
                _module[i] = NULL;
                return true;
            }
        }
        return false;
}


void AHB::system_uptime(void) {
        if (curMillis - prevSystemtime_ut > 1000) {
          ss_ut++; 
          if (ss_ut >=60) {
            ss_ut = 0; mm_ut++; 
            if (mm_ut >=60) {
              mm_ut = 0; hh_ut++; 
              if (hh_ut>=24) {
                dd_ut++; hh_ut=0;
              }
            }
          } 
          prevSystemtime_ut = curMillis;
          
          //PrintSystemUpTime();
          
          if (_nodeType==Master){
            system_hwtime(); //извлечение времени из DS3231
            //PrintSystemHwTime();
          }
          else {
              if (ss_hw< 60) {
                ss_hw++;
              } 
              else {
                ss_hw=0; 
                if (mm_hw < 60) {
                  mm_hw++;
                }
                else {
                  mm_hw=0;
                  if (hh_hw <24) {
                    hh_hw++;
                  }
                  else{
                    hh_hw=0;
                  }
                }
              };
              //PrintSystemHwTime();
          }
        } 
} 

void AHB::PrintSystemUpTime (void){
  //Serial.println(); 
  Serial.print("System Up Time - ");
  if (dd_ut<10)  Serial. print (F("0")); 
  Serial. print (dd_ut);  
  Serial. print (F(" - ")); 
  if (hh_ut<10)  Serial. print (F("0")); 
  Serial. print (hh_ut);  
  Serial. print (F(":")); 
  if (mm_ut<10)  Serial. print (F("0")); 
  Serial. print (mm_ut);  
  Serial. print (F(":")); 
  if (ss_ut<10)  Serial. print (F("0")); 
  Serial. print (ss_ut);  
  Serial. print(F("  "));    
}

void AHB::PrintSystemHwTime (void){
  //Serial.println(); 
  Serial.print("System HW Time - ");
//Время
  if (hh_hw<10)  Serial. print (F("0")); 
  Serial. print (hh_hw, DEC);  
  Serial. print (F(":")); 
  if (mm_hw<10)  Serial. print (F("0")); 
  Serial. print (mm_hw, DEC);  
  Serial. print (F(":")); 
  if (ss_hw<10)  Serial. print (F("0")); 
  Serial. print (ss_hw, DEC);  
  Serial. print (F(" - ")); 
//День недели
  switch(dayOfWeek_hw){

  case 1:
    Serial.print(" Пн. - "); //Serial.println("Monday");
    break;
  case 2:
    Serial.print(" Вт. - "); //Serial.println("Tuesday");
    break;
  case 3:
    Serial.print(" Ср. - "); //Serial.println("Wednesday");
    break;
  case 4:
    Serial.print(" Чт. - "); //Serial.println("Thursday");
    break;
  case 5:
    Serial.print(" Пт. - "); //Serial.println("Friday");
    break;
  case 6:
    Serial.print(" Суб. - "); //Serial.println("Saturday");
    break;
  case 7:
    Serial.print(" Вс. - "); //Serial.println("Sunday");
    break;
  }
//Дата  
  if (dd_hw<10)  Serial. print (F("0")); 
  Serial. print (dd_hw, DEC);  
  Serial. print (F(".")); 
  if (month_hw<10){
    Serial.print("0");
  }  
  Serial.print(month_hw, DEC);
  Serial.print(".20");  
  Serial.print(year_hw, DEC);
//Температура CPU
 Serial.print(" - T CPU - "); //Serial.println(temp_hw);
}




void AHB::system_hwtime(void){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);  
  temp_hw=readDS3231temp();
  ss_hw=second;
  mm_hw=minute;
  hh_hw=hour;
  dayOfWeek_hw=dayOfWeek;
  dd_hw=dayOfMonth;
  month_hw=month;
  year_hw=year;
}


uint8_t AHB::print_ss_ut(void){return ss_ut;}
uint8_t AHB::print_mm_ut(void){return mm_ut;}
uint8_t AHB::print_hh_ut(void){return hh_ut;}
uint8_t AHB::print_dd_ut(void){return dd_ut;}

uint8_t AHB::print_ss_hw(void){return ss_hw;}
uint8_t AHB::print_mm_hw(void){return mm_hw;}
uint8_t AHB::print_hh_hw(void){return hh_hw;}
uint8_t AHB::print_dd_hw(void){return dd_hw;}
uint8_t AHB::print_month_hw(void){return month_hw;}
uint8_t AHB::print_year_hw(void){return year_hw;}
uint8_t AHB::print_dayofweak_hw(void){return dayOfWeek_hw;}

uint8_t AHB::print_ss_rn(uint8_t i){return ss_ut_rn[i];}
uint8_t AHB::print_mm_rn(uint8_t i){return mm_ut_rn[i];}
uint8_t AHB::print_hh_rn(uint8_t i){return hh_ut_rn[i];}
uint8_t AHB::print_dd_rn(uint8_t i){return dd_ut_rn[i];}

// Convert normal decimal numbers to binary coded decimal
byte AHB::decToBcd(byte val){return( (val/10*16) + (val%10) );}
// Convert binary coded decimal to normal decimal numbers
byte AHB::bcdToDec(byte val){return( (val/16*10) + (val%16) );}

float AHB::readDS3231temp()
{
    uint8_t msb, lsb;
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(DS3231_REG_TEMPERATURE);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
    while(!Wire.available()) {};
    msb = Wire.read();
    lsb = Wire.read();
    return ((((short)msb << 8) | (short)lsb) >> 6) / 4.0f;
}

void AHB::setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, 
byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void AHB::readDS3231time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,
byte *month,byte *year)
{

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());

}

byte AHB::ahbSend(uint8_t target, byte len, byte data[8]) { //3 Моя логика
    return ahbSend(0, 0, target, 0, _nodeId, len, data); //[8]
}

byte AHB::ahbSend(uint8_t type, uint8_t target, byte len, byte data[8]) { //4 Моя логика
    return ahbSend(type, 0, target, 0, _nodeId, len, data); //[8]
}

byte AHB::ahbSend(uint8_t type, uint8_t cmd, uint8_t target, byte len, byte data[8]) { //5 Моя логика
    return ahbSend(type, cmd, target, 0, _nodeId, len, data); //[8]
}

byte AHB::ahbSend(uint8_t type, uint8_t cmd,  uint8_t target, uint8_t port, byte len, byte data[8]) { //6 Моя логика
    return ahbSend(type, cmd, target, port, _nodeId, len, data); //[8]
}

byte AHB::ahbSend(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source, byte len, byte data[8]) { //7 Моя логика
        bool state,broadcast_route=false;
        byte errors=0,i;
        signed char busId_route;
        if(source == 0) source = _nodeId;
        busId_route=-1;

        //Определяем в какую шину кидать пакет
        for(signed char busId=0; busId<AHB_BUSNUM; busId++) {
          //Проверяем что шина подключена
          //Serial.println("!"); //_busAddr[busId]->address_network_get(0)
          if (_busAddr[busId] != 0x00){
          
            //Если target broadcast и broadcast есть в этой сети то кидаем туда            
            if (target==_busAddr[busId]->address_network_get(0)){
                broadcast_route=true;
                #ifdef AHB_DEBUG
                Serial.print(F("TX Route target -  "));Serial.print(target);Serial.print(F(" to Bus - "));
                Serial.print(busId);  Serial.print(F(" Form source - "));Serial.println(source);
                #endif //AHB_DEBUG
                //Отправка в шину busId
                //state = _busAddr[busId]->ahbSend_V(type, cmd, target,  port, source, len, data);
                //Отправка в очередь сообщений с busId  
                state = ahbAddSendMsgQueue(type, cmd, target, port, source, len, data, busId);
                if(!state) errors++;                     
            }
            
            //Пробегаем по таблице маршрутов сети
            for (int i=0; i<_busAddr[busId]->net_size_get();i++){
              //Если получатель в сети 
              if (target==_busAddr[busId]->address_network_get(i)&&_busAddr[busId]->address_network_get(i)!=0){
               Serial.print(F("Target ")); Serial.print(target); Serial.print(F(" locate to bus ")); Serial.println(busId);
              busId_route=busId;
              }
            }
          }
        }
        
        if(busId_route!=-1){
          if(_busAddr[busId_route] != 0x00){
             //#ifdef AHB_DEBUG
             Serial.print(F("TX Route target - "));Serial.print(target);Serial.print(F(" to Bus - "));
             Serial.print(busId_route); Serial.print(F(" Form source - "));Serial.println(source);
             //#endif //AHB_DEBUG
             //Отправка в шину busId_route
             //state = _busAddr[busId_route]->ahbSend_V(type, cmd, target,  port, source, len, data);
             
             //Отправка в очередь сообщений с busId_route   
             state = ahbAddSendMsgQueue(type, cmd, target, port, source, len, data, busId_route);
             if(!state) errors++;          
          }
        }
        else{
          if(!broadcast_route) {
            //#ifdef AHB_DEBUG
            Serial.print(F("TX No route for target - ")); Serial.print(target);
            Serial.print(F(" NOT Send !!!!"));  Serial.print(F(" Form source - "));Serial.println(source);
            //#endif //AHB_DEBUG
          }
        }
        return errors;
}

void AHB::ahbReceive(void) {
        ahbPacket pkg;
        ahbReceive(pkg, false);  //нужно видимо установить по умолчанию false
}

bool AHB::ahbReceive(ahbPacket &pkg) {
        return ahbReceive(pkg, true);
}

bool AHB::ahbReceive(ahbPacket &pkg, bool routing) { //Нахуй роутинг вообще вызывается, нужны флаги из конфига
        typedef enum pkg_state_enum {
           pkg_for_unknown,
           pkg_for_my_broadcast,
           pkg_for_my_address,
           pkg_for_net,
           pkg_for_ptp,
           pkg_for_my_broadcast_other_net,   ///
           pkg_for_my_other_net
        }pkg_state_ENUM;

        //Пришедшие сообщения по target (кому) делятся на
        //1. Домашней сети       1.1 Да         1.2 Нет(аварийный  пакет)
        //2. Моему узлу          2.1 Мне.       2.2 Мне по Broadcast 2.3 В другую сеть подключенную ко мне
        //3. Не моему узлу       3.1 Моей сети  3.2 Не моей сети
        //Пришедшие сообщения по source (от кого) делятся на
        //4. От Домашней сети    4.1 Да         4.2 Нет(аварийный  пакет)
        //5. От узла подкл ко мне непосредственно по физике CAN1.1-CAN1.2 или UART1.1-UART1.1
        //6. От узла подк к узлам моих сетей через узлы маршрутизаторы CAN1.1-CAN1.2-MODBUS_RTU2.1-MODBUS_RTU2.2
                  
        //По обработке пакеты делятся на
        //1. Игнорируем если не мне (другому узлу моей сети или узлу другой не моей сети)
        //2. Обрабатываем те что мне от сети или от P-t-P
        //3. Маршрутизируем в другую сеть от сети или от P-t-P
        //4. Ретранслируем в P-t-P (если target пакета принадлежит подключенному ко мне P-t-P)
                    
        bool checkBusReceive = false; //Наличие пакета в шине
        uint8_t ReceiveBusType;       //Тип шины в которую пришел пакет;
        signed char busId_route;
        uint8_t packet_state;
        for(signed char busId=0; busId<AHB_BUSNUM; busId++) { //Перебираем все шины на пришедший пакет
            if(_busAddr[busId] != 0x00) {
                busId_route = 0;
                bool target_my_net = false;
                bool source_my_net = false;
                //Проверяем есть ли пришедний пакет из этой шины
                checkBusReceive = _busAddr[busId]->ahbReceive_V(pkg);
                
                //Если пакет пришел то начинаем его разбор на предмет игнора, ретрансляции или маршрутизации
                if(checkBusReceive) {
                   
                   //Заполняем очередь пришедших сообщений
                   ahbAddRcvMsgQueue(pkg);
                   
                   //Все что ниже вынести в отдельную функцию
                   ReceiveBusType = _busAddr[busId]->busType();
                   pkg.meta.busId = busId;
                   pkg.meta.busType = ReceiveBusType;
                                    
                   packet_state=pkg_for_unknown;
                   
                   //перебираем таблицу маршрутов сети в которой пришел пакет 
                    for (int i=0; i<_busAddr[busId]->net_size_get();i++){ 
                      //смотрим есть ли target в таблице маршрутов, игнорируем 0 не по адресу 0
                      if ((pkg.meta.target==_busAddr[busId]->address_network_get(i)&&_busAddr[busId]->address_network_get(i)!=0)||(_busAddr[busId]->address_network_get(0)==pkg.meta.target&&i==0)){
                        //Если получатель в таблице адресов сети из которой пришел пакет то ставим флаг
                        target_my_net = true;
                        //Serial.println(F("target_my_net = true"));
                      }

                      if (pkg.meta.source==_busAddr[busId]->address_network_get(i)&&_busAddr[busId]->address_network_get(i)!=0){
                        //Если отправитель в таблице адресов сети из которой пришел пакет то ставим флаг
                        source_my_net = true;
                        //Serial.println(F("source_my_net = true"));
                      }
                    }       
                    
                    if (pkg.meta.target==0&&source_my_net==true){ //Если target 0 но source из моей сети 
                      packet_state=pkg_for_my_broadcast; 
                    } 
                    
                    if (pkg.meta.target == _nodeId){ //Если target я
                      packet_state=pkg_for_my_address; 
                    }  
                    
                    bool target_my_other_net = false;
                    if (target_my_net == false) { //Если target не в таблице адресов физ сети то он либо в моей другой сети либо в чужой
                      //Serial.println(F("target_my_net == false!!!"));
                      for(signed char busId_=0; busId_<AHB_BUSNUM; busId_++) { //Перебираем все шины

                        
                        if(_busAddr[busId_] != 0x00) { //Проверка вызван ли класс иначе падает
                          
                          for (int x=0; x<_busAddr[busId]->net_size_get(); x++){                           
                            //смотрим есть ли target в таблице маршрутов, игнорируем 0 не по адресу 0
                            if ((pkg.meta.target==_busAddr[busId_]->address_network_get(x)&&_busAddr[busId_]->address_network_get(x)!=0)||(_busAddr[busId_]->address_network_get(0)==pkg.meta.target&&x==0)){
                                //Если получатель в таблице адресов сети из которой пришел пакет то ставим флаг
                                target_my_other_net = true;
                                busId_route = busId_;
                            }   
                          }
                        }
                      }
                    }
                    
                    if (pkg.meta.target==0&&source_my_net == false) {
                      packet_state=pkg_for_my_broadcast_other_net;
                    }
                    
                    if (target_my_other_net == true) {
                      packet_state=pkg_for_net; 
                    }
                    
                    //packet_state=pkg_for_ptp ПОКА НЕ РЕАЛИЗОВАНО
                    
                    switch (packet_state){
                      case pkg_for_unknown: {
                        #ifdef AHB_DEBUG
                        Serial.print(F("RX target pkg_for_unknown IGNORE"));
                        Serial.print(F(" BUS ID - "));Serial.print(busId);
                        Serial.print(F(" Target - "));Serial.print(pkg.meta.target); 
                        Serial.print(F(" Source - "));Serial.print(pkg.meta.source);
                        Serial.print(F("  Length: 0x"));Serial.print(pkg.len);
                        for(byte i=0; i<pkg.len; i++) {
                          Serial.print(F(" 0x"));Serial.print(pkg.data[i]);
                        }
                        Serial.println();
                        #endif
                        break;
                      }
                      case pkg_for_my_broadcast: {
                        #ifdef AHB_DEBUG
                        Serial.print(F("RX target pkg_for_my_broadcast IN JOB"));
                        Serial.print(F(" BUS ID - "));Serial.print(busId);
                        Serial.print(F(" Target - "));Serial.print(pkg.meta.target);
                        Serial.print(F(" Source - "));Serial.print(pkg.meta.source);
                        Serial.print(F("  Length: 0x"));Serial.print(pkg.len);
                        for(byte i=0; i<pkg.len; i++) {
                          Serial.print(F(" 0x"));Serial.print(pkg.data[i]);
                        }
                        Serial.println();
                        #endif
                        ahbProcess(pkg);
                        ahbRxProcessingNode(pkg);
                        break;
                      }
                      case pkg_for_my_address: {
                        #ifdef AHB_DEBUG
                        Serial.print(F("RX target pkg_for_my_address IN JOB"));
                        Serial.print(F(" BUS ID - "));Serial.print(busId);
                        Serial.print(F(" Target - "));Serial.print(pkg.meta.target);
                        Serial.print(F(" Source - "));Serial.print(pkg.meta.source);
                        Serial.print(F("  Length: 0x"));Serial.print(pkg.len);
                        for(byte i=0; i<pkg.len; i++) {
                          Serial.print(F(" 0x"));Serial.print(pkg.data[i]);
                        }
                        Serial.println();
                        #endif
                        ahbProcess(pkg);
                        ahbRxProcessingNode(pkg);
                        break;
                      }
                      case pkg_for_net: {
                        #ifdef AHB_DEBUG
                        Serial.print(F("RX target pkg_for_net, ROUTE TO BUS - ")); Serial.print(_busAddr[busId_route]->busType());
                        Serial.print(F(" BUS ID - "));Serial.print(busId_route);
                        Serial.print(F(" Target - "));Serial.print(pkg.meta.target);
                        Serial.print(F(" Source - "));Serial.print(pkg.meta.source);
                        Serial.print(F("  Length: 0x"));Serial.print(pkg.len);
                        for(byte i=0; i<pkg.len; i++) {
                          Serial.print(F(" 0x"));Serial.print(pkg.data[i]);
                        }
                        Serial.println();
                        #endif //AHB_DEBUG
                        ahbAddSendMsgQueue(pkg.meta.type, pkg.meta.cmd, pkg.meta.target, pkg.meta.port, pkg.meta.source, pkg.len, pkg.data, busId_route);
                        //ahbSend(pkg.meta.type, pkg.meta.cmd, pkg.meta.target, pkg.meta.port, pkg.meta.source, pkg.len, pkg.data);
                        break;
                      }
                      case pkg_for_ptp: {
                        #ifdef AHB_DEBUG
                        Serial.print(F("RX target pkg_for_ptp, ROUTE TO BUS - ")); Serial.print(_busAddr[busId_route]->busType());
                        Serial.print(F(" BUS ID - "));Serial.print(busId_route);
                        Serial.print(F(" Target - "));Serial.print(pkg.meta.target);
                        Serial.print(F(" Source - "));Serial.print(pkg.meta.source);
                        Serial.print(F("  Length: 0x"));Serial.print(pkg.len);
                        for(byte i=0; i<pkg.len; i++) {
                          Serial.print(F(" 0x"));Serial.print(pkg.data[i]);
                        }
                        Serial.println();
                        #endif // AHB_DEBUG
                        break;
                      }
                      case pkg_for_my_broadcast_other_net: {
                        #ifdef AHB_DEBUG
                        Serial.print(F("RX target pkg_for_my_broadcast_other_net IN JOB"));
                        Serial.print(F(" BUS ID - "));
                        Serial.print(busId_route);
                        Serial.print(F(" Target - "));
                        Serial.print(pkg.meta.target);
                        Serial.print(F(" Source - "));
                        Serial.print(pkg.meta.source);
                        Serial.print(F("  Length: 0x"));
                        Serial.print(pkg.len);
                        for(byte i=0; i<pkg.len; i++) {
                          Serial.print(F(" 0x"));
                          Serial.print(pkg.data[i]);
                        }
                        Serial.println();
                        #endif // AHB_DEBUG
                        ahbProcess(pkg);
                        break;
                      }
                      case pkg_for_my_other_net: {
                        #ifdef AHB_DEBUG
                        Serial.print(F("RX target pkg_for_my_other_net IN JOB"));
                        Serial.print(F(" BUS ID - "));
                        Serial.print(busId_route);
                        Serial.print(F(" Target - "));
                        Serial.print(pkg.meta.target);
                        Serial.print(F(" Source - "));
                        Serial.print(pkg.meta.source);
                        Serial.print(F("  Length: 0x"));
                        Serial.print(pkg.len);
                        for(byte i=0; i<pkg.len; i++) {
                          Serial.print(F(" 0x"));
                          Serial.print(pkg.data[i]);
                        }
                        Serial.println();
                        #endif // AHB_DEBUG
                        ahbProcess(pkg);
                        break;
                      }
                      default:{
                       
                        break;
                      }
                    }
                    
                  /**
                  if(routing) {
                        //Resend to every interface except the one we received it on
                        //Если роутинг кидаем на все шины кроме самого себя - ЭТО Хуйня
                        ahbSend_M(pkg.meta.type, pkg.meta.target, pkg.meta.port, pkg.meta.source, pkg.len, pkg.data, pkg.meta.busId);
                  }
                  */
                  
                  //Еще реализовать ретрансляцию на UART и пр. шины, Видимо в конструкторе класса
                  
                  
                  return true;
                }
            }
        }
        return false;
}

void AHB::ahbRxProcessingNode(ahbPacket &pkg){
        byte i;

        //cc_ut_rn[pkg.meta.source]=pkg.data[4];
        //byte data[4] = {dd_hw,hh_hw, mm_hw, ss_hw}; //Отправляем аппаратное время
        byte data[4] = {dd_ut,hh_ut, mm_ut, ss_ut}; //Отправляем свой UP_time мастеру или запросившему
        bool status_send = false;
        switch(pkg.meta.cmd) {
            case AHB_CMD_F_NMT_PING:{ //Если нам PING отвечаем PONG, если от мастера устанавливаем время
              if (_nodeType!=Master){
               
                 /**Serial.println();
                 Serial.println("Begin date/time - ");
                 Serial.print(pkg.data[0]);Serial.print(" ");
                 Serial.print(pkg.data[1]);Serial.print(" ");
                 Serial.print(pkg.data[2]);Serial.print(" ");
                 Serial.print(pkg.data[3]);Serial.print(" ");
                 Serial.print(pkg.data[4]);Serial.print(" ");
                 Serial.print(pkg.data[5]);Serial.print(" ");
                 Serial.println(pkg.data[6]);
                 Serial.println("End date/time"); */
                 hh_hw        = pkg.data[0];
                 mm_hw        = pkg.data[1];
                 ss_hw        = pkg.data[2];
                 dd_hw        = pkg.data[3];
                 month_hw     = pkg.data[4];
                 year_hw      = pkg.data[5];
                 dayOfWeek_hw = pkg.data[6];
              }
              PrintSystemUpTime();  PrintSystemHwTime(); Serial.println(F("Receive PING"));
              status_send=ahbAddSendMsgQueue(AHB_PKGTYPE_UNICAST, AHB_CMD_F_NMT_PONG, pkg.meta.source, pkg.meta.port, _nodeId, sizeof(data), data, pkg.meta.busId);
              
              #ifdef AHB_DEBUG
              if (status_send) Serial.println(F("AHB_CMD_PONG UNICAST-OK"));
              else Serial.println(F("AHB_CMD_PONG UNICAST-FAIL"));
              #endif
              
              break;
            }
            case AHB_CMD_F_NMT_PONG:{  //Если получили PONG
              NodeGuard_OK[pkg.meta.source][1] = true; //Статус узла Заполняем двумерный массив где [Node ID][Live status]
              //Заполняем полученый UPTIME от узлов
              dd_ut_rn[pkg.meta.source]=pkg.data[0];
              hh_ut_rn[pkg.meta.source]=pkg.data[1];
              mm_ut_rn[pkg.meta.source]=pkg.data[2];
              ss_ut_rn[pkg.meta.source]=pkg.data[3];
              break;
            }
           default:{
                       
              break;
           }        
        }
        
}

void AHB::ahbProcess(ahbPacket &pkg) {
        byte i;
        byte data[8];
        //Internal logic
       
        //modules
        for(i=0; i<AHB_MODNUM; i++) {
            if(_module[i] != NULL) {
                Serial.print(F("exec")); Serial.println(); Serial.flush();
                _module[i]->process(pkg);
            }
        }

        //Hooked functions
        for(i = 0; i<AHB_HOOKNUM; i++) {
            if(_hooks[i].execute != 0) {
                if(
                    (_hooks[i].type == 0xFF || _hooks[i].type == pkg.meta.type) &&
                    (_hooks[i].target == 0  || _hooks[i].target == pkg.meta.target) &&
                    (_hooks[i].port == -1   || _hooks[i].port == pkg.meta.port) &&
                    (_hooks[i].firstByte == 0xFF || (pkg.len > 0 && _hooks[i].firstByte == pkg.data[0]))
                ) {
                    _hooks[i].execute(pkg);
                }
            }
        }
}

bool AHB::ahbAddSendMsgQueue(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source, byte len, byte data[8], signed char busId){
        //Заполняем очередь сообщений
        //Перебор очереди
        for (int i=0; i<30; i++){
            //Если source == 0 то ячейка пустая, т.к source не может быть 0
            if (Message_TX_cell[i][4]==0){
                Message_TX_cell[i][0]=type;
                Message_TX_cell[i][1]=cmd;
                Message_TX_cell[i][2]=target;
                Message_TX_cell[i][3]=port;
                Message_TX_cell[i][4]=source;
                Message_TX_cell[i][5]=len;
                Message_TX_cell[i][6]=data[0];
                Message_TX_cell[i][7]=data[1];
                Message_TX_cell[i][8]=data[2];
                Message_TX_cell[i][9]=data[3];
                Message_TX_cell[i][10]=data[4];
                Message_TX_cell[i][11]=data[5];
                Message_TX_cell[i][12]=data[6];
                Message_TX_cell[i][13]=data[7];
                Message_TX_cell[i][14]=busId;
                #ifdef AHB_DEBUG
                Serial.print(F("TX Message add to Queue. Target - "));Serial.print(target);
                Serial.print(F(" Bus - ")); Serial.println(busId);
                #endif //AHB_DEBUG
                return true; //Сообщение добавлено в очередь  
            }            
        } 
        #ifdef AHB_DEBUG
        Serial.print(F("TX Queue FULL!!!! Message do not add to Queue. Target - "));Serial.print(target);
        Serial.print(F(" Bus - ")); Serial.println(busId);
        #endif // AHB_DEBUG
        return false; //Очередь переполнена. Сообщение НЕ добавлено в очередь
} 
           
byte AHB::ahbSendMsgQueue(){
        bool state=false;
        //Отправляем сообщения и очищаем при успехе очередь
        for (int i=0; i<30; i++){
          //Если source != 0 то ячейка не пустая, т.к source не может быть 0, отправляем сообщение
            if (Message_TX_cell[i][4]!=0){
                byte data[8];
                data[0]=Message_TX_cell[i][6];
                data[1]=Message_TX_cell[i][7];
                data[2]=Message_TX_cell[i][8];
                data[3]=Message_TX_cell[i][9];
                data[4]=Message_TX_cell[i][10];
                data[5]=Message_TX_cell[i][11];
                data[6]=Message_TX_cell[i][12];
                data[7]=Message_TX_cell[i][13];
                if (curMicros-prevTimeSndMsgQueue>10000){
                  state = _busAddr[Message_TX_cell[i][14]]->ahbSend_V(Message_TX_cell[i][0], Message_TX_cell[i][1], Message_TX_cell[i][2],  Message_TX_cell[i][3], Message_TX_cell[i][4], Message_TX_cell[i][5], data);
                  prevTimeSndMsgQueue=curMicros;
                }
                if (state) {
                    #ifdef AHB_DEBUG
                    Serial.print(F("TX Message send from Queue . Target - "));Serial.print(Message_TX_cell[i][2]);
                    Serial.print(F(" Bus - ")); Serial.println(Message_TX_cell[i][7]);
                    #endif //AHB_DEBUG
                    Message_TX_cell[i][0]=0;
                    Message_TX_cell[i][1]=0;
                    Message_TX_cell[i][2]=0;
                    Message_TX_cell[i][3]=0;
                    Message_TX_cell[i][4]=0;
                    Message_TX_cell[i][5]=0;
                    Message_TX_cell[i][6]=0;
                    Message_TX_cell[i][7]=0;
                    Message_TX_cell[i][8]=0;
                    Message_TX_cell[i][9]=0;
                    Message_TX_cell[i][10]=0;
                    Message_TX_cell[i][11]=0;
                    Message_TX_cell[i][12]=0;
                    Message_TX_cell[i][13]=0;
                    Message_TX_cell[i][14]=0;
                    return true;    
                }
                else {
                   #ifdef AHB_DEBUG
                   Serial.print(F("TX Message NOT send from Queue . Target - "));Serial.print(Message_TX_cell[i][2]);
                   Serial.print(F(" Bus - ")); Serial.println(Message_TX_cell[i][7]);
                   #endif //AHB_DEBUG
                   return false;
                }
               
            }
           
        }
} 

bool AHB::ahbAddRcvMsgQueue(ahbPacket &pkg){ // Очередь приема команд

}

void AHB::ahbNodeGuard(uint8_t bus_Type) {
      bool state =false;
      uint8_t CMD = AHB_CMD_F_NMT_PING;
      byte data[7] = {hh_hw, mm_hw, ss_hw, dd_hw, month_hw, year_hw, dayOfWeek_hw};
      //Если узел мастер то 
      if (_nodeType==Master){
         if  (curMillis-prevTimeNodeGuard > intervalNodeGuard*1000) {        
            for(signed char busId=0; busId<AHB_BUSNUM; busId++) {
               //перебираем все шины и находим CAN
               if(_busAddr[busId] != 0x00&&_busAddr[busId]->busType()==bus_Type) {
                  
                  for (int i=0; i<_busAddr[busId]->net_size_get();i++){ 
                       //смотрим есть ли target в таблице маршрутов, игнорируем 0 не по адресу 0
                     if (_busAddr[busId]->address_network_get(i)!=0){                         
                        NodeGuard_OK[_busAddr[busId]->address_network_get(i)][0] = true; //Наличие в таблице маршрутов

                        //Добавим в очередь
                        state=ahbAddSendMsgQueue(1, CMD, _busAddr[busId]->address_network_get(i), 0, _nodeId, sizeof(data),data , busId);
                        // Если не удалось отправить PING - очередь переполнена
                        if (!state) {
                          //  state=ahbAddSendMsgQueue(1, CMD, _busAddr[busId]->address_network_get(i), 0, _nodeId, sizeof(data),data , busId);
                            ahbSendMsgQueue();
                            ahbAddSendMsgQueue(1, CMD, _busAddr[busId]->address_network_get(i), 0, _nodeId, 1,data , busId);
                        }
                        NodeGuard_OK[i][1]=false; //Статус узла
                     }
                  }  
                  
                  //int i;
                  //i=num_node_loop;
                  //state=ahbSend(1, CMD, _busAddr[busId]->address_network_get(i), 0, _nodeId, sizeof(data),data);
                  //num_node_loop++;
                  //if (num_node_loop>_busAddr[busId]->net_size_get()) {num_node_loop=0;}
                  
               }
            }
            prevTimeNodeGuard = curMillis;
         }
      }
      

    //перебираем все таблицы маршрутов CAN
    //отправляем PING
    //Заполняем таблицу PONG
    
    //Выводим таблицу PONG
    
    //Если узел слейв то ждем от мастера пинг 30 сек
    //Если получили то сбрасываем таймер
    //Если не получили PONG то PING мастеру
    //Если получили PONG то сбрасываем таймер
    //Если не получили PONG то ???????? 
    
    //Если узел Node то ждем от мастера пинг 30 сек
    //Если получили PONG то сбрасываем таймер
    //Если не получили PONG то PING мастеру
    //Если получили PONG то сбрасываем таймер
    //Если не получили PONG мастера то PING слейву
    //Если получили PONG то ???????
}

void AHB::ahbNodeGuard_print(void) {
      if  (curMillis-prevTimeNodeGuard_print > intervalNodeGuard_print*1000) {
          //Serial.println(F("Node Guard"));
           for (int i=0; i<255;i++){ //Бежим по всему адресному пространству всей сети!!! 1-255, а не конкретной CAN сети
                if (NodeGuard_OK[i][0]){ //Наличие узла в таблице маршрутов
                    Serial.print(F("Node - "));Serial.print(i); 
                    if (NodeGuard_OK[i][1]) { //Статус узла
                        Serial.println(F(" is - OK"));
                    }
                    else {
                        Serial.println(F(" is - "));
                    }
                }
           }
           prevTimeNodeGuard_print = curMillis;
      }
}


ahbPacket AHB::loop(void) {
        WTD();
        byte i;
        curMillis = millis(); // print system time
        curMicros = micros();
        ahbPacket pkg;
        
        // здесь основной код протокола NMT etc провереа флагов

        //Packet handling
        //This only receives a single packet. We could loop here, but doing it this way 
        //allows the user code to still somewhat execute in environments with a lot of messages…
        ahbReceive(pkg);
        if (_nodeType==Master){
          ahbNodeGuard(type_CAN); 
          ahbNodeGuard_print();
        }
        //Modules
        for(i=0; i<AHB_MODNUM; i++) {
            if(_module[i] != NULL) {
                _module[i]->loop();
            }
        }
        
        //Отправка очереди ahbSendMsgQueue()
        ahbSendMsgQueue();
        
        system_uptime(); //Подсчет Up Time
        

        
        
        //SetAlarm(0, &errorStateBlink, 62, 62);       
        //установка флагов по таймерам
        //ahbTimers(_nodeType);       
        //Отправка из очереди
        //ahbSend_M_Queue();
        //ahbNMT(_nodeType);
        //ahbHeartbeat(_nodeType);
        //ahbPDO();
        //ahbSDO();
        //ahbSync(_nodeType);
        //ahbEmcy(_nodeType);

        
        return pkg;
}



//void AHB::ahbTimers() {}

#endif /* AHB__C */

/**
byte AHB::ahbSend_M(byte type, unsigned int target, unsigned int source, char port, byte len, byte data[8], uint8_t busType,signed char skip) { //8

        bool state;
        byte errors=0,i;
        if(source == 0) source = _nodeId;

        //Здесь полная хуйня, пакет отправляется во все сети перебирая все интерфейсы. 
        //Внутри я уже проверяю тип, далее проверяем маршруты
        //А вот надо ли заранее передавать тип интерфейса?        
       //Откуда я знаю заранее какой адрес какого типа.       
       //Нужно с типом оставить но ваять новый код      
       //!!! есть два слоя AHB::AHB это софт слой и AHB::AHB_COMM это хард слой с железом
       //1.AHB::AHB в этом слое работает сложный роутинг и выбор типа интерфейса и протокола зависит от таблиц маршрутизации
       //2.AHB::AHB_COMM (AHB_CAN, AHB_UART, AHB_MODBUS_RTU) здесь все более хардово, выбор типа протокола и пр.
       //3.AHB_CAN и пр. здесь без выбора типа (уже определен классом, но полный доступ к железу)
       //Причем нас интереует ??? ТХ в основном, по RX думаем.
               
        for(signed char busId=0; busId<AHB_BUSNUM; busId++) {
          //Нужно переделать фильтр не на пропускает а куда кидать
          if (_busAddr[busId] != 0x00){
            for (int i=0; i<255;i++){
              if (_busAddr[busId]->address_network_get(i)==target&&_busAddr[busId]->address_network_get(i)!=0){
              Serial.print(F("@@@ TX target Addr in my network type - "));
              Serial.print(_busAddr[busId]->busType());
              Serial.print(F(" - is - "));
              Serial.println(_busAddr[busId]->address_network_get(i));
              }
            }
            
            if( _busAddr[busId]->busType()==busType){ //если шина CAN то кидаем в нее, нудно еще проверять на принадлежность сети
          
              Serial.print(F("@@@ TX SEND TO BUS - "));Serial.println(_busAddr[busId]->busType());
              //Skip не пропускает сам на себя из ресива но кидает во все шины а это ХРЕНЬ!!!!!
              if(_busAddr[busId] != 0x00 && busId != skip) {
                state = _busAddr[busId]->ahbSend_V(type, 0, target,  port, source, len, data);
                if(!state) errors++;
              }
              
            }
            
            
          }
        }
        
        //Skip не пропускает сам на себя из ресива
        if(skip < 0) {
            //Local source, check for actions
            ahbPacket pkg;
            pkg.meta.type = type;
            pkg.meta.target = target;
            pkg.meta.source = source;
            pkg.meta.port = port;
            pkg.len = len;
            for(i=0; i<len; i++) pkg.data[i] = data[i];

            ahbProcess(pkg);
        }
        return errors;

}
*/