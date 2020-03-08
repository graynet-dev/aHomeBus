#include "ahb.h"
//CONFIG

//NODE
#define NODE_ADDR 4
AHB ahb0(NODE_ADDR, Master); //AHB ahb0(0, 100, Master); //FOR MEGA
AHB_MASTER ahbMaster0(NODE_ADDR);

//Ethernet + Web
#define ETH_SS 10
#define SD_SS 4
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xFA};
IPAddress ip(192,168,2,104);
IPAddress gateway(192,168,2,1);
IPAddress dns_ip(192,168,2,1);
IPAddress subnet(255,255,255, 0);

AHB_ETH ahb_eth0(ETH_SS);
AHB_WEB ahb_web0(SD_SS);

//CAN BUS
#define MCP_CS 52
  
uint8_t can_addr_net[20] = {0,21,20,19, 15, 14, 13, 12, 11, 10, 9, 8, 7,6,5, 4}; //,100, 17,16,55,56,
uint8_t size_can_addr_net = sizeof(can_addr_net)/sizeof(can_addr_net[0]);

AHB_CAN ahbCan0(MCP_CS, CAN_250KBPS, MCP_8MHZ, 2, can_addr_net, size_can_addr_net); // 53 MEGA 52 DUE

//MODBUS_RTU BUS
//uint8_t modbus_rtu_net[1] = {100};
//uint8_t size_modbus_rtu_addr_net = sizeof(modbus_rtu_addr_net)/sizeof(modbus_rtu_addr_net[0]);

//AHB_MODBUS_RTU ahbModBusRtu0(0,3,0,modbus_rtu_addr_net,size_modbus_rtu_addr_net);

//////modbus_t telegram[2];
//////uint16_t au16data[16]; //!< data array for modbus network sharing

//MODBUS_TCP BUS
//uint8_t modbus_tcp_net[1] = {200};
//uint8_t size_modbus_tcp_addr_net = sizeof(modbus_tcp_addr_net)/sizeof(modbus_tcp_addr_net[0]);
//AHB_MODBUS_TCP ahbModBusTcp0(10);

//UART BUS
//uint8_t uart_addr_net[] = {0,77,78};
//uint8_t size_uart_addr_net = sizeof(uart_addr_net)/sizeof(uart_addr_net[0]);

//AHB_UART ahbUart0(Serial,uart_addr_net,size_uart_addr_net);

unsigned long lastSend;


void setup() {
  Serial.begin(115200);Serial.print(F("AHB Node - "));Serial.println(ahb0._nodeId);
  ahb0.begin();

  Serial.print(F("Attach CAN..."));   if(ahb0.busAttach(&ahbCan0) < 0)       {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  Serial.print(F("Attach MASTER..."));if(!ahb0.masterAttach(&ahbMaster0)) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  //Serial.print(F("Attach UART..."));if(ahb0.busAttach(&ahbUart0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  //Serial.print(F("Attach ModBus RTU...")); if(ahb0.busAttach(&ahbModBusRtu0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  //Serial.print(F("Attach ModBus TCP..."));if(ahb0.busAttach(&ahbModBusTcp0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}

    ahb_eth0.begin(mac, ip, dns_ip, gateway, subnet);  
   //ahb_web0.begin(mac, ip, dns_ip, gateway, subnet); 
   //ahb_web0.webAttach(&ahb0);  
   Serial.print(F("Attach WEB..."));if(!ahb_web0.webAttach(&ahb0)) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
}

void CAN_test (){
  if (Serial.available()>0){
    byte inByte;
    inByte = Serial.read () - '0';
    if (inByte == 0) {ahbCan0.mcp2515_reset(); Serial.println("Pres 0");}
    else if (inByte == 1) {ahbCan0.begin();  Serial.println("Pres 1");}
    else if (inByte == 2) {ahbCan0.mcp2515_reset(); ahbCan0.begin();  Serial.println("Pres 2");}
    else if (inByte == 3) { Serial.print(ahbCan0.checkError()); Serial.println(" checkError Pres 3");}
    else if (inByte == 4) { Serial.print(ahbCan0.errorCountRX()); Serial.println(" errorCountRX Pres 4");}
    else if (inByte == 5) { Serial.print(ahbCan0.errorCountTX()); Serial.println(" errorCountTX Pres 5");}
    else if (inByte == 6) { Serial.print(ahbCan0.getError()); Serial.println(" getError Pres 6");}
    else if (inByte == 7) { byte data0[2] = {50,1}; ahb0.ahbSend(AHB_PKGTYPE_UNICAST, 167, 100, 0, ahb0._nodeId,  sizeof(data0), data0);}
    else if (inByte == 8) { byte data0[2] = {50,1}; ahb0.ahbSend(AHB_PKGTYPE_UNICAST, 167, 100, 0, ahb0._nodeId,  sizeof(data0), data0);}
    else if (inByte == 9) while(1);
  }   
}

  
void loop() {
  CAN_test();
  ahb_web0.loop_web();
  ahbPacket pkg = ahb0.loop();
  if(pkg.meta.busId != -1) {}//If bus is -1 there was no new packet received

  if(millis() > lastSend + 5000) { //every 5 seconds

      byte data[1] = {70}; byte data1[2] = {50,1};
      //Прямая посылка в шину
      //bool sndStat0 = ahbCan0.ahbSend_V(0, 167, 0, 0, ahb0._nodeId,  sizeof(data), data); 
      //Посылка в обработчик с роутингом
      //ahb0.ahbSend(AHB_PKGTYPE_UNICAST, 167, 100, 0, ahb0._nodeId,  sizeof(data1), data1);
      //ahb0.ahbSend(AHB_PKGTYPE_MULTICAST, 167,  0, 0, ahb0._nodeId,  sizeof(data), data);
      //ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 55, 0, ahb0._nodeId,  sizeof(data1), data1); //3
      //ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 21, 0, ahb0._nodeId,  sizeof(data1), data1); //2
      //ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167,  0, 0, ahb0._nodeId,  sizeof(data1), data1); 
      //ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167,  0, 0, 99          ,  sizeof(data1), data1);
      //ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 21, 0, 99          ,  sizeof(data1), data1); //1 
      //ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 20, 0, ahb0._nodeId,  sizeof(data1), data1); //4

  /*
   byte data1[2] = {AHB_CMD_0B,1};
    Serial.println(F("MSG to 115 BEGIN------------------------------------"));
    byte sndStat1 = ahb0.ahbSend(1,  115, 0,  sizeof(data1), data1); //AHB_PKGTYPE_UNICAST
    if(sndStat1 == CAN_OK){
      msg_num++;
      Serial.print(F("Message to 115 sent successfully! - "));Serial.println(msg_num);
    } else {Serial.println(F("Error sending to 115 message..."));}
 */
    lastSend = millis();
  }
}
