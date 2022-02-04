#include <SPI.h>
#include <RH_RF95.h>
 
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
void setup() 
{
      Serial.begin(9600);
    while (!Serial); // Wait for serial port to be available
    if (!rf95.init())
        Serial.println("init failed");
    rf95.setFrequency(915.0);
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // You can change the modulation parameters with eg
    // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 2 to 20 dBm:
    rf95.setTxPower(20, false);
}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission
int i = 0

void loop()
{
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server

 if(i=0) {
   uint8_t data[] = "Small package";
      rf95.send(data, sizeof(data));
      i+=1
 }
 elif(i=1) {
   uint8_t data[] = "Medium Package - Hello There";
      rf95.send(data, sizeof(data));
      i+=1
 }
 else{
   uint8_t data[] = "Big package - Despite the use for Arduino, Python > C++";
      rf95.send(data, sizeof(data));
      i=0
 }
      Serial.println("Waiting for packet to complete..."); delay(10);
      rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  Serial.println("Waiting for reply..."); delay(10);
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply. Houston, we have a problem");
  }
  delay(1000);
}