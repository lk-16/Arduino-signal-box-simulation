/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "ZugtastenControl.h"
#include "Zugtaste.h"

//Methoden der Klasse besetztmeldungControl
ZugtastenControl::ZugtastenControl(int anzahlZugtasten, int zugtastenPins[])

{
    _anzahlZugtasten = anzahlZugtasten;
    _pzugtasten = new Zugtaste *[_anzahlZugtasten];
    for (int zugtastenNr = 0; zugtastenNr < _anzahlZugtasten; zugtastenNr++)
        _pzugtasten[zugtastenNr] = new Zugtaste(zugtastenPins[zugtastenNr]);
}

int ZugtastenControl::getZugtastenAnzahl()
{
    return _anzahlZugtasten;
}

boolean ZugtastenControl::getZugtastenstatus(int zugtastenNr)
{
    return _pzugtasten[zugtastenNr]->getzugtastenstatus();
}

boolean ZugtastenControl::zugtastenGedrueckt()
{
    int zugtastenpress = 0;
    for (int i = 0; i < _anzahlZugtasten; i++)
    {
        if (ZugtastenControl::getZugtastenstatus(i) == LOW)
            zugtastenpress++;
    }
    return zugtastenpress;
}