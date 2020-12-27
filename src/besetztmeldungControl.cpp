/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Gleisbild.h"

//Methoden der Klasse besetztmeldungControl
besetztmeldungControl::besetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int anzahlMelder, int registerPins[4])
    : actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
    _anzahlMelder = anzahlMelder;
    pbesetztmelder = new besetztmelder *[_anzahlMelder];
    for (int besetztmelderNr = 0; besetztmelderNr < _anzahlMelder; besetztmelderNr++)
        pbesetztmelder[besetztmelderNr] = new besetztmelder(gleisPins[besetztmelderNr], ledsGelb[besetztmelderNr], ledsRot[besetztmelderNr], registerPins);
}

boolean besetztmeldungControl::getBesetztmelderstatus(int besetztmelder)
{
    return pbesetztmelder[besetztmelder]->besetztmelderAuslesen();
}

void besetztmeldungControl::setBesetztmelderBeleuchtung(int besetztmelder, boolean besetztmelderLichtstatus)
{
    pbesetztmelder[besetztmelder]->setBesetztmelderLicht(besetztmelderLichtstatus);
}