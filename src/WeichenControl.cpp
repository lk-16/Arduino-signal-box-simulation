/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "WeichenControl.h"
#include "Weiche.h"

//Methoden der Klasse besetztmeldungControl
WeichenControl::WeichenControl(int anzahlWeichen, int weichenPinGerade[], int weichenPinKurve[], int weichenLedPinGerade[], int weichenLedPinKurve[], int adressWeichenposition[], int weichentimeout, int wt[], int wgt, int registerPins[4])
{
    _anzahlWeichen = anzahlWeichen;
    //array wird definiert
    _pWeiche = new Weiche *[_anzahlWeichen];
    //und initialisiert
    for (int weichenNr = 0; weichenNr < _anzahlWeichen; weichenNr++)
        _pWeiche[weichenNr] = new Weiche(weichenNr, weichenPinGerade[weichenNr], weichenPinKurve[weichenNr], weichenLedPinGerade[weichenNr], weichenLedPinKurve[weichenNr], adressWeichenposition[weichenNr], weichentimeout, wt[weichenNr], wgt, registerPins);
}

void WeichenControl::weichenWechseln()
{
    for (int weichenNr = 0; weichenNr < _anzahlWeichen; weichenNr++)
        _pWeiche[weichenNr]->weicheWechsel();
}

void WeichenControl::weichenStellen(int weichenNr, boolean weichenlage)
{
    if(weichenlage)_pWeiche[weichenNr]->weicheGerade();
    else _pWeiche[weichenNr]->weicheKurve();
}

void WeichenControl::weichenGerade(int weichenNr)
{
    _pWeiche[weichenNr]->weicheGerade();
}

void WeichenControl::weichenKurve(int weichenNr)
{
    _pWeiche[weichenNr]->weicheKurve();
}

void WeichenControl::weichenRelaisHIGH()
{
    for (int weichenNr = 0; weichenNr < _anzahlWeichen; weichenNr++)
        _pWeiche[weichenNr]->weicheRelaisHIGH();
}

void WeichenControl::weichenRelaisLOW()
{
    for (int weichenNr = 0; weichenNr < _anzahlWeichen; weichenNr++)
        _pWeiche[weichenNr]->weicheRelaisLOW();
}

void WeichenControl::weichenpositionenEEPROM()
{
    for (int weichenNr = 0; weichenNr < _anzahlWeichen; weichenNr++)
        _pWeiche[weichenNr]->weichenpositionEEPROM();
}

void WeichenControl::weichenSchalten()
{
    for (int weichenNr = 0; weichenNr < _anzahlWeichen; weichenNr++)
        _pWeiche[weichenNr]->weicheSchalten();
}

void WeichenControl::setWeichenfestlegung(int weichenNr, boolean festlegestatus, int fahrstrassennr)
{
    _pWeiche[weichenNr]->setWeichenfestlegung(festlegestatus, fahrstrassennr);
}

boolean WeichenControl::getWeichenfestlegung(int weichenNr)
{
    return _pWeiche[weichenNr]->getWeichenfestlegung();
}