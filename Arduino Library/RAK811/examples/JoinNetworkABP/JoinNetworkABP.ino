

#include "RAK811.h"
#include "SoftwareSerial.h"
#define WORK_MODE LoRaWAN   //  LoRaWAN or LoRaP2P
#define JOIN_MODE ABP    //  OTAA or ABP
#if JOIN_MODE == OTAA
String DevEui = "60C5A8FFFE000001";
String AppEui = "70B3D57EF00047C0";
String AppKey = "5D833B4696D5E01E2F8DC880E30BA5FE";
#else JOIN_MODE == ABP
String NwkSKey = "3432567afde4525e7890cfea234a5821";
String AppSKey = "a48adfc393a0de458319236537a11d90";
String DevAddr = "00112233";
#endif
#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
#define ATSerial Serial 
SoftwareSerial DebugSerial(RXpin,TXpin);    // Declare a virtual serial port
char* buffer = "72616B776972656C657373";

RAK811 RAKLoRa(ATSerial);



void setup() {
 DebugSerial.begin(115200);
 while(DebugSerial.read()>= 0) {}
 while(!DebugSerial);
 DebugSerial.println("StartUP");

 ATSerial.begin(115200);

}

void loop() {
 
 DebugSerial.println(RAKLoRa.rk_getVersion());
 delay(500);
 DebugSerial.println(RAKLoRa.rk_getBand());
 delay(500);
 if(RAKLoRa.rk_setWorkingMode(WORK_MODE))
 {
  DebugSerial.println("you set Working mode is OK!");
  if (RAKLoRa.rk_initABP(DevAddr,NwkSKey,AppSKey))
    {
      DebugSerial.println("you init ABP parameter is OK!");
      if (RAKLoRa.rk_joinLoRaNetwork(JOIN_MODE))
       {
           DebugSerial.println("you join Network is OK!");
           ATSerial.setTimeout(8000);
           String ver = ATSerial.readStringUntil('\n');
           DebugSerial.println(ver);
           ATSerial.setTimeout(2000);
           if (ver == STATUS_JOINED_SUCCESS)
           {
            while (RAKLoRa.rk_sendData(1,1,buffer))
            {
              DebugSerial.println(RAKLoRa.rk_recvData());
            }
           }
        }
     }
 }
 delay(5000);
}
