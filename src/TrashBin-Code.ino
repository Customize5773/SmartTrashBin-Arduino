#include <WiFi.h>
#include <ESP32Servo.h>
#include <NewPing.h>
#include <Adafruit_SSD1306.h>
#include <ESP_Mail_Client.h>

// Pin definitions
#define EXTERNAL_TRIGGER_PIN 5
#define EXTERNAL_ECHO_PIN 18
#define INTERNAL_TRIGGER_PIN 19
#define INTERNAL_ECHO_PIN 21
#define SERVO_PIN 13
#define OLED_SDA 4
#define OLED_SCL 15

// Constants
#define PROXIMITY_THRESHOLD_CM 30
#define BIN_HEIGHT_CM 40       // Measure this for your specific bin
#define FILL_THRESHOLD_PERCENT 80
#define LID_OPEN_TIME 5000     // 5 seconds

// WiFi and Email Settings
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "your_email@gmail.com"
#define AUTHOR_PASSWORD "your_app_password"
#define RECIPIENT_EMAIL "recipient@email.com"

// Global objects
Servo lidServo;
NewPing externalSensor(EXTERNAL_TRIGGER_PIN, EXTERNAL_ECHO_PIN, 200);
NewPing internalSensor(INTERNAL_TRIGGER_PIN, INTERNAL_ECHO_PIN, BIN_HEIGHT_CM);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
SMTPSession smtp;

// Global variables
bool emailSent = false;
unsigned long lidOpenTime = 0;
bool lidOpen = false;

void setupDisplay() {
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display allocation failed");
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Smart Trash Bin");
  display.display();
  delay(2000);
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void smtpCallback(SMTP_Status status) {
  if (status.success()) {
    Serial.println("Email sent successfully");
  } else {
    Serial.printf("Error code: %d, Reason: %s\n", status.code(), status.description().c_str());
  }
}

void sendEmailAlert(float fillPercentage) {
  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "Smart Trash Bin";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Trash Bin Alert!";
  message.addRecipient("Maintainer", RECIPIENT_EMAIL);
  
  String textContent = "The trash bin is ";
  textContent += String(fillPercentage);
  textContent += "% full. Please empty it soon!";
  message.text.content = textContent.c_str();

  if (!smtp.connect(&config)) {
    Serial.println("SMTP connection failed");
    return;
  }

  if (!smtp.isLoggedIn()) {
    Serial.println("SMTP login failed");
    return;
  }

  smtp.setSystemTime(time(nullptr), 0);
  smtp.setCallback(smtpCallback);

  if (!smtp.send(&message)) {
    Serial.println("Error sending email");
  }
}

void setup() {
  Serial.begin(115200);
  setupDisplay();
  connectWiFi();
  
  lidServo.attach(SERVO_PIN);
  lidServo.write(0);  // Close lid initially
  
  ESP_Mail_Client::initializeSSL();
}

void loop() {
  // Handle lid operation
  unsigned int externalDistance = externalSensor.ping_cm();
  if (externalDistance > 0 && externalDistance < PROXIMITY_THRESHOLD_CM && !lidOpen) {
    lidServo.write(90);  // Open lid
    lidOpen = true;
    lidOpenTime = millis();
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Lid Open");
    display.display();
  }

  if (lidOpen && (millis() - lidOpenTime >= LID_OPEN_TIME)) {
    lidServo.write(0);  // Close lid
    lidOpen = false;
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Lid Closed");
    display.display();
  }

  // Handle fill level monitoring
  unsigned int internalDistance = internalSensor.ping_cm();
  if (internalDistance == 0) return;  // Ignore invalid readings
  
  float fillPercentage = ((BIN_HEIGHT_CM - internalDistance) / (float)BIN_HEIGHT_CM) * 100;
  fillPercentage = constrain(fillPercentage, 0, 100);

  // Update display
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Fill Level: ");
  display.print(fillPercentage, 1);
  display.print("%");
  
  if (fillPercentage >= FILL_THRESHOLD_PERCENT) {
    display.print("\nALERT! Bin full");
    if (!emailSent) {
      sendEmailAlert(fillPercentage);
      emailSent = true;
    }
  } else {
    emailSent = false;
  }
  display.display();

  delay(1000);  // Adjust delay as needed
}
