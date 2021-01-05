/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "ZugtastenControl.h"
#include "Zugtaste.h"

//Methoden der Klasse besetztmeldungControl
zugtastenControl::zugtastenControl(int anzahlZugtasten, int zugtastenPins[])

{
    _anzahlZugtasten = anzahlZugtasten;
    _pzugtasten = new zugtasten *[_anzahlZugtasten];
    for (int zugtastenNr = 0; zugtastenNr < _anzahlZugtasten; zugtastenNr++)
        _pzugtasten[zugtastenNr] = new zugtasten(zugtastenPins[zugtastenNr]);
}

int zugtastenControl::getZugtastenAnzahl()
{
    return _anzahlZugtasten;
}

boolean zugtastenControl::getZugtastenstatus(int zugtastenNr)
{
    return _pzugtasten[zugtastenNr]->getzugtastenstatus();
}

boolean zugtastenControl::zugtastenGedrueckt()
{
    int zugtastenpress = 0;
    for (int i = 0; i < _anzahlZugtasten; i++)
    {
        if (zugtastenControl::getZugtastenstatus(i) == HIGH)
            zugtastenpress++;
    }
    return zugtastenpress;
}