# Projektregeln

## Arbeitsbeginn und Projektkontext

- Lies zu Arbeitsbeginn `docs/PROJEKT.md` für Auftrag, Architektur und offene Entscheidungen.
- Lies vor Codeänderungen und Tests `docs/TESTS.md`. Prüfe vorhandenen Code und Konfigurationen, bevor du Änderungen vornimmst.
- Kommuniziere auf Deutsch. Unterscheide bestätigte Fakten, Annahmen und Empfehlungen; kläre wesentliche Unklarheiten mit dem Nutzer.

## Code, Hardware und Tests

- Bewahre bestehende Funktionalität und halte Änderungen auf den beauftragten Umfang begrenzt. Bevorzuge einfache, wartbare Lösungen ohne unnötige Abhängigkeiten.
- Prüfe vor Änderungen an Pins, ADC, Verdrahtung oder Board die vorhandenen Projektangaben. Fehlende Hardwaredaten bleiben offen und werden mit dem Nutzer geklärt.
- Lies dazu `docs/PINBELEGUNG.md`, die Markdown-Referenz der bisherigen Excel-Pinbelegung.
- Prüfe vor Änderungen an Build-, Upload- oder IDE-Einstellungen die vorhandene `platformio.ini` und die Build-Anleitung in `docs/PROJEKT.md`.
- Führe nach Codeänderungen die relevanten verfügbaren Tests aus. Protokolliere tatsächlich ausgeführte Tests mit Ergebnis und Einschränkungen in `docs/TESTS.md`; unterscheide Build-Erfolg und Hardwareprüfung.
- Ein Auftrag zum Git-Push ist kein Auftrag zum Flashen von Hardware. Firmware-Uploads erfolgen nur bei entsprechendem Nutzerauftrag.
- Bewahre Zugangsdaten und API-Schlüssel außerhalb von Quellcode und Markdown-Dateien auf. Destruktive Aktionen benötigen eine entsprechende Freigabe.

## Dokumentation und Zusammenarbeit

- Halte betroffene Dokumentation innerhalb des Auftrags aktuell. Projektspezifische Markdown-Dokumentation gehört unter `docs/`; diese `AGENTS.md` ist die Ausnahme.
- Weise bei paralleler Agentenarbeit eindeutige Dateizuständigkeiten zu und vermeide gleichzeitige Änderungen derselben Dateien.
- Fasse zum Abschluss Änderungen, tatsächlich ausgeführte Tests und offene Punkte zusammen. Beachte eine vom Nutzer vorgegebene Auswahl der zu commitenden und zu pushenden Dateien.

## Verbindliche CREO-Prüfung durch einen Subagenten

- Vor jedem Commit und Push den Subagenten `creo_pruefer` einsetzen. Seine Rolle steht in `.codex/agents/creo_pruefer.toml`. Falls die Laufzeit keine benannten Rollen unterstützt, diese Datei lesen und ihren Auftrag an einen normalen Subagenten übergeben.
- Die Prüfung umfasst den gesamten zukünftigen Repository-Stand im Git-Index, nicht nur neu hinzugefügte Dateien. Pro CREO-Basisdateiname darf ausschließlich die höchste numerische Version enthalten sein.
- Der Subagent darf solche Versionsfehler ohne weitere Bestätigung durch gezieltes `git add` und `git rm --cached` beheben. Ältere lokale Dateien bleiben erhalten. Keine Git-Historie umschreiben.
- Während der Subagent den Index bearbeitet, darf kein anderer Agent Git-Mutationen ausführen. Vor Commit oder Push auf sein Ergebnis warten und den endgültigen Diff prüfen. Nach einem Pull erneut prüfen; bei notwendigen Korrekturen den Subagenten einsetzen.
- `CREO-Zeichnungen/` muss vollständig flach bleiben: keinerlei Unterordner. Hilfsskripte und Agentenkonfiguration gehören außerhalb dieses Ordners.
- Git-Befehle und diese gezielten Bereinigungen sind im Rahmen des Auftrags ohne zusätzliche Rückfrage autorisiert. Technische Sandbox-Freigaben bleiben davon unberührt.
- Der Subagent wird bei diesen Arbeitsabläufen gestartet; er ist kein dauerhaft laufender Hintergrunddienst.
