#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

// Pin LoRa untuk Arduino Nano
#define SCK 13
#define MISO 12
#define MOSI 11
#define SS 10
#define RST 9
#define DIO0 2

// Sensor Pin
#define DHTPIN 4
#define DHTTYPE DHT22
#define GAS_PIN A0
#define LDR_PIN A1
#define SOIL_PIN A2
#define VIB_PIN 3
#define SOS_BUTTON 5 // Tombol Darurat

DHT dht(DHTPIN, DHTTYPE);

// STRUCT DATA (Harus sama dengan Master)
struct SensorPacket {
    float temperature;
    float humidity;
    int gas;
    int light;
    int soil;
    int vibration;
    bool sos; // Fitur Tambahan SOS
    uint32_t nodeID; 
};

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(VIB_PIN, INPUT_PULLUP);
    pinMode(SOS_BUTTON, INPUT_PULLUP);

    Serial.println("Initializing LoRa Client...");
    if (!LoRa.begin(433E6)) { // Sesuaikan frekuensi LoRa Ra-02 kamu
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop() {
    SensorPacket data;
    
    // 1. Baca Sensor
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();
    data.gas = analogRead(GAS_PIN);
    data.light = analogRead(LDR_PIN);
    data.soil = analogRead(SOIL_PIN);
    data.vibration = digitalRead(VIB_PIN);
    data.sos = (digitalRead(SOS_BUTTON) == LOW); // SOS aktif jika ditekan
    data.nodeID = 101; // ID Unik Node ini

    // 2. Kirim Data via LoRa
    Serial.print("Sending packet to Master...");
    LoRa.beginPacket();
    LoRa.write((uint8_t*)&data, sizeof(data));
    LoRa.endPacket();
    Serial.println(" Sent!");

    // 3. Delay atau Deep Sleep
    delay(5000); // Simulasi setiap 5 detik
}
