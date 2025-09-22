# TTGO Nightscout Monitor

A simple, at-a-glance monitor for Nightscout glucose readings running on the LilyGO TTGO T-Display (ESP32, 1.14" LCD). It shows the latest glucose value, a trend arrow, and time since the last reading. Put it in a central place at home for quick checks without reaching for your phone.

> Important: This is a hobby project, not a medical device. Do not rely on it for treatment decisions.

---

## Why

I’m not a programmer—just a parent of a child with Type 1 Diabetes. I wanted a small display in the hallway that shows my son’s glucose levels at a glance. This project pulls data from Nightscout and renders it clearly on the TTGO T-Display.

---

## Features

- Large on-screen glucose value with ASCII trend arrows:
  - `^^` DoubleUp, `^` SingleUp, `/` FortyFiveUp, `->` Flat, `\` FortyFiveDown, `v` SingleDown, `vv` DoubleDown
- Time since last reading (in minutes)
- Local time display (configurable timezone, defaults to Europe/Warsaw)
- One-button brightness control (5 levels)
- Refresh every 60 seconds

---

## Hardware

- LilyGO TTGO T-Display ESP32 (ST7789 240×135 LCD, commonly called “1.14 inch”)
- USB-C cable (make sure it’s a data-capable cable)

Pins used by this sketch:
- Backlight (BL): GPIO 4 (PWM)
- Button: GPIO 35 (cycles brightness)

Power: USB-C is sufficient. Battery support is not covered in this repo.

---

## Getting Started

### 1) Arduino IDE + ESP32 core

- Install Arduino IDE:
  - https://www.arduino.cc/en/software

- Install the ESP32 core:
  - Arduino IDE → File → Preferences → “Additional Boards Manager URLs” add:
    ```
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    ```
  - Tools → Board → Boards Manager → Search “ESP32” → Install

- Recommended ESP32 core version: 2.0.17
  - There is a known compile/display issue with some versions; 2.0.17 is confirmed working.
  - Related discussion: https://github.com/Bodmer/TFT_eSPI/issues/3346

### 2) Required libraries

Install these from Sketch → Include Library → Manage Libraries:

- TFT_eSPI by Bodmer
- ArduinoJson by Benoit Blanchon
- ezTime by Rop Gonggrijp

### 3) Configure TFT_eSPI for TTGO T-Display

- Find `User_Setup_Select.h` inside your Arduino libraries folder. Example:
  - macOS: `~/Documents/Arduino/libraries/TFT_eSPI/User_Setup_Select.h`
  - Windows: `Documents\Arduino\libraries\TFT_eSPI\User_Setup_Select.h`

- Edit:
  - Comment out:
    ```cpp
    // #include <User_Setup.h>
    ```
  - Un-comment:
    ```cpp
    #include <User_Setups/Setup25_TTGO_T_Display.h>
    ```

Save the file.

### 4) Nightscout URL + token

You need your Nightscout endpoint plus a token with at least read access. Example:
```
https://YOUR-SITE.herokuapp.com/api/v1/entries.json?token=YOUR-TOKEN
```

This repo uses a `secrets.h` file (not committed) to keep credentials private.

- Copy `include/secrets.example.h` to `include/secrets.h`
- Fill in:
  - `WIFI_SSID`, `WIFI_PASS`
  - `NIGHTSCOUT_URL` (full URL including token)

### 5) Board and Port

- Tools → Board → “ESP32 Dev Module” (or a specific TTGO T-Display board if available in your core)
- Tools → Port → Select the correct COM/tty
- Recommended settings:
  - Upload Speed: 921600 (or 115200 if unstable)
  - Flash Frequency: 80 MHz
  - Flash Size: 4MB
  - Partition Scheme: Default or “Huge APP” (either works for this sketch)

### 6) Upload

- Open `src/main.ino`
- Click “Upload” in Arduino IDE
- The display should initialize and update every 60 seconds

---

## What You’ll See

- Large glucose value on the left, trend arrow on the right
- Bottom line: local time and “X min” since the last reading (capped at “20+min”)
- Press the Button (GPIO 35) to cycle brightness (5 levels)

---

## Configuration

- Timezone: Change `myTZ.setLocation("Europe/Warsaw")` to your TZ
  - Valid TZ names: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones

- Refresh interval: Adjust `delay(60000);` in `loop()` (milliseconds)

- ASCII trend arrows:
  - Mapped from Dexcom direction strings: `DoubleUp`, `SingleUp`, `FortyFiveUp`, `Flat`, `FortyFiveDown`, `SingleDown`, `DoubleDown`, `NotComputable`, `RateOutOfRange`

---

## Troubleshooting

- Board not detected:
  - Try a different USB-C cable (ensure it supports data)
  - Install CP210x / CH34x drivers (Windows/macOS)
- Display stays blank:
  - Confirm TFT_eSPI setup uses `Setup25_TTGO_T_Display.h`
  - Use ESP32 core 2.0.17
- No time / timezone wrong:
  - `ezTime` syncs via NTP after Wi‑Fi connects; ensure internet access
  - Set the correct timezone identifier
- Nightscout fetch fails:
  - Test your URL in a browser first
  - Ensure token permissions are correct
  - Verify your instance supports `/api/v1/entries.json`
- JSON parse issues:
  - Nightscout responses can vary; ensure the first element contains `sgv`, `direction`, and `mills`

---

## Security

- Do not commit `include/secrets.h`
- Use the provided `.gitignore`
- Treat Nightscout tokens like passwords

---

## Roadmap / Ideas

- Wi‑Fi Manager (captive portal) to configure networks without recompiling
- Dexcom Share login (where permitted; mind the terms of service)
- OTA updates
- Low battery indicator (if battery/charger board is used)
- Enhanced UI themes (colors, fonts, icons)

---

## Credits

- LilyGO TTGO T-Display: https://github.com/Xinyuan-LilyGO/TTGO-T-Display
- Nightscout: https://nightscout.github.io/
- TFT_eSPI: https://github.com/Bodmer/TFT_eSPI
- ezTime: https://github.com/ropg/ezTime
- ArduinoJson: https://arduinojson.org/

---

## Disclaimer

This project is provided “as is” with no warranties. It is not a medical device and should not be used as the basis for therapy decisions. Always follow your healthcare provider’s guidance.

---

## License

MIT — see LICENSE for details.
