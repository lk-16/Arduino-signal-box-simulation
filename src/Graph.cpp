/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 10.04.2022
 */

#include "Graph.h"

Graph::Graph(int anzahlKnoten, Gleissymbol knoten[], int nachbarn[][3])
{
    _anzahlKnoten = anzahlKnoten;
    //_knoten = new Gleissymbol *[_anzahlKnoten];
    _knoten = knoten;
    _nachbarn = new int *[_anzahlKnoten];
    for (int i = 0; i < _anzahlKnoten; i++)
    {
        _nachbarn[i] = nachbarn[i];
    }
}

void Graph::prepare()
{
    for (int i = 0; i < _anzahlKnoten; i++)
    {
        _knoten[i].prepare();
    }
}

void Graph::updateSymbole()
{
    for (int i = 0; i < _anzahlKnoten; i++)
    {
        _knoten[i].update();

        if (_knoten[i].isAnfang() && _knoten[i].getBesetztmelderstatus())
        {
            resetMarkierungen();
            getKnoten(i)->setMarkierung(true);
            getKnoten(i)->setAnfang(false);
            getKnoten(nextWay(i, getKnoten(i)->getWeg()))->setAnfang(true); // verschiebe den Anfang
            if (getKnoten(i)->getHauptsignal() != nullptr)                  // wenn es ein Hauptsignal gibt
            {
                getKnoten(i)->getHauptsignal()->hauptsignalSchalten(0);
            }
            if (getKnoten(i)->getWeiche() != nullptr)
            {
                getKnoten(i)->getWeiche()->setWeichenfestlegung(false, getKnoten(i)->getWeg());
            }
            getKnoten(i)->setFahrstrassenelement(getKnoten(i)->getWeg(), false); // setze das Fahrstrassenelement zurück
        }
    }
}

boolean Graph::equals(Gleissymbol *symbol1, Gleissymbol *symbol2)
{
    if (symbol1 == symbol2)
        return true;
    else
        return false;
}

int Graph::nextWay(Gleissymbol *symbol, int fahrstrassenNr_vorgaenger, boolean vorgaengerAktiv)
{
    if (getKnotenNr(symbol) < 0)
        return -1; // wenn der Knoten nicht zu finden
    else
        return nextWay(getKnotenNr(symbol), fahrstrassenNr_vorgaenger, vorgaengerAktiv);
}

int *Graph::sucheFlankenschutzweiche(int vorgaenger, int weiche, int nachfolger)
{
    // Flankenfahrtrichtung ermitteln
    int counter = 0;
    int moeglFlankenfahrt = -1;
    for (int i = 0; i < _maxNachbarn; i++)
    {
        if (_nachbarn[weiche][i] == vorgaenger)
        {
            counter++;
        }
        else if (_nachbarn[weiche][i] == nachfolger)
        {
            counter++;
        }
        else
        {
            moeglFlankenfahrt = _nachbarn[weiche][i];
        }
    }
    if (counter != 2)
    {
        return nullptr;
    }

    // ermittle die Flankenschutzweiche
    int vorgaengerKnoten = weiche;
    int aktuellerKnoten = moeglFlankenfahrt;
    while (getKnoten(aktuellerKnoten)->getWeiche() == nullptr)
    {
        if (_nachbarn[aktuellerKnoten][1] < 0)
        { // Dieser Teil kann/muss nicht abgesichert zu werden, da er eine Sackgasse auf dem Stellpult ist.
            return nullptr;
        }
        if (_nachbarn[aktuellerKnoten][0] == vorgaengerKnoten)
        {
            vorgaengerKnoten = aktuellerKnoten;
            aktuellerKnoten = _nachbarn[aktuellerKnoten][1];
        }
        else
        {
            vorgaengerKnoten = aktuellerKnoten;
            aktuellerKnoten = _nachbarn[aktuellerKnoten][0];
        }
    }
    if (!weichenAusgang(vorgaengerKnoten, aktuellerKnoten))
    {
        int *ergebnis = (int *)calloc(3, sizeof(int)); // Achtung mit free, den Speicher abschließend wieder freigeben
        ergebnis[0] = vorgaengerKnoten;
        ergebnis[1] = aktuellerKnoten;
        ergebnis[2] = _nachbarn[aktuellerKnoten][0];
        return ergebnis;
    }
    else
    {
        // Serial.print("Error in sucheFlankenschutzweiche(): Der Flankenschutz eines Weichenknotens konnte nicht bestimmt werden. KnotenNr.:");
        // Serial.println(weiche);
        return nullptr;
    }
}

