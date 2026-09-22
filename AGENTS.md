# Projektregeln

## Verbindliche CREO-Prüfung durch einen Subagenten

- Vor jedem Commit und Push den Subagenten `creo_pruefer` einsetzen. Seine Rolle steht in `.codex/agents/creo_pruefer.toml`. Falls die Laufzeit keine benannten Rollen unterstützt, diese Datei lesen und ihren Auftrag an einen normalen Subagenten übergeben.
- Die Prüfung umfasst den gesamten zukünftigen Repository-Stand im Git-Index, nicht nur neu hinzugefügte Dateien. Pro CREO-Basisdateiname darf ausschließlich die höchste numerische Version enthalten sein.
- Der Subagent darf solche Versionsfehler ohne weitere Bestätigung durch gezieltes `git add` und `git rm --cached` beheben. Ältere lokale Dateien bleiben erhalten. Keine Git-Historie umschreiben.
- Während der Subagent den Index bearbeitet, darf kein anderer Agent Git-Mutationen ausführen. Vor Commit oder Push auf sein Ergebnis warten und den endgültigen Diff prüfen. Nach einem Pull erneut prüfen; bei notwendigen Korrekturen den Subagenten einsetzen.
- `CREO-Zeichnungen/` muss vollständig flach bleiben: keinerlei Unterordner. Hilfsskripte und Agentenkonfiguration gehören außerhalb dieses Ordners.
- Git-Befehle und diese gezielten Bereinigungen sind im Rahmen des Auftrags ohne zusätzliche Rückfrage autorisiert. Technische Sandbox-Freigaben bleiben davon unberührt.
- Der Subagent wird bei diesen Arbeitsabläufen gestartet; er ist kein dauerhaft laufender Hintergrunddienst.
