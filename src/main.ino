#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <ezTime.h>

#include "secrets.h"  // Define WIFI_SSID, WIFI_PASS, NIGHTSCOUT_URL

// Pins and PWM setup for TTGO T-Display
const int backlightPin = 4;   // GPIO for backlight control (TTGO T-Display)
const int buttonPin    = 35;  // GPIO for button
const int pwmChannel   = 0;
const int pwmFrequency = 5000;
const int pwmResolution = 8;

// Brightness levels (0-255)
int backlightLevels[5] = {10, 30, 60, 120, 220};
byte brightnessIndex = 2;  // start at medium level
int buttonStateLatch = 0;

// Trend mapping
const char* DEXCOM_TREND_DIRECTIONS[] = {
  "None",
  "DoubleUp",
  "SingleUp",
  "FortyFiveUp",
  "Flat",
  "FortyFiveDown",
  "SingleDown",
  "DoubleDown",
  "NotComputable",
  "RateOutOfRange"
};

const char* DEXCOM_TREND_ARROWS[] = {
  "",
  "^^",
  "^",
  "/",
  "->",
  "\\",
  "v",
  "vv",
  "?",
  "-"
};

// Data
int currentGlucoseLevel = 0;
String trendDirection = "";
String directionString = "";
String timeAgo = "";

Timezone myTZ;   // local timezone
Timezone utcTZ;  // UTC for calculations
unsigned long lastUpdateTime = 0; // epoch seconds of last reading
unsigned long currentTimeUTC = 0;

TFT_eSPI tft = TFT_eSPI(135, 240);

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  int dot = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (++dot % 40 == 0) Serial.println();
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
}

void setupDisplay() {
  tft.init();
  tft.setRotation(3); // Landscape, USB on the left
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void setupTimezone() {
  waitForSync();               // NTP sync (ezTime)
  myTZ.setLocation("Europe/Warsaw");
  utcTZ.setLocation("UTC");
}

void getGlucoseData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  HTTPClient http;
  http.begin(NIGHTSCOUT_URL);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);

    // Adjust capacity if your Nightscout response adds fields
    StaticJsonDocument<2048> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err) {
      Serial.print("JSON error: ");
      Serial.println(err.c_str());
      http.end();
      return;
    }

    // Read the first (latest) record
    currentGlucoseLevel = doc[0]["sgv"] | 0;
    directionString = doc[0]["direction"] | "";

    // Map to ASCII arrow
    trendDirection = "";
    for (int i = 0; i < 10; i++) {
      if (directionString == DEXCOM_TREND_DIRECTIONS[i]) {
        trendDirection = DEXCOM_TREND_ARROWS[i];
        break;
      }
    }

    // Mills is usually epoch ms; convert to seconds
    String millsString = doc[0]["mills"] | "";
    if (millsString.length() >= 3) {
      millsString.remove(millsString.length() - 3); // strip ms
      lastUpdateTime = millsString.toInt();
    }

    currentTimeUTC = utcTZ.now();
    int minutesSince = 0;
    if (currentTimeUTC >= lastUpdateTime && lastUpdateTime > 0) {
      minutesSince = (currentTimeUTC - lastUpdateTime) / 60;
    }
    timeAgo = (minutesSince > 20) ? "20+min" : String(minutesSince) + " min";
  } else {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
  }

  http.end();
}

void displayData() {
  tft.fillScreen(TFT_BLACK);

  // Big glucose and arrow on top row
  tft.setTextSize(8);
  int screenWidth = tft.width();
  int glucoseWidth = tft.textWidth(String(currentGlucoseLevel));
  int arrowWidth = tft.textWidth(trendDirection);

  tft.setCursor(10, 10);
  tft.print(currentGlucoseLevel);

  tft.setCursor(screenWidth - arrowWidth - 10, 10);
  tft.print(trendDirection);

  // Bottom row: local time and timeAgo
  tft.setTextSize(3);
  tft.setCursor(10, 100);
  tft.print(myTZ.dateTime("H:i") + " " + timeAgo);
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);

  setupWiFi();
  setupDisplay();
  setupTimezone();

  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(backlightPin, pwmChannel);
  ledcWrite(pwmChannel, backlightLevels[brightnessIndex]);
}

void loop() {
  // Brightness button (simple debounce)
  if (digitalRead(buttonPin) == LOW) {
    delay(50);
    if (digitalRead(buttonPin) == LOW && buttonStateLatch == 0) {
      buttonStateLatch = 1;
      brightnessIndex = (brightnessIndex + 1) % 5;
      ledcWrite(pwmChannel, backlightLevels[brightnessIndex]);
      delay(300);
    }
  } else {
    buttonStateLatch = 0;
  }

  getGlucoseData();
  displayData();

  delay(60000); // Update every minute
}
