/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 10.04.2022
 */

/**
 * Diese Datei enthält die Klasse Graph. In dieser werden die Gleissymbole gespeichert.
 * @author Lennart Klüner
 * @file Graph.h
 */

#ifndef Graph_h
#define Graph_h
#include <Arduino.h>
#include <Actor.h>
#include "Gleissymbol.h"

/**Die Klasse Graph speichert alle Gleissymbole in einem Graphen.
 */
class Graph
{
    private:
        int maxNachbarn = 3;
        int _anzahlKnoten;
        class Gleissymbol *_knoten;
        int **_nachbarn;

    public:
    /** Erstellt einen Graphen
     * @param [in] anzahlKnoten Die Anzahl der Knoten wird angegeben.
     * @param [in] knoten Array mit allen Knoten des Graphen.
    */
        Graph(int anzahlKnoten, Gleissymbol knoten[], int nachbarn[][3]);
        boolean equals(Gleissymbol *symbol1, Gleissymbol *symbol2);
        boolean noWay(int knotenNr);
        boolean wegSuchen(Gleissymbol *start, Gleissymbol *ziel);/**<Die Methode gibt zurück, ob es einen weg gibt und markiert Ihn, wenn möglich*/
        void resetMarkierungen();/**<Setzt alle Markierungen zurück auf False*/
        boolean isReset();/**<Methode zur Überprüfung des Resets derMarkierungen*/
        Gleissymbol * getKnoten(int knotenNr);/**<Gibt den Knoten unter der KnotenNr zurück*/
        Gleissymbol * getNachbar(int knotenNr, int nachbar = 0);/**<Gibt den Nachbarn(nachbar) des Knoten (KnoteNr) zurück*/
};
#endif