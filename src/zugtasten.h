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

#ifndef zugtasten_h
#define zugtasten_h
#include <Arduino.h>
#include <Schieberegister.h>
#include "actors.h"

/**Die Klasse gibt weiter ob Zugtasten gedrückt wurden.
 * Die Klasse speichert den Pin der Zugtaste, und gibt ihren Status aus
 */
class zugtasten
{
private:
  //Pins
  int _weckerPin;
  int _zugtastenPin;

public:
  /** Übergibt alle Pin die für die Zugtaste wichtig sind. Alle 
   * @param[in] zugtastenPin Der Pin, an dem der Taster für die Zugtaste angeschlossen ist. (Pullup-Wiederstand nicht vergessen 1kOhm)
  */
  zugtasten(int zugtastenPin); 
  boolean getzugtastenstatus();                     /**<Gibt den Status der Zugtasten aus(HIGH = gedrückt, LOW = ungedrückt*/
};

#endif