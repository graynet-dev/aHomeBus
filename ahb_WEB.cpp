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

void AHB_WEB::GetSwitchState(EthernetClient cl){
    //if (digitalRead(2)) {
    //    cl.println("<p>Switch state: ON</p><button type=\"button\" style=\"color: green;\" onclick=\"GetSwitchState()\">On</button>");
    //}
    //else {
    //    cl.println("<p>Switch state: OFF</p><button type=\"button\" style=\"color: red;\" onclick=\"GetSwitchState()\">Off</button>");
    //}
            cl.println("<p style='color:#c29306'>");
            cl.print("System tyme - "); 
            //Время
            if (_ahb->print_hh_hw()<10)  cl.print (F("0")); 
            cl.print (_ahb->print_hh_hw());  
            cl.print (F(":")); 
            if (_ahb->print_mm_hw()<10)  cl.print (F("0")); 
            cl.print (_ahb->print_mm_hw());  
            cl.print (F(":")); 
            if (_ahb->print_ss_hw()<10)  cl.print (F("0")); 
            cl.print (_ahb->print_ss_hw());  
            cl.print (F("       "));  
            //День недели
            switch(_ahb->print_dayofweak_hw()){
              case 1:
                cl.print(" Monday ");
                break;
              case 2:
                cl.print(" Tuesday ");
                break;
              case 3:
                cl.print(" Wednesday ");
                break;
              case 4:
                cl.print(" Thursday ");
                break;
              case 5:
                cl.print(" Friday ");
                cl;
              case 6:
                cl.print(" Saturday ");
                break;
              case 7:
                cl.print(" Sunday ");
              break;
            }          
            //Дата
            if (_ahb->print_dd_hw()<10)  cl.print (F("0")); 
            cl.print (_ahb->print_dd_hw());  
            cl.print (F(".")); 
            if (_ahb->print_month_hw()<10)  cl.print (F("0")); 
            cl.print (_ahb->print_month_hw());  
            cl.print (F(".")); 
            if (_ahb->print_year_hw()<10)  cl.print (F("0")); 
            cl.print (_ahb->print_year_hw());  
            cl.println("</p>");
            
            cl.println("<table border=1 bgcolor='#ffffff'>");
            cl.println("<tr><td>Node</td><td>CAN Status</td><td>Up Time</td><td>Reboot</td><td>RX/TX</td></tr>");
        
            for (int i = 4; i<255; i++) { //Надо с 0
              if (_ahb->NodeGuard_OK_check(i,0)) {
                cl.println("<tr>");
                cl.println("<td>");
                //client.print ("Node - ");
                //client.print (i);
                cl.print(node_name[i]);
                cl.println("</td>");
                if (i!= _ahb->_nodeId){ 
                  if (!_ahb->NodeGuard_OK_check(i,1)){
                    cl.println("<td bgcolor=red>");
                    cl.println (F("<font color=black><center>FAIL</center></font>"));
                    cl.println("</td>");
                  }
                  else 
                  {
                    cl.println("<td>");
                    cl.println (F("<font color=green><center>OK</center></font>"));
                    cl.println("</td>");
                  }
                }
                else{
                  if (i== _ahb->_nodeId){
                    cl.println("<td>");
                    cl.println (F("<font color=green><center>"));
                    if (_ahb->_nodeType==1){cl.println (F("Master - OK"));}
                    if (_ahb->_nodeType==2){cl.println (F("Slave - OK"));}
                    cl.println (F("</center></font>"));
                    cl.println("</td>");                  
                  }
                  else{
                    cl.println("<td bgcolor=yellow>");
                    cl.println (F("<font>OFF</font>"));
                    cl.println("</td>");
                  }            
                }
          
                cl.println("<td>");
                if (i!= _ahb->_nodeId) { 
                  if (_ahb->print_dd_rn(i)<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_dd_rn(i));  
                  cl.print (F("  ")); 
                  if (_ahb->print_hh_rn(i)<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_hh_rn(i));  
                  cl.print (F(":")); 
                  if (_ahb->print_mm_rn(i)<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_mm_rn(i));  
                  cl.print (F(":")); 
                  if (_ahb->print_ss_rn(i)<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_ss_rn(i));               
                }
                else {
                  if (_ahb->print_dd_ut()<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_dd_ut());  
                  cl.print (F("  ")); 
                  if (_ahb->print_hh_ut()<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_hh_ut());  
                  cl.print (F(":")); 
                  if (_ahb->print_mm_ut()<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_mm_ut());  
                  cl.print (F(":")); 
                  if (_ahb->print_ss_ut()<10)  cl.print (F("0")); 
                  cl.print (_ahb->print_ss_ut()); 
                }
                cl.println("</td>");  
                
                cl.println("<td><center>");
                cl.print(_ahb->print_reboot(i));
                cl.println("</center></td>");
                
                if (_ahb->print_rx_error_rn(i)<50 && _ahb->print_tx_error_rn(i)<50){
                  cl.println("<td>");
                }
                else
                {
                  cl.println("<td bgcolor=red>");
                }
                
                cl.print(F("<font color=green><center>"));
                cl.print(_ahb->print_rx_error_rn(i));
                cl.print(F("/"));
                cl.print(_ahb->print_tx_error_rn(i));
                cl.print(F("</center></font>"));
                cl.println("</td>");
                cl.println("</tr>");
              }
              //_ahb->NodeGuard_OK[i][1]=false; //Статус узла
            }

            cl.println("</table>");                  
}

