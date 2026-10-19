# AGENTS.md – Allgemeine Agentenregeln

## 1. Arbeitsbeginn und Projektkontext

- Lies und beachte diese Anweisungen vor Arbeitsbeginn.

- Lies `docs/PROJEKT.md`, sofern vorhanden, um Auftrag, Projektstand, Architektur und offene Entscheidungen zu verstehen.

- Lies `docs/TESTS.md` vor Implementierungen und Tests, sofern vorhanden.

- Prüfe vorhandenen Code, Konfigurationen und Dokumentation, bevor du Änderungen vornimmst.

- Erfinde keine Anforderungen, Schnittstellen, Zugangsdaten oder technischen Eigenschaften.

## 2. Kommunikation und Entscheidungen

- Kommuniziere auf Deutsch, sachlich, präzise und ohne unnötige Höflichkeitsfloskeln.

- Weise konstruktiv auf Probleme, Risiken und Schwachstellen hin.

- Stelle bei wesentlichen Unklarheiten gezielte Rückfragen, bevor du irreversible oder weitreichende Entscheidungen triffst.

- Triff bei unwesentlichen Details nachvollziehbare Annahmen und kennzeichne sie.

- Vergleiche bei wichtigen technischen Entscheidungen geeignete Alternativen mit Vor- und Nachteilen.

- Unterscheide bestätigte Fakten, Annahmen und Empfehlungen.

## 3. Implementierung

- Bewahre bestehende Funktionalität, sofern keine Änderung ausdrücklich beauftragt wurde.

- Bevorzuge einfache, wartbare und nachvollziehbare Lösungen.

- Vermeide unnötige Abhängigkeiten, Duplikate und übermäßige Abstraktion.

- Halte Änderungen auf den erforderlichen Umfang begrenzt.

- Entferne keine bestehenden Funktionen oder Daten ohne entsprechende Berechtigung.

- Hole vor destruktiven oder schwer rückgängig zu machenden Aktionen eine Freigabe ein.

- Beachte bestehende Architektur, Namenskonventionen und Projektstandards.

## 4. Dokumentation

- Halte die Dokumentation bei relevanten Erkenntnissen, Entscheidungen, Implementierungen und Testergebnissen aktuell.

- Dokumentiere offene Fragen, bekannte Einschränkungen und noch ausstehende Aufgaben.

- Vermeide redundante Dokumentation.

- Speichere projektspezifische Markdown-Dokumentation grundsätzlich unter `docs/`.

- `AGENTS.md` im Projektstamm ist eine ausdrückliche Ausnahme.

- Erstelle fehlende Dokumente nur, wenn sie für das Projekt erforderlich sind oder ihre Erstellung beauftragt wurde.

## 5. Tests und Qualitätssicherung

- Prüfe vor Tests die vorhandenen Testanforderungen.

- Führe nach Änderungen die relevanten verfügbaren Tests aus.

- Dokumentiere ausgeführte Tests in `docs/TESTS.md` mit Datum, Vorgehen, Ergebnis und Einschränkungen, sofern das Projekt dieses Protokoll verwendet.

- Unterscheide geplante, erfolgreich ausgeführte, fehlgeschlagene und nicht ausführbare Tests.

- Behaupte niemals, Tests durchgeführt zu haben, wenn sie nicht tatsächlich ausgeführt wurden.

- Melde Fehler und nicht überprüfte Funktionen ausdrücklich.

## 6. Zusammenarbeit mehrerer Agenten

- Stelle sicher, dass Subagenten die für ihren Arbeitsbereich geltenden Anweisungen erhalten.

- Definiere bei paralleler Arbeit eindeutige Verantwortlichkeiten.

- Vermeide gleichzeitige Änderungen mehrerer Agenten an denselben Dateien.

- Führe parallele Arbeitsergebnisse kontrolliert zusammen.

- Dokumentiere relevante Entscheidungen und Erkenntnisse für nachfolgende Agenten.

## 7. Sicherheit

- Speichere Zugangsdaten, API-Schlüssel und andere Geheimnisse niemals im Quellcode oder in Markdown-Dateien.

