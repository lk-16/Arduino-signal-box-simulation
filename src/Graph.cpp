/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 10.04.2022
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
        /*if (_knoten[i].isAnfang() && _knoten[i].getBesetztmelderstatus()) // wenn anfang und der Besetztmelder besetzt ist.
        {
            if (_knoten[i].getWeiche() != nullptr) // wenn es eine Weiche gibt
            {
                Serial.println("Weiche");
                getKnoten(i)->getWeiche()->setWeichenfestlegung(false, getKnoten(i)->getWeg());
            }
            if (getKnoten(i)->getBesetztmelder() != nullptr)//wenn es einen Besetztmelder gibt
                getKnoten(i)->getBesetztmelder()->setFahrstrassenelement(getKnoten(i)->getWeg(), false); // binde den Besetztmelder in die Fahrstraße ein
            if (getKnoten(i)->getHauptsignal() != nullptr && (nextWay(i, getKnoten(i)->getWeg()) > i) == getKnoten(i)->getHauptsignal()->getRichtung())
            {
                getKnoten(i)->getHauptsignal()->hauptsignalSchalten(0); // Schalte das Signal, wenn vorhanden auf Halt
            }
            _knoten[i].setAnfang(false);
            _knoten[i].setFahrstrassenelement(_knoten[i].getWeg(), false);
            _knoten[i].setWeg(0);
            _knoten[i].setMarkierung(true);
            getKnoten(nextWay(i,getKnoten(i)->getWeg()))->setAnfang(true);
        }*/
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
        //if (vorgaenger == nullptr) // wenn es keinen Vorgänger gibt
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
        /*else // wenn es einen vorgänger gibt
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
        }*/
        return -1;
    }
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
    Serial.println(knotenNr);
    // stelle das Symbol als Fahrstrasse ein
    getKnoten(knotenNr)->setFahrstrassenelement(getKnoten(knotenNr)->getWeg(), true);
    if (getKnoten(knotenNr)->getWeiche() != nullptr) // wenn es eine Weiche gibt
    {
        Serial.println("Weiche");
        Serial.println(richtungGerade(knotenNr, nextWay(knotenNr, getKnoten(knotenNr)->getWeg())));
        getKnoten(knotenNr)->getWeiche()->setWeichenposition(richtungGerade(knotenNr, nextWay(knotenNr, getKnoten(knotenNr)->getWeg()))); // schlate Weiche in die richtige Position
        getKnoten(knotenNr)->getWeiche()->setWeichenfestlegung(true, getKnoten(knotenNr)->getWeg());
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
        getKnoten(knotenNr)->setMarkierung(true);

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

boolean Graph::richtungGerade(Gleissymbol *weichensymbol, Gleissymbol *nachbar)
{
    if (getKnotenNr(weichensymbol) < 0 || getKnotenNr(nachbar) < 0)
        return -1;
    else
        return richtungGerade(getKnotenNr(weichensymbol), getKnotenNr(nachbar));
}
boolean Graph::richtungGerade(int weichensymbolNr, int nachbarNr)
{
    if (_nachbarn[weichensymbolNr][2] == nachbarNr)
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