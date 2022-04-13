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

boolean Graph::wegSuchen(Gleissymbol *start, Gleissymbol *ziel)
{
    int i;
    while (start == &_speicher[i][0])
    {
        /* code */
    }
    

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
        for(int j = 0; j = _anzahlKnoten; j++)//durchlaufe seine Nachbarn
        {
            if(_speicher[i][j].getMarkierung()==true)
            return false;

        }
    }
    return true;
}
