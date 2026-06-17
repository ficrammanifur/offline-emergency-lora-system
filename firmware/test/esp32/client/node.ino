#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

SPIClass spiLoRa(VSPI);

// Definisi Pin Sensor & Aktuator
#define BUTTON_PIN 27
#define DHTPIN 13
#define DHTTYPE DHT22
#define SOIL_PIN 34
#define RELAY_KIPAS1 25
#define RELAY_KIPAS2 26

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Inisialisasi Relay (Active LOW)
  pinMode(RELAY_KIPAS1, OUTPUT);
  pinMode(RELAY_KIPAS2, OUTPUT);
  
  // Set HIGH di awal agar relay dalam kondisi OFF saat pertama kali menyala
  digitalWrite(RELAY_KIPAS1, HIGH);
  digitalWrite(RELAY_KIPAS2, HIGH);

  // Inisialisasi LoRa SPI
  spiLoRa.begin(18, 19, 23, 5);
  LoRa.setSPI(spiLoRa);
  LoRa.setPins(5, 14, 4); // DIO0 aman di Pin 4

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Client gagal!");
    while (1);
  }

  LoRa.setSyncWord(0xF3);
  LoRa.enableCrc();
  Serial.println("LoRa Client Remote Site (TEST MODE) Ready!");
}

// Fungsi khusus untuk membaca sensor dan mengirim data seketika
void bacaDanKirim() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();
  int soil = analogRead(SOIL_PIN);
  int tombol = digitalRead(BUTTON_PIN); 

  // Baca status raw dari pin relay
  int kipas1_raw = digitalRead(RELAY_KIPAS1);
  int kipas2_raw = digitalRead(RELAY_KIPAS2);

  // --- INVERSI LOGIKA ACTIVE LOW ---
  // Jika pin LOW (0) = Relay fisik ON (1). Jika pin HIGH (1) = Relay fisik OFF (0).
  int statusKipas1 = (kipas1_raw == LOW) ? 1 : 0;
  int statusKipas2 = (kipas2_raw == LOW) ? 1 : 0;

  // Antisipasi pembacaan DHT gagal
  if (isnan(suhu) || isnan(kelembaban)) {
    suhu = 0.0;
    kelembaban = 0.0;
  }

  // Bungkus semua data ke format CSV
  String paketData = String(suhu, 1) + "," + 
                     String(kelembaban, 1) + "," + 
                     String(soil) + "," + 
                     String(tombol) + "," + 
                     String(statusKipas1) + "," + 
                     String(statusKipas2);

  // Kirim ke Basecamp via LoRa
  LoRa.beginPacket();
  LoRa.print(paketData);
  LoRa.endPacket();

  // Print dengan Timestamp untuk cek kecepatan pengiriman (Realtime test)
  Serial.print(millis());
  Serial.println(" ms -> Test Kirim: " + paketData);
}

void loop() {
  // --- Skenario 1: Relay 1 Nyala ---
  digitalWrite(RELAY_KIPAS1, LOW);
  digitalWrite(RELAY_KIPAS2, HIGH);
  Serial.println("\n-> Relay 1 ON");
  bacaDanKirim(); 
  delay(3000); // Jeda 3 detik (Aman untuk DHT22)

  // --- Skenario 2: Semua Mati ---
  digitalWrite(RELAY_KIPAS1, HIGH);
  digitalWrite(RELAY_KIPAS2, HIGH);
  Serial.println("\n-> Semua OFF");
  bacaDanKirim(); 
  delay(2500); // UBAH DI SINI: Minimal 2.5 detik agar aman saat membaca sensor!

  // --- Skenario 3: Relay 2 Nyala ---
  digitalWrite(RELAY_KIPAS1, HIGH);
  digitalWrite(RELAY_KIPAS2, LOW);
  Serial.println("\n-> Relay 2 ON");
  bacaDanKirim(); 
  delay(3000); // Jeda 3 detik (Aman untuk DHT22)

  // --- Skenario 4: Semua Mati ---
  digitalWrite(RELAY_KIPAS1, HIGH);
  digitalWrite(RELAY_KIPAS2, HIGH);
  Serial.println("\n-> Semua OFF");
  bacaDanKirim(); 
  delay(2500); // UBAH DI SINI: Minimal 2.5 detik agar aman saat membaca sensor!
}
