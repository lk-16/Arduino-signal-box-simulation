/* Libary zum testen:
    ich möchte Weicehn über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Gleisbild.h"

//Methoden der Klasse actors
actors::actors(int anzahl, int sh, int st, int ds)
{
  //pinMode(actors::getWecker(), OUTPUT); //wird nicht übergeben
  _registerAnzahl = anzahl;
  _shPin = sh;
  _stPin = st;
  _dsPin = ds;
}

/*void actors::setWecker(int wecker)
{
  _weckerPin = wecker;
  actors::digitalSchalten(_weckerPin, HIGH); //Realis wird auf nicht aktiv gestellt
}

int actors::getWecker()
{
  return _weckerPin;
}*/

void actors::blinken(int ledPin)
{
  if (millis() % 1000 > 500)
  {
    actors::digitalSchalten(ledPin, LOW);
  }
  else
  {
    actors::digitalSchalten(ledPin, HIGH);
  }
}

void actors::digitalSchalten(int PinNr, boolean newPinStatus)
{

  static schieberegister *register1 = new schieberegister(_registerAnzahl, _shPin, _stPin, _dsPin); //ein Objekt der Klasse Schieberegister wird beim ersten aufrufen der Funktion erstellt

  if (PinNr < 100 && PinNr > 0)
    digitalWrite(PinNr, newPinStatus); //wenn die Pin Nummer zwischen 0 und 100 liegt werden die Pin am Arduino angesteuert
  else if (PinNr > 100 && PinNr < 200)
    register1->setPin(PinNr - 100, newPinStatus); //wenn die Nummer zwischen 100 und 200 liegt, nutze das erste Schieberegister in kombination mit der Bibliothek
}

void actors::setRegisterPins(int anzahl, int sh, int st, int ds)
{
  _registerAnzahl = anzahl;
  _shPin = sh;
  _stPin = st;
  _dsPin = ds;
}
