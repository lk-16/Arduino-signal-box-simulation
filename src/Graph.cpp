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

boolean Graph::equals(Gleissymbol *symbol1, Gleissymbol *symbol2)
{
    if (symbol1 == symbol2)
        return true;
    else
        return false;
}

int Graph::nextWay(Gleissymbol *symbol)
{
    int counter = 0;
    while (counter < _anzahlKnoten && !equals(symbol, &_knoten[counter])) // suche nach der Nr des Knotens
    {
        counter++;
    }
    if (counter >= _anzahlKnoten)
        return -1; // wenn kein Knoten gefunden wurde gibt -1 zurück
    else
        return nextWay(counter);
}

int Graph::nextWay(int knotenNr)
{
    int counter = 0;
    while (counter < _maxNachbarn && _nachbarn[knotenNr][counter] >= 0)
    // suche Solange nach Nachbarn, bis am Ende des Arrays oder bei Wert außerhalb des Werte bereichs oder
    {
        if (_knoten[_nachbarn[knotenNr][counter]].getMarkierung() == false && _knoten[_nachbarn[knotenNr][counter]].isFree())//wenn der Knoten nicht markiert ist und frei
        {
            return _nachbarn[knotenNr][counter];//gib den Knoten zurück
        }
        counter++;
    }
    return -1;
}

int Graph::wegSuchen(Gleissymbol *start, Gleissymbol *ziel)
{
    start->setMarkierung(true);
    if (equals(start, ziel)) // wenn am Ziel
    {
        _fahrstrassenzaehler++;//erhöhe den Zähler für die Nummer um 1
        start->setWeg(_fahrstrassenzaehler);
        return 1;
    }
    else
    {
        boolean found = false; // wenn rückgabe von weg suchen true, dann wurde ein Weg gefunden
        while (nextWay(start) >= 0 && !found)
        {
            int laenge = 0;
            laenge = wegSuchen(&_knoten[nextWay(start)], ziel);
            if(laenge > 0)
            {
                start->setWeg(_fahrstrassenzaehler);
                found = true;
                return laenge +1;
            }
        }
        return -1;
    }
}

void Graph::resetMarkierungen(int fahrstrassenNr)
{
    for (int i = 0; i < _anzahlKnoten; i++)
    {
        _knoten[i].setMarkierung(false);
        if(_knoten[i].getWeg()== fahrstrassenNr)
        _knoten[i].setWeg(0);
    }
}

boolean Graph::isReset()
{
    for (int i = 0; i < _anzahlKnoten; i++) // durchlauf jeden Knoten
    {
        if (_knoten[i].getMarkierung() == true || _knoten[i].getWeg()== true)
            return false;
    }
    return true;
}

Gleissymbol *Graph::getKnoten(int knotenNr)
{
    if (knotenNr < _anzahlKnoten && knotenNr >= 0)
        return &_knoten[knotenNr];
    else
    {
        Serial.println("Error: Der Gesuchte Knoten existiert nicht. (Graph.cpp, getKnoten())");
        return nullptr;
    }
}

Gleissymbol *Graph::getNachbar(int knotenNr, int nachbar)
{
    if (knotenNr < _anzahlKnoten && knotenNr >= 0 && nachbar < _maxNachbarn && nachbar >= 0 && _nachbarn[knotenNr][nachbar] > -1)

        return &_knoten[_nachbarn[knotenNr][nachbar]];
    else
    {
        Serial.println("Error: Der gesuchte Nachbar der Knotens existiert nicht. (Graph.cpp, getNachbar())");
        return nullptr;
    }
}
