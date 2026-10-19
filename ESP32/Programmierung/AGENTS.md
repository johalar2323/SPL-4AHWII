# ESP32-Programmierung

- Der Repository-Stamm liegt zwei Ebenen höher. Die dortige `AGENTS.md` gilt auch hier, einschließlich der CREO-Prüfung vor Commit und Push.
- Lies zu Arbeitsbeginn `../../docs/PROJEKT.md` und vor Änderungen oder Tests `../../docs/TESTS.md`.
- Lies vor Arbeiten an Pins, ADC oder Verdrahtung `../../docs/PINBELEGUNG.md`. Verwende diese Markdown-Datei als Pinreferenz; ungeklärte Angaben bleiben offen.
- Bibliotheken werden über `lib_deps` in `platformio.ini` verwaltet. Lokale Projektklassen liegen unter `include/`, `src/` und `lib/`; heruntergeladene Pakete unter `.pio/` gehören nicht ins Git.
- Dokumentation und Testergebnisse gehören nach `../../docs/`. Ein Git-Push berechtigt nicht zum Firmware-Upload.
