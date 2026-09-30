#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 15
#define SCL_PIN 16

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(F("=== SSD1309 128x64 OLED Test ==="));

  Wire.begin(SDA_PIN, SCL_PIN);

  uint8_t oledAddr = 0x3C;
  Wire.beginTransmission(0x3C);
  bool found = (Wire.endTransmission() == 0);
  if (!found) {
    Wire.beginTransmission(0x3D);
    if (Wire.endTransmission() == 0) {
      oledAddr = 0x3D;
      found = true;
    }
  }

  if (!found) {
    Serial.println(F("[FEHLER] Kein SSD1309 gefunden (0x3C/0x3D)"));
    Serial.println(F("[HINWEIS] OLED GND->GND, VCC->3.3V, SDA->P15, SCL->P16"));
    return;
  }
  Serial.print(F("[OK] Display gefunden an 0x"));
  Serial.println(oledAddr, HEX);

  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddr)) {
    Serial.println(F("[FEHLER] Init fehlgeschlagen"));
    return;
  }
  Serial.println(F("[OK] Display initialisiert"));

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 10);
  display.println(F("SSD1309"));
  display.setCursor(20, 32);
  display.println(F("128x64"));
  display.display();
  delay(2000);
}

void loop() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.println(F("HALLO"));
  display.display();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.println(F("123,45"));
  display.display();
  delay(1000);
}