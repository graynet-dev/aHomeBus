/**
  aHomeBus CAN to UDP gateway

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

#ifndef AHB_GW_CAN_TO_OKBIT_C
#define AHB_GW_CAN_TO_OKBIT_C

    #include "ahb_gw_can_to_okbit.h"
   
    



AHB_GW_CAN_TO_OKBIT::AHB_GW_CAN_TO_OKBIT(uint32_t localport, uint32_t remoteport, IPAddress gw_ip,int sub_id, int id, int device, int firmware1, int firmware2): //, 
_interface(){

 dest_gw_can_to_okbit = gw_ip;
 localPort_okbit= localport;
 remPort_okbit=remoteport;
 
  _sub_id = sub_id;
  _id = id;
  _device = device;
  _firmware[0] = firmware1;
  _firmware[1] = firmware2;

}



byte AHB_GW_CAN_TO_OKBIT::begin() {
     _interface.begin(localPort_okbit);
  
    Serial.print("GW_CAN_TO_UDP address ");
    Serial.print(dest_gw_can_to_okbit);
    Serial.print(" on Remote port ");
    Serial.print(remPort_okbit);  
    Serial.print(" on local port ");
    Serial.println(localPort_okbit);

    //////_HandlerUDP.begin(sub_id, id, device, firmware[0], firmware[1]); //подключение класса обработчика протокола OkBIT UDP
    
    //EEPROM.begin(512);
    //_HandlerUDP.eeprom_read();
    
     lastErr = 0; //Херня!!!! Обработать наличие ошибок. Лезть в класс чужой либы.
    
        return lastErr;
}

uint8_t AHB_GW_CAN_TO_OKBIT::checkError(void){}
uint8_t AHB_GW_CAN_TO_OKBIT::errorCountRX(void){}
uint8_t AHB_GW_CAN_TO_OKBIT::errorCountTX(void){}
uint8_t AHB_GW_CAN_TO_OKBIT::getError(void){}

uint8_t AHB_GW_CAN_TO_OKBIT::net_size_get(void){
        return net_size;
}

uint8_t AHB_GW_CAN_TO_OKBIT::address_network_get(uint8_t i){
        return all_network_address[i];   
}

uint8_t AHB_GW_CAN_TO_OKBIT::busType(){
        return type_GW_CAN_TO_OKBIT;
}

void AHB_GW_CAN_TO_OKBIT::parsing(char inPacket[255], int len, uint32_t gateIP) {

  String uStart;//udp start
  int packet_to_dec[30];// массив значений пакета в DEC
  char bufChar[2]; // буферная вспомогательная строка
  unsigned int crc; // контрольная сумма пакета

  for (int i = 0; i < 22; i = i + 1) { //udp start packet
    uStart = uStart + String(inPacket[i]);
  }

  crc = 0;
  for (int i = 0; i < len; i = i + 2) { //разбивка пакета на байты
    (String(inPacket[i]) + String(inPacket[1 + i])).toCharArray(bufChar, 3);
    sscanf(bufChar, "%x", &packet_to_dec[i / 2]);
    if (len - 4 > i) crc = crc + packet_to_dec[i / 2];
  }

  unsigned int crcPack = ((packet_to_dec[(len / 2 - 1) - 1] << 8) | packet_to_dec[(len / 2 - 1)]);


  if (uStart != "4F4B4249542D554450AAAA") {

    this->status_err = 0;
  }
  else {
    this->status_err = 1;
    if (crc != crcPack) {
      this->status_err = 0;
    }
    else {
      this->status_err = 1;

      int long_pack  = packet_to_dec[11]; //длина пакета

      unsigned int in_sub_id = packet_to_dec[12]; //подсеть ID

      unsigned int in_id = packet_to_dec[13]; //ID

      unsigned int in_device = (packet_to_dec[14] << 8) | packet_to_dec[15] ; //код девайса

      in_cmd = (packet_to_dec[16] << 8) | packet_to_dec[17] ; //код команды

      unsigned int in_subto_id = packet_to_dec[18]; //подсеть ID

      unsigned int in_to_id = packet_to_dec[19]; //ID

      if (long_pack == 11 || long_pack == 13 || long_pack == 15 || long_pack == 17 || long_pack == 19 ) {
        _vol[1] = (packet_to_dec[20] << 8) | packet_to_dec[21] ; //значение 1
      }
      if (long_pack == 13 || long_pack == 15 || long_pack == 17 || long_pack == 19 ) {
        _vol[2] = (packet_to_dec[22] << 8) | packet_to_dec[23] ; //значение 2
      }
      if (long_pack == 15 || long_pack == 17 || long_pack == 19 ) {
        _vol[3] = (packet_to_dec[24] << 8) | packet_to_dec[25] ; //значение 2
      }
      if (long_pack == 17 || long_pack == 19 ) {
        _vol[4] = (packet_to_dec[26] << 8) | packet_to_dec[27] ; //значение 2
      }
      if (long_pack == 19 ) {
        _vol[5] = (packet_to_dec[28] << 8) | packet_to_dec[29] ; //значение 2
      }

    }
  }

  if (this->status_err == 1) {

    if (this->in_cmd == 30) {

      holdingRegs[_vol[1]] = _vol[2];
      _holdingRegs[_vol[1]] = _vol[2];
      String backup = "000B";
      backup.toCharArray(replyGate, backup.length() + 2);
    }


    if (this->in_cmd == 255 || this->in_cmd == 20) {
     unsigned long mid = _nodeId; //ESP.getChipId();
     //unsigned long mid = 1;
      unsigned int mid_b[2];
      mid_b[0] = mid >> 16;
      mid_b[1] = mid & 0xFFFF;

      this->build(_sub_id, _id, _device, 13, _sub_id, _id, _firmware[0], _firmware[1], mid_b[0], mid_b[1]);//передача верcие прошивки и серийного номера на сборку пакета
    }

    if (this->in_cmd == 64) {
      #ifdef AHB_EEPROM_USE
       this->eeprom_write(gateIP);
      #endif
      String backup = "000B";
      backup.toCharArray(replyGate, backup.length() + 2);
    }
  }
}

void AHB_GW_CAN_TO_OKBIT::build(int b_sub_id, int b_id, int b_device, int b_cmd, int b_subto_id, int b_to_id, unsigned int b_vol1, unsigned int b_vol2, unsigned int b_vol3, unsigned int b_vol4) {
  String b_pack;
  char myStr[3];
  int b_len_pack;
  String buf_pack;

  b_pack = "4F4B4249542D554450AAAA";

  if (b_cmd == 10) b_len_pack = 9;  //считать SN шлюза, версию прощивки
  if (b_cmd == 11) b_len_pack = 9;  //ответ все хорошо
  if (b_cmd == 20) b_len_pack = 9; //Поиск всех онлайн устройств
  if (b_cmd == 25) b_len_pack = 9; //Считать/передать все значения ячеек ОЗУ
  if (b_cmd == 63) b_len_pack = 9; //Работа шлюза по DHCP
  if (b_cmd == 64) b_len_pack = 9; //Привязать шлюз к серверу  
  if (b_cmd == 255) b_len_pack = 9; //поиск всех Шлюзов и ethernet устройств по широковещательному запросу

  if (b_cmd == 26) b_len_pack = 11; //Считать/передать количество ошибок на шине ( 1- кол-воошибок)
  if (b_cmd == 40) b_len_pack = 11; //Смена Sub ID (1 - новый адрес подсети)  
  if (b_cmd == 41) b_len_pack = 11; //Смена ID (1 - новый адрес устройства)  
  
  if (b_cmd == 21) b_len_pack = 13; //Считать/передать тип устройства, версию прошивки (1- тип устройства, 
                                   //2 - версия)
  if (b_cmd == 23) b_len_pack = 13; //Считать/передать статус входа (1 - адрес входа, 2- значение)
  if (b_cmd == 24) b_len_pack = 13; //Считать/передать значение ячейки ОЗУ (1 - адрес ячейки, 2 - значение)
  if (b_cmd == 30) b_len_pack = 13; //Присвоение значения ОЗУ (1 - адрес канал, 2 - значение)
  if (b_cmd == 60) b_len_pack = 13; //Запись значения настройки в модуль (1 - адрес канала, 2 - значение )

  if (b_cmd == 31) b_len_pack = 15; //Присвоение двух значений ОЗУ (1 - адрес канала, 2 - значение 1, 
                                    // 3 - значение 2)                
  if (b_cmd == 61) b_len_pack = 15; //Запись значений настройки в модуль (1 - адрес канала, 2 - значение 1, 
                                    //3- значение 2)  
                                    
  if (b_cmd == 12) b_len_pack = 17;  //ответ ошибка (1 - колличество ошибок в ОЗУ, 2- код последней ошибки)
                                     //0013/D - передать SN шлюза, версию прощивки (1 - Значение прошивки 1, 
                                     //2 - значение прошивки 2, 3 - серийный номер 1, 4 - серийный номер 2)
  if (b_cmd == 13) b_len_pack = 17; //передать SN шлюза, версию прощивки 
                                    //(1 - Значение прошивки 1, 2 - значение прошивки 2, 
                                    //3 - серийный номер 1, 4 - серийный номер 2)
  if (b_cmd == 62) b_len_pack = 17; //смена IP шлюза (1 - адрес, 2 - адрес, 3 - адрес, 4 - адрес)
  
  if (b_cmd == 22) b_len_pack = 19; //Считат/передать коментарий устройства(n-е количество буквенный коментарий)
  if (b_cmd == 55) b_len_pack = 19; //Запись текстового значения (примечание для модуля)
  
  if (b_cmd == 70) b_len_pack = 23; // Запись значения сценария (1 - адрес канала,  2 - N -сценария, 
                                    //3 - Sub ID получателя, 4 - id получателя , 5 - адрес канала ОЗУ,
                                    //6 - значение 1, 7 - значение 2)
  
  //Длинна пакета 11,13,15,17,19

  sprintf(myStr, "%02X", b_len_pack );
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_sub_id );
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_id );
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_device >> 8);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_device & 0xFF);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_cmd >> 8);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_cmd & 0xFF);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_subto_id);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", b_to_id);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  if (b_len_pack == 13 || b_len_pack == 15 || b_len_pack == 17) {
    sprintf(myStr, "%02X", b_vol1 >> 8);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;

    sprintf(myStr, "%02X", b_vol1 & 0xFF);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;

    sprintf(myStr, "%02X", b_vol2 >> 8);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;

    sprintf(myStr, "%02X", b_vol2 & 0xFF);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;
  }

  if (b_len_pack == 15 || b_len_pack == 17) {
    sprintf(myStr, "%02X", b_vol3 >> 8);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;

    sprintf(myStr, "%02X", b_vol3 & 0xFF);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;
  }

  if ( b_len_pack == 17) {
    sprintf(myStr, "%02X", b_vol4 >> 8);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;

    sprintf(myStr, "%02X", b_vol4 & 0xFF);
    buf_pack = myStr;
    b_pack = b_pack +  buf_pack;
  }



  int packet_to_dec[30];
  char b_myStr[100];
  char bufChar[2];
  int len = b_pack.length() + 3;

  b_pack.toCharArray( b_myStr, len);



  unsigned int crc = 0;
  for (int i = 0; i < len; i = i + 2) { //разбивка пакета на байты
    (String( b_myStr[i]) + String( b_myStr[1 + i])).toCharArray(bufChar, 3);
    sscanf(bufChar, "%x", &packet_to_dec[i / 2]);
    if (len - 4 > i) crc = crc + packet_to_dec[i / 2];
  }

  sprintf(myStr, "%02X", crc >> 8);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  sprintf(myStr, "%02X", crc & 0xFF);
  buf_pack = myStr;
  b_pack = b_pack +  buf_pack;

  b_pack.toCharArray(replyGate, b_pack.length() + 2);

}
#ifdef AHB_EEPROM_USE

void AHB_GW_CAN_TO_OKBIT::eeprom_read() {
  //for (int i=0; i<4; i++) {
  //gateIP[i] = EEPROM.read(i);
  //}
 
}

void AHB_GW_CAN_TO_OKBIT::eeprom_write(uint32_t gateIP) {
  //EEPROM.write(3, gateIP >> 24);
  //EEPROM.write(2, (gateIP & 0xFF0000) >> 16);
  //EEPROM.write(1, (gateIP & 0xFF00) >> 8);
  //EEPROM.write(0, (gateIP & 0xFF));
  //delay(100);
  //        //EEPROM.commit(); //для ESP8266
  //this->eeprom_read();
}

#endif

void AHB_GW_CAN_TO_OKBIT::holding_update() {
  for (int i = 0; i < 10; i++) {
    if (_holdingRegs[i] != holdingRegs[i]) { //если старое значение не равно новому
      _holdingRegs[i] = holdingRegs[i];//присвоить старому значению новое
      this->build(_sub_id, _id, _device, 30, _sub_id, _id, i, holdingRegs[i]);//передача верcие прошивки и серийного номера на сборку пакета
    status_err = 1;
      return;
    }
    else status_err = 0;
    
  }
}

void AHB_GW_CAN_TO_OKBIT::SetNodeId(uint8_t nodeId){
_nodeId=nodeId;
}

bool AHB_GW_CAN_TO_OKBIT::ahbSend_V(uint8_t type, uint8_t cmd, uint8_t target, uint8_t port, uint8_t source,  uint8_t len, byte data[8]){
      //uint16_t type_gw;
      //type_gw = 8000+source;
      //this->build(2, 4, 8004, 13, 0, 0, 5, 6, 1, 4);
      this->build(target,source+target*100, 8000+source, cmd, 0, 0, 1, 1, target, source);     
      _interface.beginPacket(_interface.remoteIP(), _interface.remotePort());
      _interface.write(replyGate);
      _interface.endPacket();  

       //uint32_t rxId = 0x80000000;
       //rxId = (type & 0xFFFFFFFF)<<28 | (cmd & 0xFFFFFFFF)<<20 | (target & 0xFFFFFFFF)<<12 | (port & 0xFFFFFFFF)<<8 | source ;
       //sprintf(buffer, "ID: %.8lX  Data: %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n\r",
       //       rxId, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
       //Serial.print("Okbit ");Serial.print(dest_gw_can_to_okbit);Serial.print(":");Serial.println(remPort_okbit);             
     
}



bool AHB_GW_CAN_TO_OKBIT::ahbReceive_V(ahbPacket &pkg){

  int packetSize = _interface.parsePacket();
  if (packetSize)
  {
    // получаем входящие UDP-пакеты:
    Serial.print("Received "); //  "Получено %d байт от %s, порт %d%"
    Serial.print(packetSize);
    Serial.print(" bytes from ");
    Serial.print(_interface.remoteIP());
    Serial.print(", port ");
    Serial.println(_interface.remotePort());
    
    int len = _interface.read(incomingPacket, 255);
    if (len > 0){
      incomingPacket[len] = 0;
    }
    Serial.print("Okbit UDP packet contents: ");
    Serial.println(incomingPacket);
    
    parsing(incomingPacket, len, _interface.remoteIP());//обработка полученного пакета

    // отправляем ответ на IP-адрес и порт, с которых пришел пакет:
    if (status_err == 1) {
     status_err = 0;
      _interface.beginPacket(_interface.remoteIP(), _interface.remotePort());
      _interface.write(replyGate);
      _interface.endPacket();
       
      //this->build(2, 4, 8004, 13, 0, 0, 5, 6, 1, 4); //передача верcие прошивки и серийного номера на сборку пакета
     // _interface.beginPacket(_interface.remoteIP(), _interface.remotePort());
     // _interface.write(replyGate);
     // _interface.endPacket();  
      
      //this->build(2, 4, 8004, 22, 0, 0, 7777);//передача верcие прошивки и серийного номера на сборку пакета
      //_interface.beginPacket(_interface.remoteIP(), _interface.remotePort());
      //_interface.write(replyGate);
      //_interface.endPacket();  
    
    }
    //pkg.meta = ahbCanAddrParse(rxId);
    //pkg.len = len;
        #ifdef AHB_DEBUG  
        Serial.print(F("  Length: 0x"));
        //Serial.print(len);
        #endif //AHB_DEBUG 
        
        for(byte i=0; i<len; i++){ 
          //pkg.data[i] = rxBuf[i];
          #ifdef AHB_DEBUG  
          Serial.print(F(" 0x"));
          //Serial.print(pkg.data[i]);
          //Serial.print(F(" | "));
         #endif //AHB_DEBUG 
        }
          
        #ifdef AHB_DEBUG 
          Serial.println();  
        #endif //AHB_DEBUG
        
        return true;    
  }
  else
  {
    return false; 
  }
  
 //this->build(_sub_id, _id, _device, 13, _sub_id, _id, _firmware[0], _firmware[1], mid_b[0], mid_b[1]);
 //build(int b_sub_id, int b_id, int b_device, int b_cmd, int b_subto_id, int b_to_id, unsigned int b_vol1, unsigned int b_vol2, unsigned int b_vol3, unsigned int b_vol4)
 // this->build(_sub_id, _id, _device, 30, _sub_id, _id, i, holdingRegs[i]);//передача верcие прошивки и серийного номера на сборку пакета
/**
 holding_update();// Обработчик измениния регистров, в случае изменения состояния отправляеться запрос на сервер с передачей парамета
  if (status_err == 1) {
   status_err = 0;
    Serial.print("UDP reply packet: %s\n"); //, replyGate);
    _interface.beginPacket(dest_gw_can_to_okbit, remPort_okbit);
    _interface.write(replyGate);
    _interface.endPacket();
  }
*/
  //debounce(); // вызов функции обработки нажатий кнопок

  //digitalWrite(lamp1, holdingRegs[1]); // вкл\выкл - лампы в зависимости от пришедшей команды по UDP или по нажатию кнопки
  //digitalWrite(lamp2, holdingRegs[2]);
  //digitalWrite(lamp3, holdingRegs[3]); 
  

  /*  int packetSize = _interface.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = _interface.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(_interface.remotePort());

    // read the packet into packetBufffer
    _interface.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
  */
}

/*void AHB_GW_CAN_TO_OKBIT::debounce(void) { //Обработчик нажатия кнопок

  if ( bouncer1.update() ) {
    if ( bouncer1.read() == LOW) {
      if ( holdingRegs[1] == LOW ) {
       holdingRegs[1] = HIGH;
      } else {
       holdingRegs[1] = LOW;
      }
    }
  }

  if ( bouncer2.update() ) {
    if ( bouncer2.read() == LOW) {
      if ( holdingRegs[2] == LOW ) {
       holdingRegs[2] = HIGH;
      } else {
       holdingRegs[2] = LOW;
      }
    }
  }

  if ( bouncer3.update() ) {
    if ( bouncer3.read() == LOW) {
      if ( holdingRegs[3] == LOW ) {
       holdingRegs[3] = HIGH;
      } else {
       holdingRegs[3] = LOW;
      }
    }
  }
}
*/

  
#endif //AHB_GW_CAN_TO_OKBIT_C
