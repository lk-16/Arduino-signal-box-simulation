/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 10.04.2022
*/

#include "Gleissymbol.h"

Gleissymbol::Gleissymbol(Besetztmelder *besetztmelder = nullptr, Weiche *weiche = nullptr, Hauptsignal *signal = nullptr)
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
Hauptsignal *Gleissymbol::getHausptsignal()
{
    return _signal;
}
void Gleissymbol::update()
{
    if (_signal != nullptr)//wenn es ein signal gibt.
        _signal->signalSperren();
    if (_weiche != nullptr) //wenn es eine Weiche gibt
    {
        _weiche->weicheWechsel();
        _weiche->weicheSchalten();
    }
    if (_besetztmelder != nullptr)  //wenn es einen Besetztmelder gibt.
        _besetztmelder->besetztmelderAuslesen(LOW, *_weiche);
}

void Gleissymbol::setMarkierung(boolean status)
{
    _markiert = status;
}

boolean Gleissymbol::getMarkierung()
{
    return _markiert;
}