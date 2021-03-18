

void led_Flash(uint16_t flashes, uint16_t delaymS)
{
  uint16_t index;
  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}

#ifdef tx
void packet_is_OK()
{
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                    //get the IRQ status
  Serial.print(F(" "));
  Serial.print(TXPacketL);
  Serial.print(F(" Bytes SentOK"));
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);
  LT.printIrqStatus();
}

void packet_is_Error()
{
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                    //get the IRQ status
  Serial.print(F(" SendError,"));
  Serial.print(F("Length,"));
  Serial.print(TXPacketL);
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);
  LT.printIrqStatus();
  delay(packet_delay);                                     //change LED flash so packet error visible
  delay(packet_delay);
  digitalWrite(LED1, HIGH);
  delay(packet_delay);
  delay(packet_delay);
  digitalWrite(LED1, LOW);
}
#endif

#ifdef rx

void printElapsedTime()
{
  float seconds;
  seconds = millis() / 1000;
  Serial.print(seconds, 0);
  Serial.print(F("s"));
}


void print_Test1Count()
{
  //prints running totals of the powers of received packets
  int8_t index;
  uint32_t j;

  Serial.print(F("Test Packets "));
  Serial.println(RXpacketCount);
  Serial.print(F("Test Cycles "));
  Serial.println(Mode1_Cycles);

  Serial.println();
  for (index = 30; index >= 0; index--)
  {
    Serial.print(index-18);
    Serial.print(F("dBm,"));
    j = Test1Count[index];
    Serial.print(j);
    Serial.print(F("  "));
  }
  Serial.println();

  Serial.print(F("CSV"));
  for (index = 30; index >= 0; index--)
  {
    Serial.print(F(","));
    j = Test1Count[index];
    Serial.print(j);
  }
  Serial.println();
}


void processPacket()
{
  int8_t lTXpower;
  uint8_t packettype;
  uint32_t temp;

  packettype = LT.readRXPacketType();                             //need to know the packet type so we can decide what to do

  if (packettype == TestPacket)
  {
    if (RXBUFFER[0] == ' ')
    {
    lTXpower = 0;
    }

    if (RXBUFFER[0] == '+')
    {
    lTXpower = ((RXBUFFER[1] - 48) * 10) +  (RXBUFFER[2] - 48);   //convert packet text to power
    }
    
    if (RXBUFFER[0] == '-')
    {
    lTXpower = (((RXBUFFER[1] - 48) * 10) +  (RXBUFFER[2] - 48)) * -1;  //convert packet text to power
    }

    Serial.print(F(" ("));

    if (RXBUFFER[0] != '-')
    {
    Serial.write(RXBUFFER[0]);
    }
    
    Serial.print(lTXpower);
    Serial.print(F("dBm)"));

    if (updateCounts)
    {
      temp = (Test1Count[lTXpower+18]);
      Test1Count[lTXpower+18] = temp + 1;
    }
  }

  if (packettype == TestMode1)
  {
    //this is a command to switch to TestMode1 also updates totals and logs
    updateCounts = true;
    Serial.println();
    Serial.println(F("End test sequence"));

    if (Mode1_Cycles > 0)
    {
      print_Test1Count();
    }

    Serial.println();
    Mode1_Cycles++;
  }

}


void packet_is_OK()
{
  uint16_t IRQStatus;

  IRQStatus = LT.readIrqStatus();                //read the LoRa device IRQ status register

  RXpacketCount++;

  printElapsedTime();                            //print elapsed time to Serial Monitor
  Serial.print(F("  "));
  LT.printASCIIPacket(RXBUFFER, RXPacketL - 3);  //print the packet as ASCII characters

  Serial.print(F(",RSSI,"));
  Serial.print(PacketRSSI);
  Serial.print(F("dBm,SNR,"));
  Serial.print(PacketSNR);
  Serial.print(F("dB,Length,"));
  Serial.print(RXPacketL);
  Serial.print(F(",Packets,"));
  Serial.print(RXpacketCount);
  Serial.print(F(",Errors,"));
  Serial.print(errors);
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);

  processPacket();
}



void packet_is_Error()
{
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                   //read the LoRa device IRQ status register

  printElapsedTime();                               //print elapsed time to Serial Monitor

  if (IRQStatus & IRQ_RX_TIMEOUT)                   //check for an RX timeout
  {
    Serial.print(F(" RXTimeout"));
  }
  else
  {
    errors++;
    Serial.print(F(" PacketError"));
    Serial.print(F(",RSSI,"));
    Serial.print(PacketRSSI);
    Serial.print(F("dBm,SNR,"));
    Serial.print(PacketSNR);
    Serial.print(F("dB,Length,"));
    Serial.print(LT.readRXPacketL());               //get the real packet length
    Serial.print(F(",Packets,"));
    Serial.print(RXpacketCount);
    Serial.print(F(",Errors,"));
    Serial.print(errors);
    Serial.print(F(",IRQreg,"));
    Serial.print(IRQStatus, HEX);
    LT.printIrqStatus();                            //print the names of the IRQ registers set
  }
}

#endif
