# 🚀 Kurulum Kılavuzu - ESP32 + Xiaozhi AI Radyo

## 📋 Gereksinimler

### Hardware
- ✅ **ESP32 Geliştirme Kartı** (Xiaozhi modülü ile uyumlu)
- ✅ **Xiaozhi AI Modülü** (I2S audio output)
- ✅ **WiFi Bağlantısı** (2.4GHz)
- ✅ **USB Kablo** (ESP32 programlama için)
- ✅ **Speaker/Amplifier** (ses çıkışı için)
- ✅ **Jumper Kablolar** (I2S bağlantı için)

### Software
- ✅ **Arduino IDE** (v2.0+)
- ✅ **ESP32 Board Package**
- ✅ **ArduinoJson** kütüphanesi
- ✅ **Xiaozhi AI** kütüphanesi

---

## 🔧 Arduino IDE Kurulumu

### 1️⃣ Arduino IDE İndir
- **Windows/Mac/Linux**: [arduino.cc/en/software](https://arduino.cc/en/software)
- En son versiyonu yükle (v2.0+)

### 2️⃣ ESP32 Board Support Ekle

1. **File → Preferences** aç
2. **Additional Boards Manager URLs** bölümüne ekle:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. **OK** butonuna sol
4. **Tools → Board → Board Manager**
5. `esp32` ara ve **by Espressif Systems** kur
6. Tamamlanmasını bekle (~2 dakika)

### 3️⃣ Gerekli Kütüphaneleri Yükle

**Tools → Manage Libraries** aç ve ara:

| Kütüphane | Versiyon | Yayıncı |
|-----------|----------|---------|
| `ArduinoJson` | 7.0.0+ | Benoit Blanchon |
| `Xiaozhi SDK` | (Modülün belgesi) | Xiaozhi |
| `WiFi` | Built-in | Arduino |
| `HTTPClient` | Built-in | Arduino |

> **Not**: Xiaozhi kütüphanesi modülünün belgelerinde bulunabilir

---

## 📝 ESP32 Konfigürasyonu

### 1️⃣ Board Seçimi

1. **Tools → Board → ESP32**
2. **ESP32 Dev Module** seç (veya senin kullandığın model)

### 2️⃣ Port Seçimi

1. USB kabloyu bilgisayara bağla
2. **Tools → Port**
3. `/dev/ttyUSB0` (Linux) veya `COM3` (Windows) gibi bulunacak
4. Seç

### 3️⃣ Upload Ayarları

```
Tools → 
├─ Board: ESP32 Dev Module
├─ Port: /dev/ttyUSB0
├─ Programmer: ESP32 S2 Serial Download Mode
├─ CPU Frequency: 240 MHz
├─ Flash Frequency: 80 MHz
├─ Flash Mode: QIO
└─ Flash Size: 4MB
```

---

## 💾 Dosyaları Kopyala

### Adım 1: Proje Klasörü Oluştur

```bash
mkdir ~/Arduino/sketches/RadioTool
cd ~/Arduino/sketches/RadioTool
```

### Adım 2: Dosyaları Kopyala

Bu repository'den:
- ✅ `radiotolll.h` → Kopyala
- ✅ `esp32_xiaozhi_example.ino` → Kopyala

Klasöre yapıştır:
```
~/Arduino/sketches/RadioTool/
├── radiotolll.h
└── esp32_xiaozhi_example.ino
```

### Adım 3: Arduino IDE'de Aç

1. **File → Open** → `esp32_xiaozhi_example.ino` seç
2. IDE otomatik olarak `radiotolll.h`'yi bulur

---

## ⚙️ Kodu Yapılandır

### 1️⃣ WiFi Bilgilerini Gir

`esp32_xiaozhi_example.ino` satır 10-11:

```cpp
const char* ssid = "YOUR_WIFI_SSID";        // ← WiFi adı
const char* password = "YOUR_WIFI_PASSWORD"; // ← WiFi şifresi
```

**Örnek:**
```cpp
const char* ssid = "HomeWiFi";
const char* password = "MySecurePassword123";
```

### 2️⃣ I2S Pin Ayarları (Xiaozhi için)

Satır 14-16:

```cpp
#define I2S_WS 22   // Word Select (Data clock)
#define I2S_SD 21   // Serial Data (Audio data)
#define I2S_SCLK 26 // Serial Clock (Bit clock)
```

> **Önemli**: Pin numaralarını Xiaozhi modülünün belgesinden kontrol et!

### 3️⃣ Cloudflare API URL

`radiotolll.h` satır 17:

```cpp
const char* API_URL = "https://round-breeze-8496.muhammetefecan45.workers.dev/?s=";
```

> Bu zaten ayarlanmış durumda - değiştirme!

---

## 🔌 Bağlantı Şeması

### I2S Audio Bağlantısı

```
ESP32 Pin        →  Xiaozhi Modülü
────────────────────────────────
  22 (LRCLK)    → I2S_WS (Word Select)
  21 (DOUT)     → I2S_SD (Serial Data)
  26 (BCLK)     → I2S_SCLK (Bit Clock)
  GND           → GND
  3.3V          → VCC (gerekirse)
```

### Speaker Bağlantısı

```
Xiaozhi Modülü
└─ I2S Output
   └─ Audio Amplifier
      └─ Speaker
         └─ Ses Çıkışı 🔊
```

---

## 📤 ESP32'ye Yükle

### 1️⃣ Kodu Derle

**Sketch → Verify** (Ctrl+R)

Hata var mı kontrol et. Varsa:
- Headers eksik mi?
- Kütüphaneler yüklü mü?
- Dil doğru mu?

### 2️⃣ ESP32'ye Upload Et

**Sketch → Upload** (Ctrl+U)

Terminal çıkışında görülecek:
```
Compiling sketch...
Uploading...
Writing at 0x00000000... (0 %)
...
Hard resetting via RTS pin...
✅ TAMAMLANDI!
```

### 3️⃣ Serial Monitor'ı Aç

**Tools → Serial Monitor** (Ctrl+Shift+M)

- Baud Rate seç: `115200`
- Gördüğün çıktı:

```
🚀 ESP32 + Xiaozhi AI Radyo Başlanıyor...
📶 WiFi'ya bağlanıyor: HomeWiFi
.....
✅ WiFi Bağlandı!
IP: 192.168.1.100
✅ Hazır! Radyo komutlarını kullanabilirsin.
```

---

## 🎯 Test Et

### Seri Terminal'de Test

Serial Monitor'a şunu yaz ve Enter'e bas:

```
kral
```

Beklenen çıktı:
```
📝 Seri input: kral
📡 API çağrısı: https://.../?s=kral
✅ API Yanıtı: {"status":"ok","radio":"Kral Pop",...}
🎙️ Radyo: Kral Pop
🔗 Stream URL: https://dygedge.radyotvonline.net/kralpop/playlist.m3u8
🎵 Streaming başlanıyor: https://...
✅ Streaming başarıyla başladı
✅ Çalınıyor: Kral Pop
```

Radyo sesini duymalısın! 🔊

### Diğer Komutlar

```
power    → Power FM
slow     → Slow Türk
metro    → Metro FM
joy      → Joy Türk
stop     → Radyoyu Durdur
```

---

## 🎤 Xiaozhi AI ile Ses Komutları

### Örnek Konuşmalar

```
👤 User: "Kral radyosu çal"
🤖 Xiaozhi: [Ses tanır] → MCP play_radio tool → ESP32
🎵 Radyo: Kral Pop çalmaya başlar

👤 User: "Radyoyu kapatıyor musun?"
🤖 Xiaozhi: [Ses tanır] → MCP play_radio stop → ESP32
⏹️ Radyo: Durdurulur
```

> **Not**: Xiaozhi entegrasyonunun tam detayları modülün belgesinde bulunur

---

## 🐛 Sorun Giderme

### ❌ "Compilation Error: 'RadioTool' was not declared"

**Çözüm:**
- `radiotolll.h` dosyası aynı klasörde mı?
- Satır: `#include "radiotolll.h"` doğru mu?

### ❌ "WiFi Bağlanamadı"

**Çözüm:**
- SSID ve password doğru mu?
- WiFi ağı 2.4GHz mi? (ESP32 5GHz'i desteklemez)
- Ağda ESP32 bağlanmasına izin var mı?
- Router'ı yeniden başlat

### ❌ "API Hata: http_error_404"

**Çözüm:**
- Internet bağlantınız var mı?
- `radiotolll.h` satır 17'deki URL doğru mu?
- Cloudflare Worker online mi?

### ❌ "Ses Çıkmıyor"

**Çözüm:**
- I2S pin bağlantılarını kontrol et
- Speaker bağlı mı?
- Amplifier çalışıyor mu?
- Serial Monitor'da hata var mı?

---

## 📊 DEBUG MOD

Daha fazla bilgi görmek için satır 8'de yapıştır:

```cpp
#define DEBUG_MODE 1
```

Bunu ekle `radiotolll.h`'ye:

```cpp
#ifdef DEBUG_MODE
  #define DEBUG_LOG(fmt, ...) ESP_LOGI(RADIO_TAG, fmt, ##__VA_ARGS__)
#else
  #define DEBUG_LOG(fmt, ...) 
#endif
```

---

## ✅ Kontrol Listesi

- [ ] Arduino IDE 2.0+ yüklü
- [ ] ESP32 Board Package yüklü
- [ ] ArduinoJson kütüphanesi yüklü
- [ ] WiFi bilgileri girildi
- [ ] I2S pin numaraları doğru (Xiaozhi belgesinden)
- [ ] Serial Monitor 115200 baud'a ayarlı
- [ ] Kod derlenip yüklendi
- [ ] "Hazır!" mesajı görülüyor
- [ ] Radyo komutu çalışıyor
- [ ] Ses duyuluyor

---

## 📞 Yardım

Hata aldıysan:
1. Serial Monitor çıktısını kopyala
2. `radiotolll.h` ve `esp32_xiaozhi_example.ino` kontrol et
3. README.md deki Troubleshooting bölümünü oku
4. Xiaozhi modülü belgesini kontrol et

---

**Hazırlayanlar**: Efecan32 + GitHub Copilot  
**Tarih**: May 2026  
**Status**: ✅ Tam Fonksiyonel
