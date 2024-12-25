#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6fpsLRJkd"
#define BLYNK_TEMPLATE_NAME "Smart IoT"
#define BLYNK_AUTH_TOKEN "zj6RkE16DmSxLZ2MH0UjsTGVlMZs6g0l"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>
#include <stdio.h>

/* VIRTUAL PINS FOR BLYNK */
#define LightPin V0
#define TempPin V1
#define HumiPin V2
#define SoilPin V3
#define WaterPin V4

char ssid[] = "Hoai Nam";
char pass[] = "17092003";

// double w[] = {-2.0000000000000018, 1.999999999999997, -2.9999999999999987, 1.0, -2.000000000000001};
// double c = 8000.0;

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

char lightValue[3];
char tempValue[3];
char humiValue[4];
char soilValue[5];
char waterValue[2];

BlynkTimer timer;

void sendToBlynk() {
  // try to parse packet
  char LoRaData[25];
  Serial.println("Start Receive...");
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
      
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  Blynk.virtualWrite(LightPin, random(0, 800));
  Blynk.virtualWrite(TempPin, random(15, 45));
  Blynk.virtualWrite(HumiPin, random(0, 100));
  Blynk.virtualWrite(SoilPin, random(0, 100));
  Blynk.virtualWrite(WaterPin, random(2000, 8000));
} 

void setup() {

  // Begin serial communication
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(923000000)) {
    Serial.println(".");
    delay(500);
  }
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendToBlynk);
}

void loop() {

  // Request for receiving new LoRa packet within 1000 ms
  Blynk.run();
  timer.run();
}