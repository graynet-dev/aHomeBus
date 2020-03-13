/**
  aHomeBus Modbus RTU interface

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

  This interface depends on the  Modbus-Master-Slave-for-Arduino library.
    Download: https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino

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

#ifndef AHB_MODBUS_RTU__C
#define AHB_MODBUS_RTU__C

    #include "ahb_comm_MODBUS_RTU.h"
    #include "ahb_proto.h"
    #include "ModbusRtu.h" 

AHB_MODBUS_RTU::AHB_MODBUS_RTU():
_interface(){

}

AHB_MODBUS_RTU::AHB_MODBUS_RTU(uint8_t u8id):
_interface(u8id){

}

AHB_MODBUS_RTU::AHB_MODBUS_RTU(uint8_t u8id, uint8_t u8serno):
_interface(u8id,u8serno){

}

AHB_MODBUS_RTU::AHB_MODBUS_RTU(uint8_t u8id, uint8_t u8serno, uint8_t u8txenpin, uint8_t *route_table, uint8_t netsize):
_interface(u8id,u8serno,u8txenpin){

       all_network_address = new uint8_t[netsize];

       for (int i=0; i<netsize; i++){
            this->all_network_address[i]=route_table[i];
            #ifdef AHB_DEBUG
            //Serial.print(F("Address network CAN - "));Serial.println(all_network_address[i]);
            #endif //AHB_DEBUG
       } 
        //this->
       net_size = netsize;  
}

void AHB_MODBUS_RTU::SetNodeId(uint8_t nodeId) {}

byte AHB_MODBUS_RTU::begin() {
     _interface.begin(9600); // baud-rate at 19200
     _interface.setTimeOut( 5000 ); // if there is no answer in 5000 ms, roll over
  //modbus_t telegram[2];
  //u32wait = millis() + 1000;
  //u8state = u8query = 0; 
     lastErr = 0; //Херня!!!! Обработать наличие ошибок. Лезть в класс чужой либы.

     // telegram 0: read
  telegram[0].u8id = 2;//Slaves; // slave address
  telegram[0].u8fct = 3; // function code (this one is registers read)
  telegram[0].u16RegAdd = 0; // start address in slave
  telegram[0].u16CoilsNo = 11; // number of elements (coils or registers) to read
  telegram[0].au16reg = au16data; // pointer to a memory array in the Arduino
  
    // telegram 1: write a single register
  telegram[1].u8id = 10;//; // slave address
  telegram[1].u8fct = 6; // function code (this one is write a single register)
  telegram[1].u16RegAdd = 1; // start address in slave
  telegram[1].u16CoilsNo = 1; // number of elements (coils or registers) to read
  telegram[1].au16reg = au16data+6; // pointer to a memory array in the Arduino       

        return lastErr;
}

uint8_t AHB_MODBUS_RTU::net_size_get(void){
        return net_size;
}

uint8_t AHB_MODBUS_RTU::address_network_get(uint8_t i){
        return all_network_address[i];   
}

uint8_t AHB_MODBUS_RTU::busType(){
        return type_MODBUS_RTU;
}


bool AHB_MODBUS_RTU::ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]){
       //Serial.println("Send Modbus_RTU");
        // Разбираем ввод и формируем телеграмму
        //type
        //cmd
        //port
        //data[8]
        //Из них тянем данные
        //source куда вернуть ответ при приеме
  telegram[0].u8id = target;//Slaves; // slave address
  telegram[0].u8fct = 3; // function code (this one is registers read)
  telegram[0].u16RegAdd = 0; // start address in slave
  telegram[0].u16CoilsNo = 11; // number of elements (coils or registers) to read
  telegram[0].au16reg = au16data; // pointer to a memory array in the Arduino        
        if (!_interface.query(telegram[0])){//если 0 т.е нормальная отправка
          //1. Установить флаг что была отправка и можно начать поллинг
          send_modbus_rtu=true;
          modbus_rtu_request_sender=source;
        }

}

bool AHB_MODBUS_RTU::ahbReceive_V(ahbPacket &pkg){
        int x;
        //Serial.println("Receive Modbus_RTU");
        //1 Если нгет флага отправки то поллиг не начинаем. Если есть начали поллинг
        //Если результат поллинга данные то выдаем и снимаем флаг
        //Если флаг не снят в течении 15 сек перестаем полить
        if (send_modbus_rtu){
            x=_interface.poll(); // check incoming messages
            if (_interface.getState() == COM_IDLE) { //Это что за проверка такая?
              //Serial.println(x);
              //Нужно сформировать pkg
              //pkg.meta = ahbCanAddrParse(rxId);
              //pkg.meta.type =
              //pkg.meta.cmd =
              //pkg.meta.port =
              //pkg.meta.target =
              //pkg.meta.source =
              //pkg.meta.busId =
              //pkg.meta.busType =
              //pkg.len =
              //
              if (telegram[0].au16reg[0]==0) {
                Serial.println("Power - OFF");
              }
              else{
                Serial.println("Power - ON");
              }   //telegram[0].au16reg[0] , DEC
              Serial.print(" Fan - ");
              if (telegram[0].au16reg[1]==0) {Serial.println("Auto speed");}
              else if (telegram[0].au16reg[1]==1){Serial.println("High speed");}
              else if (telegram[0].au16reg[1]==2){Serial.println("Mid speed");}
              else if (telegram[0].au16reg[1]==3){Serial.println("Low speed");}
              //Serial.println(telegram[0].au16reg[1] , DEC);
              //Serial.print(" ");
              Serial.print(" Home - ");
              if (telegram[0].au16reg[2]==0) {Serial.println("Cooling");}
              if (telegram[0].au16reg[2]==1) {Serial.println("Heating");}
              if (telegram[0].au16reg[2]==2) {Serial.println("Ventilation");}
              //Serial.println(telegram[0].au16reg[2] , DEC);
              //Serial.print(" ");
              Serial.print(" Temp - ");
              Serial.println(telegram[0].au16reg[3]/10 );
              //Serial.print(" ");
              Serial.print(" Display is - "); 
              if (telegram[0].au16reg[4]=0){ Serial.println("Unlock");}else{Serial.println("Lock");}
              //Serial.print(" ");
              Serial.print(" MM - ");
              Serial.print(telegram[0].au16reg[5] , DEC);  
              //Serial.print(" ");
              Serial.print(" HH - ");
              Serial.print(telegram[0].au16reg[6] , DEC);
              //Serial.print(" ");
              Serial.print(" Week- ");
              Serial.println(telegram[0].au16reg[7] , DEC);
              //Serial.print(" ");
              Serial.print(" Room temp - ");
              Serial.println(telegram[0].au16reg[8]/10 );
              //Serial.print(" ");
              Serial.print(" Valve - ");
              if (telegram[0].au16reg[9]==0) {Serial.println("Off");} else {Serial.println("On");}
              //Serial.println(telegram[0].au16reg[9] , DEC);
              //Serial.print(" ");
              Serial.print(" FAN2 - ");
              if (telegram[0].au16reg[10]==0) {Serial.println("0");} 
              else if (telegram[0].au16reg[10]==1){Serial.println("Hi");}
              else if (telegram[0].au16reg[10]==2){Serial.println("Mi");}
              else if (telegram[0].au16reg[10]==3){Serial.println("Lo");}
              else if (telegram[0].au16reg[10]==4){Serial.println("Off");}
              Serial.println("__________________________________");
              send_modbus_rtu=false;
            }          
        
        }
}

  
#endif /* AHB_MODBUS_RTU__C */
