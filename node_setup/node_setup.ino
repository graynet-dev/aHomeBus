#include "node_setup.h"

//SerialCommand cmd_led_on_("ON", cmd_led_on);
//SerialCommand cmd_led_off_("OFF", cmd_led_off);

void setup(){
  Serial.begin(115200);
  Serial.println("Enter nomber :");
  Serial.println("333 - for read_full_eeprom");
  Serial.println("444 - for read_node_addres");
  Serial.println("555 - for erace eeprom");
  Serial.println("0-255 - for save node address");
}


void setup_node (){
  if (Serial.available()>0){
    String serial_input;
    //char node_id[4];
    unsigned int serial_input_int;
    uint8_t rez;
    serial_input = Serial.readString ();
    serial_input_int=serial_input.toInt();
    //serial_input.toCharArray(node_id, 4);

    if (serial_input_int == 0 || serial_input_int <256){
      //Serial.println(serial_input[0]);
      //Serial.println(serial_input[1]);
      //Serial.println(serial_input[2]);
      //Serial.println(serial_input[3]);
      rez = serial_input_int;
      //rez=(serial_input[0]-48)*16*16+(serial_input[1]-48)*16+serial_input[2]-48;
      //Serial.println(rez);
      //Serial.println(rez,HEX);
      //Serial.println(serial_input);
      //Serial.println(serial_input_int);
      //Serial.println(node_id);
      //Serial.println(atoi(node_id[1]));
      //Serial.println(atoi(node_id),HEX);
      write_eeprom(0,rez);
    }
    else{
      if (serial_input_int == 333) read_full_eeprom();
      else if (serial_input_int == 444) read_node_addres();   
      else if (serial_input_int == 555) erace_eeprom();    
    }
  }
}

void erace_eeprom() {
  Serial.println("Erasing...");
  for (unsigned int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0x00);
  }
  Serial.println("Done");
  Serial.println("Enter nomber :");
  Serial.println("333 - for read_full_eeprom");
  Serial.println("444 - for read_node_addres");
  Serial.println("555 - for erace eeprom");
  Serial.println("0-255 - for save node address");
}


void read_full_eeprom() {
  byte check;
  for (unsigned int i = 0 ; i < EEPROM.length() ; i++) {
    check = EEPROM.read(i);
    Serial.print("0x");
    Serial.print(i, HEX);
    Serial.print(" => ");
    Serial.print(check);
    Serial.print(" => ");
    Serial.println(check, HEX);

  }
  Serial.println("Done");
  Serial.println("Enter nomber :");
  Serial.println("333 - for read_full_eeprom");
  Serial.println("444 - for read_node_addres");
  Serial.println("555 - for erace eeprom");
  Serial.println("0-255 - for save node address");
}

void write_eeprom(unsigned int _cfgAddrStart, unsigned int id){
  EEPROM.put(_cfgAddrStart, id);
  Serial.print ("PUT node address is - ");
  Serial.print (id);
  Serial.print (" | ");
  Serial.println (id,HEX);
  Serial.println("Enter nomber :");
  Serial.println("333 - for read_full_eeprom");
  Serial.println("444 - for read_node_addres");
  Serial.println("555 - for erace eeprom");
  Serial.println("0-255 - for save node address");
}

void read_node_addres(){
  unsigned int cfg = 0xFFFF;
  EEPROM.get(0, cfg);
  Serial.print ("GET Node address is - ");
  Serial.print (cfg);
  Serial.print (" | ");
  Serial.println (cfg,HEX);
  Serial.println("Enter nomber :");
  Serial.println("333 - for read_full_eeprom");
  Serial.println("444 - for read_node_addres");
  Serial.println("555 - for erace eeprom");
  Serial.println("0-255 - for save node address");
}

void loop(){
setup_node ();
}
