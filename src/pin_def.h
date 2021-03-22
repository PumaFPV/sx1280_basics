
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