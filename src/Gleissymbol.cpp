/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 10.04.2022
*/

#include "Gleissymbol.h"

Gleissymbol::Gleissymbol(Besetztmelder *besetztmelder, Weiche *weiche, Hauptsignal *signal)
{
    _besetztmelder = besetztmelder;
    _weiche = weiche;
    _signal = signal;
}
Besetztmelder *Gleissymbol::getBesetztmelder()
{
    return _besetztmelder;
}
Weiche *Gleissymbol::getWeiche()
{
    return _weiche;
}
Hauptsignal *Gleissymbol::getHauptsignal()
{
    return _signal;
}
void Gleissymbol::update()
{
    if (_signal != nullptr) // wenn es ein signal gibt.
        _signal->signalSperren();
    if (_weiche != nullptr) // wenn es eine Weiche gibt
    {
        _weiche->weicheWechsel();
        _weiche->weicheSchalten();
    }
    if (_besetztmelder != nullptr) // wenn es einen Besetztmelder gibt.
        _besetztmelder->besetztmelderAuslesen(LOW, *_weiche);
}

boolean Gleissymbol::isFree()
{
    boolean free = true;
    if (_besetztmelder != nullptr && (_besetztmelder->getFahrstrassenelement() || _besetztmelder->besetztmelderAuslesen(LOW, *_weiche)))//wenn besetztmelder kein Fahrstraßenelemnt und nicht besetzt
    {
        free = false;
    }
    if (_signal != nullptr && _signal->getSignalsperre())//das signal nicht gesperrt, wenn vorhanden
    {
        free = false;
    }
    if (_weiche != nullptr && _weiche->getWeichenfestlegung())//die Weiche nicht festgelegt
    {
        free = false;
    }
    return free;
}

void Gleissymbol::setMarkierung(boolean status)
{
    _markiert = status;
}

boolean Gleissymbol::getMarkierung()
{
    return _markiert;
}

void Gleissymbol::setWeg(int status)
{
    _weg = status;
}

int Gleissymbol::getWeg()
{
    return _weg;
}