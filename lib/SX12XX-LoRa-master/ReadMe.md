# SX12XX Library

### Library installation

To install the library select the 'Clone or download' button on the main Github page, then select 'Download Zip'. In the Arduino IDE select 'Sketch' then 'Include Library'. Next select 'Add .ZIP library' and browse to and select the ZIP file you downloaded, it's called 'SX12xx-master.zip'.


### Warning
**The Semtech devices that this library supports are all 3.3V logic level devices so do not use directly with 5V logic level Arduinos, some form of logic level conversion is needed.** There are no specific logic level converters I could recommend. 

### Support for new Devices

New LoRa devices keep appearing all the time, but they cost money, so if you have found the library useful you can support of the purchase of new modules, so they can be tested, by making a donation here;

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=4EMBJU5C2UGNS)

This library supports the SX126x, SX127x and SX128x Semtech LoRa devices. There is a wide range of example programs for these devices. These Semtech devices are used to manufacture a range of LoRa modules sold by companies such as Hope, Dorji, NiceRF and others. The library does not support LoRa modules with a UART based interface such as those from Ebyte and Microchip. 

The objective of the library was to allow the same program sketches to be used across the range of UHF lora modules SX126x and SX127x (UHF) as well as the 2.4Ghz SX128x modules. A sketch written for the SX1278 (for example) should then run with very minor changes on the SX1262 or SX1280. However, whilst the SX126x and SX128x modules use the same style of device programming, the SX127x programming is completely different. The function style used for the SX126x and SX128x devices has been copied to create a matching style for the SX127x.

A conventional lora library normally uses a buffer of data created within the Arduino sketch to contain the data that is sent as a packet. This library has those functions, see the example programs 3 and 4 in the 'Basics' folder. There are examples for sending\receiving a simple character buffer ("Hello World") and for sending\receiving a data structure which can also include a character buffer. 


###Direct access to lora device internal data buffer
 
An additional library feature has been implemented to enable variables or character data to be written direct to the lora devices internal buffer. This has the benefit of not requiring a memory buffer in the Arduino and also lends itself to a simple way of sending and receiving packets. For instance this is the routine to create a packet for transmission taken from the 'LowMemory' folder;

	LT.startWriteSXBuffer(0);                         //start the write at location 0
	LT.writeBuffer(trackerID, sizeof(trackerID));     //= 13 bytes (12 characters plus null (0) at end)
	LT.writeUint32(TXpacketCount);                    //+4 = 17 bytes
	LT.writeFloat(latitude);                          //+4 = 21 bytes
	LT.writeFloat(longitude);                         //+4 = 25 bytes
	LT.writeUint16(altitude);                         //+2 = 27 bytes
	LT.writeUint8(satellites);                        //+1 = 28 bytes
	LT.writeUint16(voltage);                          //+2 = 30 bytes
	LT.writeInt8(temperature);                        //+1 = 31 bytes total to send
	len = LT.endWriteSXBuffer();

This is the matching code for the receiver;

	LT.startReadSXBuffer(0);               //start buffer read at location 0
	LT.readBuffer(receivebuffer);          //read in the character buffer
	txcount  = LT.readUint32();            //read in the TXCount
	latitude = LT.readFloat();             //read in the latitude
	longitude = LT.readFloat();            //read in the longitude
	altitude = LT.readUint16();            //read in the altitude
	satellites = LT.readUint8();           //read in the number of satellites
	voltage = LT.readUint16();             //read in the voltage
	temperature = LT.readInt8();           //read in the temperature
	RXPacketL = LT.endReadSXBuffer();
  

Clearly as with other methods of sending data the order in which the packet data is created in the transmitter has to match the order that it is read in the receiver.



###Considerations for pin usage

Pins settings and usage must be set up the the 'Settings.h' file that is include in each sketch folder. Program **2\_Register\_Test** does not use a 'Settings.h' file however. 

The library supports the SPI based LoRa modules and these all require that the SPI bus pins, SCK, MOSI and MISO are connected. All modules also need a NSS (chip select pin) and NRESET (reset) pin. In theory the NRESET pin could be omitted, but the programs would loose the ability to reset the device. All devices need the RFBUSY pin to be used also. 