- Verwende Umgebungsvariablen oder geeignete Secret-Management-Lösungen.

- Nutze in Beispielen und Testdaten anonymisierte beziehungsweise synthetische Informationen.

- Führe keine destruktiven Aktionen ohne entsprechende Freigabe aus.

## 8. Abschluss einer Arbeitsphase

Prüfe vor Abschluss:

- Sind die beauftragten Anforderungen umgesetzt?

- Wurde vorhandene Funktionalität erhalten?

- Wurden relevante Tests ausgeführt oder Einschränkungen dokumentiert?

- Ist die betroffene Dokumentation aktuell?

- Sind offene Fragen und verbleibende Risiken benannt?

Fasse abschließend die wesentlichen Änderungen, Testergebnisse und offenen Punkte zusammen.

## 9. Projektdateien

- `docs/PROJEKT.md`: Auftrag, Projektstand, Architektur, Entscheidungen und nächste Schritte.

- `docs/TESTS.md`: Testanforderungen, Ergebnisse und Testprotokoll.

Projektspezifische Anforderungen gehören in die entsprechende Projektdokumentation und nicht in diese allgemeine Regeldatei.

## Projektspezifische Ergänzungen

Die allgemeinen Regeln oben stammen aus der persönlichen Vorlage `%USERPROFILE%/.codex/AGENTS.md`. Die folgenden Regeln ergänzen sie für dieses Repository.

- Lies verbindlich zu Arbeitsbeginn `docs/PROJEKT.md` und vor Codeänderungen oder Tests `docs/TESTS.md`.
- Lies vor Änderungen an Pins, ADC, Verdrahtung oder Board `docs/PINBELEGUNG.md`. Fehlende Hardwaredaten bleiben offen und werden mit dem Nutzer geklärt.
- Prüfe vor Änderungen an Build-, Upload- oder IDE-Einstellungen `ESP32/Programmierung/platformio.ini` und die Build-Anleitung in `docs/PROJEKT.md`.
- Unterscheide Build-Erfolg und Hardwareprüfung. Ein Auftrag zum Git-Push berechtigt nicht zum Flashen; Firmware-Uploads erfolgen nur bei entsprechendem Nutzerauftrag.
- Beachte eine vom Nutzer vorgegebene Auswahl der zu commitenden und zu pushenden Dateien.

## Verbindliche CREO-Prüfung durch einen Subagenten

- Vor jedem Commit und Push den Subagenten `creo_pruefer` einsetzen. Seine Rolle steht in `.codex/agents/creo_pruefer.toml`. Falls die Laufzeit keine benannten Rollen unterstützt, diese Datei lesen und ihren Auftrag an einen normalen Subagenten übergeben.
- Die Prüfung umfasst den gesamten zukünftigen Repository-Stand im Git-Index, nicht nur neu hinzugefügte Dateien. Pro CREO-Basisdateiname darf ausschließlich die höchste numerische Version enthalten sein.
- Der Subagent darf solche Versionsfehler ohne weitere Bestätigung durch gezieltes `git add` und `git rm --cached` beheben. Ältere lokale Dateien bleiben erhalten. Keine Git-Historie umschreiben.
- Während der Subagent den Index bearbeitet, darf kein anderer Agent Git-Mutationen ausführen. Vor Commit oder Push auf sein Ergebnis warten und den endgültigen Diff prüfen. Nach einem Pull erneut prüfen; bei notwendigen Korrekturen den Subagenten einsetzen.
- `CREO-Zeichnungen/` muss vollständig flach bleiben: keinerlei Unterordner. Hilfsskripte und Agentenkonfiguration gehören außerhalb dieses Ordners.
- Git-Befehle und diese gezielten Bereinigungen sind im Rahmen des Auftrags ohne zusätzliche Rückfrage autorisiert. Technische Sandbox-Freigaben bleiben davon unberührt.
- Der Subagent wird bei diesen Arbeitsabläufen gestartet; er ist kein dauerhaft laufender Hintergrunddienst.
