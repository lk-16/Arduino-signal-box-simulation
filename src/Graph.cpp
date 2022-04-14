/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 10.04.2022
*/

#include "Graph.h"

Graph::Graph(int anzahlKnoten, Gleissymbol speicher[/*anzahlKnoten*/][4])
{
    _anzahlKnoten = anzahlKnoten;
    _speicher = new Gleissymbol *[_anzahlKnoten]; // erstelle ein Pointer auf ein Array
    for (int i = 0; i < _anzahlKnoten; i++)       // weise den Parameter speicher _speicher zu
    {
        _speicher[i] = speicher[i];
    }
}

boolean Graph::equals(Gleissymbol *symbol1, Gleissymbol *symbol2)
{
    return true;
}

boolean Graph::noWay(int KnotenNr)
{
    return true;
}

boolean Graph::wegSuchen(Gleissymbol *start, Gleissymbol *ziel)
{
    if(equals(start,ziel))
    {
        start->setMarkierung(true);
        return true;
    }
    else
    {
        if(start->getMarkierung())
        {
            return false;
        }
        
        
        //suche die eigenen Verbindungen
        int knotenNr=0;//counter
        while(!(knotenNr >=_anzahlKnoten) && !equals(&_speicher[knotenNr][0],start))//mach solange i nicht größer ist als anzahl Knoten und solange _speicher und start nicht gleich sind
        {
            knotenNr++;
        }
        if(noWay(knotenNr))//wenn kein weiterer weg
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
        _speicher[i][0].setMarkierung(false);
    }
}

boolean Graph::isReset()
{
    for(int i = 0; i < _anzahlKnoten; i++)//durchlauf jeden Knoten
    {
        for(int j = 0; j < 4; j++)//durchlaufe seine Nachbarn...................................................................................................................................
        {
            if(_speicher[i][j].getMarkierung()==true)
            return false;

        }
    }
    return true;
}

Gleissymbol * Graph::getKnoten(int knotenNr)
{
    if(knotenNr<_anzahlKnoten && knotenNr >= 0)
    return &_speicher[knotenNr][0];
    else
    {
        Serial.println("Error: Der Gesuchte Knoten existiert nicht. (Graph.cpp, getKnoten())");
        return nullptr;
    }
}

Gleissymbol * Graph::getNachbar(int knotenNr, int nachbar = 1)
{
    if(knotenNr<_anzahlKnoten && knotenNr >= 0 &&  nachbar < 4 && nachbar > 0)
    return &_speicher[knotenNr][nachbar];
    else
    {
        Serial.println("Error: Der gesuchte Nachbar der Knotens existiert nicht, die Zahl liegt nicht im Wertebereich zwischen 1 und 4. (Graph.cpp, getNachbar())");
        return nullptr;
    }
}
