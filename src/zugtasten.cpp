/* Libary zum testen:
    ich möchte Weicehn über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "zugtasten.h"


//Methoden der Klasse Weichen

//definieren von für alle Methoden wichtige Informationen Pins etc.
zugtasten::zugtasten(int zugtastenPin)
{
  _zugtastenPin = zugtastenPin;
  pinMode(_zugtastenPin, INPUT_PULLUP);
}

boolean zugtasten::getzugtastenstatus()                          //boolean um den Zugtastenstatus zu erfahren.
{
  boolean _zugtastenstatus = digitalRead(_zugtastenPin);
  return _zugtastenstatus;
}
