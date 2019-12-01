/**
  aHomeBus io module - digital outputs

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

  Based on iSysBus - 2010 Patrick Amrhein, www.isysbus.org

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

#ifndef AHB_IO_DOUT__C
#define AHB_IO_DOUT__C

    #include <Arduino.h>
    #include <inttypes.h>
    #include <ahb.h>

    AHB_IO_DOUT::AHB_IO_DOUT(byte cfgId) {
        _cfgId=cfgId;
    }

    bool AHB_IO_DOUT::cfgRead(unsigned int address) {
        if(_control == NULL) return false;
        byte temp,temp2,check;
        #ifdef EEPROM_USE
        check = EEPROM.read(address);
        #endif
        if(check == 0xFF || check == 0x00) return false;
        if(((check & 0xF0) >> 4) != _cfgId) return false;

        for(temp=0; temp<_items; temp++) {
            if(_config[temp].pin == 0xFF || _config[temp].pin == 0x00) { //Slot is free
                #ifdef EEPROM_USE
                EEPROM.get(address+1, _config[temp]);
                #endif

                if(_config[temp].pin == 0xFF || _config[temp].pin == 0x00) return false;

                ::pinMode(_config[temp].pin, OUTPUT);

                temp2 = _config[temp].init;
                _config[temp].last = temp2;

                if(_config[temp].invert) temp2 ^= 0xFF;
                //@TODO analog
                ::digitalWrite(_config[temp].pin, temp2);

                //Poll other nodes for last state
                byte data[1] = {AHB_CMD_REQ};
                
                //ahbSend(uint8_t type, uint8_t target, byte len, byte data[8])
                //_control->ahbSend_M(AHB_PKGTYPE_MULTICAST, _config[temp].target, sizeof(data), data);
                _control->ahbSend(AHB_PKGTYPE_MULTICAST, _config[temp].target, sizeof(data), data);
                return true;
            }
        }
        return false;
    }

    bool AHB_IO_DOUT::cfgWrite(ahbIoDOut &cfg) {
        if(_control == NULL) {
            #ifdef AHB_DEBUG
                Serial.println(F("No controller...")); Serial.flush();
            #endif
            return false;
        }
        
        unsigned int address;

        if(cfg.pin == 0xFF) {
            #ifdef AHB_DEBUG
                Serial.println(F("No pin...")); Serial.flush();
            #endif
            return false;
        }

        address = _control->cfgFindFreeblock(sizeof(cfg)+1, _cfgId);
        if(address == 0) {
            #ifdef AHB_DEBUG
                Serial.println(F("Got no address...")); Serial.flush();
            #endif
            return false;
        }
        address++; //Skip header
        #ifdef EEPROM_USE
        EEPROM.put(address, cfg);
        #endif
        
        return true;
    }

    bool AHB_IO_DOUT::cfgReset(void) {
        if(_config == NULL) return true;
        free(_config);
        _items = 0;
        return true;
    }

    bool AHB_IO_DOUT::cfgReserve(byte objects) {
        cfgReset();
        _config = (ahbIoDOut *) calloc(objects, sizeof(ahbIoDOut));
        if(_config==NULL) {
            _items = 0;
            return false;
        }else{
            _items = objects;
            return true;
        }
    }

    bool AHB_IO_DOUT::process(ahbPacket &pkg) {
        if(_control == NULL) return false;
        byte i,temp;
        
        if(pkg.len > 0) {
            switch(pkg.data[0]) {
                //@TODO AHB_IO_DOUT_PULSE
                case AHB_CMD_1B:
                    if(pkg.meta.type != AHB_PKGTYPE_MULTICAST || pkg.len != 2) break;
                    for(i=0; i<_items; i++) {
                        if(_config[i].pin != 0xFF && _config[i].pin != 0x00 && (_config[i].target == pkg.meta.target || _config[i].target == 0)) {
                            if(_config[i].invert) {
                                digitalWrite(_config[i].pin, (pkg.data[1] ^ 1));
                            }else{
                                digitalWrite(_config[i].pin, pkg.data[1]);
                            }
                        }
                    }
                break;

                case AHB_CMD_PER:
                    if(pkg.meta.type != AHB_PKGTYPE_MULTICAST || pkg.len != 2) break;
                    for(i=0; i<_items; i++) {
                        if(_config[i].pin != 0xFF && _config[i].pin != 0x00 && (_config[i].target == pkg.meta.target || _config[i].target == 0)) {
                            switch(_config[i].mode) {
                                case AHB_IO_DOUT_LIN:
                                    if(_config[i].invert) {
                                        analogWrite(_config[i].pin, map(pkg.data[1], 0, 100, 255, 0));
                                    }else{
                                        analogWrite(_config[i].pin, map(pkg.data[1], 0, 100, 0, 255));
                                    }
                                break;
                                case AHB_IO_DOUT_LED:
                                    temp = pow (2, (pkg.data[1] / _ledFactor)) - 1;
                                    if(_config[i].invert) temp = 255-temp;
                                    analogWrite(_config[i].pin, temp);
                                break;
                                //@TODO AHB_IO_DOUT_SER
                            }
                        }
                    }
                break;
            }
        }
        return true;
    }

    bool AHB_IO_DOUT::loop(void) {
        if(_control == NULL) return false;
        return true;
    }

    bool AHB_IO_DOUT::attach(unsigned int target, byte pin, byte mode, bool invert, bool init) {
        if(_control == NULL) return false;
        
        ahbIoDOut cfg;

        cfg.target = target;
        cfg.pin = pin;
        cfg.invert = invert;
        cfg.mode = mode;
        cfg.init = init;

        cfgWrite(cfg);
        _control->hookDetachModule(_cfgId);
        return _control->hookAttachModule(this);
    }

#endif /* AHB_IO_DOUT__C */
