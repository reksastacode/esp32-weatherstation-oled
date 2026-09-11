# 🌤️ ESP32 Weather Station with OLED & Animated Snow Effect

An ESP32-based desktop weather station that fetches real-time weather data from the OpenWeatherMap API and displays it on an SSD1306 OLED screen alongside a custom animated snowflake background effect.

![ESP32 Weather Station](https://img.shields.io/badge/Board-ESP32-blue)
![Display](https://img.shields.io/badge/Display-SSD1306%20OLED-brightgreen)
![API](https://img.shields.io/badge/API-OpenWeatherMap-orange)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

---

## ✨ Features

- **Real-Time Weather Updates:** Fetches temperature, humidity, and weather conditions using the OpenWeatherMap REST API.
- **Animated Snow Background:** Continuous graphics rendering loop generating non-blocking animated snowfall across the display.
- **Configurable Units:** Easy toggle between Metric (°C), Imperial (°F), or Standard (Kelvin) units.
- **Non-Blocking Fetch Timer:** Uses `millis()` for periodic data fetching without freezing the display animations.
- **Auto WiFi Reconnect Setup:** Configured for seamless network initialization with timeout handling.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 Development Board** | 1 | NodeMCU-32S, ESP32 WROOM, or equivalent |
| **0.96" I2C OLED Display** | 1 | SSD1306 driver (128x64 pixels resolution) |
| **Breadboard & Jumper Wires** | — | Standard Dupont wires for connections |
| **Micro-USB / USB-C Cable** | 1 | Power supply and programming connection |

---

## 🔌 Wiring & Pinout

Connect the SSD1306 OLED display to your ESP32 board via I2C pins:

| SSD1306 OLED Pin | ESP32 Pin | Note |
| :--- | :--- | :--- |
| **VCC** | `3V3` (or `5V`) | Power Input |
| **GND** | `GND` | Ground Connection |
| **SDA** | `GPIO 21` | I2C Data Line |
| **SCL** | `GPIO 22` | I2C Clock Line |

*Note: If your board uses different default I2C pins, update the `Wire.begin(SDA, SCL)` line in `setup()` accordingly.*

---

## 📦 Software Dependencies

To compile and upload this sketch, make sure you install the following libraries in your Arduino IDE:

1. **[ArduinoJson](https://arduinojson.org/)** (v6.x recommended)
2. **[Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)**
3. **[Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)**

### Installing via Arduino IDE Library Manager:
1. Go to **Tools** > **Manage Libraries...** (or `Ctrl+Shift+I` / `Cmd+Shift+I`).
2. Search for each library name above and click **Install**.

---

## 🔑 Getting an OpenWeatherMap API Key

1. Go to [OpenWeatherMap](https://openweathermap.org/) and sign up for a free account.
2. Navigate to your **API Keys** tab under your account dashboard.
3. Generate a new API key (Free plan allows up to 60 calls/minute, which is more than enough for this project).
4. Copy the key for configuration.

---

## 🚀 Setup & Configuration

1. **Clone or Download** this repository:
   ```bash
   git clone https://github.com/your-username/esp32-weather-station.git
   ```
2. Open the code in **Arduino IDE**.
3. Locate the configuration lines at the top of the sketch and fill in your details:

   ```cpp
   // WiFi Credentials
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";

   // Weather API Settings
   const char* apiKey = "YOUR_OPENWEATHERMAP_API_KEY";
   const float LAT = 37.7749;  // Replace with your Latitude
   const float LON = -122.4194; // Replace with your Longitude

   // Unit System: "metric" (°C), "imperial" (°F), or "standard" (K)
   const char* units = "metric";
   ```

4. Select your ESP32 board in **Tools > Board > ESP32 Arduino**.
5. Select the correct COM port in **Tools > Port**.
6. Click **Upload**.

---

## 💻 How It Works

- **Startup:** The ESP32 connects to WiFi and displays a connecting message on the screen.
- **Data Fetch:** Once connected, it sends an HTTP GET request to the OpenWeatherMap API to parse the current JSON payload.
- **Rendering:** It draws the temperature, humidity, and status text while rendering dynamic circular snowflake particles falling in the background.
- **Polling Interval:** Weather data automatically refreshes every **10 minutes** (`fetchInterval = 10 * 60 * 1000 ms`), while the snowfall animation continues to render smoothly in real time.

---