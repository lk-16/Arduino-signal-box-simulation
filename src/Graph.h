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
#include "List.h"
#include "Zugtaste.h"

/**Die Klasse Graph speichert alle Gleissymbole in einem Graphen.
 */
class Graph
{
private:
    List _aktivFahrstrassen;
    int _maxNachbarn = 3;
    int _anzahlKnoten;
    class Gleissymbol *_knoten;
    int **_nachbarn;                       // 2D Array wird im Konstruktor initialisiert Es enthält alle Nachbarnummern, des Knotens in der entsprechenden Zeile
    unsigned int _fahrstrassenzaehler = 0; // die Fahrstraén werden mit steigender Markierung versehen, als Referenz für die nächste Zahl wird fahrstrassenzähler verwendet

public:
    /** Erstellt einen Graphen
     * @param [in] anzahlKnoten Die Anzahl der Knoten wird angegeben.
     * @param [in] knoten Array mit allen Knoten des Graphen. Die Verbindungen, die über Kurvenstellung erreicht werden, müssen am Ende stehen!!
     * @param [in] nachbarn Die Verknüpfunge werden mit diesem Array übergeben. nachbarn[knotennr][maxTiefe(beim Gleisbild 3)], alle nicht belegten Plätze müssen mit z.B. -1 belegt sein (außerhalb des Wertebereichs)
     */
    Graph(int anzahlKnoten, Gleissymbol knoten[], int nachbarn[][3]);
    void prepare();                                                                                       /**<Die Weichenrelais werden auf HIGH gestellt und die Weichenposition wird aus dem EEPROM abgerufen und ausgeführt. Die Methode wird am Anfang benötigt, so wird sicher gestellt, das alle Weichen in der vom Stellpult ange-nommenen Lage sind*/
    void updateSymbole();                                                                                 /**>Alle Symbole im Graphen werden aktualisiert, die Besetztmelder, Weichen, Signale. Funktionen wie Hp= manuell*/
    boolean equals(Gleissymbol *symbol1, Gleissymbol *symbol2);                                           /**<Gibt zurück, ob Zwei zeiger auf ein Gleissymbol auf das selbe zeigen.*/
    int nextWay(Gleissymbol *symbol, int fahrstrassenNr_vorgaenger = 0, boolean vorgaengerAktiv = false); /**<Die Methode gibt die Nr des nächsten nicht markierten Nachbarn der angegebenen Knotennummer zurück, wenn angegeben, den Knoten, der mit der FahrstrassenNr versehen ist. WEnn vorgaenger Aktiv, wird statt nach einer Fahrstraße, der Vorgänger des aktuellen Knotens übergeben, so kann ein logisch Sinnvoller WEg zum Beispiel bei WEichen bestimmt werden.*/
    int nextWay(int knotenNr, int fahrstrassenNr_vorgeaenger = 0, boolean vorgaengerAktiv = false);       /**<Die Methode gibt die Nr des nächsten nicht markierten Nachbarn der angegebenen Knotennummer zurück, wenn angegeben, den Knoten, der mit der FahrstrassenNr versehen ist. WEnn vorgaenger Aktiv, wird statt nach einer Fahrstraße, der Vorgänger des aktuellen Knotens übergeben, so kann ein logisch Sinnvoller WEg zum Beispiel bei WEichen bestimmt werden.*/
    int wegSuchen(Gleissymbol *start, Gleissymbol *ziel, Gleissymbol *vorgaenger = nullptr);              /**<Die Methode gibt die länge eines Weges zurück, ob es einen weg gibt und markiert Ihn, wenn möglich, zuvor müssen die Markierungen resetet (resetMarkierungen) werden. Der Vorgaenger wird nur für die korrekte Weichenposition bei Gleissymbolen mit WEichenbnötigt.*/
    boolean fahrstrasseEinstellen(Zugtaste *taste1, Zugtaste *taste2);                                    /**<Stellt die Fahrstraße von der ersten zur zweiten Zugtaste ein. Die Reihenfolge der Zugtasten wird hierbei selbstständig bestimmt.*/
    boolean fahrstrasseEinstellen(Gleissymbol *start, Gleissymbol *ziel);                                 /**<Der Weg zwischen den beiden Gleissymbolen wird gesucht, Die Reihenfolge ist dabei festgelegt und wird nicht überprüft.*/
    void symbolZuFahrstrasse(Gleissymbol *symbol);                                                        /**<setzt die Fahrstraße beginnend mit dem Symbol um*/
    void symbolZuFahrstrasse(int knotenNr);                                                               /**<setzt die Fahrstraße beginnend mit dem Symbol um*/
    void resetMarkierungen(int fahrstrassenNr = 0);                                                       /**<Setzt alle Markierungen zurück auf False*/
    boolean isReset();                                                                                    /**<Methode zur Überprüfung des Resets derMarkierungen*/
    Gleissymbol *getKnoten(int knotenNr);                                                                 /**<Gibt den Knoten unter der KnotenNr zurück*/
    Gleissymbol *getNachbar(int knotenNr, int nachbar = 0);                                               /**<Gibt den Nachbarn(nachbar) des Knoten (KnoteNr) zurück*/
    int getKnotenNr(Gleissymbol *symbol);                                                                 /**<Git die Knotennr zum übergebenen Objekt zurück. Wenn nicht im Graph zu finden return -1*/
    boolean isKnotenNr(int knotenNr);                                                                     /**<validiert, ob die übergebene Knoten Nummer existiert*/
    boolean richtungGerade(Gleissymbol *vorgaenger, Gleissymbol *weichensymbol, Gleissymbol *nachfolger); /**<Gibt zurück, ob die Richtung zwischen zwei Nachbarn bei gerade oder bei einer Weiche als Kurve verläuft.*/
    boolean richtungGerade(int vorgaenger, int weichensymbolNr, int nachfolger);                          /**<Gibt zurück, ob die Richtung zwischen zwei Nachbarn bei gerade oder bei einer Weiche als Kurve verläuft.*/
    boolean weichenAusgang(int vorgaenger, int aktuellerKnoten);                                          /**<Kommt die Fahrstraße über einen Weichenausgang auf die Weiche false, über den Eingang, also den Teil über den man zwei Abzweige nehmen kan, dann true*/
};
#endif