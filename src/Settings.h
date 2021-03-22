#define LORA_DEVICE DEVICE_SX1280               //we need to define the device we are using  

//LoRa Modem Parameters
const uint32_t Frequency = 2445000000;           //frequency of transmissions
const int32_t Offset = 0;                        //offset frequency for calibration purposes  
const uint8_t Bandwidth = LORA_BW_0400;          //LoRa bandwidth
const uint8_t SpreadingFactor = LORA_SF7;        //LoRa spreading factor
const uint8_t CodeRate = LORA_CR_4_5;            //LoRa coding rate

#ifdef is_Tx

const uint8_t TXpower = 10;                      //Power for transmissions in dBm

#endif

const uint16_t packet_delay = 1000;             //mS delay between packets

#define RXBUFFER_SIZE 32                        //RX buffer size  

const int8_t TXpower = 10;                       //Transmit power used when sending packet starting test sequence
const int8_t start_power = 10;                   //link test starts at this transmit power, maximum +12dBm
const int8_t end_power = -18;                    //and ends at this power, minimum -18dBm 
const uint8_t ThisNode = 'T';                    //this identifies the node in transmissions


#define packet_delay 250                         //mS delay between packets
#define mode_delaymS 2000                        //mS delay after sending start test sequence  
