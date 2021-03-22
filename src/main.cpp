#include <Arduino.h>

#include <SPI.h>
#include "SX128XLT.h"
#include "ProgramLT_Definitions.h"

#include "pin_def.h"

#include "Settings.h"

int8_t TestPower;
uint8_t TXPacketL;

uint32_t RXpacketCount;
uint32_t errors;

uint8_t RXBUFFER[RXBUFFER_SIZE];                 //create the buffer that received packets are copied into

uint8_t RXPacketL;                               //stores length of packet received
int8_t  PacketRSSI;                              //stores RSSI of received packet
int8_t  PacketSNR;                               //stores signal to noise ratio of received packet

uint32_t Test1Count[32];                         //buffer where counts of received packets are stored, -18dbm to +12dBm
uint32_t Mode1_Cycles = 0;                       //count the number of cyles received
bool updateCounts = false;                       //update counts set to tru when first TestMode1 received, at sequence start

SX128XLT LT;

#include "progs.h"

void setup() {

  #ifdef tx
    pinMode(LED1, OUTPUT);                        //setup pin as output for indicator LED
    led_Flash(2, 125);                            //two quick LED flashes to indicate program start

    Serial.begin(115200);
    Serial.println();
    Serial.print(__TIME__);
    Serial.print(F(" "));
    Serial.println(__DATE__);

    Serial.println(F("10_LoRa_Link_Test_Transmitter Starting"));

    SPI.begin();

    if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, LORA_DEVICE))
    {
      Serial.println(F("Device found"));
      led_Flash(2, 125);
      delay(1000);
    }
    else
    {
      Serial.println(F("No device responding"));
      while (1)
      {
        led_Flash(50, 50);                                      //long fast speed flash indicates device error
      }
    }

    LT.setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate);

    Serial.println();
    LT.printModemSettings();                                    //reads and prints the configured LoRa settings, useful check
    Serial.println();
    LT.printOperatingSettings();                                //reads and prints the configured operting settings, useful check
    Serial.println();
    Serial.println();
    LT.printRegisters(0x900, 0x9FF);                              //print contents of device registers
    Serial.println();
    Serial.println();

    Serial.print(F("Transmitter ready"));
    Serial.println();
  #endif

  #ifdef rx

    pinMode(LED1, OUTPUT);                        //setup pin as output for indicator LED
    led_Flash(2, 125);                            //two quick LED flashes to indicate program start

    Serial.begin(115200);
    Serial.println();
    Serial.print(__TIME__);
    Serial.print(F(" "));
    Serial.println(__DATE__);
    Serial.println(F("20_LoRa_Link_Test_Receiver Starting"));
    Serial.println();

    if (BUZZER > 0)
    {
      pinMode(BUZZER, OUTPUT);
      digitalWrite(BUZZER, HIGH);
      delay(50);
      digitalWrite(BUZZER, LOW);
    }

    //setup SPI, its external to library on purpose, so settings can be mixed and matched with other SPI devices
    SPI.begin();

    //SPI beginTranscation is normally part of library routines, but if it is disabled in library
    //a single instance is needed here, so uncomment the program line below
    //SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

    //setup hardware pins used by device, then check if device is found
    if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, LORA_DEVICE))
    {
      Serial.println(F("LoRa Device found"));
      led_Flash(2, 125);
      delay(1000);
    }
    else
    {
      Serial.println(F("No device responding"));
      while (1)
      {
        led_Flash(50, 50);                                       //long fast speed LED flash indicates device error
      }
    }

    //this function call sets up the device for LoRa using the settings from settings.h
    LT.setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate);

    Serial.println();
    LT.printModemSettings();                                     //reads and prints the configured LoRa settings, useful check
    Serial.println();
    LT.printOperatingSettings();                                //reads and prints the configured operting settings, useful check
    Serial.println();
    Serial.println();

    Serial.print(F("Receiver ready - RXBUFFER_SIZE "));
    Serial.println(RXBUFFER_SIZE);
    Serial.println();
  #endif

}

void loop() {
  
  #ifdef tx

    Serial.println(F("Start Test Sequence"));
    Serial.print(F("Start Packet> "));

    uint8_t buff[3];                    //the packet is built in this buffer

    digitalWrite(LED1, digitalRead(BUTTON));

    buff[0] = digitalRead(BUTTON);
    buff[1] = '9';
    buff[2] = '9';
    TXPacketL = sizeof(buff);

    LT.printASCIIPacket(buff, sizeof(buff));

    //digitalWrite(LED1, HIGH);
    TXPacketL = LT.transmitAddressed(buff, sizeof(buff), TestMode1, Broadcast, ThisNode, 5000, start_power, WAIT_TX);
    //delay(500);                //longer delay, so that the start test sequence is obvious
    //digitalWrite(LED1, LOW);

    if (TXPacketL == 0)
    {
      packet_is_Error();
    }
    else
    {
      packet_is_OK();
    }


    Serial.println(F("Finished Test Sequence"));
    Serial.println();
  #endif

  #ifdef rx
    RXPacketL = LT.receiveAddressed(RXBUFFER, RXBUFFER_SIZE, 15000, WAIT_RX); //wait for a packet to arrive with 15seconds (15000mS) timeout

    Serial.println(RXBUFFER[0]);
/*
    if (RXPacketL == 0)                            //if the LT.receive() function detects an error, RXpacketL == 0
    {
      packet_is_Error();
    }
    else
    {
      packet_is_OK();
    }
*/

    digitalWrite(LED1, RXBUFFER[0]);                       //LED off

  #endif
}