Of the DIO pins the library in standard form only uses DIO0 (SX127X) and DIO1 (SX126X and SX128X). The pin definitions for DIO1 and DIO2 (SX127x) and DIO2 and DIO3 (SX126x and SX128x) are not currently used by the library or examples so can be defined as -1 meaning they will not be configured. 

The Dorji DRF1262 and DRF1268 modules has a SW pin which must be configured, it provides power to the antenna switch used on these modules. 
Some SX126x modules may have RX or TX enable pins, these are currently not supported by the library. 

Some of the SX128x modules do have RX or TX enable pins, such as the Ebyte modules, these are supported by the library, and you need to define the pins RX_EN and TX_EN pins used, otherwise leave unused by defining them as -1.  

### Testing
For testing the library and the example programs I used a board of my own design, it uses a 3.3V/8Mhz Arduino Pro Mini which is soldered with a minimum amount of other components onto a board to which you can plug in a LoRa device as a Mikrobus style module. The board is small enough to be used for a GPS tracker application using the connections for a GPS and display as shown in the picture. The Pro Mini used includes a supply reverse protection diode and a fuse, so the board does not need these components. See the [**Easy Pro Mini**](https://github.com/StuartsProjects/Devices/tree/master/Easy%20Pro%20Mini) folder for details. 
<br>
  
All example programs were checked against version 1.8.10 of the Arduino IDE, and the latest copies of any external libraries, as of 16/12/19. The operating system was Windows 10. 

### Program examples

The Examples folder contains a number of practical working applications. There is an example for a very low sleep current sensor transmitter and matching receiver. There are examples for remote control of outputs and servos. There is a GPS tracker transmitter and receiver application. These applications utilise LoRa for the communications so even at low powers they can operate over several kilometres. 

There are demonstrations on how to send data as a plain character array, as a structure and by writing variables direct to the LoRa devices internal buffer.  

There are additional program examples for testing devices, antennas and long distance links.

The Settings.h file contains the settings for the LoRa device such as frequency, spreading factor, bandwidth and coding rate.  The example programs use a frequency of 434.000Mhz or 2.445Ghz for the SX128x, you will need to check if that frequency is permitted in your part of the World. The radio frequency spectrum is not a free for all, which frequencies, transmitter powers and duty cycles you are permitted to use varies by region and country. By default CRC checking is added to transmitted packets and used to check for errors on reception.

The first program to test a layout and connections would be the Example program in the Basics folder **2\_Register_Test**, this just does a simple register print of the LoRa device. If this program does not work, then the rest of the example programs wont either. This program is self contained, it does not need the library installed to operate. 

With an example program written and tested on this SX127x library the example should work with some minor changes with the SX126x and SX128x devices. Many of the example programs have already been tested and are working on SX126x, conversion typically takes less than a minute. 

There are still some issues to attend to and changes to be made, see the section 'Changes Required to Library' at the bottom of this document. 


# Library Functions

All of the library functions are public and can be accessed from users sketches. 

The basic functions will be described in the order that example program **3\_LoRa\_Transmitter** uses them.

**SPI.begin()**

Standard Arduino library function. Sets up SPI. The library then internally uses;

**SPI.beginTransaction(SPISettings(LTspeedMaximum, LTdataOrder, LTdataMode))**

before every use of the SPI and this function after it;

**SPI.endTransaction()**

The parameters used are LTspeedMaximum, LTdataOrder and LTdataMode are defined in SX12XXLT_Definitions.h as;


	LTspeedMaximum  8000000
	LTdataOrder     MSBFIRST
	LTdataMode      SPI_MODE0

The use of SPI.beginTransaction and SPI.endTransaction can be disabled by commenting out this define at the top of the relevant SX12XXLT.cpp file;

	#define USE_SPI_TRANSACTION        


**begin(NSS, NRESET, DIO0, DIO1, DIO2, LORA\_DEVICE)** (SX127X library)

**begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, SW, LORA\_DEVICE)**     (SX126X library)

**begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX\_EN, TX\_EN, LORA\_DEVICE)**     (SX128X library)

