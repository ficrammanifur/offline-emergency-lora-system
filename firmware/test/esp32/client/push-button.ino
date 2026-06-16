#include <SPI.h>
#include <LoRa.h>

SPIClass spiLoRa(VSPI);

// Menggunakan pin yang aman dari masalah booting
#define BUTTON_PIN 27 

int lastState = HIGH;

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  // Inisialisasi SPI
  spiLoRa.begin(18, 19, 23, 5);
  LoRa.setSPI(spiLoRa);
  
  // CS=5, RST=14, DIO0=4 (menghindari GPIO2)
  LoRa.setPins(5, 14, 4);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Client gagal!");
    while (1);
  }

  LoRa.setSyncWord(0xF3);
  LoRa.enableCrc(); // Mengaktifkan pengecekan error bawaan LoRa

  Serial.println("LoRa Client Ready! Menunggu input tombol...");
}

void sendData(String msg) {
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  // Cek apakah ada perubahan status (State Change Detection)
  if (buttonState != lastState) {
    
    // Debouncing sederhana
    delay(50); 
    
    // Memastikan statusnya masih sama setelah debounce
    if (digitalRead(BUTTON_PIN) == buttonState) {
      if (buttonState == LOW) {
        sendData("1");
        Serial.println("Transmitted: ON (1)");
      } else {
        sendData("0");
        Serial.println("Transmitted: OFF (0)");
      }
    }
    
    // Simpan status terakhir
    lastState = buttonState;
  }
}
