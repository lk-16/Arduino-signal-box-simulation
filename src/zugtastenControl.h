/*Libary zum testen:
 *  Ich möchte Weicehn über diese Libary schalten und
 *  vielleicht auch Signale für die Modelleisenbahn
 *  Lennart Klüner 05.09.2020
 *  gesamt: ca. 12.000 Zeichen
 *  ca.  1.600 Worte
 * 
 * Auto-Layout: Strg k, Strg f
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


class ZugtastenControl
{
private:
    int _anzahlZugtasten;/**<Speichert die Anzahl der Erstellten Objekte der Klasse Zugtasten.*/
    class Zugtaste **_pzugtasten;/**<Pointer auf ein Array von objekten der Klasse zugtasten.*/
public:
    ZugtastenControl(int anzahlZugtasten, int zugtastenPins[]);/**<Konstruktor, erstellt das Array von objekten nach der angegeben Anzahl.*/
    int getZugtastenAnzahl();/**<gibt die Anzahl an erstellten Objekten der Klasse zugtasten an.*/
    boolean getZugtastenstatus(int zugtastenNr);/**<Gibt den den Status der angegeben Zugtaste aus.*/
    boolean zugtastenGedrueckt();
};

#endif