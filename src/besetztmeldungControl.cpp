/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "BesetztmeldungControl.h"
#include "Besetztmelder.h"

//Methoden der Klasse besetztmeldungControl
BesetztmeldungControl::BesetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int anzahlMelder, int registerPins[4])
    : Actor(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
    _anzahlMelder = anzahlMelder;
    //array wird definiert
    _pbesetztmelder = new Besetztmelder *[_anzahlMelder];
    //und initialisiert
    for (int besetztmelderNr = 0; besetztmelderNr < _anzahlMelder; besetztmelderNr++)
        _pbesetztmelder[besetztmelderNr] = new Besetztmelder(gleisPins[besetztmelderNr], ledsGelb[besetztmelderNr], ledsRot[besetztmelderNr], registerPins);
}


//aus der Klasse Control werden die entgegengenommenen Befehle auf die einzelnen 
boolean BesetztmeldungControl::getBesetztmelderstatus(int besetztmelder, boolean besetztmelderBeleuchtung)
{
    return _pbesetztmelder[besetztmelder]->besetztmelderAuslesen(besetztmelderBeleuchtung);
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