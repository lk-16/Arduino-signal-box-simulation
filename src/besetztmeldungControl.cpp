/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "besetztmeldungControl.h"
#include "besetztmelder.h"

//Methoden der Klasse besetztmeldungControl
besetztmeldungControl::besetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int anzahlMelder, int registerPins[4])
    : actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
    _anzahlMelder = anzahlMelder;
    //array wird definiert
    _pbesetztmelder = new besetztmelder *[_anzahlMelder];
    //und initialisiert
    for (int besetztmelderNr = 0; besetztmelderNr < _anzahlMelder; besetztmelderNr++)
        _pbesetztmelder[besetztmelderNr] = new besetztmelder(gleisPins[besetztmelderNr], ledsGelb[besetztmelderNr], ledsRot[besetztmelderNr], registerPins);
}


//aus der Klasse Control werden die entgegengenommenen Befehle auf die einzelnen 
boolean besetztmeldungControl::getBesetztmelderstatus(int besetztmelder, boolean besetztmelderBeleuchtung)
{
    return _pbesetztmelder[besetztmelder]->besetztmelderAuslesen(besetztmelderBeleuchtung);
}

void besetztmeldungControl::setBesetztmelderBeleuchtung(int besetztmelder, boolean besetztmelderLichtstatus)
{
    _pbesetztmelder[besetztmelder]->setBesetztmelderLicht(besetztmelderLichtstatus);
}
void besetztmeldungControl::setFahrstrassenelement(int besetztmelderNr, int fahrstrassenNr, boolean fahrstrassenstatus)
{
    _pbesetztmelder[besetztmelderNr]->setFahrstrassenelement(fahrstrassenNr, fahrstrassenstatus);
}
boolean besetztmeldungControl::getFahrstrassenelement(int besetztmelderNr)
{
    return _pbesetztmelder[besetztmelderNr]->getFahrstrassenelement();
}
