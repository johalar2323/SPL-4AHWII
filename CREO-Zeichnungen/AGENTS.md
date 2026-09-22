# CREO-Dateien

- In diesem Verzeichnis dürfen keinerlei Unterordner angelegt werden. Alle Dateien müssen direkt in `CREO-Zeichnungen/` liegen. Diese flache Struktur ist eine zwingende Projektvorgabe.
- Im gesamten aktuellen Repository-Stand darf pro CREO-Basisdateiname nur die höchste numerische Versionsnummer enthalten sein (z. B. `gitter.prt.5`). Das gilt auch für andere versionierte CREO-Dateien, etwa `.asm` und `.drw`. Versionsnummern numerisch vergleichen: `.10` ist neuer als `.9`.
- Beim Hinzufügen einer neueren Version bereits getrackte ältere Versionen mit `git rm --cached -- <Pfad>` aus dem Repository entfernen. Nur neue Dateien auszuwählen genügt nicht. Ältere lokale Dateien niemals löschen; keine Historie umschreiben.
- Die verbindliche Prüfung und automatische Bereinigung durch `creo_pruefer` ist in der übergeordneten `AGENTS.md` geregelt.
