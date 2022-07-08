/* Creates a WiFI access point (default IP is 192.168.4.1) opens up Udp port 8888 and accepts data.
   hacked together by Alexandre Castonguay for Yesica Duarte
   REQUIRES : ESP32, tested on a node32s model.
*/
// matches with version 5 of the client

/// TODO : Merge with the sensor receive code and sending to Unity ////

#include <WiFi.h>
#include <WiFiUdp.h>
unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data

//char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char packetBuffer[20 + 1]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back
//bool sendReset = true;

WiFiUDP Udp;

#define BUTTON_CHEST 26 // GIOP21 pin connected to button
#define BUTTON_BELLY 34

void setup()
{
  Serial.begin(9600);
  Serial.println();

  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP("ESPdatos", "respiracion");

  if (result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }

  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);


  pinMode(BUTTON_CHEST, INPUT);//pushbutton pin as an pull-up input
  pinMode(BUTTON_BELLY, INPUT);//pushbutton pin as an pull-up input
}


void loop()
{
  int Push_Chest = digitalRead(BUTTON_CHEST);
  int Push_Belly = digitalRead(BUTTON_BELLY);

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();

  if (packetSize)
  {

    ///////////////////CHEST//////Udp.print(c)////////////////////////////
    if (Push_Chest == LOW)//condition to a button or a received message form a web page
    {
      Udp.stop();
      Serial.println("chest LOW");
      //Serial.println(sendReset);
      Udp.beginPacket(Udp.remoteIP(), 8889);
      Udp.print('c'); // b is the message the client is waiting for in order to reboot
      Udp.endPacket();
      //sendReset = false;      // We sent the reset message
      //Serial.println(sendReset);
      Udp.begin(localPort);
    }

    ////////////////BELLY//////Udp.print(b)////////////////////////
        if (Push_Belly == LOW)//condition to a button or a received message form a web page
    {
      Udp.stop();
      Serial.println("belly LOW");
      //Serial.println(sendReset);
      Udp.beginPacket(Udp.remoteIP(), 8889);
      Udp.print('b'); // b is the message the client is waiting for in order to reboot
      Udp.endPacket();
      //sendReset = false;      // We sent the reset message
      //Serial.println(sendReset);
      Udp.begin(localPort);
    }
    /* Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                   packetSize,
                   Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                   Udp.destinationIP().toString().c_str(), Udp.localPort(),
                   ESP.getFreeHeap());
    */
    // read the packet into packetBufffer
    //int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    int n = Udp.read(packetBuffer, 20);

    packetBuffer[n] = 0;
    //Serial.println("Contents:");
    Serial.println(packetBuffer);

    //    send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(ReplyBuffer); // "acknowledged\r\n"
    Udp.endPacket();
  }


  delay(100);

}
