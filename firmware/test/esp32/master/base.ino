#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <WebServer.h>

// --- KREDENSIAL WI-FI ---
const char* ssid = "FRISS";       // Nama Wi-Fi
const char* password = "mamahfris"; // Password Wi-Fi

WebServer server(80); // Server berjalan di port 80 HTTP
SPIClass spiLoRa(VSPI);

// Variabel Global untuk menyimpan data terakhir dari LoRa Client
String lastSuhu = "--";
String lastHum = "--";
String lastSoil = "--";
String lastTombol = "--";
String lastKipas1 = "--";
String lastKipas2 = "--";
String lastRSSI = "--";
unsigned long lastPacketTime = 0;

// Fungsi untuk membangun dan mengirim halaman HTML Dashboard
void handleRoot() {
  // Desain HTML + CSS (Dark / Cyberpunk Theme untuk Command Center)
  String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<meta http-equiv=\"refresh\" content=\"2\">"; // Auto refresh halaman tiap 2 detik
  html += "<title>Gembot Command Center</title>";
  html += "<style>";
  html += "body { background-color: #0d0d0d; color: #00ffcc; font-family: 'Courier New', Courier, monospace; margin: 0; padding: 20px; text-align: center; }";
  html += "h1 { color: #ff0055; text-transform: uppercase; letter-spacing: 2px; border-bottom: 1px solid #333; padding-bottom: 10px; }";
  html += ".container { display: flex; flex-wrap: wrap; justify-content: center; gap: 15px; margin-top: 20px; }";
  html += ".card { background: #1a1a1a; border: 1px solid #333; border-radius: 8px; padding: 20px; width: 150px; box-shadow: 0 0 15px rgba(0, 255, 204, 0.05); }";
  html += ".card h3 { font-size: 14px; color: #888; margin-top: 0; letter-spacing: 1px; }";
  html += ".card p { font-size: 24px; font-weight: bold; margin: 10px 0 0; }";
  html += ".rssi { margin-top: 30px; font-size: 12px; color: #555; }";
  html += ".alert { color: #ff0055; }";
  html += ".on { color: #00ffcc; }";
  html += "</style></head><body>";
  
  html += "<h1>Gembot Basecamp</h1>";
  html += "<div class=\"container\">";
  
  // Kotak Sensor
  html += "<div class=\"card\"><h3>SUHU</h3><p>" + lastSuhu + " &deg;C</p></div>";
  html += "<div class=\"card\"><h3>KELEMBABAN</h3><p>" + lastHum + " %</p></div>";
  html += "<div class=\"card\"><h3>SOIL MOISTURE</h3><p>" + lastSoil + "</p></div>";
  
  // Kotak Status Aktuator & Tombol (Sudah menyesuaikan logika Active LOW Client)
  String tombolStatus = (lastTombol == "0") ? "<span class='alert'>DITEKAN</span>" : "STANDBY";
  html += "<div class=\"card\"><h3>TOMBOL</h3><p>" + tombolStatus + "</p></div>";
  
  String kipas1Status = (lastKipas1 == "1") ? "<span class='on'>ON</span>" : "<span class='alert'>OFF</span>";
  html += "<div class=\"card\"><h3>KIPAS 1 (SUHU)</h3><p>" + kipas1Status + "</p></div>";
  
  String kipas2Status = (lastKipas2 == "1") ? "<span class='on'>ON</span>" : "<span class='alert'>OFF</span>";
  html += "<div class=\"card\"><h3>KIPAS 2 (HUM)</h3><p>" + kipas2Status + "</p></div>";
  
  html += "</div>";
  html += "<div class=\"rssi\">Signal Strength (RSSI): " + lastRSSI + " dBm | Last Update: " + String((millis() - lastPacketTime)/1000) + "s ago</div>";
  html += "</body></html>";

  // Kirim halaman ke browser
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  // 1. Setup Koneksi Wi-Fi
  Serial.println();
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Berhasil Terhubung!");
  Serial.print("Buka browser dan ketik IP berikut: http://");
  Serial.println(WiFi.localIP()); 

  // 2. Setup Web Server
  server.on("/", handleRoot); 
  server.begin();
  Serial.println("Web Server Aktif.");

  // 3. Setup LoRa SPI
  spiLoRa.begin(18, 19, 23, 5);
  LoRa.setSPI(spiLoRa);
  LoRa.setPins(5, 14, 4); // DIO0 di Pin 4 agar stabil

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Master gagal inisialisasi!");
    while (1);
  }
  
  LoRa.setSyncWord(0xF3); // Harus sama persis dengan Client
  LoRa.enableCrc();       // Mengaktifkan pengecekan error paket
  Serial.println("LoRa Master Gateway Ready. Sistem Standby...");
}

void loop() {
  // Wajib dipanggil terus agar server web responsif
  server.handleClient();

  // Cek apakah ada paket data yang masuk dari Remote Site
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }
    
    lastRSSI = String(LoRa.packetRssi());
    lastPacketTime = millis(); // Catat waktu terakhir terima paket

    // Proses Parsing / Memecah String CSV
    int comma1 = incoming.indexOf(',');
    int comma2 = incoming.indexOf(',', comma1 + 1);
    int comma3 = incoming.indexOf(',', comma2 + 1);
    int comma4 = incoming.indexOf(',', comma3 + 1);
    int comma5 = incoming.indexOf(',', comma4 + 1);

    // Jika format data sesuai (punya 5 koma), simpan dan tampilkan
    if (comma1 != -1 && comma2 != -1 && comma3 != -1 && comma4 != -1 && comma5 != -1) {
      lastSuhu = incoming.substring(0, comma1);
      lastHum = incoming.substring(comma1 + 1, comma2);
      lastSoil = incoming.substring(comma2 + 1, comma3);
      lastTombol = incoming.substring(comma3 + 1, comma4);
      lastKipas1 = incoming.substring(comma4 + 1, comma5);
      lastKipas2 = incoming.substring(comma5 + 1);
      
      // --- TAMPILAN UNTUK SERIAL MONITOR ---
      Serial.println("\n=== PAKET BARU DARI REMOTE SITE ===");
      Serial.println("Suhu          : " + lastSuhu + " °C");
      Serial.println("Kelembaban    : " + lastHum + " %");
      Serial.println("Soil Moisture : " + lastSoil);
      Serial.println("Tombol        : " + String(lastTombol == "0" ? "DITEKAN" : "STANDBY"));
      Serial.println("Kipas 1 (Suhu): " + String(lastKipas1 == "1" ? "ON" : "OFF"));
      Serial.println("Kipas 2 (Hum) : " + String(lastKipas2 == "1" ? "ON" : "OFF"));
      Serial.println("RSSI          : " + lastRSSI + " dBm");
      Serial.println("===================================");
    } else {
      Serial.println("Data masuk dengan format tidak valid: " + incoming);
    }
  }
}
