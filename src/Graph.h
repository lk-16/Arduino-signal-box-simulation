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
    int _anzahlKnoten;
        class Gleissymbol **_speicher;
    public:
    /** Erstellt einen Graphen
     * @param [in] anzahlKnoten Die Anzahl der Knoten wird angegeben.
     * @param [in] speicher Array des Graphen, die Verbindungen sind hier gespeichert.
    */
        Graph(int anzahlKnoten, Gleissymbol speicher[][4]);
        boolean equals(Gleissymbol *symbol1, Gleissymbol *symbol2);
        boolean noWay(int knotenNr);
        boolean wegSuchen(Gleissymbol *start, Gleissymbol *ziel);/**<Die Methode gibt zurück, ob es einen weg gibt und markiert Ihn, wenn möglich*/
        void resetMarkierungen();/**<Setzt alle Markierungen zurück auf False*/
        boolean isReset();/**<Methode zur Überprüfung des Resets derMarkierungen*/
        Gleissymbol * getKnoten(int knotenNr);
        Gleissymbol * getNachbar(int knotenNr, int nachbar = 1);
};
#endif