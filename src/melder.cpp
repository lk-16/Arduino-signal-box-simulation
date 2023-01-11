/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

#include "Arduino.h"
#include "Melder.h"
#include "Actor.h"

// Methoden der Klasse Melder
Melder::Melder(String melderName, int tueMelderLed, int weckerPin, int wutPin, int registerPins[4])
    : Actor(registerPins[0], registerPins[1], registerPins[2], registerPins[3]) // Led des Melders, Name des Melders(für Serielle Kommunitation z.B. FTÜ,GTÜ)
{
  Serial.begin(9600); // Die Serielle Kommunikation wird gestartet

  // Anschlussdefinition
  _melderName = melderName;
  _tueMelderLed = tueMelderLed; //übernehmen der Variablen in die Klasse
  _wutPin = wutPin;
  _weckerPin = weckerPin;
  // definieren ob Anschlüsse Aus- oder Eingang sind
  pinMode(_tueMelderLed, OUTPUT); // Tastenüberwachung led Melder
  pinMode(_wutPin, INPUT_PULLUP); // Tastenüberwachung Weckerunterbrechertaste
  pinMode(_weckerPin, OUTPUT);

  digitalSchalten(_weckerPin, HIGH);
}

void Melder::tueMelder(ZugtastenControl ZugtastenControl) // tastenüberwachung
{
  if (ZugtastenControl.zugtastenGedrueckt()) // wenn eine Zugtaste gedrückt ist
  {
    if ((millis() - _melderStartzeit >= _melderTimeout))
    {
      _tueMelderStatus = true; // FTÜ-Melder ist false keine Warnung, true melder und ggf. wecker schlägt an
      Melder::blinken(_tueMelderLed);

      if (millis() - _melderStartzeit >= (_melderTimeout * 2) && _wutAktivierung == LOW) // Wenn 10 s lang eine Taste gedrückt wurde und nicht die Wut gedrückt wurde
      {
        Actor::digitalSchalten(_weckerPin, LOW); // Relais schaltet --> Wecker klingelt
      }
      if (digitalRead(_wutPin) == LOW) // wenn Weckerunterbrechertaste gedrückt
      {
        _wutAktivierung = HIGH;                   // die Weckerunterbrechertaste wurde gedrückt
        Actor::digitalSchalten(_weckerPin, HIGH); // Relais fällt zurück --> Wecker wird gestoppt
      }
      Serial.println(_melderName); // Meldername und Status wird über den seriellen Monitor angezeigt............................... Ausgabe der Meldernamen funktioniert noch nicht
    }
  }

  else // wenn keine Mehr gedrückt wird,
  {
    _melderStartzeit = millis();                 // setze den Timer zurück
    _tueMelderStatus = false;                    // den Status auf 0
    Melder::digitalSchalten(_tueMelderLed, LOW); // Schalte die Led aus
    Melder::digitalSchalten(_weckerPin, HIGH);   // Mache den Wecker aus
    _wutAktivierung = false;                     // beim Beenden der Störung wird die Unterbrechung wieder aufgehoben
  }
}