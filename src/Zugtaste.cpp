/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Zugtaste.h"

// Methoden der Klasse Weichen

// definieren von für alle Methoden wichtige Informationen Pins etc.
Zugtaste::Zugtaste(int zugtastenPin, boolean richtung, Gleissymbol *symbol)
{
  _zugtastenPin = zugtastenPin;
  _symbol = symbol;
  _richtung = richtung;
  pinMode(_zugtastenPin, INPUT_PULLUP);
}

boolean Zugtaste::getzugtastenstatus() // boolean um den Zugtastenstatus zu erfahren.
{
  boolean _zugtastenstatus = digitalRead(_zugtastenPin); // invertiert, damit das ganz noch mit der Zugtastensteuerung funktioniert. da interne Pullups verwendet werden
  return _zugtastenstatus;
}

void Zugtaste::setGleissymbol(Gleissymbol *symbol)
{
  _symbol = symbol;
}

Gleissymbol *Zugtaste::getGleissymbol()
{
  return _symbol;
}

boolean Zugtaste::getRichtung()
{
  return _richtung;
}
