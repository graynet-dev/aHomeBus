
#include "ahb_WEB.h"


AHB_WEB::AHB_WEB(uint8_t _cspin) : _ping(pingSocket, (uint16_t)random(0, 255)) { // 
   _interface_eth.init(_cspin);
}

void AHB_WEB::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet){
   _interface_eth.begin(mac_address, local_ip, dns_server, gateway, subnet); // );
  //EthernetClient client;
  //EthernetServer server(80);
   Serial.println("Ethernet is OK");
   Serial.print("Node IP - ");
   Serial.println(Ethernet.localIP());
   server.begin();
   Serial.println("Web Server is OK");  
}

char AHB_WEB::webAttach(AHB *ahb) {
_ahb = ahb;
return true;
}


void AHB_WEB::Ping_node() {
 if (millis()-prevTimePingNode>2000)   { 
  Serial.println();
  Serial.print(countOnline); Serial.print(" Ping node begin: "); Serial.print(netDevicesNames[countOnline]);
  //for (countOnline = 0; countOnline < TOTAL_NET_DEVICES; countOnline++) {
    //if (countOnline == 5) { // self
      //  online[countOnline] = 1;
      //  continue;
    //}

    ICMPEchoReply echoReply = _ping(ips[countOnline], 4);
    if (echoReply.status == SUCCESS){
     online[countOnline] = 1;
     Serial.println(" OK ");
    } 
    else{
     online[countOnline] = 0;
     Serial.println(" FAIL ");
    } 
    //Serial.print(netDevicesNames[countOnline]);
    //Serial.print(": ");
    //Serial.print(online[countOnline]);
    //Serial.print(" ");
    //Serial.println(pingMessage);
    
  //}
  //Serial.println("Ping node end. ");
  countOnline++;
  if (countOnline>TOTAL_NET_DEVICES-1){countOnline=0;}
  prevTimePingNode=millis();
 }
}


#define PING_REQUEST_TIMEOUT_MS     2500

void AHB_WEB::someCriticalStuffThatCantWait(void)
{
  for (int i = 0; i < 10; i++)
  {
    if (lastPingSucceeded) {
      //Serial.print('.');
    }
  }
  //Serial.print('_');
}


void AHB_WEB::Ping_node_async(uint8_t i)
{
  //if (millis()-prevTimePingNode>1000)   {
    ICMPPing::setTimeout(PING_REQUEST_TIMEOUT_MS);
    lastPingSucceeded = false;
    ICMPEchoReply echoResult; 
    Serial.println("Starting async ping.");

    if (! _ping.asyncStart(ips[i], 3, echoResult))  //pingAddr countOnline
      {
        Serial.print("Couldn't even send our ping request?? Status: ");
        Serial.println((int)echoResult.status);
        return;
      }

      Serial.println("Ping sent ");
      //_ping.asyncComplete(echoResult);
      while (! _ping.asyncComplete(echoResult))
      {
        someCriticalStuffThatCantWait();
      }

      //if (! _ping.asyncComplete(echoResult)){
      //  Serial.println("NO ANSWER !!!"); 
      //}
      //else{
      //  Serial.println("ANSWER !!!"); 
      //}

  if (echoResult.status != SUCCESS)
  {
    online[i] = 0;
    sprintf(buffer, "Echo request failed; %d", echoResult.status);
  } else {
    online[i] = 1;
    lastPingSucceeded = true;
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoResult.data.seq,echoResult.addr[0],echoResult.addr[1],echoResult.addr[2],echoResult.addr[3],
            REQ_DATASIZE,millis() - echoResult.data.time, echoResult.ttl);
  }

  //countOnline++;
  //if (countOnline>TOTAL_NET_DEVICES-1){countOnline=0;}  
  
   Serial.println(buffer);
  // prevTimePingNode=millis();
 //} 

  
}

