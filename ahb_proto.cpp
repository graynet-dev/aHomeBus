/*PIC18 */
/*Node Guard, Heartbeat и Boot-up.*/
/*void _CO_COMM_NMTE_HeartBeatAccessEvent(void);
void _CO_COMM_NMTE_GuardTimeAccessEvent(void);
void _CO_COMM_NMTE_LifeFactorAccessEvent(void);
void _CO_COMM_NMTE_Open(void);
void _CO_COMM_NMTE_Close(void);
void _CO_COMM_NMTE_RXEvent(void);
void _CO_COMM_NMTE_TXEvent(void);
void _CO_COMM_NMTE_LSTimerEvent(void);
#define mNMTE_SetHeartBeat(HeartBeat) _uNMTEHeartBeat.word = HeartBeat;
#define mNMTE_GetHeartBeat()      _uNMTEHeartBeat.word
#define mNMTE_SetGuardTime(GuardTime) _uNMTEGuardTime.word = GuardTime;
#define mNMTE_GetGuardTime()      _uNMTEGuardTime.word
#define mNMTE_SetLifeFactor(LifeFactor) _uNMTELifeFactor.byte = LifeFactor;
#define mNMTE_GetLifeFactor()     _uNMTELifeFactor.byte
*/
/*PIC18 */
/*NMT*/
/*void _CO_COMM_NMT_RXEvent(void);
void _CO_COMM_NMT_Open(void);
void _CO_COMM_NMT_Close(void);
#define mNMT_Start()          COMM_STATE_STOP = 0
#define mNMT_Stop()           COMM_STATE_STOP = 1
#define mNMT_GotoPreopState()     {COMM_STATE_PREOP = 1; COMM_STATE_OPER = 0; COMM_STATE_STOP = 0;}
#define mNMT_GotoOperState()      {COMM_STATE_PREOP = 0; COMM_STATE_OPER = 1; COMM_STATE_STOP = 0;}
#define mNMT_StateIsStopped()     COMM_STATE_STOP
#define mNMT_StateIsOperational()   (COMM_STATE_OPER && (!COMM_STATE_STOP))
#define mNMT_StateIsPreOperational()  (COMM_STATE_PREOP && (!COMM_STATE_OPER) && (!COMM_STATE_STOP))
*/
/*KaCanOpen*/
/*
void NMT::send_nmt_message(uint8_t node_id, Command cmd);
void NMT::broadcast_nmt_message(Command cmd)
void NMT::reset_all_nodes()
void NMT::discover_nodes()
void NMT::process_incoming_message(const Message& message)
void NMT::register_device_alive_callback(const DeviceAliveCallback& callback)
void NMT::register_new_device_callback(const NewDeviceCallback& callback)
*/
/*CanFestivio*/
/*
void proceedNMTstateChange(Message * m);
UNS8 slaveSendBootUp();

UNS8 masterSendNMTstateChange (CO_Data* d, UNS8 nodeId, UNS8 cs);
UNS8 masterSendNMTnodeguard (CO_Data* d, UNS8 nodeId);
UNS8 masterRequestNodeState (CO_Data* d, UNS8 nodeId);
*/

/*
NMT      -> MANAGEMENT - Node Control
            MONITORING - Node guarding, Heartbeat и 
            BOOT -boot-up

NMT_STATES = {
    0: 'INITIALISING',    - +
    4: 'STOPPED',         - +
    5: 'OPERATIONAL',     - + полный функционал
    80: 'SLEEP',          -
    96: 'STANDBY',        -
    127: 'PRE-OPERATIONAL'- + конфигурирование по SDO, PDO отключен???
}

Module Control Services -> master управляет состоянием устройств NMT slave.
  Start Remote Node     - Master устанавливает состояние выбранных узлов NMT Slaves в состояние OPERATIONAL.
  Stop Remote Node      - Master устанавливает состояние выбранных узлов NMT Slaves в состояние STOPPED
  Enter Pre-Operational - Master устанавливает состояние выбранного узла NMT Slave (или узлов) в состояние PREOPERATIONAL
  Reset Node            - переводит состояние выбранного узла NMT Slave (или узлов) из любого состояния в 
                          промежуточное состояние "сброс приложения".
  Reset Communication   - переводит состояние выбранного узла NMT Slave (или узлов) из любого состояния в 
                          промежуточное состояние "сброс обмена данными". После завершения этой службы выбранные 
                          узлы сети будут в состоянии RESET COMMUNICATION.

Error Control Services  -> определяет отказы в сети CAN
  Node Guarding Event   - показывает, что произошла ошибка сетевого узла, если она была разрешена для узла, 
                          идентифицированного по Node-ID.
  Life Guarding Event   - провайдер службы NMT на устройстве NMT Slave показывает, что произошла ошибка сетевого 
                          узла, или она была исправлена.
  Heartbeat Event       - потребитель Heartbeat показывает, что произошла ошибка heartbeat, или эта ошибка была 
                          исправлена на узле, идентифицированном по Node-ID.
  
Bootup Service          -> показывает, что его локальное состояние перешло из INITIALISING в состояние PRE-OPERATIONAL.
  Bootup Event          - 

node guarding           NMT-master последовательно (через guard time) шлет всем NMT-slave фрейм CAN remote в ответ 
                        NMT-slave отдает текущее коммуникационное состояние (stopped, operational, pre-operational).
                        Если узел не ответил на запрос NMT-masterв течении "node life time", время жизни узла, то 
                        регистрируется ошибка node-guarding event" (событие защиты узла). 
                        !!! NMT-slave так же мониторят и мастер показывает это своему хост-контроллеру через 
                        "Life guarding event" (событие защиты жизни сети). 
              
NMT_COMMANDS = {
    'OPERATIONAL': 1,
    'STOPPED': 2,
    'SLEEP': 80,
    'STANDBY': 96,
    'PRE-OPERATIONAL': 128,
    'INITIALISING': 129,
    'RESET': 129,
    'RESET COMMUNICATION': 130
}
            

SYNC     Sync producer, consumer


DATA CMS -> PDO
            SDO

DBT динамическое распределение идентификаторов

LMT: управление конфигурированием устройства

MNS
NMM
ENCY
LED
FLY
SDM, SRD
LSS

CANopen master
  Network management
  Automatic discovery of nodes on network
  Dynamically loadable drivers
  SDO client with expediated and segmented mode transfer
  EDS reader
  REST interface for accessing object-dictionary based on EDS
Virtual CANopen nodes for testing
  SDO server with expediated and segmented mode transfer
  Virtual nodes are configurable via ini files
CAN-TCP bridge
  Transfers raw CAN frames over TCP
Traffic monitoring
  A tool is included to monitor and interpret CANopen traffic
Supports Linux Socket CAN
Can be ported to other platforms with little effort

*/