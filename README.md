# TTGO-NightscoutMonitor

A simple proof of concept (POC) to monitor glucose levels in real-time using the TTGO T-Display ESP32 and the Nightscout platform.

---

## Introduction

First off, I’m not a programmer—I'm a father of a child with Type 1 Diabetes. I’m sharing this project for other parents who might be searching for a similar solution.

I built this small proof of concept (POC) to make it easier to check my son's glucose levels without constantly needing to look at my phone. The idea was simple: I wanted a device in a central spot in our home that always displays the glucose levels, so I could quickly glance at it when passing by.

This project uses the **TTGO T-Display ESP32** board to connect to Wi-Fi and fetch glucose level data from the **Nightscout** platform via API calls. Since this was my first experience with the ESP32, I’ll walk you through the setup process as I did it—not necessarily the most optimized way, but it worked for me!

---

## Hardware Requirements

- **TTGO T-Display ESP32** board (available on Aliexpress and other platforms)
- **USB-C or USB-A to USB-C** cable (ensure it's a data cable)

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (to program and upload code to the board)
- Required drivers for TTGO T-Display

---

## Setup Instructions

### Step 1: Install Required Drivers

Before connecting your TTGO board to the computer, install the necessary drivers. Refer to the [official TTGO repository](https://github.com/Xinyuan-LilyGO/TTGO-T-Display) for driver downloads. Currently, these drivers are required:

- **CHxxx Series** (for both macOS and Windows)
- **CP21xx Series** drivers

**Note:** I also installed the `CH340 Drivers` from [here](https://www.wch.cn/downloads/CH341SER_ZIP.html), which might have resolved some board recognition issues for me. This may not be necessary for everyone, but I’m mentioning it just in case.

### Step 2: Install Arduino IDE

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open the Arduino IDE and install the **ESP32 core**.

   To install the ESP32 core:
   - Go to **File > Preferences**.
   - In the **Additional Boards Manager URLs** field, add this URL:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```

3. Now, open **Boards Manager** by going to **Tools > Board > Boards Manager** and search for "ESP32".
4. Install the **ESP32** platform.

**Important Note:** There's a known compile error in the recent versions of the ESP32 core that can cause issues when using the display on the TTGO T-Display board. I recommend using version **2.0.17**, which worked for me. You can read more about this issue [here](https://github.com/Bodmer/TFT_eSPI/issues/3346).

### Step 3: Install the TFT_eSPI Library

1. To make the TTGO T-Display work with its screen, install the **TFT_eSPI** library:
   - Go to **Sketch > Include Library > Manage Libraries**.
   - Search for **TFT_eSPI** and click **Install**.

### Step 4: Configure the TFT_eSPI Library

After installing the library, we need to modify the **TFT_eSPI** library setup to work with the TTGO T-Display. Follow these steps:

1. Navigate to the file:  
   `/Documents/Arduino/libraries/TFT_eSPI/User_Setup_Select.h`.
   
2. Modify the file by:
   - Commenting out this line (add `//` at the start of the line):
     ```cpp
     // #include <User_Setup.h>  // Default setup is root library folder
     ```
   - Un-commenting this line:
     ```cpp
     #include <User_Setups/Setup25_TTGO_T_Display.h>  // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT
     ```

3. Save and close the file.

### Step 5: Select Board and Port in Arduino IDE

1. In Arduino IDE, go to **Tools > Board** and select **ESP32 Dev Module**.
2. Connect your TTGO T-Display board to your computer using a USB cable.
3. Go to **Tools > Port** and select the correct COM port (e.g., COM 4 on Windows).

**Troubleshooting Tip**: If your board isn’t recognized, try switching to a different USB-C cable (preferably USB-C to USB-C). I had issues with the USB-A to USB-C cable, but switching to a USB-C to USB-C cable solved the problem for me.

---

## Conclusion

Once everything is set up, you’re ready to upload the code and start pulling glucose data from Nightscout. This simple display should help you monitor glucose levels at a glance, like it did for me.

Feel free to fork this repository, modify the code, or share it with others. I hope it helps other parents who are looking for a similar solution!

---

## Credits

- [TTGO T-Display Repository](https://github.com/Xinyuan-LilyGO/TTGO-T-Display)
- [Nightscout Platform](https://nightscout.github.io/)

---

### Keywords:
**ESP32**, **TTGO T-Display**, **Nightscout**, **Glucose Monitor**, **Type 1 Diabetes**, **D1T**, **Real-time Glucose Monitoring**
