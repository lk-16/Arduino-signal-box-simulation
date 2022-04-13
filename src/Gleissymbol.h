/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

/**
 * Diese Datei enthält die Klasse Gleissymbol, in dieser wird jedes Objekt eines Gleissymbols gespeichert.
 * @author Lennart Klüner
 * @file Gleissymbol.h
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
    boolean _markiert = false;   //Markierungsstatus des Gleissymbols, true = markiert
    Hauptsignal * _signal = nullptr; //Pointer auf das Hauptsignal des Gleissymbols
    Weiche * _weiche =nullptr;      //Pointer auf die Weiche des Gleissymbols
    Besetztmelder * _besetztmelder = nullptr;   //Pointer auf den Besetztmelder des Gleissymbols

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
    Gleissymbol(Besetztmelder * besetztmelder, Weiche * weiche = nullptr, Hauptsignal  * signal = nullptr);//Konstruktor der Klasse Gleissymbol
    Besetztmelder * getBesetztmelder();             /**<Gibt einen Pointer auf den Besetztmelder zurück.*/
    Weiche * getWeiche();                           /**<Gibt einen Pointer auf den Weiche zurück, sonst null*/
    Hauptsignal * getHausptsignal();                /**<Gibt einen Pointer auf das Hauptsignal zurück, sonst null.*/
    void update();                                  /**<Updatet, wenn vorhanden, die Weichenposition (weicheWechsel, weicheSchalten), signalsperren, besetztmelder*/
    void setMarkierung(boolean status);             /**<true = markiert, false = nicht markiert*/
    boolean getMarkierung();                        /**<Gibt den Status der Markierung zurück.*/
};
#endif