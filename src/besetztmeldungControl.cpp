/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */
#include "Arduino.h"
#include "BesetztmeldungControl.h"
#include "Besetztmelder.h"

//Methoden der Klasse besetztmeldungControl
BesetztmeldungControl::BesetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int weichenbesetztmelder[], int anzahlMelder, int registerPins[4])
{
    _anzahlMelder = anzahlMelder;
    //array wird definiert
    _pbesetztmelder = new Besetztmelder *[_anzahlMelder];
    //und initialisiert
    for (int besetztmelderNr = 0; besetztmelderNr < _anzahlMelder; besetztmelderNr++)
        _pbesetztmelder[besetztmelderNr] = new Besetztmelder(gleisPins[besetztmelderNr], ledsGelb[besetztmelderNr], ledsRot[besetztmelderNr], weichenbesetztmelder[besetztmelderNr], registerPins);
}

Besetztmelder *BesetztmeldungControl::getBesetztmelder(int BesetztmelderNr)
{
    return _pbesetztmelder[BesetztmelderNr];
}
//aus der Klasse Control werden die entgegengenommenen Befehle auf die einzelnen Besetztmelder verteilt.
boolean BesetztmeldungControl::getBesetztmelderstatus(int besetztmelder, boolean besetztmelderBeleuchtung, WeichenControl WeichenControl)
{
    return _pbesetztmelder[besetztmelder]->besetztmelderAuslesen(besetztmelderBeleuchtung, WeichenControl);
}

void BesetztmeldungControl::setBesetztmelderBeleuchtung(int besetztmelder, boolean besetztmelderLichtstatus)
{
    _pbesetztmelder[besetztmelder]->setBesetztmelderLicht(besetztmelderLichtstatus);
}
void BesetztmeldungControl::setFahrstrassenelement(int besetztmelderNr, int fahrstrassenNr, boolean fahrstrassenstatus)
{
    _pbesetztmelder[besetztmelderNr]->setFahrstrassenelement(fahrstrassenNr, fahrstrassenstatus);
}
boolean BesetztmeldungControl::getFahrstrassenelement(int besetztmelderNr)
{
    return _pbesetztmelder[besetztmelderNr]->getFahrstrassenelement();
}

void BesetztmeldungControl::besetztmelderAusfuehren(WeichenControl WeichenControl)
{
    for (int besetztmelderNr = 0; besetztmelderNr < _anzahlMelder; besetztmelderNr++)
        _pbesetztmelder[besetztmelderNr]->besetztmelderAuslesen(LOW, WeichenControl);
}