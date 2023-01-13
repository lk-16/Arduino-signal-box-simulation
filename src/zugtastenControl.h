/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner erstellt am 05.09.2020
 */

/**
 * Diese Datei beinhaltet die Klasse zugtasten.
 * @author Lennart Klüner
 * @file ZugtastenControl.h
 */

#ifndef ZugtastenControl_h
#define ZugtastenControl_h
#include <Arduino.h>
#include "Actor.h"
#include "Gleissymbol.h"

class ZugtastenControl
{
private:
    int _anzahlZugtasten;         /**<Speichert die Anzahl der Erstellten Objekte der Klasse Zugtasten.*/
    class Zugtaste **_pzugtasten; /**<Pointer auf ein Array von objekten der Klasse zugtasten.*/
public:
    ZugtastenControl(int anzahlZugtasten, int zugtastenPins[], boolean richtung[] = nullptr); /**<Konstruktor, erstellt das Array von objekten nach der angegeben Anzahl.*/
    int getZugtastenAnzahl();                                                                 /**<gibt die Anzahl an erstellten Objekten der Klasse zugtasten an.*/
    boolean getZugtastenstatus(int zugtastenNr);                                              /**<Gibt den den Status der angegeben Zugtaste aus.*/
    boolean zugtastenGedrueckt();
    void setGleissymbol(int zugtastenNr, Gleissymbol *symbol);
    Gleissymbol *getGleissymbol(int zugtastenNr);
    Zugtaste *getZugtaste(int zugtastenNr);
    boolean isZugtaste(int zugtastenNr);
};

#endif