#include <SPI.h>
#include <LoRa.h>

SPIClass spiLoRa(VSPI);

#define BUTTON_PIN 12

void setup() {
  Serial.begin(115200);
  
  // Menggunakan internal pull-up. 
  // Jika tombol ditekan, nilainya LOW (0). Jika dilepas, HIGH (1).
  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  // Konfigurasi pin LoRa sesuai setting sebelumnya
  spiLoRa.begin(18, 19, 23, 5);
  LoRa.setSPI(spiLoRa);
  LoRa.setPins(5, 14, 2);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Client gagal!");
    while (1);
  }

  // SyncWord harus sama antara Master dan Client
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Client (Tombol) Ready!");
}

void loop() {
  // Membaca status tombol
  int buttonState = digitalRead(BUTTON_PIN);
  
  // Menentukan pesan yang akan dikirim
  String pesan = "";
  if (buttonState == LOW) {
    pesan = "TOMBOL DITEKAN";
  } else {
    pesan = "TOMBOL DILEPAS";
  }

  // Mengirim data via LoRa
  LoRa.beginPacket();
  LoRa.print(pesan);
  LoRa.endPacket();

  // Menampilkan di Serial Monitor Client untuk pantauan
  Serial.println("Mengirim: " + pesan);

  // Jeda setengah detik agar tidak terlalu membanjiri jaringan
  delay(500); 
}
