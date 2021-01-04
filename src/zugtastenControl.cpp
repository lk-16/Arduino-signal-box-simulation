/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "zugtastenControl.h"
#include "zugtasten.h"

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