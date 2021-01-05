/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "WeichenControl.h"
#include "Weiche.h"

//Methoden der Klasse besetztmeldungControl
WeichenControl::WeichenControl(int anzahlWeichen, int weichenPinGerade[], int weichenPinKurve[], int weichenLedPinGerade[], int weichenLedPinKurve[], int adressWeichenposition[], int weichentimeout, int wt[], int wgt, int registerPins[4])
    : actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
    _anzahlWeichen = _anzahlWeichen;
    //array wird definiert
    _pWeiche = new Weiche *[_anzahlWeichen];
    //und initialisiert
    for (int weichenNr = 0; weichenNr < _anzahlWeichen; weichenNr++)
        _pWeiche[weichenNr] = new Weiche(weichenNr, weichenPinGerade[weichenNr], weichenPinKurve[weichenNr], weichenLedPinGerade[weichenNr], weichenLedPinKurve[weichenNr], adressWeichenposition[weichenNr], weichentimeout, wt[weichenNr], wgt, registerPins);
}


/*//aus der Klasse Control werden die entgegengenommenen Befehle auf die einzelnen 
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
}*/
