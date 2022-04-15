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
    if(symbol1 == symbol2)return true;
    else return false;
}

int Graph::nextWay(int knotenNr)
{
    int counter = 0;
    while(counter<_maxNachbarn && _nachbarn[knotenNr][counter] >= 0)
    //suche Solange nach Nachbarn, bis am Ende des Arrays oder bei Wert außerhalb des Werte bereichs oder 
    {
        if(_knoten[_nachbarn[knotenNr][counter]].getMarkierung() == false)
        {
            return _nachbarn[knotenNr][counter];
        }
        counter++;
        
    }
    return -1;
}

boolean Graph::wegSuchen(Gleissymbol *start, Gleissymbol *ziel)
{
    if (equals(start, ziel))
    {
        start->setMarkierung(true);
        return true;
    }
    else
    {
        if (start->getMarkierung())
        {
            return false;
        }

        // suche die eigenen Verbindungen
        int knotenNr = 0;                                                          // counter
        while (!(knotenNr >= _anzahlKnoten) && !equals(&_knoten[knotenNr], start)) // mach solange i nicht größer ist als anzahl Knoten und solange _speicher und start nicht gleich sind
        {
            knotenNr++;
        }
        //if (noWay(knotenNr)) // wenn kein weiterer weg
        {
            return false;
        }
        /*else//wenn es einen weg gibt
        {
            //durchsuche jede Ecke
            int k = 0;
            while(_speicher[knotenNr][k] != nullptr)
            {

            }
        }*/
    }

    /*
        if start == ziel
            markiere
            return true

        else wenn nicht amziel
            wenn schon markiert
                return false
            else
                markieren das eigene Gleissymbol

            position des Gleissymbols start suchen im Array
            rekusion:
            rufe verbindungen mit (start=selbst, ziel =) auf

    */
    return true;
}

void Graph::resetMarkierungen()
{
    for (int i = 0; i < _anzahlKnoten; i++)
    {
        _knoten[i].setMarkierung(false);
    }
}

boolean Graph::isReset()
{
    for (int i = 0; i < _anzahlKnoten; i++) // durchlauf jeden Knoten
    {
        if (_knoten[i].getMarkierung() == true)
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
