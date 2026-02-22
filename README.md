<div align="center">

# 🌲 FOREST SENTINEL LORA SYSTEM

**Long-Range Wireless Environmental Monitoring & Emergency Detection for Remote Forest Areas**

[![Last Commit](https://img.shields.io/badge/last_update-22_Feb_2026-brightgreen?style=for-the-badge)](https://github.com/forest-sentinel/lora-system)
[![Language](https://img.shields.io/badge/language-C%2B%2B%20%7C%20Python-orange?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://cplusplus.com/)
[![Platform](https://img.shields.io/badge/platform-ESP32-red?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![LoRa](https://img.shields.io/badge/communication-LoRa-blue?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBkPSJNMTIgMkM2LjQ4IDIgMiA2LjQ4IDIgMTJzNC40OCAxMCAxMCAxMCAxMC00LjQ4IDEwLTEwUzE3LjUyIDIgMTIgMnptMCAxOGMtNC40MSAwLTgtMy41OS04LThzMy41OS04IDgtOCA4IDMuNTkgOCA4LTMuNTkgOC04IDh6IiBmaWxsPSIjZmZmIi8+PC9zdmc+)](https://lora-alliance.org/)
[![Backend](https://img.shields.io/badge/backend-Flask-black?style=for-the-badge&logo=flask&logoColor=white)](https://flask.palletsprojects.com/)
[![Power](https://img.shields.io/badge/power-Deep%20Sleep%20Optimized-green?style=for-the-badge)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html)
[![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)](LICENSE)

*Sistem monitoring darurat berbasis LoRa untuk deteksi dini kebakaran hutan, banjir, dan aktivitas seismik di area terpencil tanpa internet*

</div>

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   📑 DAFTAR ISI                                       ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

<div align="center">

[✨ Overview](#-overview) • [🧠 System Architecture](#-system-architecture) • [🔩 Hardware](#-hardware-components) • [🔌 Wiring](#-wiring-diagram) • [📡 Communication](#-communication-flow) • [⚡ Power](#-power-management-strategy) • [💻 Backend](#-backend-architecture) • [🚀 How To Run](#-how-to-run) • [📊 Future](#-future-development) • [📷 Preview](#-project-preview)

</div>

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   ✨ OVERVIEW                                          ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

**Forest Sentinel** adalah sistem monitoring lingkungan mandiri yang dirancang khusus untuk **area hutan terpencil** tanpa akses internet. Sistem ini menggabungkan teknologi **LoRa** untuk komunikasi jarak jauh (hingga 10km) dengan strategi **deep sleep** ultra-hemat daya yang memungkinkan operasi berbulan-bulan hanya dengan baterai.

### 🎯 **Tujuan Utama**
- 🔥 Deteksi dini kebakaran hutan melalui sensor asap dan suhu
- 🌊 Peringatan dini banjir di daerah aliran sungai
- 📡 Monitoring aktivitas seismik untuk potensi tanah longsor
- 📊 Visualisasi real-time melalui dashboard lokal

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   🧠 SYSTEM ARCHITECTURE                              ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

```ascii
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                         FOREST SENTINEL - SYSTEM ARCHITECTURE                        │
├─────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│  🌲 FOREST AREA (Remote)                        🏕️ BASE CAMP (Monitoring Post)       │
│                                                                                       │
│  ┌──────────────────────┐                       ┌──────────────────────┐            │
│  │   CLIENT NODE #01    │                       │   MASTER GATEWAY     │            │
│  │   [ESP32 + Sensors]  │                       │      [ESP32]         │            │
│  ├──────────────────────┤     ┌──────────┐     ├──────────────────────┤            │
│  │ • MQ-2 Gas Sensor    │     │   LORA   │     │ • LoRa Receiver       │            │
│  │ • SW-420 Vibration   │────▶│ 868/915  │────▶│ • WiFi Module         │            │
│  │ • Water Level        │     │   MHz    │     │ • Serial Output       │            │
│  │ • INMP441 Microphone │     └──────────┘     │ • Data Buffer         │            │
│  │ • Battery Management │                       └──────────┬───────────┘            │
│  └──────────────────────┘                                  │                        │
│         ▲                                                  │                        │
│         │                                                  ▼                        │
│  ┌──────────────────────┐                       ┌──────────────────────┐            │
│  │   CLIENT NODE #02    │                       │   BACKEND SERVER     │            │
│  │   [ESP32 + Sensors]  │                       │      [FLASK]         │            │
│  ├──────────────────────┤      ┌──────────┐    ├──────────────────────┤            │
│  │ • Identical Setup    │─────▶│   LoRa   │    │ • REST API           │            │
│  │ • Deep Sleep 99%     │      │  Mesh    │    │ • SQLite Database    │            │
│  │ • Event-Based TX     │      └──────────┘    │ • WebSocket          │            │
│  └──────────────────────┘                       │ • Alert Processing   │            │
│                                                  └──────────┬───────────┘            │
│                                                             │                        │
│  ┌──────────────────────┐                                  ▼                        │
│  │   CLIENT NODE #03    │                       ┌──────────────────────┐            │
│  │   [ESP32 + Sensors]  │                       │   WEB DASHBOARD      │            │
│  ├──────────────────────┤      ┌──────────┐    ├──────────────────────┤            │
│  │ • Solar Charging     │─────▶│   LoRa   │    │ • Real-time Charts   │            │
│  │ • Ultra-low Power    │      │  Relay   │    │ • Alert Notifications│            │
│  │ • ...                │      └──────────┘    │ • Historical Data    │            │
│  └──────────────────────┘                       │ • Node Management    │            │
│                                                  └──────────────────────┘            │
│                                                                                       │
└─────────────────────────────────────────────────────────────────────────────────────┘
```

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   🔩 HARDWARE COMPONENTS                              ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

### 📦 **Client Node (Sensor Unit)**

| Komponen | Spesifikasi | Fungsi |
|----------|-------------|--------|
| **ESP32-S3** | Xtensa® 32-bit LX7 | Kontrol utama, deep sleep management |
| **RFM95W LoRa** | 868/915 MHz, +20dBm | Komunikasi jarak jauh (10km LOS) |
| **MQ-2 Gas Sensor** | LPG, Smoke, CO | Deteksi asap kebakaran |
| **SW-420 Vibration** | Digital output | Deteksi getaran tanah |
| **Water Level Sensor** | Analog, 0-4.5cm | Monitoring ketinggian air |
| **INMP441** | I2S, -26dBFS | Rekaman suara untuk analisis |
| **Battery** | 3.7V LiPo 2000mAh | Sumber daya utama |
| **Solar Charger** | TP4056 + 5V panel | Pengisian daya otomatis |

### 🖥️ **Master Gateway**

| Komponen | Spesifikasi | Fungsi |
|----------|-------------|--------|
| **ESP32** | Dual-core, WiFi | Gateway utama |
| **RFM95W LoRa** | 868/915 MHz | Penerima data dari client |
| **Micro SD Card** | SPI interface | Backup data lokal |
| **LED Indicator** | RGB | Status indikator |

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   🔌 WIRING DIAGRAM - CLIENT NODE                     ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

```ascii
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                         ESP32 CLIENT NODE - WIRING DIAGRAM                           │
├─────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│  ┌───────────────────────────────────────────────────────────────────────────────┐  │
│  │                              ESP32-S3 BOARD                                     │  │
│  ├───────────────────────────────────────────────────────────────────────────────┤  │
│  │                                                                               │  │
│  │  ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐             │  │
│  │  │ 3V3 │    │ GND │    │ EN  │    │ VP  │    │ VN  │    │ 34  │             │  │
│  │  └──┬──┘    └──┬──┘    └─────┘    └─────┘    └─────┘    └──┬──┘             │  │
│  │     │          │                                           │                 │  │
│  │     ▼          ▼                                           ▼                 │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                           LoRa Module (RFM95W)                        │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │  NSS    SCK    MISO    MOSI    RST    DIO0    GND    3.3V             │   │  │
│  │  │   │      │       │       │       │       │       │       │             │   │  │
│  │  │   ▼      ▼       ▼       ▼       ▼       ▼       ▼       ▼             │   │  │
│  │  │  GPIO5  GPIO18  GPIO19  GPIO23  GPIO14  GPIO26   GND     3.3V          │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  │                                                                               │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                              SENSORS                                   │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │                                                                       │   │  │
│  │  │  MQ-2 Gas Sensor     SW-420 Vibration     Water Level Sensor         │   │  │
│  │  │  ┌─────────────┐     ┌─────────────┐     ┌─────────────┐             │   │  │
│  │  │  │ A0 •───────┐│     │ DO •───────┐│     │ A0 •───────┐│             │   │  │
│  │  │  │ VCC•───┐   ││     │ VCC•───┐   ││     │ VCC•───┐   ││             │   │  │
│  │  │  │ GND•─┐ │   ││     │ GND•─┐ │   ││     │ GND•─┐ │   ││             │   │  │
│  │  │  └──────┼─┼───┼┘     └──────┼─┼───┼┘     └──────┼─┼───┼┘             │   │  │
│  │  │         │ │   │              │ │   │              │ │   │              │   │  │
│  │  │         ▼ ▼   ▼              ▼ ▼   ▼              ▼ ▼   ▼              │   │  │
│  │  │      GPIO34 3.3V GND      GPIO27 3.3V GND      GPIO32 3.3V GND        │   │  │
│  │  │                                                                       │   │  │
│  │  │  INMP441 Microphone                                                  │   │  │
│  │  │  ┌─────────────────────┐                                             │   │  │
│  │  │  │ WS •────────────┐   │                                             │   │  │
│  │  │  │ SCK•────────┐   │   │                                             │   │  │
│  │  │  │ SD •────┐   │   │   │                                             │   │  │
│  │  │  │ VCC•─┐  │   │   │   │                                             │   │  │
│  │  │  │ GND•─┼──┼───┼───┼───┼┐                                            │   │  │
│  │  │  └──────┼──┼───┼───┼───┼┘                                            │   │  │
│  │  │         │  │   │   │   │                                             │   │  │
│  │  │         ▼  ▼   ▼   ▼   ▼                                             │   │  │
│  │  │      GPIO25 3.3V GND GPIO26 GPIO32?                                  │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  │                                                                               │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                              POWER                                     │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │                                                                       │   │  │
│  │  │  ┌────────┐      ┌────────┐      ┌────────┐                          │   │  │
│  │  │  │ LiPo   │─────▶│ TP4056 │─────▶│ ESP32  │                          │   │  │
│  │  │  │ 3.7V   │      │ Charger│      │ Vin    │                          │   │  │
│  │  │  └────────┘      └────────┘      └────────┘                          │   │  │
│  │  │                      │                                               │   │  │
│  │  │                      ▼                                               │   │  │
│  │  │                 ┌────────┐                                          │   │  │
│  │  │                 │ Solar  │                                          │   │  │
│  │  │                 │ Panel  │                                          │   │  │
│  │  │                 │ 5V     │                                          │   │  │
│  │  │                 └────────┘                                          │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  └───────────────────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────────────────┘
```

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   🔌 WIRING DIAGRAM - MASTER GATEWAY                  ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

```ascii
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                         ESP32 MASTER GATEWAY - WIRING DIAGRAM                        │
├─────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│  ┌───────────────────────────────────────────────────────────────────────────────┐  │
│  │                              ESP32 BOARD                                        │  │
│  ├───────────────────────────────────────────────────────────────────────────────┤  │
│  │                                                                               │  │
│  │  ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐             │  │
│  │  │ 3V3 │    │ GND │    │ EN  │    │ D4  │    │ D5  │    │ D18 │             │  │
│  │  └──┬──┘    └──┬──┘    └─────┘    └──┬──┘    └──┬──┘    └──┬──┘             │  │
│  │     │          │                      │         │          │                 │  │
│  │     ▼          ▼                      ▼         ▼          ▼                 │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                           LoRa Module (RFM95W)                        │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │  NSS    SCK    MISO    MOSI    RST    DIO0    GND    3.3V             │   │  │
│  │  │   │      │       │       │       │       │       │       │             │   │  │
│  │  │   ▼      ▼       ▼       ▼       ▼       ▼       ▼       ▼             │   │  │
│  │  │  GPIO5  GPIO18  GPIO19  GPIO23  GPIO14  GPIO26   GND     3.3V          │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  │                                                                               │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                           SD CARD MODULE                               │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │  CS     SCK    MOSI    MISO    VCC     GND                              │   │  │
│  │  │   │      │       │       │       │       │                              │   │  │
│  │  │   ▼      ▼       ▼       ▼       ▼       ▼                              │   │  │
│  │  │  GPIO4  GPIO18  GPIO23  GPIO19  5V      GND                             │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  │                                                                               │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                           LED INDICATOR                                │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │  Red LED ────┐                                                         │   │  │
│  │  │  Green LED ───┼──── GPIO12, GPIO13, GPIO27 via 220Ω resistors         │   │  │
│  │  │  Blue LED ────┘                                                         │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  │                                                                               │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                           CONNECTIVITY                                 │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │  • Built-in WiFi → HTTP POST to Flask Backend                         │   │  │
│  │  │  • USB Serial → Debug & Logging                                       │   │  │
│  │  │  • Optional Ethernet via ETH board                                    │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  │                                                                               │  │
│  │  ┌───────────────────────────────────────────────────────────────────────┐   │  │
│  │  │                           POWER                                         │   │  │
│  │  ├───────────────────────────────────────────────────────────────────────┤   │  │
│  │  │  ┌────────┐                                                            │   │  │
│  │  │  │ 5V USB │───────────▶ ESP32 Vin                                     │   │  │
│  │  │  └────────┘                         (Continuous Operation)            │   │  │
│  │  └───────────────────────────────────────────────────────────────────────┘   │  │
│  └───────────────────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────────────────┘
```

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   📡 COMMUNICATION FLOW                               ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

### 📦 **Data Packet Format**

```
┌─────────┬────────┬──────────┬──────────┬──────────┐
│ NODE_ID │  TYPE  │  VALUE1  │  VALUE2  │ BATTERY  │
├─────────┼────────┼──────────┼──────────┼──────────┤
│   01    │  FIRE  │    78    │   350    │   3.4    │
└─────────┴────────┴──────────┴──────────┴──────────┘
```

| Field | Deskripsi | Range/Contoh |
|-------|-----------|---------------|
| `NODE_ID` | Identitas unik node | 01, 02, 03... |
| `TYPE` | Jenis kejadian | `FIRE`, `FLOOD`, `VIB`, `TEST` |
| `VALUE1` | Nilai sensor utama | Suhu (°C), Ketinggian (cm) |
| `VALUE2` | Nilai threshold/baku | Ambang batas |
| `BATTERY` | Tegangan baterai | 3.0 - 4.2 V |

### 🔄 **Communication Sequence**

```ascii
┌─────────┐     ┌─────────┐     ┌─────────┐     ┌─────────┐     ┌─────────┐
│ SENSOR  │────▶│ CLIENT  │────▶│ MASTER  │────▶│ BACKEND │────▶│DASHBOARD│
│ TRIGGER │     │ ESP32   │     │ ESP32   │     │ FLASK   │     │         │
└─────────┘     └─────────┘     └─────────┘     └─────────┘     └─────────┘
    │               │               │               │               │
    ▼               ▼               ▼               ▼               ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                           TIME LINE                                      │
├─────────────────────────────────────────────────────────────────────────┤
│ 0ms    │          │          │          │          │          │        │
│        ▼          ▼          ▼          ▼          ▼          ▼        │
│  ├──────┴──────┬──┴──────┬──┴──────┬──┴──────┬──┴──────┬──┴──────┬──┤
│                                                                         │
│ • Sensor trigger (vibration/intruder)                                   │
│ • ESP32 wake from deep sleep (10μA → 80mA)                             │
│ • Sensor reading (100ms)                                               │
│ • LoRa TX (500ms) - Packet: "01|FIRE|78|350|3.4"                      │
│ • LoRa RX at Master - Validate packet                                  │
│ • Master sends HTTP POST to Flask (WiFi)                               │
│ • Flask processes & stores in SQLite                                   │
│ • WebSocket push to dashboard (real-time)                              │
│ • Client returns to deep sleep                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

### 📡 **LoRa Configuration**

| Parameter | Value |
|-----------|-------|
| Frequency | 915 MHz (US) / 868 MHz (EU) |
| Spreading Factor | SF12 (max range) |
| Bandwidth | 125 kHz |
| Coding Rate | 4/5 |
| TX Power | +20 dBm |
| Range | Up to 10 km (line of sight) |

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   ⚡ POWER MANAGEMENT STRATEGY                         ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

### 📊 **Power Profile - Client Node**

```ascii
                           POWER CONSUMPTION OVER TIME
    ┌─────────────────────────────────────────────────────────────────────┐
    │                                                           ACTIVE    │
    │  Current (mA)                                           ┌────┐     │
    │                                                         │    │     │
    │  80mA ─────────────────────────────────────────────────┘    └─────►
    │                                                                   
    │                                                                   
    │                                                                   
    │                                                                   
    │  10μA ────┐                 ┌─────────────────┐                 
    │           │                 │                 │                 
    │           │                 │                 │                 
    │           │                 │                 │                 
    │      DEEP SLEEP         DEEP SLEEP        DEEP SLEEP            
    │      (99% time)          (99% time)        (99% time)           
    └─────────────────────────────────────────────────────────────────────►
             15 min               15 min               15 min          Time
```

### ⚡ **Power States**

| Mode | Current | Duration | Frequency | Description |
|------|---------|----------|-----------|-------------|
| **Deep Sleep** | 10 μA | 15 min (default) | 99% | Semua sensor mati, RTC aktif |
| **Wake & Sense** | 80 mA | 100 ms | Setiap 15 min | Baca sensor, tidak ada event |
| **Transmit** | 80 mA | 500 ms | Saat event | Kirim data via LoRa |
| **Peak** | 120 mA | 50 ms | Rare | LoRa TX + sensor bersamaan |

### 🔋 **Battery Life Calculation**

```
Battery Capacity: 2000 mAh
Daily Consumption:
- Deep Sleep: 10μA × 23.9h = 0.239 mAh
- Wake & Sense: 80mA × 0.1s × 96x = 0.213 mAh
- Transmit: 80mA × 0.5s × 10x (est) = 0.111 mAh
Total Daily: ~0.563 mAh

Estimated Battery Life: 2000 mAh / 0.563 mAh per day ≈ 3,552 days ≈ 9.7 years
* Theoretically with perfect battery. Realistically: 6-12 months with self-discharge
```

### 🔌 **Power Saving Techniques**

- ✅ **Deep Sleep** dengan timer dan interrupt wake-up
- ✅ **Peripheral power gating** - matikan sensor saat tidak digunakan
- ✅ **Optimized LoRa** - gunakan SF7 untuk jarak dekat, SF12 hanya jika perlu
- ✅ **Event-based transmission** - tidak mengirim jika tidak ada perubahan signifikan
- ✅ **Battery voltage monitoring** untuk deteksi low battery

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   💻 BACKEND ARCHITECTURE                             ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

### 🏗️ **Backend Structure**

```ascii
┌─────────────────────────────────────────────────────────────────────────┐
│                        FLASK BACKEND ARCHITECTURE                        │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                           │
│  ┌───────────────────────────────────────────────────────────────────┐  │
│  │                           Flask App                                │  │
│  │   ┌───────────────────────────────────────────────────────────┐   │  │
│  │   │                        Routes                             │   │  │
│  │   │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │   │  │
│  │   │  │  /api/data   │  │  /api/nodes  │  │  /api/alerts │   │   │  │
│  │   │  │  POST/GET    │  │  GET/PUT     │  │  GET/PATCH   │   │   │  │
│  │   │  └──────────────┘  └──────────────┘  └──────────────┘   │   │  │
│  │   └───────────────────────────────────────────────────────────┘   │  │
│  │                              │                                     │  │
│  │                              ▼                                     │  │
│  │   ┌───────────────────────────────────────────────────────────┐   │  │
│  │   │                      WebSocket (Flask-SocketIO)           │   │  │
│  │   │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │   │  │
│  │   │  │  connect     │  │  new_data    │  │  new_alert   │   │   │  │
│  │   │  │  event       │  │  broadcast   │  │  broadcast   │   │   │  │
│  │   │  └──────────────┘  └──────────────┘  └──────────────┘   │   │  │
│  │   └───────────────────────────────────────────────────────────┘   │  │
│  │                              │                                     │  │
│  │                              ▼                                     │  │
│  │   ┌───────────────────────────────────────────────────────────┐   │  │
│  │   │                    Database (SQLAlchemy)                   │   │  │
│  │   │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │   │  │
│  │   │  │    Nodes     │  │   Readings   │  │    Alerts    │   │   │  │
│  │   │  │  - id        │  │  - id        │  │  - id        │   │   │  │
│  │   │  │  - node_id   │  │  - node_id   │  │  - node_id   │   │   │  │
│  │   │  │  - location  │  │  - timestamp │  │  - type      │   │   │  │
│  │   │  │  - battery   │  │  - value1    │  │  - severity  │   │   │  │
│  │   │  │  - last_seen │  │  - value2    │  │  - timestamp │   │   │  │
│  │   │  └──────────────┘  └──────────────┘  └──────────────┘   │   │  │
│  │   └───────────────────────────────────────────────────────────┘   │  │
│  └───────────────────────────────────────────────────────────────────┘  │
│                              │                                           │
│                              ▼                                           │
│  ┌───────────────────────────────────────────────────────────────────┐  │
│  │                      Web Dashboard (HTML/JS)                      │  │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐           │  │
│  │  │  Real-time   │  │  Historical  │  │  Node Map    │           │  │
│  │  │  Charts      │  │  Data Table  │  │  & Status    │           │  │
│  │  └──────────────┘  └──────────────┘  └──────────────┘           │  │
│  └───────────────────────────────────────────────────────────────────┘  │
│                                                                           │
└─────────────────────────────────────────────────────────────────────────┘
```

### 📊 **Database Schema (ERD)**

```ascii
┌───────────────────┐       ┌───────────────────┐       ┌───────────────────┐
│      NODES        │       │     READINGS      │       │     ALERTS        │
├───────────────────┤       ├───────────────────┤       ├───────────────────┤
│ id (PK) INTEGER   │──────▶│ id (PK) INTEGER   │       │ id (PK) INTEGER   │
│ node_id TEXT      │       │ node_id TEXT (FK) │       │ node_id TEXT (FK) │
│ location TEXT     │       │ timestamp DATETIME│       │ type TEXT         │
│ battery FLOAT     │       │ sensor_type TEXT  │       │ severity INTEGER  │
│ last_seen DATETIME│       │ value1 FLOAT      │       │ description TEXT  │
│ status TEXT       │       │ value2 FLOAT      │       │ timestamp DATETIME│
│ created_at DATETIME│      │ created_at DATETIME│      │ acknowledged BOOL │
└───────────────────┘       └───────────────────┘       └───────────────────┘
         │                           │                           │
         │                           │                           │
         ▼                           ▼                           ▼
┌─────────────────────────────────────────────────────────────────────┐
│                         RELATIONSHIPS                                │
│  • Nodes memiliki banyak Readings (one-to-many)                     │
│  • Nodes memiliki banyak Alerts (one-to-many)                       │
│  • node_id sebagai foreign key di Readings dan Alerts               │
└─────────────────────────────────────────────────────────────────────┘
```

### 📁 **Project Structure**

```
forest-sentinel/
├── 📁 client/
│   ├── src/
│   │   ├── main.cpp
│   │   ├── sensors/
│   │   │   ├── gas_sensor.cpp
│   │   │   ├── vibration.cpp
│   │   │   └── water_level.cpp
│   │   ├── lora/
│   │   │   └── lora_comm.cpp
│   │   └── power/
│   │       └── sleep_manager.cpp
│   ├── platformio.ini
│   └── include/
│       └── config.h
│
├── 📁 master/
│   ├── src/
│   │   ├── main.cpp
│   │   ├── lora_receiver.cpp
│   │   ├── wifi_manager.cpp
│   │   └── sd_card.cpp
│   └── platformio.ini
│
├── 📁 backend/
│   ├── app.py
│   ├── models.py
│   ├── database.db
│   ├── requirements.txt
│   ├── 📁 templates/
│   │   └── dashboard.html
│   └── 📁 static/
│       ├── style.css
│       └── script.js
│
├── README.md
├── LICENSE
└── docs/
    └── wiring_diagrams/
```

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   🚀 HOW TO RUN                                       ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

### 📋 **Prerequisites**

| Component | Requirement |
|-----------|-------------|
| **ESP32 Development** | PlatformIO / Arduino IDE |
| **Python** | 3.8+ with pip |
| **LoRa Modules** | RFM95W / RFM96W |
| **Sensors** | MQ-2, SW-420, Water Level, INMP441 |

---

### 1️⃣ **Setup Client Node**

```bash
# Clone repository
git clone https://github.com/yourusername/forest-sentinel-lora.git
cd forest-sentinel-lora/client

# Install dependencies via PlatformIO
pio lib install "milesburton/DallasTemperature"
pio lib install "adafruit/Adafruit Unified Sensor"
pio lib install "sandeepmistry/LoRa"

# Configure client (edit config.h)
nano include/config.h
```

```cpp
// config.h - Client Configuration
#define NODE_ID "01"
#define LORA_FREQUENCY 915E6  // 915 MHz
#define DEEP_SLEEP_SECONDS 900  // 15 minutes
#define GAS_THRESHOLD 50  // Smoke threshold
#define WATER_THRESHOLD 30  // Water level threshold (cm)

// Upload to ESP32
pio run --target upload --environment esp32-s3

// Monitor serial output
pio device monitor
```

---

### 2️⃣ **Setup Master Gateway**

```bash
cd ../master

# Configure WiFi and backend
nano include/config.h
```

```cpp
// config.h - Master Configuration
#define LORA_FREQUENCY 915E6
#define WIFI_SSID "YourWiFi"
#define WIFI_PASSWORD "YourPassword"
#define BACKEND_URL "http://192.168.1.100:5000/api/data"
#define SD_CARD_ENABLED true

// Upload to ESP32
pio run --target upload --environment esp32

// Monitor
pio device monitor
```

---

### 3️⃣ **Setup Backend Server**

```bash
cd ../backend

# Create virtual environment
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Initialize database
python -c "from app import app, db; app.app_context().push(); db.create_all()"

# Run Flask server
python app.py

# Access dashboard
# Open browser: http://localhost:5000
```

**requirements.txt:**
```
Flask==2.3.2
Flask-SQLAlchemy==3.0.5
Flask-SocketIO==5.3.4
python-socketio==5.9.0
eventlet==0.33.3
pandas==2.0.3
```

---

### 4️⃣ **Configuration Files**

**platformio.ini (Client):**
```ini
[env:esp32-s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
board_build.partitions = huge_app.csv

lib_deps = 
    sandeepmistry/LoRa @ ^0.8.0
    adafruit/Adafruit MQTT Library @ ^2.5.0
    adafruit/Adafruit Unified Sensor @ ^1.1.9
```

**app.py (Backend):**
```python
from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO, emit
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import json

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database.db'
db = SQLAlchemy(app)
socketio = SocketIO(app)

class Reading(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    node_id = db.Column(db.String(10))
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)
    sensor_type = db.Column(db.String(20))
    value1 = db.Column(db.Float)
    value2 = db.Column(db.Float)

@app.route('/api/data', methods=['POST'])
def receive_data():
    data = request.json
    # Parse LoRa packet: "01|FIRE|78|350|3.4"
    reading = Reading(
        node_id=data['node_id'],
        sensor_type=data['type'],
        value1=data['value1'],
        value2=data['value2']
    )
    db.session.add(reading)
    db.session.commit()
    
    # Broadcast via WebSocket
    socketio.emit('new_data', data)
    
    return jsonify({"status": "success"}), 200

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000, debug=True)
```

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   📊 FUTURE DEVELOPMENT                               ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

### 🚧 **Roadmap**

```ascii
┌─────────────────────────────────────────────────────────────────────────────────┐
│                         DEVELOPMENT ROADMAP 2024-2025                           │
├─────────────────────────────────────────────────────────────────────────────────┤
│                                                                                   │
│  Q1 2024    Q2 2024    Q3 2024    Q4 2024    Q1 2025    Q2 2025    Q3 2025     │
│    │          │          │          │          │          │          │          │
│    ▼          ▼          ▼          ▼          ▼          ▼          ▼          │
│  ┌────┐     ┌────┐     ┌────┐     ┌────┐     ┌────┐     ┌────┐     ┌────┐     │
│  │v1.0│────▶│v1.5│────▶│v2.0│────▶│v2.5│────▶│v3.0│────▶│v3.5│────▶│v4.0│     │
│  └────┘     └────┘     └────┘     └────┘     └────┘     └────┘     └────┘     │
│    │          │          │          │          │          │          │          │
│    ▼          ▼          ▼          ▼          ▼          ▼          ▼          │
│ • Basic     • Solar    • LoRaWAN  • Mesh     • ML on   • Mobile   • Satellite  │
│ • LoRa      • Charging • Gateway  • Network  • Edge     • App      • Backhaul  │
│ • 3 Sensors • Web      • Cloud    • Multi-   • Predict- • Push     • Global    │
│ • Deep Sleep• Dashboard• Sync     • hop      • tions    • Notif    • Coverage  │
│                                                                                   │
└─────────────────────────────────────────────────────────────────────────────────┘
```

### 🔮 **Planned Features**

| Priority | Feature | Description | Status |
|----------|---------|-------------|--------|
| 🔴 **High** | LoRaWAN Integration | Koneksi ke The Things Network | Planned |
| 🔴 **High** | AES Encryption | Enkripsi data LoRa | In Progress |
| 🟡 **Medium** | ML Fire Prediction | TensorFlow Lite untuk prediksi | Research |
| 🟡 **Medium** | Mobile App | React Native for alerts | Planned |
| 🟢 **Low** | GPS Module | Tracking node location | Future |
| 🟢 **Low** | Solar MPPT | Maximum power point tracking | Future |

### 🤝 **Contributing**

Kami sangat terbuka untuk kontribusi! Area yang bisa dikerjakan:

- 📝 **Dokumentasi** – Perbaiki README, tambah wiring diagrams
- 🐛 **Bug Fixes** – Laporkan atau perbaiki issue
- ✨ **Fitur Baru** – Implementasi fitur dari roadmap
- 🔧 **Testing** – Uji coba di lingkungan nyata

```bash
# Cara berkontribusi
1. Fork repository
2. Buat branch fitur (git checkout -b feature/AmazingFeature)
3. Commit perubahan (git commit -m 'Add some AmazingFeature')
4. Push ke branch (git push origin feature/AmazingFeature)
5. Open Pull Request
```

---

╔══════════════════════════════════════════════════════════════════════════════════════╗
║                                   📷 PROJECT PREVIEW                                  ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

### 🖥️ **Web Dashboard Preview**

```ascii
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                         FOREST SENTINEL - MONITORING DASHBOARD                       │
├─────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│  ┌───────────────────────────────────────────────────────────────────────────────┐  │
│  │                           SYSTEM STATUS OVERVIEW                               │  │
│  ├───────────────────────────────────────────────────────────────────────────────┤  │
│  │                                                                               │  │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │  │
│  │  │   TOTAL      │  │   ACTIVE     │  │   ALERTS     │  │   BATTERY    │     │  │
│  │  │   NODES      │  │   NODES      │  │   TODAY      │  │   AVG        │     │  │
│  │  │     ╔══╗     │  │     ╔══╗     │  │     ╔══╗     │  │     ╔══╗     │     │  │
│  │  │     ║12║     │  │     ║10║     │  │     ║03║     │  │     ║78║     │     │  │
│  │  │     ╚══╝     │  │     ╚══╝     │  │     ╚══╝     │  │     ╚══╝     │     │  │
│  │  │    Devices   │  │   Connected  │  │  New Alerts  │  │    Health %   │     │  │
│  │  └──────────────┘  └──────────────┘  └──────────────┘  └──────────────┘     │  │
│  └───────────────────────────────────────────────────────────────────────────────┘  │
│                                                                                       │
│  ┌───────────────────────────────────────────────────────────────────────────────┐  │
│  │                           NODE STATUS GRID                                     │  │
│  ├───────────────────────────────────────────────────────────────────────────────┤  │
│  │                                                                               │  │
│  │  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐              │  │
│  │  │   NODE 01       │  │   NODE 02       │  │   NODE 03       │              │  │
│  │  │   Forest A      │  │   Forest B      │  │   River C       │              │  │
│  │  ├─────────────────┤  ├─────────────────┤  ├─────────────────┤              │  │
│  │  │  🔥 FIRE        │  │  💧 FLOOD       │  │  🌊 NORMAL      │              │  │
│  │  │  78°C / 350ppm  │  │  45cm / 30cm   │  │  24°C / 65%     │              │  │
│  │  │  ⚡ 3.7V ████░  │  │  ⚡ 4.1V █████  │  │  ⚡ 3.9V ████▒  │              │  │
│  │  │  🕒 10:32 AM    │  │  🕒 10:30 AM   │  │  🕒 10:28 AM    │              │  │
│  │  └─────────────────┘  └─────────────────┘  └─────────────────┘              │  │
│  │                                                                               │  │
│  │  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐              │  │
│  │  │   NODE 04       │  │   NODE 05       │  │   NODE 06       │              │  │
│  │  │   Hill D        │  │   Valley E      │  │   Ridge F       │              │  │
│  │  ├─────────────────┤  ├─────────────────┤  ├─────────────────┤              │  │
│  │  │  🌫️ SMOKE       │  │  🟢 OK          │  │  ⚠️ LOW BAT     │              │  │
│  │  │  120ppm / 42°C  │  │  22°C / 1013hPa│  │  25°C / 68%     │              │  │
│  │  │  ⚡ 4.0V ████▒  │  │  ⚡ 4.2V █████  │  │  ⚡ 3.2V ██░░░  │              │  │
│  │  │  🕒 10:25 AM    │  │  🕒 10:22 AM   │  │  🕒 10:20 AM    │              │  │
│  │  └─────────────────┘  └─────────────────┘  └─────────────────┘              │  │
│  └───────────────────────────────────────────────────────────────────────────────┘  │
│                                                                                       │
│  ┌───────────────────────────────────────────────────────────────────────────────┐  │
│  │                           REAL-TIME ALERT LOG                                  │  │
│  ├───────────────────────────────────────────────────────────────────────────────┤  │
│  │  [10:32:15] 🚨 CRITICAL: FIRE detected at Node 01 - Temp: 78°C, Smoke: 350ppm │  │
│  │  [10:30:22] ⚠️ WARNING: Flood level rising at Node 02 - Current: 45cm        │  │
│  │  [10:28:07] ℹ️ INFO: Node 03 - Normal operation                              │  │
│  │  [10:25:43] 🔋 LOW BATTERY: Node 06 - 3.2V, please replace soon             │  │
│  │  [10:22:18] ✅ ACKNOWLEDGED: Alert #1234 - Fire risk mitigated              │  │
│  └───────────────────────────────────────────────────────────────────────────────┘  │
│                                                                                       │
│  ┌───────────────────────────────────────────────────────────────────────────────┐  │
│  │                           HISTORICAL CHART                                      │  │
│  ├───────────────────────────────────────────────────────────────────────────────┤  │
│  │  Temperature Trend - Node 01 (Last 24h)                                       │  │
│  │  80°C ┼───────────╔══╗────────────────────────────────────────────────────   │  │
│  │  60°C ┼────────╔══╝  ╚══╗───────────────────────────────────────────────   │  │
│  │  40°C ┼────╔═══╝       ╚═══╗────────────────────────────────────────────   │  │
│  │  20°C ┼═╗──╝                ╚══╗─────────────────────────────────────────   │  │
│  │       └────────────────────────────────────────────────────────────────────   │  │
│  │        00:00    06:00    12:00    18:00    23:59                            │  │
│  └───────────────────────────────────────────────────────────────────────────────┘  │
│                                                                                       │
└─────────────────────────────────────────────────────────────────────────────────────┘
```

### 📱 **Mobile View Preview**

```ascii
┌─────────────────────┐
│  FOREST SENTINEL    │
├─────────────────────┤
│                     │
│  🔴 ALERT: FIRE     │
│  Node 01 - 78°C     │
│  10:32 AM           │
├─────────────────────┤
│  NODE STATUS        │
│  ┌───────────────┐  │
│  │01 🔥 78° 3.7V │  │
│  │02 💧 45cm 4.1V│  │
│  │03 ✅ 24° 3.9V │  │
│  │04 🌫️ 42° 4.0V│  │
│  │05 ✅ 22° 4.2V│  │
│  │06 ⚠️ 25° 3.2V│  │
│  └───────────────┘  │
├─────────────────────┤
│  BATTERY OVERVIEW   │
│  01: ████░░ 70%    │
│  02: █████ 85%     │
│  03: ████▒ 75%     │
│  04: ████▒ 78%     │
│  05: █████ 90%     │
│  06: ██░░░ 45%     │
├─────────────────────┤
│  📊 View Charts     │
│  ⚙️ Settings        │
└─────────────────────┘
```

---

## 📝 License

<div align="center">

```
MIT License

Copyright (c) 2026 Forest Sentinel Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
```

[![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)](LICENSE)

</div>

---

## 👥 Team & Contributors

<div align="center">

| Role | Name | Contact |
|------|------|---------|
| **Project Lead** | Forest Sentinel Team | [@forest-sentinel](https://github.com) |
| **Hardware Engineer** | - | - |
| **Backend Developer** | - | - |
| **UI/UX Designer** | - | - |

</div>

---

## 🙏 Acknowledgments

- **LoRa Alliance** for long-range communication standard
- **Espressif** for amazing ESP32 platform
- **PlatformIO** for excellent embedded IDE
- **Flask** community for lightweight backend framework

---

<div align="center">

```
╔══════════════════════════════════════════════════════════════════════════════════╗
║                                                                                   ║
║     🌲 FOREST SENTINEL LORA SYSTEM - Protecting Forests with Technology         ║
║                                                                                   ║
║     ⭐ Star us on GitHub! · 🐛 Report Bug · 📫 Request Feature                  ║
║                                                                                   ║
╚══════════════════════════════════════════════════════════════════════════════════╝
```

**Built with ❤️ for forest conservation | Version 1.0.0 | Last Updated: 22 Feb 2026**

</div>
