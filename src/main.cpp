//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <ETH.h>
#include <SPI.h>

#define LED_PIN_INTERNAL    25


//The udp library class
WiFiUDP udp;

int udp_len;
int udp_seq[32] = {};

//set up to connect to an existing network
const char* ssid = "Starlink MMG";
const char* password = "mmg1199#";

const int udpPort = 3333;

bool eth_connected = false;
bool first_udp_seq = true;

//////////////////////////////////////////////////////////////////////

void initWiFi() {
  Serial.println("Setup WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println("Setup done");
}

//////////////////////////////////////////////////////////////////////

void initUdp() {
  //initializes the UDP state
  //This initializes the transfer buffer
  Serial.println("UDP Receiver starts");
  Serial.print(udpPort);
  Serial.println(" Port");
  udp.begin(WiFi.localIP(), udpPort);
}

//////////////////////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(115200);                   // initialize serial
  while (!Serial);

  pinMode(LED_PIN_INTERNAL, OUTPUT);

  initWiFi();

  initUdp();

}

void loop() {
  
  if (WiFi.status() == WL_CONNECTED) {

    if (udp.parsePacket() > 0) {

      if (udp.available()) {

          udp_len = udp.available();

          for(int i = 0; i < udp_len; i++) {
              int udp_dec = udp.read();
              
              udp_seq[i] = {udp_dec};

              if (first_udp_seq == true) {
                  Serial.print("UDP DATAGRAM: ");
                  first_udp_seq = false;
              }
            
              Serial.print(udp_seq[i]); Serial.print(" ");
          }

          Serial.println(" ");
          first_udp_seq = true;
      }
    }
  }
}