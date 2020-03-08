#include "ahb.h"
//CONFIG

//NODE
#define NODE_ADDR 5
AHB ahb0(NODE_ADDR, Slave); //AHB ahb0(0, 100,Slaver); //FOR MEGA
AHB_SLAVE ahbSlave0(NODE_ADDR);

//Ethernet + Web
#define ETH_SS 10
#define SD_SS 4
byte mac[] = {0xDE, 0xAD, 0xBE, 0xE1, 0xF0, 0xF5};
IPAddress ip(192,168,2,105);
IPAddress gateway(192,168,2,1);
IPAddress dns_ip(192,168,2,1);
IPAddress subnet(255,255,255, 0);
IPAddress dest_gw_can_to_udp(192, 168, 2, 101);
IPAddress dest_gw_can_to_okbit(192, 168, 2, 101);

AHB_ETH ahb_eth0(ETH_SS);
AHB_WEB ahb_web0(SD_SS);
//AHB_OKBIT_UDP ahb_okbit_udp(ETH_SS);

unsigned int localPort_udp = 8888;
unsigned int   remPort_udp = 54105;

unsigned int localPort_okbit = 6400;
unsigned int   remPort_okbit = 6500;

AHB_GW_CAN_TO_UDP ahb_gw_can_to_udp0(localPort_udp, remPort_udp, dest_gw_can_to_udp); //

int sub_id = 0; //адресс подсети
int id = 0; //адрес устройства
int device = 7001; //тип устройства смотрим в модуле
int firmware[2] = {0,9}; //версия прошивки

AHB_GW_CAN_TO_OKBIT ahb_gw_can_to_okbit0(localPort_okbit, remPort_okbit, dest_gw_can_to_okbit,sub_id,id,device,firmware[0],firmware[1]);

//CAN BUS
#define MCP_CS 52 //53 mega2560
  
uint8_t can_addr_net[20] = {0,21,20,19, 15, 14, 13, 12, 11, 10, 9, 8, 7,6,5, 4}; //,100, 17,16,55,56,
uint8_t size_can_addr_net = sizeof(can_addr_net)/sizeof(can_addr_net[0]);

AHB_CAN ahbCan0(MCP_CS, CAN_250KBPS, MCP_8MHZ, 2, can_addr_net, size_can_addr_net); // 53 MEGA 52 DUE

//MODBUS_RTU BUS
//uint8_t modbus_rtu_addr_net[] = {100,101,102,103};
//uint8_t size_modbus_rtu_addr_net = sizeof(modbus_rtu_addr_net)/sizeof(modbus_rtu_addr_net[0]);
//AHB_MODBUS_RTU ahbModBusRtu0(0,1,10,modbus_rtu_addr_net, size_modbus_rtu_addr_net);

//UART BUS
//uint8_t uart_addr_net[] = {0,55,56};
//uint8_t size_uart_addr_net = sizeof(uart_addr_net)/sizeof(uart_addr_net[0]);
//AHB_UART ahbUart0(Serial,uart_addr_net,size_uart_addr_net);

int msg_num = 0;
int msg_num2 = 0;
int msg_num3 = 0;

unsigned long lastSend;

void setup() {
  //Initialize Serial port
  Serial.begin(115200);Serial.print(F("AHB Node - "));Serial.println(ahb0._nodeId);
  //ahb0.begin();
  
  //Attach the previously defined CAN-Bus to our controller
  Serial.print(F("Attach CAN..."));if(ahb0.busAttach(&ahbCan0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  Serial.print(F("Attach SLAVE..."));if(!ahb0.slaveAttach(&ahbSlave0)) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  //Serial.print(F("Attach UART..."));if(ahb0.busAttach(&ahbUart0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
  //Serial.print(F("Attach Modbus_RTU..."));if(ahb0.busAttach(&ahbModBusRtu0) < 0) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}

  
   ahb_eth0.begin(mac, ip, dns_ip, gateway, subnet);  
   //ahb_gw_can_to_udp0.begin();
   //Т.к не передаем параметры атач не нужен Serial.print(F("Attach ETH..."));if(!ahb_eth0.EthAttach(&ahb0)) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}   
   
   //ahb_web0.begin();  
   Serial.print(F("Attach WEB..."));if(!ahb_web0.webAttach(&ahb0)) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}

   Serial.print(F("Attach GW_CAN_TO_UDP..."));if(!ahb0.busAttach(&ahb_gw_can_to_udp0)) {Serial.println(F("Error!"));}else{Serial.println(F("Done!"));}
   Serial.print(F("Attach GW_CAN_TO_OKBIT..."));if(!ahb0.busAttach(&ahb_gw_can_to_okbit0)) {Serial.println(F("Err2or!"));}else{Serial.println(F("Done!"));}
}

void CAN_test (){
  if (Serial.available()>0){
    byte inByte;
    inByte = Serial.read () - '0';
    if (inByte == 0) {ahbCan0.mcp2515_reset(); Serial.println("Pres 0");}
    else if (inByte == 1) {ahbCan0.begin();  Serial.println("Pres 1");}
    else if (inByte == 2) while(1);
     else if (inByte == 3) {
           byte data1[2] = {50,1};
            ahb0.ahbSend(AHB_PKGTYPE_UNICAST, 167, 100, 0, ahb0._nodeId,  sizeof(data1), data1);
      }
    //else if (inByte == 4) ;
    //else if (inByte == 5) ;
    //else if (inByte == 8) ;
    //else if (inByte == 9) ;
  }   
}

void loop() {
 CAN_test();
 ahb_web0.loop_web();
 ahbPacket pkg = ahb0.loop();
  //loop() handles all recurring tasks like processing received messages and returns a packet
  if(pkg.meta.busId != -1) { //If bus is -1 there was no new packet received
    }
  //Sending a message
  if(millis() > lastSend + 3000) { //every 3 seconds
    //Send a message to group 0x1234 instructing it to turn on
    msg_num2++;
    // byte data1[2] = {50,1};
     //ahb0.ahbSend(AHB_PKGTYPE_UNICAST, 167, 100, 0, ahb0._nodeId,  sizeof(data1), data1);
 //Serial.println(F("END------------------------------------"));
  /** byte data[4] = {ASB_CMD_PING, 1,msg_num2,0};
   byte sndStat = asb0.asbSend(ASB_PKGTYPE_UNICAST,  0x121, 0x00, sizeof(data), data);
    if(sndStat == CAN_OK){
      msg_num++;
      Serial.print(F("Message sent successfully! - "));
      Serial.println(msg_num);
    } else {
      Serial.println(F("Error sending message..."));
    }
        byte sndStat2 = asb0.asbSend(ASB_PKGTYPE_MULTICAST,  0x0104,  sizeof(data), data);
    if(sndStat2 == CAN_OK){
      msg_num++;
      Serial.print(F("Message sent successfully! - "));
      Serial.println(msg_num);
    } else {
      Serial.println(F("Error sending message..."));
    }*/
    
    lastSend = millis();
  }

}
