#ifndef AHB_WEB_C
#define AHB_WEB_C

#include "ahb_WEB.h"

AHB_WEB::AHB_WEB(uint8_t _cspin) {  
   // Инициировать SD карту
}

void AHB_WEB::begin(void){

   //_interface_eth.begin(mac_address, local_ip, dns_server, gateway, subnet); // );
   //Serial.println("Ethernet is OK");
   //Serial.print("Node IP - ");
   //Serial.println(Ethernet.localIP());
   server.begin();
   Serial.println("Web Server is OK"); 
}

bool AHB_WEB::webAttach(AHB *ahb) {
  _ahb = ahb;
  return true;
}

void AHB_WEB::loop_web() {

  if(_ahb->_nodeType==Master || _ahb->_nodeType==Slave){
      
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
            client.println("Refresh: 10");  // refresh the page automatically every 5 sec
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
            client.println("<tr><td>Node</td><td>CAN Status</td><td>Up Time</td><td>RX/TX</td></tr>");
        
            for (int i = 4; i<255; i++) { //Надо с 0
              if (_ahb->NodeGuard_OK_check(i,0)) {
                client.println("<tr>");
                client.println("<td>");
                //client.print ("Node - ");
                //client.print (i);
                client.print(node_name[i]);
                client.println("</td>");
                if (i!= _ahb->_nodeId){ 
                  if (!_ahb->NodeGuard_OK_check(i,1)){
                    client.println("<td bgcolor=red>");
                    client.println (F("<font color=black><center>FAIL</center></font>"));
                    client.println("</td>");
                  }
                  else 
                  {
                    client.println("<td>");
                    client.println (F("<font color=green><center>OK</center></font>"));
                    client.println("</td>");
                  }
                }
                else{
                  if (i== _ahb->_nodeId){
                    client.println("<td bgcolor=gray>");
                    client.println (F("<font><center>My node - "));
                    if (_ahb->_nodeType==1){client.println (F("Master"));}
                    if (_ahb->_nodeType==2){client.println (F("Slave"));}
                    client.println (F("</center></font>"));
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
                  client.print (F("  ")); 
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
                if (_ahb->print_rx_error_rn(i)<50 && _ahb->print_tx_error_rn(i)<50){
                  client.println("<td>");
                }
                else
                {
                  client.println("<td bgcolor=red>");
                }
                
                client.print(F("<font color=green><center>"));
                client.print(_ahb->print_rx_error_rn(i));
                client.print(F("/"));
                client.print(_ahb->print_tx_error_rn(i));
                client.print(F("</center></font>"));
                client.println("</td>");
                client.println("</tr>");
              }
              //_ahb->NodeGuard_OK[i][1]=false; //Статус узла
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

#endif  ///AHB_WEB_C