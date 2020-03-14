/**
  aHomeBus protocol 

  @copyright 2019 Ivan Raspopov
  
  Based on aSysBus - 2015-2017 Florian Knodt, www.adlerweb.info

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

#ifndef AHB_PROTO__H
#define AHB_PROTO__H

    #include <Arduino.h>
    #include <inttypes.h>
    
//CMD IN ID   FRAME
//NMT - управление сетью
    #define AHB_CMD_F_NMT_NULL                    0  //
    #define AHB_CMD_F_NMT_PING                    1  //
    #define AHB_CMD_F_NMT_PONG                    2  //
    #define AHB_CMD_F_NMT_ACCIDENT_SEND           4  //
    #define AHB_CMD_F_NMT_ACCIDENT_ANSWER         5  //
    #define AHB_CMD_F_NMT_HEARTBEAT               6  //
    #define AHB_CMD_F_NMT_HEARTBEAT_TO_SLAVE      7
    #define AHB_CMD_F_NMT_BOOT                    8  //
    #define AHB_CMD_F_NMT_REBOOT                  9  //
//PDO - передача коротких сообщений за такт     
    #define AHB_CMD_F_PDO_CMD_SEND                16  //
    #define AHB_CMD_F_PDO_CMD_ANSWER              17  //
    
    #define AHB_CMD_F_PDO_DATA_REQUEST            32  //
    #define AHB_CMD_F_PDO_DATA_ANSWER             33  //
    #define AHB_CMD_F_PDO_DATA_RECIVE             34  //
//SDO - передача длинных сообщений за n тактов   
    #define AHB_CMD_F_SDO_SEND_TX_INIT_REQUEST    48  //
    #define AHB_CMD_F_SDO_SEND_TX_INIT_ANSWER     49  //
    #define AHB_CMD_F_SDO_SEND_TX_BEGIN           50  //
    #define AHB_CMD_F_SDO_SEND_TX_PROCESS         51  //
    #define AHB_CMD_F_SDO_ANSWER_TX_PROCESS_OK    52  //
    #define AHB_CMD_F_SDO_SEND_TX_REPEAT          53  //
    #define AHB_CMD_F_SDO_ANSWER_TX_REPEAT_OK     53  //
    #define AHB_CMD_F_SDO_SEND_TX_BEGIN           55  //
//SYNC - команда всем начать синхронизацию
    #define AHB_CMD_F_SYNC_ALL                    64  //
    
//CMD IN DATA FRAME
    //#define AHB_PKGTYPE_BROADCAST 0x00
    #define AHB_PKGTYPE_MULTICAST 0
    #define AHB_PKGTYPE_UNICAST   1

    #define AHB_CMD_LEGACY_8B     02
    #define AHB_CMD_BOOT          21
    #define AHB_CMD_REQ           40
    #define AHB_CMD_0B            50 //0-Bit, generic pulse
    #define AHB_CMD_1B            51 //1-Bit, on/off
    #define AHB_CMD_PER           52 //%
    #define AHB_CMD_PING          70
    #define AHB_CMD_PONG          71
    #define AHB_CMD_CFG_READ      80 //2-byte address
    #define AHB_CMD_CFG_WRITE     81 //2-byte-address + data
    #define AHB_CMD_CFG_COMMIT    82 //2-byte-address
    #define AHB_CMD_IDENT         85 //Change local address, 2-byte-address
    #define AHB_CMD_S_TEMP        0xA0 //x*0.1°C, int
    #define AHB_CMD_S_HUM         0xA1 //x*0.1%RH, unsigned int
    #define AHB_CMD_S_PRS         0xA2 //x*0.1hPa, unsigned int
    #define AHB_CMD_S_LUX         0xA5 //unsigned long
    #define AHB_CMD_S_UV          0xA6 //*0.1, unsigned int
    #define AHB_CMD_S_IR          0xA7 //unsigned long
    #define AHB_CMD_S_PM25        0xB0 //TBD
    #define AHB_CMD_S_PM10        0xB1 //TBD
    #define AHB_CMD_S_VOLT        0xC0 //x*0.01V, int
    #define AHB_CMD_S_AMP         0xC1 //x*0.01A, int
    #define AHB_CMD_S_PWR         0xC2 //x*0.1W or VA, long
    #define AHB_CMD_S_PER         0xD0 //%, byte
    #define AHB_CMD_S_PML         0xD1 //‰, unsingned int
    #define AHB_CMD_S_PPM         0xD2 //Parts per million, unsingned int
    #define AHB_CMD_S_PY          0xD5 //smth per Year, unsinged int
    #define AHB_CMD_S_PMo         0xD6 //smth per Month, unsinged int
    #define AHB_CMD_S_PD          0xD7 //smth per Day, unsinged int
    #define AHB_CMD_S_PH          0xD8 //smth per Hour, unsinged int
    #define AHB_CMD_S_PM          0xD9 //smth per Minute, unsinged int (RPM, Pulse PM, etc)
    #define AHB_CMD_S_PS          0xDA //smth per Second, unsinged int

    /**
     * Packet metadata
     * Contains all data except things related to the actual payload
     */
typedef struct {
      /**
       * Message type
       *  not use -> Broadcast
       *  0 -> Multicast
       *  0 -> Unicast
       */
      uint32_t type = 0;
      /*
      Command
      */
      uint32_t cmd = 0;
      /**
       * Port
       * 0x0 - 0xF
       * Only used in Unicast Mode, otherwise -1
       */
       uint32_t port = 0;

      /**
       * Target address
       * Unicast:      0x01 - 0xFF
       * Milticast:    0x01 - 0xFF
       * 0x00 = broadcast
       */
       uint32_t target = 0;

      /**
       * Source address
       * 0x01 - 0xFF
       * 0x00 = invalid packet
       */
       uint32_t source = 0;

      /**
       * Interface the message originated from
       * @see AHB::busAttach
       */
      signed char busId = -1;
      
      /**
       * busType from  typedef enum enum_busType
       * 
       */
      uint8_t busType = 0;
} ahbMeta;

    /**
     * Complete Packet
     * Contains metadata, a length indicator and the payload itself
     * @see ahbMeta
     */
typedef struct {
        /**
         * @see ahbMeta
         */
        ahbMeta meta;
        /**
         * length in bytes, 0-8. -1 indicates invalid packet
         */
       char len = -1;
        /**
         * payload
         */
        byte data[8];
} ahbPacket;



#endif /* AHB_PROTO__H */

