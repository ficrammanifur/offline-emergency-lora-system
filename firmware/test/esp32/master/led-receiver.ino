#include <SPI.h>
#include <LoRa.h>

SPIClass spiLoRa(VSPI);

// Menggunakan pin yang aman dari masalah booting
#define LED_PIN 26

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 

  // Inisialisasi SPI
  spiLoRa.begin(18, 19, 23, 5);
  LoRa.setSPI(spiLoRa);
  
  // CS=5, RST=14, DIO0=4 (menghindari GPIO2)
  LoRa.setPins(5, 14, 4); 

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Master gagal!");
    while (1);
  }

  LoRa.setSyncWord(0xF3);
  LoRa.enableCrc(); // Wajib diaktifkan juga di sisi penerima

  Serial.println("LoRa Master Ready. Menunggu sinyal...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    String incomingData = "";
    while (LoRa.available()) {
      incomingData += (char)LoRa.read();
    }
    
    Serial.print("Received: ");
    Serial.print(incomingData);
    Serial.print(" | RSSI: ");
    Serial.println(LoRa.packetRssi());

    // Logika kontrol LED yang lebih efisien
    if (incomingData == "1") {
      digitalWrite(LED_PIN, HIGH);
    } 
    else if (incomingData == "0") {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
