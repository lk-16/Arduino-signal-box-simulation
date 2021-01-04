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
 * @file zugtasten.h
*/

#ifndef zugtastenControl_h
#define zugtastenControl_h
#include <Arduino.h>
#include "actors.h"


class zugtastenControl
{
private:
    int _anzahlZugtasten;/**<Speichert die Anzahl der Erstellten Objekte der Klasse Zugtasten.*/
    class zugtasten **_pzugtasten;/**<Pointer auf ein Array von objekten der Klasse zugtasten.*/
public:
    zugtastenControl(int anzahlZugtasten, int zugtastenPins[]);/**<Konstruktor, erstellt das Array von objekten nach der angegeben Anzahl.*/
    int getZugtastenAnzahl();/**<gibt die Anzahl an erstellten Objekten der Klasse zugtasten an.*/
    boolean getZugtastenstatus(int zugtastenNr);/**<Gibt den den Status der angegeben Zugtaste aus.*/
};

#endif