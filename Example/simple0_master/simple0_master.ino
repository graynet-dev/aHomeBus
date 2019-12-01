
#include "ahb.h"
//#include <Ethernet2.h>
//#include <EthernetUdp2.h>

  #define MCP_CS 52
  #define ETH_SS 10
  #define SD_SS 4
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xFA};
  IPAddress ip(192,168,2,104);
  IPAddress gateway(192,168,2,1);
  IPAddress dns_ip(192,168,2,1);
  IPAddress subnet(255,255,255, 0);
  
//uint16_t au16data[16]; //!< data array for modbus network sharing

uint8_t can_addr_net[21] = {0,21,20,19,18,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,100}; //17,16,55,56,
uint8_t size_can_addr_net = sizeof(can_addr_net)/sizeof(can_addr_net[0]);
//uint8_t modbus_rtu__net[1] = {100};
//uint8_t uart_addr_net[] = {0,77,78};
//uint8_t size_uart_addr_net = sizeof(uart_addr_net)/sizeof(uart_addr_net[0]);
AHB ahb0(4, Master);
//AHB ahb0(0, 100, Master);
//const uint8_t sizecan_net = 24;
AHB_CAN ahbCan0(MCP_CS, CAN_500KBPS, MCP_16MHZ, 2, can_addr_net, size_can_addr_net); // 53 MEGA 52 DUE
//AHB_UART ahbUart0(Serial,uart_addr_net,size_uart_addr_net);
//AHB_MODBUS_RTU ahbModBusRtu0(0,3,0,modbus_rtu_addr_net,size_modbus_rtu_addr_net);
//AHB_MODBUS_TCP ahbModBusTcp0(10);

//modbus_t telegram[2];

AHB_WEB ahb_web0(ETH_SS);

//int msg_num = 0;
//int msg_num2 = 0;
//int msg_num3 = 0;
unsigned long lastSend;


void setup() {
  
  //Initialize Serial port
  Serial.begin(115200);Serial.print(F("AHB Node - "));Serial.println(ahb0._nodeId);
  ahb0.begin();

  //Attach the previously defined CAN-Bus to our controller
  Serial.print(F("Attach CAN..."));if(ahb0.busAttach(&ahbCan0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  //Serial.print(F("Attach WEB..."));if(ahb0.webAttach(&ahb_web0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}

  //init ETH
   ahb_web0.begin(mac, ip, dns_ip, gateway, subnet); //void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);
   ahb_web0.webAttach(&ahb0);

  for (int i=0; i<ahbCan0.net_size; i++)
  {
   // Serial.print(F("Node! i -  "));Serial.print(i);Serial.print(F(" is - "));Serial.println(ahbCan0.address_network_get(i));
    }
 /** 
    Serial.print(F("Attach UART..."));if(ahb0.busAttach(&ahbUart0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
    // ahbUart0.address_network_set(uart_addr_net, size_uart_addr_net);
 */
 /** 
    Serial.print(F("Attach ModBus RTU...")); if(ahb0.busAttach(&ahbModBusRtu0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  */
/**
  // telegram 0: read registers
  telegram[0].u8id = 1; // slave address
  telegram[0].u8fct = 3; // function code (this one is registers read)
  telegram[0].u16RegAdd = 0; // start address in slave
  telegram[0].u16CoilsNo = 4; // number of elements (coils or registers) to read
  telegram[0].au16reg = au16data; // pointer to a memory array in the Arduino
  // telegram 1: write a single register
  telegram[1].u8id = 1; // slave address
  telegram[1].u8fct = 6; // function code (this one is write a single register)
  telegram[1].u16RegAdd = 4; // start address in slave
  telegram[1].u16CoilsNo = 1; // number of elements (coils or registers) to read
  telegram[1].au16reg = au16data+4; // pointer to a memory array in the Arduino
*/
/**  
    Serial.print(F("Attach ModBus TCP..."));if(ahb0.busAttach(&ahbModBusTcp0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
*/


}



  
void loop() {

  ahb_web0.loop_web();

  if(millis() > lastSend + 5000) { //every 5 seconds

      byte data[1] = {70}; byte data1[2] = {50,1};
      bool sndStat0 = ahbCan0.ahbSend_V(0, 167, 0, 0, ahb0._nodeId,  sizeof(data), data); 
      ahb0.ahbSend(AHB_PKGTYPE_UNICAST, 167, 100, 0, ahb0._nodeId,  sizeof(data1), data1);
      ahb0.ahbSend(AHB_PKGTYPE_MULTICAST, 167,  0, 0, ahb0._nodeId,  sizeof(data), data);
      ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 55, 0, ahb0._nodeId,  sizeof(data1), data1); //3
      
      ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 21, 0, ahb0._nodeId,  sizeof(data1), data1); //2
      ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167,  0, 0, ahb0._nodeId,  sizeof(data1), data1); 
      ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167,  0, 0, 99          ,  sizeof(data1), data1);
      ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 21, 0, 99          ,  sizeof(data1), data1); //1 
      ahb0.ahbSend(AHB_PKGTYPE_UNICAST  , 167, 20, 0, ahb0._nodeId,  sizeof(data1), data1); //4

     Serial.println(F("END------------------------------------"));
  /*
   byte data1[2] = {AHB_CMD_0B,1};
    Serial.println(F("MSG to 115 BEGIN------------------------------------"));
    byte sndStat1 = ahb.ahbSend(1,  115, 0,  sizeof(data1), data1); //AHB_PKGTYPE_UNICAST
    if(sndStat1 == CAN_OK){
      msg_num++;
      Serial.print(F("Message to 115 sent successfully! - "));Serial.println(msg_num);
    } else {Serial.println(F("Error sending to 115 message..."));}
 */
    lastSend = millis();
   
  }

  ahbPacket pkg = ahb0.loop();
  if(pkg.meta.busId != -1) { //If bus is -1 there was no new packet received

  }
}