int Graph::nextWay(int knotenNr, int fahrstrassenNr_vorgaenger, boolean vorgaengerAktiv)
{
    if (isKnotenNr(knotenNr)) // wenn die angegebene KnotenNr eine KnotenNr ist
    {

        boolean fahrstrasse = false;
        if (fahrstrassenNr_vorgaenger > 0 && !vorgaengerAktiv)
            fahrstrasse = true;
        int counter = 0;
        while (counter < _maxNachbarn && _nachbarn[knotenNr][counter] >= 0)
        // suche Solange nach Nachbarn, bis am Ende des Arrays oder bei Wert außerhalb des Werte bereichs oder
        {
            if (_knoten[_nachbarn[knotenNr][counter]].getMarkierung() == false && ((_knoten[_nachbarn[knotenNr][counter]].isFree() && !fahrstrasse) || (fahrstrasse && _knoten[_nachbarn[knotenNr][counter]].getWeg() == fahrstrassenNr_vorgaenger))) // wenn der Knoten nicht markiert ist und frei, oder er wenn gefragt einer Fahrstrasse entspricht
            {
                if (_knoten[knotenNr].getWeiche() != nullptr && vorgaengerAktiv) // wenn aktueller Knoten eine Weiche und vorgaänger aktiv
                {
                    if (weichenAusgang(fahrstrassenNr_vorgaenger, knotenNr) && counter > 0) // wenn aktueller knoten von der kleinen seite der Weiche kommt
                    {
                        return _nachbarn[knotenNr][counter];
                    }
                    else if (!weichenAusgang(fahrstrassenNr_vorgaenger, knotenNr) && counter == 0)
                    {
                        return _nachbarn[knotenNr][0];
                    }
                }
                else
                {
                    return _nachbarn[knotenNr][counter]; // gib den Knoten zurück
                }
            }
            counter++;
        }
    }
    return -1;
}

int Graph::wegSuchen(Gleissymbol *start, Gleissymbol *ziel, Gleissymbol *vorgaenger)
{

    start->setMarkierung(true);
    if (equals(start, ziel)) // wenn am Ziel
    {
        _fahrstrassenzaehler++; // erhöhe den Zähler für die Nummer um 1
        start->setWeg(_fahrstrassenzaehler);
        return 1;
    }
    else
    {
        if (vorgaenger != nullptr) // wenn es einen Vorgänger gibt
        {
            while (nextWay(start, getKnotenNr(vorgaenger), true) >= 0)
            {
                int laenge = wegSuchen(&_knoten[nextWay(start, getKnotenNr(vorgaenger), true)], ziel, start);
                if (laenge > 0)
                {
                    start->setWeg(_fahrstrassenzaehler);
                    return laenge + 1;
                }
            }
        }
        else // wenn es keinen vorgänger gibt
        {
            while (nextWay(start) >= 0)
            {
                int laenge = wegSuchen(&_knoten[nextWay(start)], ziel, start);
                if (laenge > 0)
                {
                    start->setWeg(_fahrstrassenzaehler);
                    return laenge + 1;
                }
            }
        }
        return -1;
    }
}

