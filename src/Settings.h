#ifdef stm32
    #define NSS PA4
    #define RFBUSY PA15
    #define NRESET PA12

    #define LED1 PC13
    #define DIO1 PB3
    #define DIO2 PB4                //not used 
    #define DIO3 PB5                 //not used
    #define RX_EN -1                //pin for RX enable, used on some SX1280 devices, set to -1 if not used
    #define TX_EN -1                //pin for TX enable, used on some SX1280 devices, set to -1 if not used  
    #define BUZZER -1               //pin for BUZZER, set to -1 if not used 
    #define BUTTON PA0
#endif

#ifdef esp32
    #define NSS 5
    #define RFBUSY 33
    #define NRESET 25

    #define LED1 22
    #define DIO1 32
    #define DIO2 35
    #define DIO3 34
    #define RX_EN -1
    #define TX_EN -1
    #define BUZZER -1
    #define BUTTON -1
#endif

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
