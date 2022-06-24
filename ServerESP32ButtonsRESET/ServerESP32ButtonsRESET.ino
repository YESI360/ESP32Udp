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

#define BUTTON_CHEST 27
#define BUTTON_BELLY 14

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


  pinMode(BUTTON_CHEST, INPUT);
  pinMode(BUTTON_BELLY, INPUT);
}


void loop()
{
  int Button_chest = digitalRead(BUTTON_CHEST);//27
  int Button_belly = digitalRead(BUTTON_BELLY);//14

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {

    //if (millis() > 20000 && sendReset == true)// Change this condition to a button or a received message form a web page. A button is less complicated.
    if (Button_chest == LOW)
    {

      Udp.stop();
      Serial.println("chest LOW");
      //Serial.println(sendReset);
      Udp.beginPacket(Udp.remoteIP(), 8889);
      Udp.print('c'); // c is the message the client is waiting for in order to reboot
      Udp.endPacket();
      //sendReset = false;      // We sent the reset message
      //Serial.println(sendReset);
      Udp.begin(localPort);
      Serial.println("reset send?");
    }

    if (Button_belly == LOW)
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
      Serial.println("reset send?");
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