boolean Graph::fahrstrasseEinstellen(Zugtaste *taste1, Zugtaste *taste2)
{
    if (taste1->getRichtung() == taste2->getRichtung())
    {
        resetMarkierungen();
        int laenge = wegSuchen(taste1->getGleissymbol(), taste2->getGleissymbol());
        resetMarkierungen();

        if (laenge > -1) // wenn eine Fahrstraße gefunden
        {
            // Richtungsüberprüfung:
            // Richtung von links nach rechts
            if ((taste1->getRichtung() && nextWay(taste1->getGleissymbol(), taste1->getGleissymbol()->getWeg()) < getKnotenNr(taste1->getGleissymbol()) &&
                 nextWay(taste2->getGleissymbol(), taste2->getGleissymbol()->getWeg()) > getKnotenNr(taste2->getGleissymbol())) || // wenn die Nummer der nächsten Taste kleiner ist als die Nummer des aktuellen Gleissymbols und die Richtung true ist
                (!taste1->getRichtung() && nextWay(taste2->getGleissymbol(), taste2->getGleissymbol()->getWeg()) < getKnotenNr(taste2->getGleissymbol()) &&
                 nextWay(taste1->getGleissymbol(), taste1->getGleissymbol()->getWeg()) > getKnotenNr(taste1->getGleissymbol())))
            {
                taste2->getGleissymbol()->setAnfang(true);
                symbolZuFahrstrasse(taste2->getGleissymbol());
                return true;
            }
            else if ((taste1->getRichtung() && nextWay(taste1->getGleissymbol(), taste1->getGleissymbol()->getWeg()) > getKnotenNr(taste1->getGleissymbol()) &&
                      nextWay(taste2->getGleissymbol(), taste2->getGleissymbol()->getWeg()) < getKnotenNr(taste2->getGleissymbol())) ||
                     (!taste1->getRichtung() && nextWay(taste2->getGleissymbol(), taste2->getGleissymbol()->getWeg()) > getKnotenNr(taste2->getGleissymbol()) &&
                      nextWay(taste1->getGleissymbol(), taste1->getGleissymbol()->getWeg()) < getKnotenNr(taste1->getGleissymbol())))
            {
                taste1->getGleissymbol()->setAnfang(true);
                symbolZuFahrstrasse(taste1->getGleissymbol());
                return true;
            }
            else
            {
                Serial.println("INFO: Die Richtung der Fahrstraße ist anhand der angegebenen Zugtasten nicht validierbar. Sie wurde nicht eingestellt. (Graph::fahrstrasseEinstellen(Zugtaste, Zugtaste)");
                return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}

boolean Graph::fahrstrasseEinstellen(Gleissymbol *start, Gleissymbol *ziel)
{
    resetMarkierungen();
    int laenge = wegSuchen(start, ziel);
    resetMarkierungen();

    if (laenge > -1) // wenn eine Fahrstraße gefunden
    {
        start->setAnfang(true);
        symbolZuFahrstrasse(start);
        return true;
    }
    else
        return false;
}
void Graph::symbolZuFahrstrasse(Gleissymbol *symbol)
{
    symbolZuFahrstrasse(getKnotenNr(symbol));
}

void Graph::symbolZuFahrstrasse(int knotenNr)
{
    // stelle das Symbol als Fahrstrasse ein
    getKnoten(knotenNr)->setFahrstrassenelement(getKnoten(knotenNr)->getWeg(), true);
    if (nextWay(knotenNr, getKnoten(knotenNr)->getWeg()) != -1 && getKnoten(nextWay(knotenNr, getKnoten(knotenNr)->getWeg()))->getWeiche() != nullptr) // wenn es eine Weiche gibt
    {
        int nextN = nextWay(knotenNr, getKnoten(knotenNr)->getWeg());
        getKnoten(knotenNr)->setMarkierung(true);

        Serial.println("Weiche");
        getKnoten(nextN)->getWeiche()->setWeichenposition(richtungGerade(knotenNr, nextN, nextWay(nextN, getKnoten(nextN)->getWeg()))); // schlate Weiche in die richtige Position
        getKnoten(nextN)->getWeiche()->setWeichenfestlegung(true, getKnoten(knotenNr)->getWeg());

        // Flankenschutzweichen
        int *ergebnis = sucheFlankenschutzweiche(knotenNr, nextN, nextWay(nextN, getKnoten(nextN)->getWeg()));
        if (ergebnis != nullptr)// wenn null zurückgegeben wird, gibt es keine Flankenschutzweichen
        {
            Serial.println(ergebnis[0]);
            Serial.println(ergebnis[1]);
            Serial.println(ergebnis[2]);
            if (_nachbarn[ergebnis[1]][2] == ergebnis[0])
                getKnoten(nextN)->setFlankenschutzweiche(getKnoten(ergebnis[1]), getKnoten(knotenNr)->getWeg(), true); // wenn der Vorgänger über eine kurve Verbunden ist, Schalte die Flankenschutzweiche gerade
            else
                getKnoten(nextN)->setFlankenschutzweiche(getKnoten(ergebnis[1]), getKnoten(knotenNr)->getWeg(), false);
        }
        free(ergebnis);
        getKnoten(knotenNr)->setMarkierung(false);
    }
    if (getKnoten(knotenNr)->getBesetztmelder() != nullptr)
        getKnoten(knotenNr)->getBesetztmelder()->setFahrstrassenelement(getKnoten(knotenNr)->getWeg(), true); // binde den Besetztmelder in die Fahrstraße ein
    if (getKnoten(knotenNr)->getHauptsignal() != nullptr && (nextWay(knotenNr, getKnoten(knotenNr)->getWeg()) > knotenNr) == getKnoten(knotenNr)->getHauptsignal()->getRichtung())
    {
        Serial.println("Hauptsignal");
        getKnoten(knotenNr)->getHauptsignal()->hauptsignalSchalten(2); // Schalte das Signal, wenn vorhanden auf Fahrt
    }

    if (nextWay(knotenNr, getKnoten(knotenNr)->getWeg()) > -1)
    {
        getKnoten(knotenNr)->setMarkierung(true); // setze aktuellen auf Markiert

        symbolZuFahrstrasse(nextWay(knotenNr, getKnoten(knotenNr)->getWeg())); // rekrusiver Aufruf, weiteres Umsetzten der Fahrstraße
    }
}
void Graph::resetMarkierungen(int fahrstrassenNr)
{
    for (int i = 0; i < _anzahlKnoten; i++)
    {
        _knoten[i].setMarkierung(false);
        if (_knoten[i].getWeg() == fahrstrassenNr)
            _knoten[i].setWeg(0);
    }
}

boolean Graph::isReset()
{
    for (int i = 0; i < _anzahlKnoten; i++) // durchlauf jeden Knoten
    {
        if (_knoten[i].getMarkierung() == true || _knoten[i].getWeg() == true)
            return false;
    }
    return true;
}

Gleissymbol *Graph::getKnoten(int knotenNr)
{
    if (isKnotenNr(knotenNr))
        return &_knoten[knotenNr];
    else
    {
        Serial.println("Error: Der Gesuchte Knoten existiert nicht. (Graph.cpp, getKnoten())");
        return nullptr;
    }
}

Gleissymbol *Graph::getNachbar(int knotenNr, int nachbar)
{
    if (isKnotenNr(knotenNr) && nachbar < _maxNachbarn && nachbar >= 0 && _nachbarn[knotenNr][nachbar] > -1)
        return &_knoten[_nachbarn[knotenNr][nachbar]];
    else
    {
        Serial.println("Error: Der gesuchte Nachbar der Knotens existiert nicht. (Graph.cpp, getNachbar())");
        return nullptr;
    }
}

int Graph::getKnotenNr(Gleissymbol *symbol)
{
    int counter = 0;
    while (counter < _anzahlKnoten && !equals(symbol, &_knoten[counter])) // suche nach der Nr des Knotens
    {
        counter++;
    }
    if (counter >= _anzahlKnoten)
        return -1; // wenn kein Knoten gefunden wurde gibt -1 zurück
    else
        return counter;
}

boolean Graph::isKnotenNr(int knotenNr)
{
    if (knotenNr >= 0 && knotenNr < _anzahlKnoten)
        return true;
    else
        return false;
}

boolean Graph::richtungGerade(Gleissymbol *vorgaenger, Gleissymbol *weichensymbol, Gleissymbol *nachfolger)
{
    if (getKnotenNr(vorgaenger) < 0 || getKnotenNr(weichensymbol) < 0 || getKnotenNr(nachfolger) < 0)
    {
        Serial.println("Error: Die angegebenen Knoten sind keine Knoten (Graph::richtungGerade)");
        return false;
    }
    else
        return richtungGerade(getKnotenNr(vorgaenger), getKnotenNr(weichensymbol), getKnotenNr(nachfolger));
}
boolean Graph::richtungGerade(int vorgaenger, int weichensymbolNr, int nachfolger)
{
    if (!isKnotenNr(weichensymbolNr) || !isKnotenNr(vorgaenger) || !isKnotenNr(nachfolger))
        Serial.println("Error: Die angegebene Weichensymbolnummer gehört nicht zum Graphen. (Graph::richtungGerade)");
    if (isKnotenNr(weichensymbolNr) && (_nachbarn[weichensymbolNr][2] == vorgaenger || _nachbarn[weichensymbolNr][2] == nachfolger)) // an Position 2 und die Weichensymbolnummer ist nicht außerhalb des Arrays
        return false;
    else
        return true;
}

boolean Graph::weichenAusgang(int vorgaenger, int aktuellerKnoten) // gibt true wenn zwei wege möglich, false wenn nur einer
{
    int counter = 0;
    while (_nachbarn[aktuellerKnoten][counter] == vorgaenger && counter < _maxNachbarn)
        counter++;
    if (counter == 1)
        return true;
    else
        return false;
}