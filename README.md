# ESP32-Kettenroboter

Ein modularer Kettenroboter als MINT-Lernprojekt: Schülerinnen und Schüler
sollen Aufbau, Sensorik und Programmierung eines mobilen Roboters kennenlernen.
Das Projekt verbindet ein Robotik-Konzept mit KiCad-Entwürfen und ersten
Arduino-Testprogrammen.

> **Projektstand:** Dieses Repository dokumentiert ein Konzept und begonnene
> Hardware-Entwicklung. Die beschriebenen Funktionen sind nicht automatisch
> als fertig aufgebaut, vollständig programmiert oder praktisch validiert zu
> verstehen. Insbesondere ist hier noch keine vollständige Fahr- und
> Autonomiesoftware enthalten.

## Konzept

Der geplante Roboter soll Hindernisse erkennen, Linien verfolgen, Abstände und
Fahrdaten erfassen und Daten per WLAN weitergeben. Das Konzept sieht vor, bis zu
20 möglichst gleiche Einheiten für den MINT-Unterricht aufzubauen.

Wesentliche Bausteine der beschriebenen Architektur:

- **Zwei Controller:** Ein ESP32-WROOM-32E übernimmt die übergeordnete Logik,
  Sensorik und WLAN-Kommunikation. Ein Arduino Pro Micro ist für zeitkritische
  Motorsteuerung, Encoder und Signaltöne vorgesehen.
- **Kettenantrieb:** Zwei N20-Getriebemotoren mit Encodern werden über einen
  Motortreiber angesteuert.
- **Sensorik:** Ein HC-SR04 für die Hinderniserkennung vorn, zwei VL53L0X für
  seitliche Abstandsmessung, ein TCS34725 für die Linienerkennung und eine
  LSM6DSOX-IMU für Beschleunigungs- und Drehratenmessung.
- **Erweiterbarer I²C-Bus:** Ein TCA/PCA9548A-Multiplexer soll Sensoren mit
  gleicher I²C-Adresse voneinander trennen. Ein zusätzlicher Anschluss am
  Gehäusedeckel ist für externe I²C-Module vorgesehen.
- **Rückmeldung und Aufzeichnung:** OLED, WS2812-LEDs und Piezo dienen der
  Ausgabe; eine microSD-Karte ist für Sensor- und Fahrdaten vorgesehen.
- **Energieversorgung:** Das Konzept beschreibt einen 1S-LiPo-Akku mit
  getrennten Boost-Versorgungen für Motoren und Logik.

Die detaillierte Beschreibung, Pin- und Busbelegungen, Stückliste sowie
Kostenschätzungen stehen in den Konzeptunterlagen. Werte daraus sind
Planungsangaben und müssen vor dem Aufbau anhand der verwendeten Bauteile
überprüft werden.

## Inhalt des Repositorys

| Pfad | Inhalt |
| --- | --- |
| [`Kettenroboter_Konzept_V1.0.html`](Kettenroboter_Konzept_V1.0.html) | Ausführliche Konzeptbeschreibung mit Architektur, Sensorik, Versorgung, Pinplan und Kalkulation |
| [`Kettenroboter_Konzept_V1.0.docx`](Kettenroboter_Konzept_V1.0.docx) | Bearbeitbare Fassung des Konzepts |
| [`Kettenroboter - Konzept V1.0.pdf`](Kettenroboter%20-%20Konzept%20V1.0.pdf) | PDF-Fassung des Konzepts |
| [`KICAD/Platine_V1.0/`](KICAD/Platine_V1.0/) | KiCad-Schaltplan- und Platinenprojekt V1.0 |
| [`Arduino Codes/`](Arduino%20Codes/) | Arduino-Sketche für einzelne Hardwaretests |
| [`Datenblätter/`](Datenbl%C3%A4tter/) | Datenblätter und ESP32-Maßzeichnung |
| [`Teilliste.xlsx`](Teilliste.xlsx) | Bauteil- und Bestellübersicht |
| [`Neue Notiz 09-22-2026.pdf`](Neue%20Notiz%2009-22-2026.pdf) | Zusätzliche Projektnotiz |

## Arduino-Testsketche

Die Sketche testen einzelne Baugruppen und sind kein zusammenhängendes
Roboterprogramm:

| Sketch | Zweck |
| --- | --- |
| [`IIC_Bus_Scanner.ino`](Arduino%20Codes/IIC_Bus_Scanner/IIC_Bus_Scanner.ino) | Sucht I²C-Geräte direkt und an den Kanälen eines Multiplexers |
| [`IIC_MUX_Test.ino`](Arduino%20Codes/IIC_MUX_Test/IIC_MUX_Test.ino) | Prüft Multiplexer, Buspegel und Kanalwahl |
| [`MUX_Test.ino`](Arduino%20Codes/MUX_Test/MUX_Test.ino) | Weiterer Multiplexer-Test |
| [`OLED_Direct_Test.ino`](Arduino%20Codes/OLED_Direct_Test/OLED_Direct_Test.ino) | Initialisiert ein OLED und zeigt Beispieltexte an |

Die Sketche verwenden derzeit SDA an GPIO 15 und SCL an GPIO 16. Im
Konzept-Pinplan ist der I²C-Hauptbus dagegen auf GPIO 21/22 vorgesehen.
Vor dem Anschließen oder Ausführen daher die tatsächliche Verdrahtung, den
jeweiligen Sketch und den passenden ESP32-Pinplan miteinander abgleichen.
Der OLED-Test verwendet die Bibliothek **Adafruit_SSD1306**.

## Sicherheit und Prüfung

Der Entwurf umfasst einen Lithium-Polymer-Akku, Ladeelektronik, DC/DC-Wandler
und Motoren. Vor einem Aufbau oder Ladebetrieb müssen insbesondere Akku,
Ladeschaltung, Schutzfunktionen, Leitungsquerschnitte und Strombelastbarkeit
für die tatsächlich ausgewählten Komponenten geprüft werden. Lithium-Akkus
nicht unbeaufsichtigt oder entgegen den Herstellerangaben laden.

ESP32-GPIOs sind nicht 5-V-tolerant. Für den im Konzept vorgesehenen HC-SR04
muss insbesondere das Echo-Signal auf einen zulässigen ESP32-Pegel angepasst
werden. Maßgeblich sind die Datenblätter der konkret eingesetzten Bauteile.

## Urheberschaft und Nutzung

Die eigenen Projektideen, konzeptionellen Entscheidungen und Ausarbeitungen
dieses Projekts stammen von **Florian (GitHub: [FloGitHild](https://github.com/FloGitHild))**.
Sie dürfen nur verwendet, bearbeitet oder weiterverbreitet werden, wenn
**Florian (FloGitHild) klar als Urheber genannt wird**. Eine Nutzung ohne diese
Namensnennung ist nicht gestattet.

Die Namensnennung ist keine pauschale Lizenz für Inhalte Dritter. Datenblätter,
Herstellerangaben, Produktnamen und sonstige übernommene Materialien
unterliegen weiterhin den Rechten und Bedingungen ihrer jeweiligen
Rechteinhaber. Für solche Inhalte gelten die jeweiligen Lizenz- und
Nutzungsbedingungen.
