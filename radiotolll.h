#ifndef __RADIO_TOOL_H__
#define __RADIO_TOOL_H__

#include <string>
#include <map>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "mcp_server.h"
#include "board.h"
#include <esp_log.h>

#define RADIO_TAG "RadioTool"

class RadioTool {
private:
  const char* API_URL = "https://round-breeze-8496.muhammetefecan45.workers.dev/?s=";
  HTTPClient http;
  std::string currentRadio = "Süper FM";
  std::string currentStream = "";
  bool isPlaying = false;
  
public:
  struct RadioInfo {
    std::string status;
    std::string query;
    std::string radioName;
    std::string streamURL;
  };

  // Radyo bilgisini Cloudflare Worker API'den al
  RadioInfo getRadioStream(const std::string& stationQuery) {
    RadioInfo info;
    info.status = "error";
    info.radioName = "Süper FM";
    info.streamURL = "";
    info.query = stationQuery;

    std::string fullURL = std::string(API_URL) + stationQuery;
    
    ESP_LOGI(RADIO_TAG, "📡 API çağrısı: %s", fullURL.c_str());

    http.begin(fullURL.c_str());
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(5000);
    
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      ESP_LOGI(RADIO_TAG, "✅ API Yanıtı: %s", payload.c_str());
      
      // JSON parse
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        info.status = doc["status"].as<std::string>();
        info.radioName = doc["radio"].as<std::string>();
        info.streamURL = doc["stream"].as<std::string>();
        
        currentRadio = info.radioName;
        currentStream = info.streamURL;
        
        ESP_LOGI(RADIO_TAG, "🎙️ Radyo: %s", info.radioName.c_str());
        ESP_LOGI(RADIO_TAG, "🔗 Stream URL: %s", info.streamURL.c_str());
      } else {
        ESP_LOGE(RADIO_TAG, "❌ JSON Parse Hatası: %s", error.f_str());
        info.status = "json_error";
      }
    } else {
      ESP_LOGE(RADIO_TAG, "❌ HTTP Hatası: %d", httpCode);
      info.status = std::string("http_error_") + std::to_string(httpCode);
    }
    
    http.end();
    return info;
  }

  // Stream'i başlat ve audio codec'e gönder
  bool startStream(const std::string& streamURL) {
    if (streamURL.empty()) {
      ESP_LOGE(RADIO_TAG, "❌ Stream URL boş!");
      return false;
    }

    try {
      ESP_LOGI(RADIO_TAG, "🎵 Streaming başlanıyor: %s", streamURL.c_str());
      
      // 🔥 DOĞRU YAKLASIM: Audio codec'i stream URL'siyle başlat
      auto audio_codec = Board::GetInstance().GetAudioCodec();
      if (!audio_codec) {
        ESP_LOGE(RADIO_TAG, "❌ Audio codec kullanılamaz!");
        return false;
      }
      
      // Stream başlat
      audio_codec->StartStreaming(streamURL);
      isPlaying = true;
      
      ESP_LOGI(RADIO_TAG, "✅ Streaming başarıyla başladı");
      return true;
    } catch (const std::exception& e) {
      ESP_LOGE(RADIO_TAG, "❌ Exception: %s", e.what());
      return false;
    }
  }

  // Radyo durdur
  void stopRadio() {
    try {
      auto audio_codec = Board::GetInstance().GetAudioCodec();
      if (audio_codec) {
        audio_codec->StopStreaming();
        isPlaying = false;
        ESP_LOGI(RADIO_TAG, "⏹️  Radyo durduruldu");
      }
    } catch (const std::exception& e) {
      ESP_LOGE(RADIO_TAG, "❌ Stop Exception: %s", e.what());
    }
  }

  // MCP Tool kaydı
  RadioTool() {
    auto& mcp_server = McpServer::GetInstance();

    mcp_server.AddTool("play_radio",
        "Radyo istasyonu çal. Desteklenen: kral, power, slow, metro, joy, fenomen, super, trt | Durdur: stop/dur",
        PropertyList({
            Property("station_name", kPropertyTypeString)
        }),
        [this](const PropertyList& p) -> ReturnValue {
            std::string station = p["station_name"].value<std::string>();
            
            // Durdur komutu
            if (station == "stop" || station == "dur") {
                stopRadio();
                return "Radyo durduruldu.";
            }
            
            // API'den radyo bilgisi al
            RadioInfo info = getRadioStream(station);
            
            if (info.status == "ok" && !info.streamURL.empty()) {
                // Stream'i başlat
                if (startStream(info.streamURL)) {
                    return "Tamam, " + info.radioName + " radyosu başlatılıyor.";
                } else {
                    return "Radyo başlamadı, lütfen tekrar deneyin.";
                }
            }
            
            return "Bu radyoyu listemde bulamadım. Desteklenen: kral, power, slow, metro, joy, fenomen, super, trt";
        });
  }

  // Mevcut durumu al
  std::string getStatus() {
    if (isPlaying) {
      return "Çalıyor: " + currentRadio;
    }
    return "Durmuş";
  }
};

#endif // __RADIO_TOOL_H__
