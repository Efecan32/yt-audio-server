// ==================== ESP32 + Xiaozhi AI RADYO TOOL ====================
// MCP Sunucusu ile entegre, Cloudflare API'si kullanarak radyo çal

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "mcp_server.h"
#include "board.h"
#include "radiotolll.h"  // 🔥 BU ÖNEMLİ!
#include <esp_log.h>

// ==================== AYARLAR ====================
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Log tag
#define APP_TAG "RadioApp"

// Global RadioTool nesnesi
RadioTool* radioTool = nullptr;

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  ESP_LOGI(APP_TAG, "\n\n🚀 ESP32 + Xiaozhi AI Radyo Başlanıyor...");
  
  // 1. WiFi'ya bağlan
  connectToWiFi();
  
  // 2. MCP Server başlat
  auto& mcp = McpServer::GetInstance();
  
  // 3. RadioTool'u oluştur ve kaydet
  // ⭐ Bu adımda play_radio tool otomatik olarak MCP'ye eklenir
  radioTool = new RadioTool();
  
  ESP_LOGI(APP_TAG, "✅ Hazır! Radyo komutlarını kullanabilirsin.");
  printAvailableCommands();
}

// ==================== LOOP ====================
void loop() {
  // MCP Server request'leri işler
  // (MCP kütüphanesi background'da çalışır)
  
  // Serial komutları test için
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase();
    
    if (input.length() > 0) {
      ESP_LOGI(APP_TAG, "📝 Seri input: %s", input.c_str());
      handleSerialCommand(input);
    }
  }
  
  delay(100);
}

// ==================== FONKSİYONLAR ====================

// WiFi'ya bağlan
void connectToWiFi() {
  ESP_LOGI(APP_TAG, "📶 WiFi'ya bağlanıyor: %s", ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    ESP_LOGI(APP_TAG, "✅ WiFi Bağlandı!");
    ESP_LOGI(APP_TAG, "IP: %s", WiFi.localIP().toString().c_str());
  } else {
    ESP_LOGE(APP_TAG, "❌ WiFi Bağlanamadı!");
  }
}

// Komutları göster
void printAvailableCommands() {
  Serial.println("\n🎙️ DESTEKLENEN KOMUTlar:");
  Serial.println("  • kral      - Kral Pop");
  Serial.println("  • power     - Power FM");
  Serial.println("  • slow      - Slow Türk");
  Serial.println("  • metro     - Metro FM");
  Serial.println("  • fenomen   - Fenomen FM");
  Serial.println("  • joy       - Joy Türk");
  Serial.println("  • super     - Süper FM");
  Serial.println("  • trt       - TRT FM");
  Serial.println("  • stop/dur  - Radyoyu Durdur");
  Serial.println("\nÖrnek: 'kral' yazıp Enter'e bas");
  Serial.println();
}

// Serial komutı işle (Test için)
void handleSerialCommand(String station) {
  if (radioTool == nullptr) {
    ESP_LOGE(APP_TAG, "❌ RadioTool initialize edilmedi!");
    return;
  }
  
  // Stop komutu
  if (station == "stop" || station == "dur") {
    radioTool->stopRadio();
    return;
  }
  
  // Radyo API'sinden bilgi al
  RadioTool::RadioInfo info = radioTool->getRadioStream(station);
  
  if (info.status == "ok" && !info.streamURL.empty()) {
    // Stream başlat
    if (radioTool->startStream(info.streamURL)) {
      Serial.print("✅ Çalınıyor: ");
      Serial.println(info.radioName.c_str());
    } else {
      Serial.println("❌ Stream başlamadı!");
    }
  } else {
    Serial.println("❌ Radyo bulunamadı!");
    printAvailableCommands();
  }
}

// ==================== XIAOZHI AI ENTEGRASYONU ====================
// Xiaozhi'den gelen metni işlemek için:
// 
// RadioTool otomatik olarak "play_radio" tool'unu MCP'ye kaydetti.
// Xiaozhi tarafından şu şekilde çağırılabilir:
//
// User: "Kral radyosu aç"
// Xiaozhi → MCP → play_radio(station_name="kral") → RadioTool
// → Cloudflare API → Audio Codec → Ses çıkışı
//
// 🎤 Xiaozhi asistan entegrasyonunun detaylarını 
// malzemeyi kontrol et, çoğu zaman belge içindedir.

// ==================== API YANITI ÖRNEK ====================
/*
Cloudflare Worker API'den alınan yanıt:
{
  "status": "ok",
  "query": "kral",
  "radio": "Kral Pop",
  "stream": "https://dygedge.radyotvonline.net/kralpop/playlist.m3u8"
}

RadioTool bunu parse eder ve audio codec'e gönderir.
*/
