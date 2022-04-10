/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

/**
 * Diese Datei enthält die Basisklasse actors, alle Klassen sind von dieser Klasse abhängig.
 * @author Lennart Klüner
 * @file Actor.h
 */

#ifndef Gleissymbol_h
#define Gleissymbol_h
#include <Arduino.h>
#include "Hauptsignal.h"
#include "Besetztmelder.h"
#include "Weiche.h"
/**
 * Die Klasse actors enthält die Grundfunktionen jeder Klasse. Jede Klase nutzt diese Klasse, von hier wird der Quellcode für das Steuern des Schieberegisters abgerufen, auch die Funktion zum Blinken einer Led ist hier gespeichert.
 */
class Gleissymbol
{
private:
    Hauptsignal * _signal = nullptr;
    Weiche * _weiche =nullptr;
    Besetztmelder * _besetztmelder = nullptr;

public:
    /**
     * Erstellt ein Objekt. welches die Bestandteile eines Gleissymbols enthält
     * @param[in] besetztmelder Besetztmelder, der dem Gleissymbol zugewiesen ist.
     * @param[in] weiche Weiche, die dem Gleissymbol zugewiesen ist.
     * @param[in] signal Signal(Hauptsignal), das dem Gleissymbol zugewiesen ist.
     * @see Besetztmelder.h
     * @see Weiche.h
     * @see Hauptsignal.h
     */
    Gleissymbol(Besetztmelder * besetztmelder, Weiche * weiche, Hauptsignal  * signal);
    Besetztmelder * getBesetztmelder();
    Weiche * getWeiche();
    Hauptsignal * getHausptsignal();
    void update();
    
};
#endif