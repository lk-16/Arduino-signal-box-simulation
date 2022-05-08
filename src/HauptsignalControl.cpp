/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "HauptsignalControl.h"
#include "Hauptsignal.h"

// Methoden der Klasse besetztmeldungControl
HauptsignalControl::HauptsignalControl(int anzahlHauptsignale, int ledPinsRot[], int ledPinsGelb[], int ledPinsGruen[], int signaltasten[], int sperrmelder[], int allgSignaltasten[3], int registerPin[4], boolean richtung[])
{
    _anzahlHauptsignale = anzahlHauptsignale;
    // array wird definiert
    _pHauptsignale = new Hauptsignal *[_anzahlHauptsignale];
    // und initialisiert
    for (int hauptsignalNr = 0; hauptsignalNr < _anzahlHauptsignale; hauptsignalNr++)
    {
        _pHauptsignale[hauptsignalNr] = new Hauptsignal(ledPinsRot[hauptsignalNr], ledPinsGruen[hauptsignalNr], ledPinsGelb[hauptsignalNr], signaltasten[hauptsignalNr], sperrmelder[hauptsignalNr], allgSignaltasten, registerPin, richtung[hauptsignalNr]);
        _pHauptsignale[hauptsignalNr]->hauptsignalSchalten(0);
    }
}

Hauptsignal *HauptsignalControl::getHauptsignal(int hauptsignalNr)
{
    return _pHauptsignale[hauptsignalNr];
}
void HauptsignalControl::hauptsignalSchalten(int hauptsignalNr, int newStatus)
{
    _pHauptsignale[hauptsignalNr]->hauptsignalSchalten(newStatus);
}

void HauptsignalControl::hauptsignaleHp0Manuell()
{
    for (int HauptsignalNr = 0; HauptsignalNr < _anzahlHauptsignale; HauptsignalNr++)
        _pHauptsignale[HauptsignalNr]->hauptsignalHp0Manuell();
}

void HauptsignalControl::hauptsignaleSperren()
{
    for (int HauptsignalNr = 0; HauptsignalNr < _anzahlHauptsignale; HauptsignalNr++)
        _pHauptsignale[HauptsignalNr]->signalSperren();
}

boolean HauptsignalControl::getHauptsignalSperren(int hauptsignalNr)
{
    return _pHauptsignale[hauptsignalNr]->getSignalsperre();
}