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

## 📑 Daftar Isi

<div align="center">

[✨ Overview](#-overview) • [🧠 System Architecture](#-system-architecture) • [🔩 Hardware](#-hardware-components) • [🔌 Wiring](#-wiring-diagram) • [📡 Communication](#-communication-flow) • [⚡ Power](#-power-management-strategy) • [💻 Backend](#-backend-architecture) • [🚀 How To Run](#-how-to-run) • [📊 Future](#-future-development) • [📷 Preview](#-project-preview)

</div>

---

## ✨ Overview

**Forest Sentinel** adalah sistem monitoring lingkungan mandiri yang dirancang khusus untuk **area hutan terpencil** tanpa akses internet. Sistem ini menggabungkan teknologi **LoRa** untuk komunikasi jarak jauh (hingga 10km) dengan strategi **deep sleep** ultra-hemat daya yang memungkinkan operasi berbulan-bulan hanya dengan baterai.

### 🎯 **Tujuan Utama**
- 🔥 Deteksi dini kebakaran hutan melalui sensor asap dan suhu
- 🌊 Peringatan dini banjir di daerah aliran sungai
- 📡 Monitoring aktivitas seismik untuk potensi tanah longsor
- 📊 Visualisasi real-time melalui dashboard lokal

---

## 🧠 System Architecture

Berikut adalah arsitektur sistem yang menunjukkan alur data dari sensor di hutan hingga ke dashboard monitoring.

```mermaid
graph TD
    subgraph A[🌲 Forest Area Remote]
        direction TB
        C1[Client Node 01<br>ESP32 + Sensors]
        C2[Client Node 02<br>ESP32 + Sensors]
        C3[Client Node 03<br>ESP32 + Sensors]
    end

    subgraph B[🏕️ Base Camp Monitoring Post]
        direction TB
        MG[Master Gateway<br>ESP32 LoRa Receiver]
        BE[Backend Server<br>Flask + SQLite]
        WD[Web Dashboard<br>Real-time Charts]
    end

    C1 -- LoRa 868/915 MHz --> MG
    C2 -- LoRa 868/915 MHz --> MG
    C3 -- LoRa 868/915 MHz --> MG
    
    MG -- Data via WiFi --> BE
    BE -- WebSocket --> WD
    MG --> SD[(SD Card Backup)]
    BE --> AL[Alert System Notifications]

    style C1 fill:#c7e9c0,stroke:#2e7d32,color:#000
    style C2 fill:#c7e9c0,stroke:#2e7d32,color:#000
    style C3 fill:#c7e9c0,stroke:#2e7d32,color:#000
    style MG fill:#ffecb3,stroke:#b26a00,color:#000
    style BE fill:#bbdefb,stroke:#0d47a1,color:#000
    style WD fill:#d1c4e9,stroke:#4a148c,color:#fff
```

---

## 🔩 Hardware Components

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

## 🔌 Wiring Diagram

### Client Node

```mermaid
graph LR
    subgraph ESP32_S3[ESP32-S3 Board]
        Pins[GPIO Pins]
    end

    subgraph Sensors[Sensors]
        MQ2[MQ-2 Gas<br>A0]
        SW420[SW-420 Vibration<br>DO]
        Water[Water Level<br>A0]
        INMP[INMP441 Mic<br>WS/SCK/SD]
    end

    subgraph LoRa[LoRa Module RFM95W]
        NSS[NSS]
        SCK[SCK]
        MISO[MISO]
        MOSI[MOSI]
        RST[RST]
        DIO0[DIO0]
    end

    subgraph Power[Power System]
        BATT[LiPo 3.7V]
        CHARGER[TP4056 Charger]
        SOLAR[Solar Panel 5V]
    end

    ESP32_S3 -- GPIO5 --> NSS
    ESP32_S3 -- GPIO18 --> SCK
    ESP32_S3 -- GPIO19 --> MISO
    ESP32_S3 -- GPIO23 --> MOSI
    ESP32_S3 -- GPIO14 --> RST
    ESP32_S3 -- GPIO26 --> DIO0

    ESP32_S3 -- GPIO34 --> MQ2
    ESP32_S3 -- GPIO27 --> SW420
    ESP32_S3 -- GPIO32 --> Water
    ESP32_S3 -- GPIO25 --> INMP

    BATT --> CHARGER
    SOLAR --> CHARGER
    CHARGER --> ESP32_S3

    style ESP32_S3 fill:#f9f,stroke:#333,stroke-width:2px,color:#000
    style Sensors fill:#ccf,stroke:#333,stroke-width:2px,color:#000
    style LoRa fill:#cfc,stroke:#333,stroke-width:2px,color:#000
    style Power fill:#fcf,stroke:#333,stroke-width:2px,color:#000
```

### Master Gateway

```mermaid
graph LR
    subgraph ESP32[ESP32 Board]
        Pins[GPIO Pins]
    end

    subgraph LoRa_M[LoRa Module RFM95W]
        NSS_M[NSS]
        SCK_M[SCK]
        MISO_M[MISO]
        MOSI_M[MOSI]
        RST_M[RST]
        DIO0_M[DIO0]
    end

    subgraph SD[SD Card Module]
        CS[CS]
        SCK_SD[SCK]
        MOSI_SD[MOSI]
        MISO_SD[MISO]
    end

    subgraph LED[LED Indicators]
        RED[Red LED]
        GREEN[Green LED]
        BLUE[Blue LED]
    end

    ESP32 -- GPIO5 --> NSS_M
    ESP32 -- GPIO18 --> SCK_M
    ESP32 -- GPIO19 --> MISO_M
    ESP32 -- GPIO23 --> MOSI_M
    ESP32 -- GPIO14 --> RST_M
    ESP32 -- GPIO26 --> DIO0_M

    ESP32 -- GPIO4 --> CS
    ESP32 -- GPIO18 --> SCK_SD
    ESP32 -- GPIO23 --> MOSI_SD
    ESP32 -- GPIO19 --> MISO_SD

    ESP32 -- GPIO12 --> RED
    ESP32 -- GPIO13 --> GREEN
    ESP32 -- GPIO27 --> BLUE

    style ESP32 fill:#f9f,stroke:#333,stroke-width:2px,color:#000
    style LoRa_M fill:#cfc,stroke:#333,stroke-width:2px,color:#000
    style SD fill:#ccf,stroke:#333,stroke-width:2px,color:#000
    style LED fill:#fcf,stroke:#333,stroke-width:2px,color:#000
```

---

## 📡 Communication Flow

### 📦 **Data Packet Format**

| Field | Deskripsi | Range/Contoh |
|-------|-----------|---------------|
| `NODE_ID` | Identitas unik node | 01, 02, 03... |
| `TYPE` | Jenis kejadian | `FIRE`, `FLOOD`, `VIB`, `TEST` |
| `VALUE1` | Nilai sensor utama | Suhu (°C), Ketinggian (cm) |
| `VALUE2` | Nilai threshold/baku | Ambang batas |
| `BATTERY` | Tegangan baterai | 3.0 - 4.2 V |

### 🔄 **Communication Sequence**

```mermaid
sequenceDiagram
    participant S as Sensor Trigger
    participant C as Client ESP32
    participant M as Master ESP32
    participant B as Backend Flask
    participant D as Dashboard UI

    S->>C: 1. Wake-up Interrupt
    C->>C: 2. Read Sensors (100ms)
    C->>M: 3. LoRa TX (500ms)<br>Packet: "01|FIRE|78|350|3.4"
    M->>M: 4. Validate Packet
    M->>B: 5. HTTP POST /api/data
    B->>B: 6. Store in SQLite
    B-->>D: 7. WebSocket Broadcast
    C->>C: 8. Return to Deep Sleep
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

## ⚡ Power Management Strategy

### 📊 **Power Profile - Client Node**

```mermaid
gantt
    title Power Consumption Over Time
    dateFormat HH:mm:ss
    axisFormat %H:%M
    
    section Deep Sleep (10μA)
    Deep Sleep 1 :done, ds1, 00:00:00, 15m
    Deep Sleep 2 :done, ds2, after ds1, 15m

    section Wake & Sense (80mA)
    Wake Up 1 :active, w1, 00:15:00, 100ms
    
    section Transmit (80mA)
    LoRa TX 1 :active, t1, 00:15:01, 500ms

    section Deep Sleep (10μA)
    Deep Sleep 3 :done, ds3, after t1, 15m

    section Wake & Sense (80mA)
    Wake Up 2 :active, w2, 00:30:00, 100ms
    
    section Transmit (80mA)
    LoRa TX 2 :active, t2, 00:30:01, 500ms
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

## 💻 Backend Architecture

### 🏗️ **Backend Structure**

```mermaid
graph TD
    subgraph Flask[Flask Application]
        Routes[Routes]
        WS[WebSocket SocketIO]
        DB[(SQLite Database)]
    end

    subgraph RoutesDetail[API Endpoints]
        D1["/api/data (POST/GET)"]
        D2["/api/nodes (GET/PUT)"]
        D3["/api/alerts (GET/PATCH)"]
    end

    subgraph WebSocket[WebSocket Events]
        W1[connect]
        W2[new_data]
        W3[new_alert]
    end

    subgraph Database[Database Models]
        T1[Nodes Table]
        T2[Readings Table]
        T3[Alerts Table]
    end

    Flask --> RoutesDetail
    Flask --> WebSocket
    Flask --> Database

    Routes --> D1
    Routes --> D2
    Routes --> D3
    WS --> W1
    WS --> W2
    WS --> W3
    DB --> T1
    DB --> T2
    DB --> T3

    Dashboard[Web Dashboard HTML/JS] --> WS
    Master[Master Gateway] --> Routes

    style Flask fill:#bbdefb,stroke:#0d47a1,color:#000
    style Dashboard fill:#d1c4e9,stroke:#4a148c,color:#000
    style Master fill:#ffecb3,stroke:#b26a00,color:#000
```

### 📊 **Database Schema (ERD)**

```mermaid
erDiagram
    NODES ||--o{ READINGS : has
    NODES ||--o{ ALERTS : has

    NODES {
        int id PK
        string node_id
        string location
        float battery
        datetime last_seen
        string status
        datetime created_at
    }

    READINGS {
        int id PK
        string node_id FK
        datetime timestamp
        string sensor_type
        float value1
        float value2
        datetime created_at
    }

    ALERTS {
        int id PK
        string node_id FK
        string type
        int severity
        string description
        datetime timestamp
        bool acknowledged
    }
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

## 🚀 How To Run

### 📋 **Prerequisites**

| Component | Requirement |
|-----------|-------------|
| **ESP32 Development** | PlatformIO / Arduino IDE |
| **Python** | 3.8+ with pip |
| **LoRa Modules** | RFM95W / RFM96W |
| **Sensors** | MQ-2, SW-420, Water Level, INMP441 |

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

## 📊 Future Development

### 🚧 **Roadmap**

```mermaid
gantt
    title Development Roadmap 2024-2025
    dateFormat YYYY-MM
    axisFormat %b %Y
    
    section v1.0 - Basic LoRa
    Basic LoRa Communication :done, 2024-01, 90d
    3 Sensors Integration :done, 2024-01, 90d
    Deep Sleep Implementation :done, 2024-02, 60d

    section v1.5 - Infrastructure
    Solar Charging :active, 2024-04, 90d
    Web Dashboard :active, 2024-05, 60d

    section v2.0 - Network
    LoRaWAN Gateway : 2024-07, 120d
    Cloud Sync : 2024-08, 90d

    section v2.5 - Intelligence
    Mesh Networking : 2024-11, 90d
    Multi-hop Routing : 2024-12, 60d

    section v3.0 - Edge AI
    ML on Edge : 2025-02, 90d
    Fire Predictions : 2025-03, 60d

    section v3.5 - Mobility
    Mobile App : 2025-05, 90d
    Push Notifications : 2025-06, 60d

    section v4.0 - Global
    Satellite Backhaul : 2025-08, 120d
    Global Coverage : 2025-09, 90d
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

## 📷 Project Preview

### 🖥️ **Web Dashboard Preview**

```mermaid
graph TD
    subgraph Dashboard[Web Dashboard Interface]
        Header[System Status Overview]
        Stats[Node Statistics Cards]
        Grid[Node Status Grid]
        Log[Real-time Alert Log]
        Chart[Historical Chart]
    end

    subgraph StatsCards[Status Cards]
        C1[Total Nodes: 12]
        C2[Active Nodes: 10]
        C3[Alerts Today: 03]
        C4[Battery Avg: 78%]
    end

    subgraph NodeGrid[6-Node Status Display]
        N1["Node 01: 🔥 FIRE<br>78°C / 350ppm<br>Battery: 3.7V"]
        N2["Node 02: 💧 FLOOD<br>45cm / 30cm<br>Battery: 4.1V"]
        N3["Node 03: 🌊 NORMAL<br>24°C / 65%<br>Battery: 3.9V"]
        N4["Node 04: 🌫️ SMOKE<br>42°C / 120ppm<br>Battery: 4.0V"]
        N5["Node 05: 🟢 OK<br>22°C / 1013hPa<br>Battery: 4.2V"]
        N6["Node 06: ⚠️ LOW BAT<br>25°C / 68%<br>Battery: 3.2V"]
    end

    subgraph AlertLog[Recent Alerts]
        A1[🚨 CRITICAL: FIRE at Node 01 - 10:32]
        A2[⚠️ WARNING: Flood at Node 02 - 10:30]
        A3[ℹ️ INFO: Node 03 Normal - 10:28]
        A4[🔋 LOW BATTERY: Node 06 - 10:25]
    end

    subgraph Historical[24h Temperature Chart]
        H1[80°C ───────╔══╗────────]
        H2[60°C ───╔══╝  ╚══╗─────]
        H3[40°C ─╔═╝       ╚═╗───]
        H4[20°C ═╝            ╚══─]
        H5[00:00    12:00    23:59]
    end

    Dashboard --> StatsCards
    Dashboard --> NodeGrid
    Dashboard --> AlertLog
    Dashboard --> Historical
```

### 📱 **Mobile View Preview**

```mermaid
graph TD
    subgraph Mobile[Mobile App Interface]
        Header[🌲 FOREST SENTINEL]
        Alert[🔴 ALERT: FIRE<br>Node 01 - 78°C<br>10:32 AM]
        Status[Node Status List]
        Batt[Battery Overview]
        Footer[📊 Charts<br>⚙️ Settings]
    end

    subgraph StatusList[Status List]
        L1[01 🔥 78° 3.7V]
        L2[02 💧 45cm 4.1V]
        L3[03 ✅ 24° 3.9V]
        L4[04 🌫️ 42° 4.0V]
        L5[05 ✅ 22° 4.2V]
        L6[06 ⚠️ 25° 3.2V]
    end

    subgraph BatteryOverview[Battery %]
        B1[01: ████░░ 70%]
        B2[02: █████ 85%]
        B3[03: ████▒ 75%]
        B4[04: ████▒ 78%]
        B5[05: █████ 90%]
        B6[06: ██░░░ 45%]
    end

    Mobile --> Alert
    Mobile --> StatusList
    Mobile --> BatteryOverview
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
```