The begin function initialises the hardware pins used by the device. The begin functions are slightly different for the SX127X, SX126X and SX128X libraries due to the different pins used. NSS, NRESET and DIO0 (SX127X) or DIO1 (SX126x and SX128X) are required, other DIOs are optional and when not used define as -1. The SX126X and SX128X devices have an RFBUSY pin. To ensure compatibility with Dorji SX1262 and SX1268 devices the SW pin needs to be defined. This pin turns on\off the antenna switch on Dorji devices. Set to -1 if not used. Some of the SX128X devices for example from eByte require TX and RX enable pins, set to -1 if your not using them.

The library examples for the SX128x do use the long form of the begin command (begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX\_EN, TX\_EN, LORA\_DEVICE) but if your device does not have TX and RX enable pins you can use the short form of the begin command;

**begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, LORA\_DEVICE)**     (SX128X library)


LoRA\_DEVICE tells the library which actual LoRa RF IC is being used, the choices are;

	DEVICE_SX1272      //SX127X library
	DEVICE_SX1276      //SX127X library  
	DEVICE_SX1277      //SX127X library
	DEVICE_SX1278      //SX127X library
    DEVICE_SX1279      //SX127X library
	DEVICE_SX1261      //SX126X library
    DEVICE_SX1262      //SX126X library
    DEVICE_SX1268      //SX126X library
    DEVICE_SX1280      //SX128X library
    DEVICE_SX1281      //SX128X library

**setMode(MODE\_STDBY\_RC)**

Sets the operation mode of the LoRa device. Choices are;

	MODE_SLEEP
	MODE_STDBY
	MODE_STDBY_RC

**setPacketType(PACKET\_TYPE\_LORA)**

Set the type of packet to use, currently only LORA is supported, choices are;

	PACKET_TYPE_LORA
	PACKET_TYPE_GFSK     (not yet implemented)
	PACKET_TYPE_FLRC     (SX128X library only)
    PACKET_TYPE_RANGING  (SX128X library only)
    PACKET_TYPE_BLE      (SX128X library only) (not yet implemented)
    PACKET_TYPE_NONE

**setRfFrequency(Frequency, Offset)**

Sets the operating frequency in hertz. A calibration offset also in hertz can be used if there is a calibration value known for a particular module.

**calibrateImage(0)**

Carries out an internal device calibration, normally carried out after setting the initial operating frequency. 

**setModulationParams(SpreadingFactor, Bandwidth, CodeRate, LDRO_AUTO)**

Sets the LoRa modem parameters for Spreading factor, Bandwidth, CodeRate and Optimisation. The options are; 

    //LoRa Spreading factors
    LORA_SF5    (SX126X and SX128X libraries only)   
    LORA_SF6
    LORA_SF7
    LORA_SF8
    LORA_SF9
    LORA_SF10
    LORA_SF11
    LORA_SF12 
    
    //LoRa Bandwidths SX127X and SX126X libraries
    LORA_BW_500  //actual 500000hz
    LORA_BW_250  //actual 250000hz
    LORA_BW_125  //actual 125000hz
    LORA_BW_062  //actual  62500hz 
    LORA_BW_041  //actual  41670hz
    LORA_BW_031  //actual  31250hz 
    LORA_BW_020  //actual  20830hz
    LORA_BW_015  //actual  15630hz
    LORA_BW_010  //actual  10420hz 
    LORA_BW_007  //actual   7810hz

    //LoRa Bandwidths SX128X library
	LORA_BW_0200  //actually 203125hz
	LORA_BW_0400  //actually 406250hz
	LORA_BW_0800  //actually 812500hz
	LORA_BW_1600  //actually 1625000hz

    //LoRa Coding rates
    LORA_CR_4_5  
    LORA_CR_4_6  
    LORA_CR_4_7  
    LORA_CR_4_8

The SX126X and SX127X devices have an low data rate optimisation setting that needs to be set when the symbol time is greater than 16mS. You can manually turn it on or off or set it to LDRO\_AUTO and the library does the calculation for you.

    //Low date rate optimisation, need to be set when symbol time > 16mS
    LDRO_OFF
    LDRO_ON
    LDRO_AUTO       //automatically calculated and set 

