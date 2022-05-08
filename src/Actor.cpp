/* Libary zum testen:
    ich möchte Weicehn über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include <Arduino.h>
#include <EEPROM.h>
#include "Actor.h"

// Methoden der Klasse actors
Actor::Actor(int anzahl, int sh, int st, int ds)
{
  // pinMode(actors::getWecker(), OUTPUT); //wird nicht übergeben
  _registerAnzahl = anzahl;
  _shPin = sh;
  _stPin = st;
  _dsPin = ds;
}

void Actor::blinken(int ledPin)
{
  if (millis() % 1000 > 500)
  {
    Actor::digitalSchalten(ledPin, LOW);
  }
  else
  {
    Actor::digitalSchalten(ledPin, HIGH);
  }
}

void Actor::digitalSchalten(int PinNr, boolean newPinStatus)
{

  static schieberegister *register1 = new schieberegister(_registerAnzahl, _shPin, _stPin, _dsPin); // ein Objekt der Klasse Schieberegister wird beim ersten aufrufen der Funktion erstellt

  if (PinNr < 100 && PinNr > 0)
    digitalWrite(PinNr, newPinStatus); // wenn die Pin Nummer zwischen 0 und 100 liegt werden die Pin am Arduino angesteuert
  else if (PinNr > 100 && PinNr < 200)
    register1->setPin(PinNr - 100, newPinStatus); // wenn die Nummer zwischen 100 und 200 liegt, nutze das erste Schieberegister in kombination mit der Bibliothek
}

void Actor::setRegisterPins(int anzahl, int sh, int st, int ds)
{
  _registerAnzahl = anzahl;
  _shPin = sh;
  _stPin = st;
  _dsPin = ds;
}
