#include <SPI.h>
#include <LoRa.h>

SPIClass spiLoRa(VSPI);

// Deklarasi pin LED di Master
#define LED_PIN 12

void setup() {
  Serial.begin(115200);

  // Atur pin 12 sebagai OUTPUT
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Pastikan LED mati saat pertama kali menyala

  // Konfigurasi pin LoRa
  spiLoRa.begin(18, 19, 23, 5);
  LoRa.setSPI(spiLoRa);
  LoRa.setPins(5, 14, 2); // CS, RST, DIO0

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Master gagal!");
    while (1);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Master (Receiver) Ready. Menunggu perintah...");
}

void loop() {
  // Cek apakah ada paket data yang masuk dari Client
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    // Baca isi pesan yang masuk
    String incomingData = "";
    while (LoRa.available()) {
      incomingData += (char)LoRa.read();
    }
    
    // Tampilkan di Serial Monitor
    Serial.print("Data diterima: ");
    Serial.print(incomingData);
    Serial.print(" | RSSI: ");
    Serial.println(LoRa.packetRssi());

    // --- LOGIKA KONTROL LED ---
    if (incomingData == "TOMBOL DITEKAN") {
      digitalWrite(LED_PIN, HIGH); // Nyalakan LED
      Serial.println("-> Status: LED MENYALA");
    } 
    else if (incomingData == "TOMBOL DILEPAS") {
      digitalWrite(LED_PIN, LOW);  // Matikan LED
      Serial.println("-> Status: LED MATI");
    }
  }
}