**setBufferBaseAddress(0x00, 0x00)**

This sets the default address for the locations in the LoRa device buffer where transmitted and received packets start. The defaults of these locations are set in the transmit and receive functions, so this function is not normally required. 
                   
**setPacketParams(PreAmblelength, LORA\_PACKET\_VARIABLE\_LENGTH, PacketLength, LORA\_CRC\_ON, LORA\_IQ\_NORMAL)**

Set the packet parameters. PreAmblelength is normally 8. There is a choice of LORA\_PACKET\_VARIABLE\_LENGTH for variable length explicit packets or LORA\_PACKET\_FIXED\_LENGTH for implicit packets. PacketLength is 1 to 255, it can be set here but is normally handled within the transmitter and receiver functions. There is the option of using a packet CRC with LORA\_CRC\_ON or not using a CRC with LORA\_CRC\_OFF. IQ can be set to LORA\_IQ\_NORMAL or LORA\_IQ\_INVERTED.

**setSyncWord(LORA\_MAC\_PRIVATE\_SYNCWORD)**

You can define the syncword here, either a 8 bit value of your own choice or the standard values of LORA\_MAC\_PRIVATE\_SYNCWORD (0x12) or LORA\_MAC\_PUBLIC\_SYNCWORD (0x34). Take care with setting your own syncwords, some values may not be compatible with other LoRa devices or can give reduced sensitivity. There is no configurable syncword for SX128x devices.

**setHighSensitivity()**

Sets LoRa device for the highest sensitivity at expense of slightly higher LNA current. The alternative is setLowPowerReceive() for lower sensitivity with slightly reduced current. 


**setDioIrqParams(MASK, DIO0\_MASK, DIO1\_MASK, DIO2\_MASK)** (SX127X library)

**setDioIrqParams(MASK, DIO0\_MASK, DIO1\_MASK, DIO2\_MASK)** (SX126X and SX128X libraries)

Sets up the how the device responds to internal events. This function is written for the SX127X to match the style used by the SX126X and SX128X devices. MASK is applied to the IRQ settings for DIO0, DIO1 and DIO2 (SX127X) and DIO1, DIO2 and DIO3 (SX126X and SX128X), its normally set to IRQ\_RADIO\_ALL (0xFFFF). Whilst the SX127X only has an 8 bit IRQ register the library has extended the function to provide additional IRQ detections that are used for the SX126X and SX127X. 

In the case of the SX127X, the function maps the internal interrupts to the DIO0, DIO1 and DIO2 pins according to this table;

	IRQ_RADIO_NONE              0x00
	IRQ_CAD_ACTIVITY_DETECTED   0x01       //active on DIO1 
	IRQ_FSHS_CHANGE_CHANNEL     0x02       //active on DIO2 
	IRQ_CAD_DONE                0x04       //active on DIO0 
	IRQ_TX_DONE                 0x08       //active on DIO0 
	IRQ_HEADER_VALID            0x10       //read from IRQ register only
	IRQ_CRC_ERROR               0x20       //read from IRQ register only
	IRQ_RX_DONE                 0x40       //active on DIO0 
	IRQ_RADIO_ALL               0xFFFF

	IRQ_TX_TIMEOUT              0x0100     //so that readIrqstatus can return additional detections 
	IRQ_RX_TIMEOUT              0x0200     //so that readIrqstatus can return additional detections  
	IRQ_NO_PACKET_CRC           0x0400     //so that readIrqstatus can return additional detections 

The SX126X library has this table;

	IRQ_RADIO_NONE                       0x0000
	IRQ_TX_DONE                          0x0001
	IRQ_RX_DONE                          0x0002
	IRQ_PREAMBLE_DETECTED                0x0004
	IRQ_SYNCWORD_VALID                   0x0008
	IRQ_HEADER_VALID                     0x0010
	IRQ_HEADER_ERROR                     0x0020
	IRQ_CRC_ERROR                        0x0040
	IRQ_CAD_DONE                         0x0080

	IRQ_CAD_ACTIVITY_DETECTED            0x0100
	IRQ_RX_TX_TIMEOUT                    0x0200
	IRQ_TX_TIMEOUT                       0x0200
	IRQ_RX_TIMEOUT                       0x0200
	IRQ_RADIO_ALL                        0xFFFF

