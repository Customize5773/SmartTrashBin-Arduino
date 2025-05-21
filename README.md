# Smart Trash Bin System with IoT Integration

![Project Demo](demo.gif) *Replace with actual demo media*

An Arduino-based smart trash bin system with automatic lid operation, fill-level monitoring, and IoT capabilities using ESP32.

This project automates waste disposal and monitoring by combining sensor technology and IoT connectivity. The system detects user proximity to open and close the lid automatically using a servo motor, ensuring hands-free operation for improved hygiene. Dual ultrasonic sensors measure both external proximity and internal bin fill levels, providing accurate and real-time monitoring.

The ESP32 microcontroller enables WiFi connectivity, allowing the system to send instant email notifications when the bin reaches a predefined fill threshold. An OLED display presents live status updates, including fill percentage and connection status, directly on the device.

With modular, configurable code, the system can be adapted for various bin sizes and environments. It’s suitable for smart homes, offices, or public spaces seeking automated, contactless waste management and remote monitoring.

## Features
- 👥 Proximity detection using HC-SR04 ultrasonic sensor
- 🚪 Servo-controlled automatic lid opening/closing
- 📊 Real-time fill level monitoring
- 📺 OLED display for system status
- 📧 Email alerts via Gmail integration
- 📶 WiFi connectivity using ESP32

## Hardware Components
| Component               | Quantity |
|-------------------------|----------|
| ESP32 Development Board | 1        |
| HC-SR04 Ultrasonic Sensor | 2       |
| SG90 Micro Servo Motor  | 1        |
| 0.96" OLED Display      | 1        |
| 330Ω Resistor           | 2        |
| Jumper Wires            | -        |
| 5V Power Supply         | 1        |

## Circuit Diagram
_Coming Soon_

**Pin Connections:**
| Component        | ESP32 GPIO |
|------------------|------------|
| External Trig    | GPIO5      |
| External Echo    | GPIO18     |
| Internal Trig    | GPIO19     |
| Internal Echo    | GPIO21     |
| Servo Control    | GPIO13     |
| OLED SDA         | GPIO4      |
| OLED SCL         | GPIO15     |

## Installation
1. Clone repository:
   ```bash
   git clone https://github.com/Customize5773/SmartTrashBin-Arduino.git
   ```
2. Install [Arduino IDE](https://www.arduino.cc/en/software) (1.8.x or newer)
3. Add ESP32 board support:
   - Add URL to Preferences: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Install ESP32 package via Boards Manager
4. Install required libraries:
   - ESP32Servo
   - NewPing
   - Adafruit SSD1306
   - ESP Mail Client

## Configuration
1. Update WiFi credentials in `TrashBin-Code.ino`:
   ```cpp
   #define WIFI_SSID "your_wifi_ssid"
   #define WIFI_PASSWORD "your_wifi_password"
   ```
2. Configure email settings:
   ```cpp
   #define AUTHOR_EMAIL "your_email@gmail.com"
   #define AUTHOR_PASSWORD "your_app_password"  // Use Gmail App Password
   #define RECIPIENT_EMAIL "recipient@email.com"
   ```
3. Adjust system parameters as needed:
   ```cpp
   #define BIN_HEIGHT_CM 40        // Actual bin height
   #define FILL_THRESHOLD_PERCENT 80
   #define PROXIMITY_THRESHOLD_CM 30
   ```

## Calibration
1. Measure actual bin height from internal sensor to bottom
2. Update `BIN_HEIGHT_CM` with measured value
3. Test proximity detection range with `PROXIMITY_THRESHOLD_CM`
4. Generate Gmail App Password if using 2FA:
   - Visit: https://myaccount.google.com/apppasswords

## Usage
1. Power on the system
2. Approaching within 30cm triggers lid opening
3. OLED displays real-time fill percentage
4. Email alert sent when fill level ≥80%
5. Lid automatically closes after 5 seconds

## Troubleshooting
| Issue                  | Solution                          |
|------------------------|-----------------------------------|
| WiFi connection failed | Verify credentials, check router |
| Email not sending      | Use App Password, check SMTP      |
| Sensor inaccuracies    | Calibrate bin height, clean sensors |
| Servo jitter           | Ensure stable power supply        |

## Contributing
1. Fork the repository
2. Create feature branch: `git checkout -b feature/new-feature`
3. Commit changes: `git commit -am 'Add new feature'`
4. Push to branch: `git push origin feature/new-feature`
5. Submit pull request

## License
[MIT License](LICENSE)

## Acknowledgements
- NewPing library by Tim Eckel
- Adafruit for SSD1306 library
- ESP Mail Client library
