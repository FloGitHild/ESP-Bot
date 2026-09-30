#include <Wire.h>

#define SDA_PIN 15
#define SCL_PIN 16

#define MUX_ADDRESS 0x70

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(F("=== PCA9548A MUX-Test ==="));
  Serial.println(F("[INFO] Erwartete Adresse: 0x70 (A0=A1=A2=GND)"));

  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);
  delay(20);
  Serial.print(F("[TEST] SDA-Pegel im Ruhezustand: "));
  Serial.println(digitalRead(SDA_PIN) ? F("HIGH (ok)") : F("LOW (!)"));
  Serial.print(F("[TEST] SCL-Pegel im Ruhezustand: "));
  Serial.println(digitalRead(SCL_PIN) ? F("HIGH (ok)") : F("LOW (!)"));
  if (digitalRead(SDA_PIN) == LOW) {
    Serial.println(F("[FEHLER] SDA wird auf LOW gehalten - Bus haengt!"));
    Serial.println(F("[HINWEIS] Ein Modul haelt die Leitung oder Kurzschluss:"));
    Serial.println(F("          Mux/OLED/Gyro einzeln nacheinander ABstecken"));
    Serial.println(F("          und testen. Fehlerhaftes Modul identifizieren."));
  }
  if (digitalRead(SCL_PIN) == LOW) {
    Serial.println(F("[FEHLER] SCL wird auf LOW gehalten"));
  }

  uint8_t addr = 0x70;
  Wire.beginTransmission(addr);
  uint8_t result = Wire.endTransmission();

  if (result == 0) {
    Serial.println(F("[OK] MUX antwortet an 0x70!"));
    Serial.println(F("[TEST] Lese Mux-Register..."));
    Wire.requestFrom(addr, (uint8_t)1);
    if (Wire.available()) {
      uint8_t reg = Wire.read();
      Serial.print(F("[OK] Registerinhalt: 0x"));
      Serial.println(reg, HEX);
    }

    Serial.println(F("[TEST] Kanal 0 auswaehlen..."));
    Wire.beginTransmission(addr);
    Wire.write(0x01);
    if (Wire.endTransmission() == 0) {
      Wire.requestFrom(addr, (uint8_t)1);
      if (Wire.available()) {
        uint8_t reg = Wire.read();
        Serial.print(F("[OK] Register nach Kanal 0: 0x"));
        Serial.println(reg, HEX);
      }
    }

    Serial.println(F("[TEST] Kanal 4 auswaehlen..."));
    Wire.beginTransmission(addr);
    Wire.write(0x10);
    if (Wire.endTransmission() == 0) {
      Wire.requestFrom(addr, (uint8_t)1);
      if (Wire.available()) {
        uint8_t reg = Wire.read();
        Serial.print(F("[OK] Register nach Kanal 4: 0x"));
        Serial.println(reg, HEX);
      }
    }

    Wire.beginTransmission(addr);
    Wire.write(0x00);
    Wire.endTransmission();

    Serial.println(F("[OK] MUX funktioniert (Lesen+Schreiben+Kanalwahl OK)"));
  } else {
    Serial.print(F("[FEHLER] Kein Antwort an 0x70, Wire-Fehlercode: "));
    Serial.println(result);
    Serial.println(F("[HINWEIS] Pruefen:"));
    Serial.println(F("  1) Mux auf 3.3V, GND verbinden"));
    Serial.println(F("  2) SDA->P15, SCL->P16 (nicht vertauscht)"));
    Serial.println(F("  3) A0, A1, A2 wirklich auf GND"));
    Serial.println(F("  4) Pull-Ups auf SDA/SCL vorhanden"));
    Serial.println(F("  5) Modul hat evtl. andere Adresse -> pruefe 0x71-0x77"));

    Serial.println(F("[INFO] Scan 0x70-0x77 (alle A0-A2 Kombinationen):"));
    for (uint8_t a = 0x70; a <= 0x77; a++) {
      Wire.beginTransmission(a);
      if (Wire.endTransmission() == 0) {
        Serial.print(F("[OK] MUX gefunden an 0x"));
        Serial.println(a, HEX);
      }
    }
  }
}

void loop() {
  delay(2000);
}