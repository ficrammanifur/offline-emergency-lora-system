#include <WiFi.h>
#include <WiFiManager.h> // Tambahkan ini
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ===== OLED CONFIGURATION ===== */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* ===== LORA PINS ===== */
#define SS 5
#define RST 14
#define DIO0 2

struct SensorPacket {
    float temperature;
    float humidity;
    int gas;
    int light;
    int soil;
    int vibration;
    bool sos;
    uint32_t nodeID;
};

QueueHandle_t dataQueue;
WiFiClient espClient;
PubSubClient client(espClient);

/* ===== TAMPILAN OLED ===== */
void updateOLED(String status, String ip) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("FOREST SENTINEL");
    display.println("---------------------");
    display.print("Status: "); display.println(status);
    display.print("IP: "); display.println(ip);
    display.print("MQTT: "); display.println(client.connected() ? "CONNECTED" : "DISCONNECT");
    display.display();
}

/* ===== TASK LORA (CORE 0) ===== */
void taskLoRa(void *pvParameters) {
    SensorPacket receivedData;
    while(1) {
        int packetSize = LoRa.parsePacket();
        if (packetSize == sizeof(SensorPacket)) {
            LoRa.readBytes((uint8_t*)&receivedData, sizeof(receivedData));
            if (xQueueSend(dataQueue, &receivedData, 0) != pdTRUE) {
                Serial.println("Queue Full!");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/* ===== TASK MQTT (CORE 1) ===== */
void taskMQTT(void *pvParameters) {
    SensorPacket packet;
    while(1) {
        if (xQueueReceive(dataQueue, &packet, portMAX_DELAY) == pdTRUE) {
            if (client.connected()) {
                StaticJsonDocument<512> doc;
                doc["node"] = packet.nodeID;
                doc["temp"] = packet.temperature;
                doc["hum"] = packet.humidity;
                doc["gas"] = packet.gas;
                doc["sos"] = packet.sos ? "EMERGENCY" : "OFF";
                
                char buffer[512];
                serializeJson(doc, buffer);
                client.publish("forest/data", buffer);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize OLED
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
    }
    display.clearDisplay();
    display.display();

    // WiFi Manager
    WiFiManager wm;
    updateOLED("Config Portal", "192.168.4.1");
    
    // Jika tidak konek ke WiFi lama, buka AP "ForestSentinel_Master"
    if (!wm.autoConnect("ForestSentinel_Master")) {
        Serial.println("Failed to connect");
        ESP.restart();
    }
    
    updateOLED("Connected", WiFi.localIP().toString());

    // Setup LoRa
    LoRa.setPins(SS, RST, DIO0);
    if (!LoRa.begin(433E6)) {
        updateOLED("LoRa Error", "");
        while(1);
    }

    client.setServer("broker.hivemq.com", 1883);
    dataQueue = xQueueCreate(20, sizeof(SensorPacket));

    xTaskCreatePinnedToCore(taskLoRa, "LoRaTask", 4096, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(taskMQTT, "MQTTTask", 4096, NULL, 1, NULL, 1);
}

void loop() {
    if (!client.connected()) {
        // Logika reconnect MQTT
        String clientId = "ForestMaster-" + String(random(1000));
        client.connect(clientId.c_str());
    }
    client.loop();
    
    // Update OLED berkala untuk status MQTT/IP
    updateOLED("Running", WiFi.localIP().toString());
    vTaskDelay(pdMS_TO_TICKS(5000));
}
