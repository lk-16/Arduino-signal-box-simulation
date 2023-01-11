/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

#include "ZugtastenControl.h"
#include "Zugtaste.h"

// Methoden der Klasse besetztmeldungControl
ZugtastenControl::ZugtastenControl(int anzahlZugtasten, int zugtastenPins[], boolean richtung[])

{
    _anzahlZugtasten = anzahlZugtasten;
    _pzugtasten = new Zugtaste *[_anzahlZugtasten];
    for (int zugtastenNr = 0; zugtastenNr < _anzahlZugtasten; zugtastenNr++)
        _pzugtasten[zugtastenNr] = new Zugtaste(zugtastenPins[zugtastenNr], richtung[zugtastenNr]);
}

int ZugtastenControl::getZugtastenAnzahl()
{
    return _anzahlZugtasten;
}

boolean ZugtastenControl::getZugtastenstatus(int zugtastenNr)
{
    if (isZugtaste(zugtastenNr))
        return _pzugtasten[zugtastenNr]->getzugtastenstatus();
    else
        return false;
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

void ZugtastenControl::setGleissymbol(int zugtastenNr, Gleissymbol *symbol)
{
    if (isZugtaste(zugtastenNr))
        _pzugtasten[zugtastenNr]->setGleissymbol(symbol);
}

Gleissymbol *ZugtastenControl::getGleissymbol(int zugtastenNr)
{
    if (isZugtaste(zugtastenNr))
        return _pzugtasten[zugtastenNr]->getGleissymbol();
    else
        return nullptr;
}

Zugtaste *ZugtastenControl::getZugtaste(int zugtastenNr)
{
    if (isZugtaste(zugtastenNr))
        return _pzugtasten[zugtastenNr];
    else
        return nullptr;
}

boolean ZugtastenControl::isZugtaste(int zugtastenNr)
{
    if (zugtastenNr < _anzahlZugtasten)
        return true;
    else
    {
        Serial.println("Error: ZugtastenNr existiert nicht. Source: ZugtastenControl::isZugtaste");
        return false;
    }
}