void AHB_WEB::loop_web(){
  #define FSH_P(p) (reinterpret_cast<const __FlashStringHelper *>(p))
  //http://arduino.ru/forum/pesochnitsa-razdel-dlya-novichkov/ajax-web-server-w5500
  const char html_page_head[] PROGMEM="\n\
  <!DOCTYPE html>\n\
  <html>\n\
  <head>\n\
  <title>aHomeBus Net State</title>\n\
    <style>\n\
    body{font-family:Geneva,Arial,Helvetica,sans-serif;-webkit-text-size-adjust:100%}\n\
    @media (min-width:1024px){.sctw{width:24.69%}.scts{width:49%}.sm{display:none}#box{max-width:1024px}}\n\
    @media (min-width:426px) and (max-width:1023px){.sctw{width:49.5%}.scts{width:100%}.sm{display:none}}\n\
    </style>";
  
  const char html_page_script[] PROGMEM="\n\
      <script>\n\
        var request=new XMLHttpRequest();\n\
        function GetSwitchState() {\n\
          request.onreadystatechange = function() {\n\
            if (this.readyState == 4) {\n\
              if (this.status == 200) {\n\
                if (this.responseText != null) {\n\
                  document.getElementById('data_to_update').innerHTML = this.responseText;\n\
                }\n\
              }\n\
            }\n\
          }\n\
          request.open('GET', 'ajax_switch?r='+Math.random(), true);\n\
          request.send(null);\n\
          setTimeout('GetSwitchState()', 10000);\n\ 
        }\n\
      </script>";
      
  const char html_page_body[] PROGMEM="\n\
  </head>\n\
  <body>\n\
  <body onload=\"GetSwitchState()\">";
  
  const char html_page_bottom[] PROGMEM="\n\
    <div id=\"data_to_update\">\n\
  <p id=\"switch_txt\">Switch state: Not requested...</p>\n\
  </div>\n\
  </body>\n\
  </html>";
  
  client = server.available();   
  if (client) { 
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {  
        char c = client.read(); 
        HTTP_req += c; 
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: keep-alive");
          client.println();
                    
          if (HTTP_req.indexOf("ajax_switch") > -1) { 
            GetSwitchState(client);
          }
          else { 
            client.println(FSH_P(html_page_head));
            client.println(FSH_P(html_page_script));
            client.println(FSH_P(html_page_body));
            if(_ahb->_nodeType==Master){
              client.println("<h1 style='color:#c29306'>aHomeBus Net State - Master</h1>");
            }
            if(_ahb->_nodeType==Slave){
              client.println("<h1 style='color:#c29306'>aHomeBus Net State - Slave</h1>");
            }            
            client.println(FSH_P(html_page_bottom));                   
          }
          
          Serial.print(HTTP_req);  
          HTTP_req = "";           
          break;
        }
        
        if (c == '\n') {
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      } 
    } 
    //delay(1);     
    client.stop();  
  } 
}





