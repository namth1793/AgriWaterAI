#include <DHT11.h>
// #include <SX127x.h>
#include <LoRa.h>
#include <SPI.h>
#include <stdio.h>

//define the pins used by the transceiver module
#define ss 10
#define rst 9
#define dio0 2

// Sensor Pins
#define LightSensorPin A0
#define SoilSensorPin A1
#define RainSensorDigPin 6
#define DHT11Pin 7

// Sub module initialization
// SX127x LoRa;
DHT11 dht11(DHT11Pin);

// Program Parameters
int temperature = 0;
int humidity = 0;
char buffer[25];

// Call Function
int getLight();
int getSoil();
int getRain();

// Setup Program
void setup() {
  Serial.begin(115200);
  pinMode(RainSensorDigPin, INPUT);
  while (!Serial)
    ;
  Serial.println("LoRa Sender");
  // setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(923000000)) {
    Serial.println(".");
    delay(500);
  }
}

// Program Loop
void loop() {
  // light sensor value
  int lightValue = getLight();
  // soil sensor value
  int soilValue = getSoil();
  // rain sensor value
  int rainValue = getRain();
  // temperature and humidity value
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  if (result == 0) {
    Serial.print("\nNhiệt độ: ");
    Serial.print(temperature);
    Serial.println(" độ C");
    Serial.print("Độ ẩm: ");
    Serial.print(humidity);
    Serial.println("%");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }

  Serial.print("Cường độ ánh sáng: ");
  Serial.print(lightValue);
  if (lightValue < 10) {
    Serial.println(" - Rất sáng");
  } else if (lightValue < 200) {
    Serial.println(" - Sáng rõ");
  } else if (lightValue < 500) {
    Serial.println(" - Sáng vừa");
  } else if (lightValue < 800) {
    Serial.println(" - Sáng yếu");
  } else {
    Serial.println(" - Tối");
  }

  Serial.print("Độ ẩm đất: ");
  Serial.print(soilValue);
  Serial.println("%");

  Serial.print("Trời mưa: ");
  if (rainValue == LOW) {
    Serial.println("Có");
  } else {
    Serial.println("Không");
  }
  delay(1000);
  while (LoRa.beginPacket() == 0) {
    Serial.print("waiting for radio ... ");
    delay(100);
  }
  sprintf(buffer, "%d%d%d%d%d", temperature, humidity, lightValue, soilValue, rainValue);
  Serial.print("Gửi dữ liệu đến ESP32: ");
  Serial.println(buffer);
  // send packet
  LoRa.beginPacket();
  LoRa.print((uint8_t)buffer);
  LoRa.endPacket();

  // Don't load RF module with continous transmit
  delay(5000);
}

int getLight() {
  int analogValue = analogRead(LightSensorPin);
  return analogValue;
}

int getSoil() {
  int analogValue = analogRead(SoilSensorPin);
  return analogValue;
}

int getRain() {
  int digitalValue = digitalRead(RainSensorDigPin);
  return digitalValue;
}