void AHB_WEB::loop_web() {
  if(_ahb->_nodeType==Master){
  //Ping_node();
    if (millis()-prevTimePing>10000){
      //Serial.print(F("Ping node - ")); Serial.println(countOnline);
      //Ping_node_async(countOnline);
      if (countOnline>TOTAL_NET_DEVICES-1){
        countOnline=0;
      }
      else{
        countOnline++;
      }
      prevTimePing=millis();
    }
      
    // listen for incoming clients
    client = server.available();
    if (client) {
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank) {
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close"); 
            client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
          
            client.print("System tyme - "); 
            //Время
            if (_ahb->print_hh_hw()<10)  client.print (F("0")); 
            client.print (_ahb->print_hh_hw());  
            client.print (F(":")); 
            if (_ahb->print_mm_hw()<10)  client.print (F("0")); 
            client.print (_ahb->print_mm_hw());  
            client.print (F(":")); 
            if (_ahb->print_ss_hw()<10)  client.print (F("0")); 
            client.print (_ahb->print_ss_hw());  
            client.print (F("       "));  
            //День недели
            switch(_ahb->print_dayofweak_hw()){
              case 1:
                client.print(" Monday ");
                break;
              case 2:
                client.print(" Tuesday ");
                break;
              case 3:
                client.print(" Wednesday ");
                break;
              case 4:
                client.print(" Thursday ");
                break;
              case 5:
                client.print(" Friday ");
                client;
              case 6:
                client.print(" Saturday ");
                break;
              case 7:
                client.print(" Sunday ");
              break;
            }          
            //Дата
            if (_ahb->print_dd_hw()<10)  client.print (F("0")); 
            client.print (_ahb->print_dd_hw());  
            client.print (F(".")); 
            if (_ahb->print_month_hw()<10)  client.print (F("0")); 
            client.print (_ahb->print_month_hw());  
            client.print (F(".")); 
            if (_ahb->print_year_hw()<10)  client.print (F("0")); 
            client.print (_ahb->print_year_hw());  
          
            client.println("<table border=1>");
            client.println("<tr><td>Node</td><td>CAN Status</td><td>Up Time</td><td>ESP Status</td></tr>");
        
            for (int i = 4; i<255; i++) { //Надо с 0
              if (_ahb->NodeGuard_OK[i][0]){
                client.println("<tr>");
                client.println("<td>");
                client.print ("Node - ");
                client.print (i);
                client.println("</td>");
                if (i!= _ahb->_nodeId){ 
                  if (!_ahb->NodeGuard_OK[i][1]){
                    client.println("<td bgcolor=red>");
                    client.println (F("<font color=black>FAIL</font>"));
                    client.println("</td>");
                  }
                  else 
                  {
                    client.println("<td>");
                    client.println (F("<font color=green>OK</font>"));
                    client.println("</td>");
                  }
                }
                else{
                  if (i== _ahb->_nodeId){
                    client.println("<td bgcolor=gray>");
                    client.println (F("<font>My node</font>"));
                    client.println("</td>");                  
                  }
                  else{
                    client.println("<td bgcolor=yellow>");
                    client.println (F("<font>OFF</font>"));
                    client.println("</td>");
                  }            
                }
          
                client.println("<td>");
                if (i!= _ahb->_nodeId) { 
                  if (_ahb->print_dd_rn(i)<10)  client.print (F("0")); 
                  client.print (_ahb->print_dd_rn(i));  
                  client.print (F("  ")); 
                  if (_ahb->print_hh_rn(i)<10)  client.print (F("0")); 
                  client.print (_ahb->print_hh_rn(i));  
                  client.print (F(":")); 
                  if (_ahb->print_mm_rn(i)<10)  client.print (F("0")); 
                  client.print (_ahb->print_mm_rn(i));  
                  client.print (F(":")); 
                  if (_ahb->print_ss_rn(i)<10)  client.print (F("0")); 
                  client.print (_ahb->print_ss_rn(i));               
                }
                else {
                  if (_ahb->print_dd_ut()<10)  client.print (F("0")); 
                  client.print (_ahb->print_dd_ut());  
                  client.print (F(":")); 
                  if (_ahb->print_hh_ut()<10)  client.print (F("0")); 
                  client.print (_ahb->print_hh_ut());  
                  client.print (F(":")); 
                  if (_ahb->print_mm_ut()<10)  client.print (F("0")); 
                  client.print (_ahb->print_mm_ut());  
                  client.print (F(":")); 
                  if (_ahb->print_ss_ut()<10)  client.print (F("0")); 
                  client.print (_ahb->print_ss_ut()); 
                }
          
                client.println("</td>");                          
                client.println("<td>");
                client.println (F("<font color=green>-ping- not dev</font>"));
                client.println("</td>");
                client.println("</tr>");
              }
            }
         
            client.println("</table>");
            client.println("</html>");
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          }
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      client.stop();
    }
  }
}

