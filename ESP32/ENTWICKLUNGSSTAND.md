# Entwicklungsstand & Projekt-Historie: PIXELBOARD (SPL 4AHWII)

> **Repository:** `https://github.com/johalar2323/SPL-4AHWII.git`  
> **Lokaler Pfad:** `C:\PTC\Creo10_Work\SPL 2026 27\SPL-4AHWII`  
> **Projekt:** ESP32 Pixelboard mit 2x WS2812 (8x32) Panels, Joysticks & Sensoren  

---

## 1. Aufgabenverteilung im Entwickler-Team

| Teammitglied | Modul / Zuständigkeit | Aktueller Status |
| :--- | :--- | :--- |
| **Person 1 (Hüseyin / User)** | **Laufschrift für 1 Panel:** Integration von `FastLED`, `LEDMatrix`, `LEDText` (Font Matrise) inkl. laufendem Sekundenzähler & Joystick-Modusumschaltung. | **Fertiggestellt & kompiliert** (v1.1) |
| **Person 2 & Person 3** | **Matrixkoordinaten (x, y Mapping):** Umrechnung der Pixelkoordinaten für Panel 1 und Einbindung des um 180° gedrehten Panels 2. | In Bearbeitung |
| **Gesamtes Team** | Zeit (NTP), Wetter, Snake-Spiel, DHT22 mit Google-Sheets-Logging, Pogo-Pins. | Geplant nach Fertigstellung der Matrix-Basis |

---

## 2. Chronologischer Entwicklungsverlauf & Versionen

### [v1.0] – 12.10.2026: Eingabesysteme (Taster & Joystick)
- **Was wurde gemacht:**
  - Vorjahres-Klasse `EntprellterTaster` analysiert (20 ms Entprellung, Kurz-/Langdruck).
  - Klasse `Joystick` via OOP-Vererbung (`class Joystick : public EntprellterTaster`) erstellt.
  - ESP32 12-Bit-ADC ($0 \dots 4095$) angebunden, Nullpunkt-Totzone ($\pm 200$) implementiert, Skalierung auf $-100 \dots +100\%$ und Richtungs-Erkennung (`Mitte`, `Oben`, `Unten`, `Links`, `Rechts`).
  - Verbindliche Pinbelegung vom Professor eingepflegt (Wi-Fi-sichere ADC1-Pins 34/35 für J1 und 32/33 für J2).
  - PlatformIO-Entwicklungsumgebung aufgesetzt und CP2102-Treiberproblem (Code 28) behoben.
- **Hardware-Test (P-07):** Erfolgreich auf ESP32-D0WD-V3 geflasht und im Seriellen Monitor verifiziert.

### [v1.1] – 12.10.2026: Pixelboard Start – Panel 1 Laufschrift mit Sekundenzähler
- **Commit:** `766d1d7` – *feat(display): LauftextPanel mit Sekundenzaehler, Joystick-Steuerung und Entwicklungsstand-Doku hinzugefuegt*
- **Was wurde gemacht:**
  - Bibliotheken `FastLED` (v3.6.0 für ESP32-Stabilität), `LEDMatrix` und `LEDText` (Aaron Liddiment) installiert und gepatcht.
  - Klasse `LauftextPanel` (`LauftextPanel_SPL`) für $32 \times 8$ WS2812 Panel mit vertikaler Zick-Zack-Verdrahtung (`VERTICAL_ZIGZAG_MATRIX`) erstellt.
  - Dynamischer Sekundenzähler integriert: Scrollender Text zeigt Sekunden seit Start an (`4AHWII PIXELBOARD: <x>s`).
  - Helligkeitsdrosselung auf 20 (von 255) zum Schutz des USB-Ports vor Überstrom.
  - Joystick-Integration: Umschaltung der Textfarbe per langem Tastendruck, Rücksetzen des Sekundenzählers per Kurzdruck.
- **Build-Test (P-08):** Erfolgreich mit PlatformIO übersetzt (289 KB Flash, 23 KB RAM).
- **Gepusht auf:** `https://github.com/johalar2323/SPL-4AHWII.git` → `ESP32/LauftextPanel/`

---

## 3. Git-Workflow Anleitung (Team-Leitfaden)

Vor jedem Arbeiten zwingend den neuesten Stand vom Server holen:
```bash
# 1. Neuesten Stand vom GitHub-Repository abholen
git pull origin main

# 2. Lokalen Status prüfen (welche Dateien wurden geändert?)
git status

# 3. Dateien für den Commit vormerken (z. B. den neuen Sketch)
git add ESP32/LauftextPanel/
git add ENTWICKLUNGSSTAND.md

# 4. Änderungen mit einer aussagekräftigen Nachricht festschreiben
git commit -m "feat(display): LauftextPanel mit Sekundenzaehler und FastLED hinzugefuegt"

# 5. Auf GitHub hochladen
git push origin main
```

> **Wichtiger Hinweis zu CREO-Zeichnungen (gemäß AGENTS.md):**  
> Der Ordner `CREO-Zeichnungen/` muss vollständig flach bleiben (keine Unterordner). Bei Zeichnungen darf immer nur die jeweils höchste Dateiversion im Git-Index eingecheckt werden.
