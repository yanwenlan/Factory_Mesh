/***
 *  This example shows LoRaWan protocol joining the network in OTAA mode, class A, region EU868.
 *  Device will send uplink every 20 seconds.
***/
// include "LoRa_OTAA.ino"
#define OTAA_PERIOD   (10000)
/*************************************
   LoRaWAN band setting:
     RAK_REGION_EU433
     RAK_REGION_CN470
     RAK_REGION_RU864
     RAK_REGION_IN865
     RAK_REGION_EU868
     RAK_REGION_US915
     RAK_REGION_AU915
     RAK_REGION_KR920
     RAK_REGION_AS923
 *************************************/
#define OTAA_BAND     (RAK_REGION_US915)
#define OTAA_DEVEUI   {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0xEB, 0x31}
#define OTAA_APPEUI   {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0xEB, 0x31}
#define OTAA_APPKEY   {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0xEB, 0x31, 0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0xEB, 0x31}

uint32_t count=0;
uint8_t sd_flag = 0;  //0:Bluetooth;  1:SD mode;

/*
*@初始化
*/
void setup()
{
  init_mesh();
  // Serial1.println("AT+CHMODE\r\n");    
  // handle_chmode();
  delay(20);
  char chmod_1[20] = "AT+CHMODE\r\n";
  // handle_chmode(chmode_1);
  process_AT_Command(chmod_1);
  delay(20);
  process_AT_Command(chmod_1);
  
  delay(100);    
  Serial1.println("AT+BUSY=0\r\n");
  delay(100);
  Serial1.println("AT+VOL=10\r\n");
  delay(1000);
  Serial1.println("AT+PLAY=sd0,26\r\n");
  // Serial.println("setup finish...\r\n");
  delay(100);
  uint8_t* temp = (uint8_t*)malloc(50*sizeof(uint8_t));  
  Serial1Read(temp);
  free(temp);
  *temp = NULL;  
  delay(100);
}

void loop()
{
    // static uint64_t last = 0;
    // static uint64_t elapsed;
    
    // if ((elapsed = millis() - last) > OTAA_PERIOD) {
    //     uplink_routine();
    //     last = millis();
    // }
    // Serial.println("after send uplink..., send to serial1...");
    if(count > 600)
    {
      // Serial.println("loop start ------->");
      // Serial1.println("AT+PLAY=sd0,26\r\n");
      // Serial1.println("AT+CHMODE\r\n");
      // Serial1Read();
      // handle_chmode();
      // Serial.println("<----------loop end");
      count=0;
    }
    
    // Serial.println("finish read...");
    // Serial.printf("Try sleep %ums..", OTAA_PERIOD);
    // api.system.sleep.all(OTAA_PERIOD);
    delay(10);
    //Serial.println("Wakeup..");
    count++;
}
