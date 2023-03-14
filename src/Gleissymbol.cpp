/*Library für die Steuerung eines SpDrS60 Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 10.04.2022
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

void Gleissymbol::prepare()
{
    if (_weiche != nullptr)
    {
        _weiche->weicheRelaisHIGH();
        _weiche->weichenpositionEEPROM();
    }
}
void Gleissymbol::update()
{
    if (_signal != nullptr) // wenn es ein signal gibt.
    {
        _signal->signalSperren();
        _signal->hauptsignalHp0Manuell();
    }
    if (_weiche != nullptr) // wenn es eine Weiche gibt
    {
        _weiche->weicheWechsel();
        _weiche->weicheSchalten();
        /*if(_besetztmelder != nullptr)
        {
            _weiche->setWeichebesetzt(_besetztmelder->besetztmelderAuslesen(LOW, *_weiche));
        }*/
    }
    if (_besetztmelder != nullptr) // wenn es einen Besetztmelder gibt.
        _besetztmelder->besetztmelderAuslesen(LOW, _weiche);
}

boolean Gleissymbol::isFree(boolean ignoreFlankenschutz)
{
    boolean free = true;
    if (getFahrstrassenelement())
    {
        free = false;
    }
    else if (_besetztmelder != nullptr && (_besetztmelder->getFahrstrassenelement() || _besetztmelder->besetztmelderAuslesen(LOW, _weiche))) // wenn besetztmelder kein Fahrstraßenelemnt und nicht besetzt
    {
        free = false;
    }
    else if (_signal != nullptr && _signal->getSignalsperre()) // das signal nicht gesperrt, wenn vorhanden
    {
        free = false;
    }
    else if (_weiche != nullptr && ignoreFlankenschutz && !_weiche->getFlankenschutz() && _weiche->getWeichenfestlegung()) // die Weiche nicht festgelegt
    {
        free = false;
    }
    return free;
}

boolean isOnlyFlankenschutz();

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
    if (!_fahrstrassenelement)
        _weg = status;
}

int Gleissymbol::getWeg()
{
    return _weg;
}

void Gleissymbol::setFahrstrassenelement(unsigned int fahrstrassennr, boolean Fahrstrassenelement)
{
    if (!_fahrstrassenelement)
    {
        setWeg(fahrstrassennr);
        _fahrstrassenelement = Fahrstrassenelement;
        if (_besetztmelder != nullptr)
            _besetztmelder->setFahrstrassenelement(fahrstrassennr, Fahrstrassenelement);
    }
    else if (fahrstrassennr == _weg)
    {
        if (!Fahrstrassenelement)
        {
            _fahrstrassenelement = Fahrstrassenelement;
            setWeg(0);
            if (_besetztmelder != nullptr)
                _besetztmelder->setFahrstrassenelement(fahrstrassennr, Fahrstrassenelement);
        }
    }
}

boolean Gleissymbol::getFahrstrassenelement()
{
    return _fahrstrassenelement;
}

void Gleissymbol::setFlankenschutzweiche(Gleissymbol *flankenschutzweiche, int fahrstrassennummer, boolean weichenposition){
    if(_weiche != nullptr && flankenschutzweiche != nullptr){
        _flankenschutzweiche = flankenschutzweiche;
        flankenschutzweiche->getWeiche()->setWeichenposition(weichenposition);
        flankenschutzweiche->getWeiche()->setFlankenschutz(true, fahrstrassennummer);
    }
}

void Gleissymbol::resetFlankenschutzweiche(int fahrstrassennummer){
    if(_weiche != nullptr && _flankenschutzweiche != nullptr)//wenn Weiche und als Flankenschutzweiche
    {
        _flankenschutzweiche->getWeiche()->setFlankenschutz(false, fahrstrassennummer);
        _flankenschutzweiche = nullptr;
    }
}

Gleissymbol *Gleissymbol::getFlankenschutzweiche(){
    return _flankenschutzweiche;
}

boolean Gleissymbol::isAnfang()
{
    return _anfang;
}

void Gleissymbol::setAnfang(boolean status)
{
    _anfang = status;
}

boolean Gleissymbol::getBesetztmelderstatus()
{
    return _besetztmelder->besetztmelderAuslesen(LOW, _weiche);
}