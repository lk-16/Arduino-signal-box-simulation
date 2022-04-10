/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 10.04.2022
*/

#include "Gleissymbol.h"


Gleissymbol::Gleissymbol(Besetztmelder * besetztmelder, Weiche * weiche, Hauptsignal * signal)
{
    _besetztmelder = besetztmelder;
    _weiche = weiche;
    _signal = signal;
}
Besetztmelder * Gleissymbol::getBesetztmelder()
{
    return _besetztmelder;
}
Weiche * Gleissymbol::getWeiche()
{
    return _weiche;
}
Hauptsignal * Gleissymbol::getHausptsignal()
{
    return _signal;
}
void Gleissymbol::update()
{
    _signal->signalSperren();
    _weiche->weicheWechsel();
    _weiche->weicheSchalten();
    _besetztmelder->besetztmelderAuslesen(LOW, *_weiche);
}