/* Libary zum testen:
    ich möchte Weicehn über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Zugtaste.h"


//Methoden der Klasse Weichen

//definieren von für alle Methoden wichtige Informationen Pins etc.
Zugtaste::Zugtaste(int zugtastenPin, Gleissymbol *symbol = nullptr)
{
  _zugtastenPin = zugtastenPin;
  _symbol = symbol;
  pinMode(_zugtastenPin, INPUT_PULLUP);
}

boolean Zugtaste::getzugtastenstatus()                          //boolean um den Zugtastenstatus zu erfahren.
{
  boolean _zugtastenstatus = digitalRead(_zugtastenPin);//invertiert, damit das ganz noch mit der Zugtastensteuerung funktioniert. da interne Pullups verwendet werden
  return _zugtastenstatus;
}