void AHB_WEB::loop_web2() {
  #define FSH_P(p) (reinterpret_cast<const __FlashStringHelper *>(p))

  const char html_page_top[] PROGMEM="<html><head><meta charset=\"utf-8\"><title>aHomeBus main page</title>\n\
    <style>\n\
    body{font-family:Geneva,Arial,Helvetica,sans-serif;-webkit-text-size-adjust:100%}\n\
    @media (min-width:1024px){.sctw{width:24.69%}.scts{width:49%}.sm{display:none}#box{max-width:1024px}}\n\
    @media (min-width:426px) and (max-width:1023px){.sctw{width:49.5%}.scts{width:100%}.sm{display:none}}\n\
    </style>\n\
      <script>\n\
        var request=new XMLHttpRequest();\n\
        function GetSwitchState() {\n\
          request.onreadystatechange = function() {\n\
            if (this.readyState == 4) {\n\
              if (this.status == 200) {\n\
                if (this.responseText != null) {\n\
                  document.getElementById('data_to_update').innerHTML = this.responseText;\n\
                }\n\
              }\n\
            }\n\
          }\n\
          request.open('GET', 'ajax_switch?r='+Math.random(), true);\n\
          request.send(null);\n\
          setTimeout('GetSwitchState()', 1000);\n\ 
        }\n\
        var o=new XMLHttpRequest();\n\
        function dR() {\n\
          o.open('GET','data.txt?r='+Math.random(),1);\n\
          o.onload=function(){\n\
            var a=this.responseText.replace(/\s*[\r\n=]+\s*/g, '=').replace(/^\s+/,'').replace(/\s+$/,'').match( /([^=]+)/g);\n\
            for (var i=0, len=a.length; i< len; i+=2) try { document.getElementById(a[i]).innerText=a[i+1]; } catch(e) {};\n\
              setTimeout('R()', 1000);\n\
          }\n\
          o.send();\n\
        }\n\
        var h=new XMLHttpRequest(),oUpd=document.getElementById('upds'),oUpw=document.getElementById('updw')\n\
        var cUpw,tUpd,e,j;\n\
        function sQry(){h.open('GET',dUrl+'?t='+Math.random(),true);h.onreadystatechange=oRSC;h.send()}\n\
        function oRSC(){ if(this.readyState!=4){return;}try{}}\n\
      </script>\n\    
    </head>";
    
    const char html_page_body[] PROGMEM="<body onload='GetSwitchState()'>\n\
      <h1>aHomeBus Net State</h1>\n\
      <div id='data_to_update'>\n\
      <p id=\"switch_txt\">Switch state: Not requested...</p>\n\
      </div>\n\
      <div id=\"box\"class=\"rb\">\n\
      text2\n\
      <div>IP Address</div><div><input id=\"na\" name=\"na\" type=\"text\" size=\"15\"/></div></div>\n\
      <div><div>Subnet</div><div><input id=\"nm\" name=\"nm\" type=\"text\" size=\"15\"/></div></div>\n\
      <div><div>Gateway</div><div><input id=\"ng\" name=\"ng\" type=\"text\" size=\"15\"/></div></div>\n\
      <div><div>DHCP</div><div><input id=\"nd\" name=\"nd\" type=\"checkbox\"/></div></div>\n\
      </div>\n\
      text3\n\
    </body>";
    
    //dR()
    
    const char html_page_bottom[] PROGMEM="</html>";

  client = server.available();
    if (client) {
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          if (c == '\n' && currentLineIsBlank) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: keep-alive");
            client.println();
            if (HTTP_req.indexOf("ajax_switch") > -1) {  // AJAX request for switch state
              // read switch state and send appropriate paragraph text
              GetSwitchState(client);
            }
            else {
              client.println(FSH_P(html_page_top));
              client.println(FSH_P(html_page_body));
              client.println(FSH_P(html_page_bottom));
            }
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






void AHB_WEB::loop_web3()
{
  // listen for incoming clients
  client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        // serch parameter from "HTTP GET"
        if(gflag){
          if(c != ' '){
              parm += c;
          }
          else{
              gflag = false;
          }
        }
        if(c == '?' && parm == ""){
          gflag = true;
        }
        //Serial.print(c);


        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header

          if(parm == ""){ // normal HTTP access
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();

            client.println("<html><head><title>Hello Arduino</title>");
            client.println("<meta name='viewport' content='width=240px' />");
            
            client.println("<script type='text/javascript'>");

            client.println("function createXMLHttpRequest(cbFunc){");
            client.println("  var XObj = new XMLHttpRequest();");
            client.println("  if(XObj) XObj.onreadystatechange = cbFunc;return XObj;}");

            client.println("function setData(val){htObj = createXMLHttpRequest(displayData);");
            client.println("  if(htObj){htObj.open('GET','/?btn='+val,true);htObj.send(null);}}");

            client.println("function getData(){htObj = createXMLHttpRequest(displayData);");
            client.println("  if(htObj){htObj.open('GET','/?=',true);htObj.send(null);}}");

            client.println("function displayData(){ if((htObj.readyState == 4) && (htObj.status == 200)){");
            client.println("  document.getElementById('result').innerHTML =  htObj.responseText;}}");

            client.println("function strT(){ getData(); timerID=setTimeout('strT()',");
            client.println("  document.getElementById('tf1').value);}"); 

            client.println("function clrT(){clearTimeout(timerID);}");
            
            client.println("</script></head><body onLoad='getData()'><form action='/' method='GET'><br />");

            client.println("<input id='btn0' type='button' value=' D0 ' onClick='setData(0)'>");
            client.println("<input id='btn1' type='button' value=' D1 ' onClick='setData(1)'>");
            client.println("<input id='btn2' type='button' value=' D2 ' onClick='setData(2)'>");
            client.println("<input id='btn3' type='button' value=' D3 ' onClick='setData(3)'><br /><br />");
            client.println("<input id='btn4' type='button' value=' D4 ' onClick='setData(4)'>");
            client.println("<input id='btn5' type='button' value=' D5 ' onClick='setData(5)'>");
            client.println("<input id='btn6' type='button' value=' D6 ' onClick='setData(6)'>");
            client.println("<input id='btn7' type='button' value=' D7 ' onClick='setData(7)'><br /><br />");

            client.println("</form><form><input id='btn100' type='button' value=' START ' onClick='strT()'>");
            client.println("<input id='btn200' type='button' value=' STOP ' onClick='clrT()'>");
            client.println("<input id='tf1' type='text' size='6' value='1000'>");
            client.println("</form><br /><div id='result'></div>");

            client.println("<br /></body></html>");

          }
          else{ // using XMLhttpObject access
  
            int check = parm.indexOf('=');
            if(check != -1){
              //Set Digital Port
              int check2 = parm.indexOf('btn');
              if(check2 != -1){
                int port = (parm.substring(check+1)).toInt();
                dout[port] = !dout[port];
                digitalWrite(port, dout[port]);
              }

             //Write JSONP Data (Digital & Analog Ports Status, Callback function name is 'cb')
               client.print("cb({\"D\":[");
                 for(int i=0; i<8; i++ ){
                   client.print(dout[i]);
                   if(i<7){
                     client.print(",");
                   }
                 }
               client.print("],\"A\":[");
                 for(int i=0; i<6; i++ ){
                   client.print(analogRead(i));
                   if(i<5){
                     client.print(",");
                   }
                 } 
               client.print("]});");
              
            }
            parm = "";
          }


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
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
  }
}


void AHB_WEB::loop_web4() {

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