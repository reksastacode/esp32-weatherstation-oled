#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "UR_SSID";
const char* password = "UR_PASSWORD";

const char* apiKey = "UR_API_KEY";
const float LAT = ; // change to what latitude you want to use
const float LON = ; // change to what longitude you want to use
const char* units = "metric"; // "metric" for Celsius, "imperial" for Fahrenheit, or "standard" for Kelvin

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long lastfetch = 0;
const unsigned long fetchInterval = 10UL * 60UL * 1000UL; 

#define MAX_SNOW 12
struct Snowflake {
  float x;
  float y;
  float speed;
  int radius;
};
Snowflake flakes[MAX_SNOW];

struct WeatherData {
  String city;
  float temp;
  float humidity;
  String Status;
  bool valid = false;
};

WeatherData weather;

void initSnow() {
  for (int i = 0; i < MAX_SNOW; i++) {
    flakes[i].x = random(0, SCREEN_WIDTH);
    flakes[i].y = random(-SCREEN_HEIGHT, 0); 
    flakes[i].speed = random(5, 15) / 10.0;  
    flakes[i].radius = random(1, 3);         
  }
}

void updateSnow() {
  for (int i = 0; i < MAX_SNOW; i++) {
    flakes[i].y += flakes[i].speed;
    
    flakes[i].x += (random(-1, 2) * 0.2);

    if (flakes[i].y > SCREEN_HEIGHT) {
      flakes[i].y = random(-10, 0);
      flakes[i].x = random(0, SCREEN_WIDTH);
    }
    
    display.fillCircle((int)flakes[i].x, (int)flakes[i].y, flakes[i].radius, SSD1306_WHITE);
  }
}

void connectWifi(){
  WiFi.begin(ssid, password);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi...");
  display.display();

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    attempts++;
  }
}

bool fetchWeather(){
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + String(LAT, 6) + "&lon=" + String(LON, 6) + 
              "&units=" + String(units)+ "&appid=" + String(apiKey);

  http.begin(url);
  int httpCode = http.GET();

  if(httpCode != 200) {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) return false;

  weather.city = "PLACEHOLDER"; 
  weather.temp = doc["main"]["temp"].as<float>();
  weather.humidity = doc["main"]["humidity"].as<int>();
  weather.Status = doc["weather"][0]["description"].as<String>();
  weather.valid = true;

  return true;
}

void showWeather(){
  display.clearDisplay();

  updateSnow();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (!weather.valid) {
    display.setCursor(0, 0);
    display.println("Fetching weather data...");
    display.display();
    return;
  } 

  display.setCursor(0, 0);
  display.println(weather.city);

  display.setTextSize(2);
  display.setCursor(0, 16);
  display.print(weather.temp, 1);
  display.print(" C");

  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("Humidity: ");
  display.print(weather.humidity);
  display.print("%");

  display.setCursor(0, 52);
  display.print("Status: ");
  display.print(weather.Status);

  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(true) delay(1000);
  }

  initSnow(); 

  display.clearDisplay();
  display.display();

  connectWifi();

  if(fetchWeather()) {
    lastfetch = millis();
  }
}

void loop() {
  if (millis() - lastfetch >= fetchInterval) {
    if(fetchWeather()) {
      lastfetch = millis();
    }
  }

  showWeather();
  delay(30); 
}