And the SX128X has this one;

	IRQ_RADIO_NONE                       0x0000
	IRQ_TX_DONE                          0x0001
	IRQ_RX_DONE                          0x0002
	IRQ_SYNCWORD_VALID                   0x0004
	IRQ_SYNCWORD_ERROR                   0x0008
	IRQ_HEADER_VALID                     0x0010
	IRQ_HEADER_ERROR                     0x0020
	IRQ_CRC_ERROR                        0x0040
	IRQ_RANGING_SLAVE_RESPONSE_DONE      0x0080

	IRQ_RANGING_SLAVE_REQUEST_DISCARDED  0x0100
	IRQ_RANGING_MASTER_RESULT_VALID      0x0200
	IRQ_RANGING_MASTER_RESULT_TIMEOUT    0x0400
	IRQ_RANGING_SLAVE_REQUEST_VALID      0x0800
	IRQ_CAD_DONE                         0x1000
	IRQ_CAD_ACTIVITY_DETECTED            0x2000
	IRQ_RX_TX_TIMEOUT                    0x4000
	IRQ_TX_TIMEOUT                       0x4000
	IRQ_RX_TIMEOUT                       0x4000
	IRQ_PREAMBLE_DETECTED                0x8000
	IRQ_RADIO_ALL                        0xFFFF 


**setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, Optimisation)**

As an alternative to setting up the LoRa device with separate functions (as described above) you can use this function. The function first sets the Frequency of operation, the frequency is in hertz as a 32 bit unsigned integer. The actual programmed operating frequency is the sum of Frequency and Offset (also 32 bit integer).

SpreadingFactor, Bandwidth and CodeRate are the LoRa modem parameters and the choices are as given for the setModulationParams() described above.

When using setupLoRa() that library function then calls the following functions using these defaults;

	setMode(MODE_STDBY_RC)
	setPacketType(PACKET_TYPE_LORA);
	setRfFrequency(Frequency, Offset);
	calibrateImage(0);
	setModulationParams(SpreadingFactor, Bandwidth, CodeRate, LDRO_AUTO);
	setBufferBaseAddress(0x00, 0x00);
	setPacketParams(8, LORA_PACKET_VARIABLE_LENGTH, 255, LORA_CRC_ON, LORA_IQ_NORMAL);
	setSyncWord(LORA_MAC_PRIVATE_SYNCWORD);
	LORA_MAC_PUBLIC_SYNCWORD = 0x34
	setHighSensitivity();
	setDioIrqParams(IRQ_RADIO_ALL, IRQ_TX_DONE, 0, 0);
 

**printModemSettings()**

Prints the current modem settings to the serial monitor for diagnostic purposes. The parameters printed for lora are ; device\_type, frequency, spreading factor, bandwidth, coding\_rate, syncword, IQ\_Status, preamble\_length.

**printOperatingSettings()**

Prints the current operating settings to the serial monitor for diagnostic purposes. The settings printed are; device\_type, version\_number, packet\_mode, header\_mode, packet\_CRC\_on\_off, AGC\_auto\_on\_off, LNA\_gain, LNA\_boostHF, LNA\_boostLF.

**printRegisters(start, end)** 

Print the device registers from start address to end address. 

**printASCIIPacket(buff, length)**

Print as ASCII characters to the serial monitor the contents of the buffer name given, for the given length.

**transmit(buff, TXPacketL, timeout, TXpower, WAIT_TX)**

Transmit the contents of the buffer name given, for the given length. With a timeout in mS, with a TXpower in dBm and wait for the transmit to complete (a blocking command). To have the LoRa device start transmitting and continue as a no blocking command use NO_WAIT. 

With **transmit** and WAIT\_TX the function returns the packet length if transmit detected no errors and 0 if errors were detected. If NO_WAIT is used you will need to check when pin DIO0 (SX127X) or DIO1 (SX126X and SX128X) goes high indicating transmission is completed.    

