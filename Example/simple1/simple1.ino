#include "ahb.h"
 
uint8_t can_addr_net[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,77,78};
int size_can_addr_net = sizeof(can_addr_net)/sizeof(can_addr_net[0]);

uint8_t uart_addr_net[] = {0,55,56};
int size_uart_addr_net = sizeof(uart_addr_net)/sizeof(uart_addr_net[0]);
//const uint8_t netsize = 24;

//uint8_t modbus_rtu_addr_net[] = {100,101,102,103};
//int size_modbus_rtu_addr_net = sizeof(modbus_rtu_addr_net)/sizeof(modbus_rtu_addr_net[0]);

AHB ahb0(5,Slave);

//AHB ahb0(0,100,Node);
AHB_CAN ahbCan0(52, CAN_500KBPS, MCP_16MHZ , 2,can_addr_net, size_can_addr_net);
//AHB_UART ahbUart0(Serial,uart_addr_net,size_uart_addr_net);
//AHB_MODBUS_RTU ahbModBusRtu0(0,1,10,modbus_rtu_addr_net, size_modbus_rtu_addr_net);


int msg_num = 0;
int msg_num2 = 0;
int msg_num3 = 0;

unsigned long lastSend;

void setup() {
  //Initialize Serial port
  Serial.begin(115200);
  Serial.print(F("AHB Node - "));
  Serial.println(ahb0._nodeId);
  ahb0.begin();
  //Attach the previously defined CAN-Bus to our controller
  Serial.print(F("Attach CAN..."));
  if(ahb0.busAttach(&ahbCan0) < 0) {
    Serial.println(F("Error!"));
  }else{
    Serial.println(F("Done!"));
  }
 /**  Serial.print(F("Attach UART..."));
  if(ahb0.busAttach(&ahbUart0) < 0) {
    Serial.println(F("Error!"));
  }else{
    Serial.println(F("Done!"));
  }*/

/** Serial.print(F("Attach Modbus_RTU..."));
  if(ahb0.busAttach(&ahbModBusRtu0) < 0) {
    Serial.println(F("Error!"));
  }else{
    Serial.println(F("Done!"));
  }
*/
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

  //loop() handles all recurring tasks like processing received messages and returns a packet
  ahbPacket pkg = ahb0.loop();
  if(pkg.meta.busId != -1) { //If bus is -1 there was no new packet received
    msg_num3++;
   // if (pkg.meta.target==ahb0._nodeId || pkg.meta.target==0x00)
   // {
   /** Serial.print("RX --->  ");
    Serial.print(msg_num3);
    Serial.print(F(" - Type: 0x"));
    Serial.print(pkg.meta.type);
    Serial.print(F(" - CMD: 0x"));
    Serial.print(pkg.meta.cmd);    
    Serial.print(F("  Target: 0x"));
    Serial.print(pkg.meta.target);
    Serial.print(F("  Port: 0x"));
    Serial.print(pkg.meta.port);
    Serial.print(F("  Source: 0x"));
    Serial.print(pkg.meta.source);
    Serial.print(F("  Length: 0x"));
    Serial.print(pkg.len);
    for(byte i=0; i<pkg.len; i++) {
      Serial.print(F(" 0x"));
      Serial.print(pkg.data[i]);
    }
    Serial.println();
    Serial.println(F(" ----------------------------------------------------------"));
  */
  //  }
  }
}

