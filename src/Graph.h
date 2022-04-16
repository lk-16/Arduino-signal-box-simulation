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
        int _maxNachbarn = 3;
        int _anzahlKnoten;
        class Gleissymbol *_knoten;
        int **_nachbarn;//2D Array wird im Konstruktor initialisiert Es enthält alle Nachbarnummern, des Knotens in der entsprechenden Zeile
        unsigned int _fahrstrassenzaehler = 0;//die Fahrstraén werden mit steigender Markierung versehen, als Referenz für die nächste Zahl wird fahrstrassenzähler verwendet

    public:
    /** Erstellt einen Graphen
     * @param [in] anzahlKnoten Die Anzahl der Knoten wird angegeben.
     * @param [in] knoten Array mit allen Knoten des Graphen.
     * @param [in] nachbarn Die Verknüpfunge werden mit diesem Array übergeben. nachbarn[knotennr][maxTiefe(beim Gleisbild 3)], alle nicht belegten Plätze müssen mit z.B. -1 belegt sein (außerhalb des Wertebereichs)
    */
        Graph(int anzahlKnoten, Gleissymbol knoten[], int nachbarn[][3]);
        boolean equals(Gleissymbol *symbol1, Gleissymbol *symbol2);/**Gibt zurück, ob Zwei zeiger auf ein Gleissymbol auf das selbe zeigen.*/
        int nextWay(Gleissymbol *symbol);
        int nextWay(int knotenNr);/**<Die Methode gibt die Nr des nächsten nicht markierten Nachbarn der angegebenen Knotennummer zurück*/
        int wegSuchen(Gleissymbol *start, Gleissymbol *ziel);/**<Die Methode gibt die länge eines Weges zurück, ob es einen weg gibt und markiert Ihn, wenn möglich*/
        void resetMarkierungen(int fahrstrassenNr = 0);/**<Setzt alle Markierungen zurück auf False*/
        boolean isReset();/**<Methode zur Überprüfung des Resets derMarkierungen*/
        Gleissymbol * getKnoten(int knotenNr);/**<Gibt den Knoten unter der KnotenNr zurück*/
        Gleissymbol * getNachbar(int knotenNr, int nachbar = 0);/**<Gibt den Nachbarn(nachbar) des Knoten (KnoteNr) zurück*/
};
#endif