#define OTAA_PERIOD   (60000)
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

void sendToRS485(uint8_t * temp, uint8_t size);
void recvCallback(SERVICE_LORA_RECEIVE_T * data);
void joinCallback(int32_t status);
void sendCallback(int32_t status);
void init_mesh();
void uplink_routine();
void loop_otaa_test();
