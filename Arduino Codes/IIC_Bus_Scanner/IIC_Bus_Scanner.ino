#include <Wire.h>

#define SDA_PIN 15
#define SCL_PIN 16

#define MUX_ADDRESS 0x70

#define SCAN_INTERVAL 2000

const char* deviceName(uint8_t addr) {
  switch (addr) {
    case 0x70: return "PCA9548A (Mux)";
    case 0x3C: return "SSD1306 OLED (Adresse 0x3C)";
    case 0x3D: return "SSD1306 OLED (Adresse 0x3D)";
    case 0x68: return "MPU6050 GY-521 (AD0=GND)";
    case 0x69: return "MPU6050 GY-521 (AD0=VCC)";
    default: return "unbekannt";
  }
}

bool muxSelect(uint8_t channel) {
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1 << channel);
  return Wire.endTransmission() == 0;
}

bool probeDevice(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

void printHeader() {
  Serial.println();
  Serial.println(F("############################################"));
  Serial.println(F("#        I2C Bus-Scanner PCA9548A          #"));
  Serial.println(F("#  prueft Mux, alle Kanaele und Adressen   #"));
  Serial.println(F("############################################"));
}

void scanChannel(uint8_t channel) {
  Serial.print(F("Kanal "));
  if (channel < 10) Serial.print(F(" "));
  Serial.print(channel);
  Serial.print(F("  |  "));

  if (!muxSelect(channel)) {
    Serial.println(F("Kanal nicht waehlbar (Mux-Fehler)"));
    return;
  }

  uint8_t found[127];
  uint8_t count = 0;

  for (uint8_t addr = 0x08; addr < 0x78; addr++) {
    if (probeDevice(addr)) {
      found[count++] = addr;
    }
  }

  if (count == 0) {
    Serial.println(F("keine Geraete"));
  } else {
    for (uint8_t i = 0; i < count; i++) {
      Serial.print(F("0x"));
      if (found[i] < 0x10) Serial.print(F("0"));
      Serial.print(found[i], HEX);
      Serial.print(F(" ("));
      Serial.print(deviceName(found[i]));
      Serial.print(F(")  "));
    }
    Serial.println();
  }
}

void scanAll() {
  Serial.println();
  Serial.println(F("Adressen-Scan gestartet..."));

  if (!probeDevice(MUX_ADDRESS)) {
    Serial.println(F("[FEHLER] PCA9548A (0x70) nicht erreichbar!"));
    Serial.println(F("[HINWEIS] Pruefen: Strom, GND, SDA=Pin21, SCL=Pin22,"));
    Serial.println(F("          Pull-Up-Widerstaende, A0-A2 Jumper = GND"));
    return;
  }

  Serial.println(F("[OK] PCA9548A erkannt unter 0x70"));
  Serial.println();

  for (uint8_t ch = 0; ch < 8; ch++) {
    scanChannel(ch);
  }

  Serial.println();
  Serial.println(F("Zusammenfassung der gefundenen Geraete:"));
  Serial.println(F("  Bus-Selbstadresse (immer): 0x70 -> PCA9548A Mux"));
  Serial.println(F("  Kanal 0 erwartet:          0x3C/0x3D -> SSD1306 OLED"));
  Serial.println(F("  Kanal 4 erwartet:          0x68/0x69 -> MPU6050 GY-521"));
  Serial.println(F("------------------------------------------------------"));
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Wire.begin(SDA_PIN, SCL_PIN);
  printHeader();
}

void loop() {
  scanAll();
  Serial.println();
  Serial.print(F("Naechster Scan in "));
  Serial.print(SCAN_INTERVAL / 1000);
  Serial.println(F(" s..."));
  delay(SCAN_INTERVAL);
}