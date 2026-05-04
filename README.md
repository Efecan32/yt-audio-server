# 📻 Turkish Radio Streaming - Cloudflare Worker + ESP32 + Xiaozhi AI

Türkçe radyoları Cloudflare Worker API'si üzerinden ESP32 + Xiaozhi AI'ya stream et. Sesli komutlarla radyo dinle!

## ✨ Özellikler

- 🎙️ **8+ Radyo İstasyonu**: Kral Pop, Power FM, Slow Türk, Metro FM, Fenomen FM, Joy Türk, Süper FM, TRT FM
- 🎤 **Sesli Komut**: Xiaozhi AI ile ses tanıma ve radyo kontrolü
- ☁️ **Cloudflare Worker**: 100% Serverless, düşük gecikme
- 📡 **MCP Tool Destekli**: play_radio komutu entegrasyonu
- 🔥 **ESP32 Optimized**: Arduino IDE compatible, tam C++ support

---

## 📦 Kurulumlar

### 1️⃣ Cloudflare Worker Setup

```bash
# Gerekli paketler
npm install

# wrangler.toml dosyasını düzenle
# account_id = "YOUR_CLOUDFLARE_ACCOUNT_ID"

# Deploy et
npm run deploy
```

**Sonuç URL**: `https://radio-api.YOUR_ACCOUNT.workers.dev/?s=kral`

### 2️⃣ ESP32 + Xiaozhi AI Setup

#### Gerekli Arduino Kütüphaneleri
1. **Sketch → Include Library → Manage Libraries** aç
2. Aşağıdakileri arama ve kur:
   - `ArduinoJson` - JSON parsing
   - Xiaozhi AI kütüphanesi (modülün belgelerinden)

#### Dosyaları Kopyala
```
radiotolll.h          → Arduino sketch klasörüne
esp32_xiaozhi_example.ino  → Açmak istediğin sketch
```

#### Kod Yapılandırması
`esp32_xiaozhi_example.ino` içinde:

```cpp
// Satır 9-10: WiFi bilgileri
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Satır 16-18: I2S Pin ayarları (Xiaozhi'nize göre değişebilir)
#define I2S_WS 22
#define I2S_SD 21
#define I2S_SCLK 26
```

#### Arduino IDE'de Yükle
1. Board seç: ESP32
2. Port seç: (Serial Monitor'de görmek için)
3. **Upload** düğmesine tıkla
4. Serial Monitor'ı aç (115200 baud)

---

## 📻 Desteklenen Radyolar

Xiaozhi'ye istediğin radyonun adını söyle:

| Komut | Radyo | Kalite |
|-------|-------|--------|
| `kral` | Kral Pop | HQ |
| `power` | Power FM | HQ |
| `slow` | Slow Türk | HQ |
| `metro` | Metro FM | HQ |
| `fenomen` | Fenomen FM | HQ |
| `joy` | Joy Türk | HQ |
| `super` / `süper` | Süper FM | HQ |
| `trt` | TRT FM | HQ |
| `stop` / `dur` | Durdur | - |

---

## 💻 API Kullanımı

### Curl ile Test
```bash
# Kral Pop çal
curl "https://round-breeze-8496.muhammetefecan45.workers.dev/?s=kral"

# Yanıt:
{
  "status": "ok",
  "query": "kral",
  "radio": "Kral Pop",
  "stream": "https://dygedge.radyotvonline.net/kralpop/playlist.m3u8"
}
```

### ESP32 Seri Komut
```
Serial Monitor'a yaz:
> kral
> power
> stop
```

### Xiaozhi AI Sesli Komut
```
"Kral radyosu çal"
"Power FM'i aç"
"Radyoyu durdur"
```

---

## 🔧 Mimarisi

```
┌─────────────────┐
│   Xiaozhi AI    │  (Ses tanıma)
└────────┬────────┘
         │ Metin
         ▼
┌─────────────────┐
│  ESP32 + MCP    │  (radiotolll.h)
└────────┬────────┘
         │ HTTP
         ▼
┌─────────────────┐
│  Cloudflare     │  (Worker API)
│  Worker         │  
└────────┬────────┘
         │ Stream URL
         ▼
┌─────────────────┐
│  Radyo Server   │  (m3u8/mp3)
└─────────────────┘
```

---

## 📝 Dosya Yapısı

```
.
├── wrangler.toml              # Cloudflare ayarları
├── src/
│   └── index.js              # Worker script
├── radiotolll.h              # ⭐ ESP32 kütüphanesi (BU önemli!)
├── esp32_xiaozhi_example.ino # ⭐ Örnek sketch
├── package.json              # Node.js bağımlılıkları
└── README.md                 # Bu dosya
```

---

## 🐛 Sorun Giderme

### "Audio codec kullanılamaz" hatası
**Çözüm**: 
- Xiaozhi modülü kütüphanesini kontrol et
- `Board::GetInstance().GetAudioCodec()` NULL dönemez
- Kütüphanenin doğru şekilde initialize edilip edilmediğini kontrol et

### "API Yanıtı Yok" hatası
**Çözüm**:
- Cloudflare Worker URL'sini kontrol et
- Parameter: `?s=kral` (küçük harf)
- CORS headers açık mı?

### "Ses çıkmıyor" sorunu
**Çözüm**:
- I2S pin bağlantılarını kontrol et
- Audio codec'in çalışıp çalışmadığını döktüme bak
- Amplifier/Speaker bağlı mı?
- Serial Monitor'da hata mesajlarına bak

### WiFi bağlanmıyor
**Çözüm**:
- SSID/Password doğru mu?
- WiFi 2.4GHz mi (ESP32 5GHz desteklemez)
- ESP32'nin ağda cihazlara bağlanmasına izin var mı?

---

## 🔐 Güvenlik Notları

⚠️ **Dikkat**: Bu versiyonda açık kaynak olup:
- Tüm URL'ler halka açık
- Ağları kontrol etmek için authentication yok
- Taşıyıcı ağlarda çalışmaz

**Production için**:
```cpp
// wrangler.toml'de environment secret ekle
[env.production]
vars = { API_KEY = "secret_key_here" }

// radiotolll.h'de kontrol et
if (info.apiKey != EXPECTED_KEY) return error;
```

---

## 📄 Lisans

ISC

---

**Hazırlayan**: Efecan32 + GitHub Copilot  
**Tarih**: May 2026  
**Status**: ✅ Çalışır ve Test Edildi