**CRCCCITT(buff, TXPacketL, 0xFFFF)**

Calculates the 16 bit CRC of the buffer given for the length given. Specify the initialise value for the CRC, 0xFFFF in this case. 

**readIrqStatus()** 

Reads the value of the IRQ register flags. 

## Receiving Packets

The **4\_LoRa\_Receiver** sketch is very similar, with the following differences;

**LT.receive(RXBUFFER, RXBUFFER\_SIZE, timeout, WAIT\_RX)**

Copy the received packet into the buffer address given with a maximum buffer size. If the RXBUFFER\_SIZE is smaller than the actual received packet then the packet will be truncated. If WAIT\_RX is selected then the program will wait for the time-out period (in mS) for a packet to arrive before signalling a timeout, this is a blocking command. To have the receiver wait continuously for a packet set the timeout to 0. To use the receiver in non-blocking mode set NO\_WAIT in which case you will need to check DIO0 (SX127X) or DIO1 (SX126X and SX128X) going high to indicate a packet has arrived. 

**readPacketRSSI()** 

Read the signal strength in dBm of the received packet. 

**readPacketSNR()**

Read the signal to noise ratio in dB of the received packet. Typical values are +10dB for strong signals and -20dB for reception at the limit.  

**printIrqStatus()**

Prints to the serial monitor the interrupt flags set.


<br>
 

## Packet Addressing

LoRa is a two way technology, each device is a transceiver. Most often on a particular frequency there will be one transmitter and one receiver. However, this may not always be the case and there could be several nodes in use on the same frequency. 

In order to keep the software simple and allow for the receipt of signals from multiple receivers or directed commands to a particular node, a basic addressing scheme can be used and is implemented by some example programs, see '17_Sensor_Transmitter' for an example. There are library routines to send and receive packets in addressed and non-addressed format so you choose which to send. When using addressed mode regardless of the data content of the actual payload each packet sent has 3 control bytes at the beginning of the packet. In the case of the sensor example mentioned above, the use of the addressing allows the receiver to know from which sensor transmitter the packet came. 

In general the control bytes have been restricted to ASCII printable characters so that they can be shown directly on a terminal monitor. The 3 bytes are;

**Packet type**. This either describes the content of the packet, which could be a GPS location payload or is a command to do something and there is no payload. Details of the packet types defined are in the library file 'ProgramLT_Definitions.h'

**Packet Destination**. The node number that the packet is destined for.

**Packet Source**. The node number that the packet was sent from.

The destination and source packet bytes mean that node ‘2’ (could be your base station receiver) can send a command that only station ‘3’ will respond to. This command could be a reset command, a request to turn on and off certain transmission types etc. Node ‘3’ can be set-up so that it only accepts commands from a particular node.

In addressed mode the 3 control bytes are automatically stripped from each received packet.

An example of the 3 control bytes from a tracker would be;

T*2

Which means there is a test packet (T) its been sent as a broadcast (*) and its from node 2.

### Compatibility

**Fully tested on 3.3V 8Mhz ATMega328P and ATMega1284P only**.
 
It was not the intention to specifically support non-Atmel platforms with the library but several programs have been tested and work on an ESP32 WROOM board and an STM32 Xnucleo board. See the Readme for ESP32 and STM32 in the ESP32 and STM32 examples folders.


### Support
The examples do work, so if for you they do not, assume there is a problem with how you have wired the modules or that your modules are faulty or that your Arduino set-up or LoRa module is faulty or unsupported. You are best placed to diagnose these issues. 

If you find a bug, or other error in the SX12xx library or examples, please let me know.


### Future Changes and Enhancements to Library

Add ppmoffset to frequency error check program Check this in program 12 LT.writeRegister(RegPpmCorrection,ppmoffset)

Check sensitivity\current for writeRegister(RegLna, 0x3B );.//at HF 150% LNA current.

Correct issue with IQ settings.

Add reliable packet send\receive code.

Add file transfer to and from SD card code.

<br>



### Stuart Robinson

### October 2020

