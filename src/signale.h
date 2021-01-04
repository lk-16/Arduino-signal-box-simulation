/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
*/

/**
 * Diese Datei enthält die Klasse signale.
 * @author Lennart Klüner
 * @file signale.h
*/

#ifndef signale_h
#define signale_h
#include <Arduino.h>
#include "actors.h"

/**
 * Die Klasse Signale enthält die Grundfunktionen jedes Signals.
 * Die Klasse speichert die Signalhaltgruppentaste und Signalsperrtaste. Über sie lässt sich der Signalstatus ausgeben.
 * Das Signal kann auf Halt gestellt und gesperrt werden. 
*/
class signale : public actors
{
private:
  int _signalstatus;           //0 = Hp0, 1 = Hp1, 2= Hp2 (rot, Fahrt, Langsamfahrt), je nach Signal auch anders vergeben
  int _signalhaltgruppentaste; //in Kombination aus Zug oder Rangiertaste am Feld kann das Signal auf Halt gestellt werden
  int _signaltaste;            //Taste die auf dem selben Tischfeld wie das Sginal ist
  int _signalSperrmelder;      //Pin des  Melder der anzeigt ob ein Signal gesperrt ist oder nicht
  int _signalsperrtaste;       //speichert den Pin der Signalsperrtaste
  int _signalentsperrtaste;    //speichert den Pin der Siganlentsperrtaste
  boolean _signalsperre;       //zeigt an ob das Signal gesperrt ist oder nicht (true Signalsperre aktiv)

protected:
  void setSignalstatus(int newSignalStatus); /**<Veränderung des Signalstatus. Dieser kann nur durch Unterklassen der Klasse Signale geändert werden, zum Beispiel die Klasse Hauptsignale.*/

public:
  /** Für die Erstellung eines Signals.
   * @param[in] signaltastenPin Der Pin an dem die Signaltaste, bzw. die zugehörige Zugtaste (beides Identisch) angeschlossen ist.
   * @param[in] sperrmelderPin Pin an dem eine Led angeschlossn ist, die anzeigt, ob das Signal gesperrt oder entsperrt ist.
   * @param[in] allgSignaltasten Das Array besteht aus Signalsperr- und entsperrtaste und Signalhaltgruppentaste bzw. den dazugehörigen Anschlüssen.
   * @param[in] registerPin Die Pins aus dem Array werden an die Oberklasse actors übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
   * @see actors(int anzahl, int sh, int st, int ds)
  */
  signale(int signaltastenPin, int sperrmelderPin, int allgSignaltasten[3], int registerPins[4]);
  int getSignalstatus();                                  /**<Ausgabe des Signalstatus, dem Signal, das gerade angezeigt wird. Ausgabe: 0, 1, 2(rot, Fahrt, Langsamfahrt)*/
  int getSignaltaste();                                 /**<Gibt den Pin der Signaltaste aus. Wird von der Klasse Hauptsignale verwendet.*/
  int getSignalhaltgruppentaste();                        /**<Gibt den Pin der Signalhaltgruppentaste aus.*/
  int getSignalsperrmelder();                             /**<Gibt den Pin des Signalsperrmelders aus.*/
  int getSignalsperrtaste();                              /**<Gibt den Pin der Signalsperrtaste aus.*/
  boolean getSignalsperre();                              /**<Gibt aus, ob das Signal gesperrt ist.*/
  void signalSperren();                                   /**<Überprüft ob ein Signal gesperrt (nur wenn Signal auf hp0 steht) oder eintsperrt werden kann und tut es wenn möglich.*/
};

#endif