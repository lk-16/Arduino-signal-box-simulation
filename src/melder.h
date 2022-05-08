/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

/**
 * Diese Datei enthält die Klasse Melder
 * @author Lennart Klüner
 * @file Melder.h
 */

#ifndef Melder_h
#define Melder_h
#include <Arduino.h>
#include "Actor.h"
#include "ZugtastenControl.h"

/**In der Klasse Melder werden Melder und deren Funktionen erstellt. Die Klasse enthält zur Zeit nur den Programmcode für die Tastenüberwachung.
 */
class Melder : public Actor
{
private:
  String _melderName;
  int _weckerPin;
  int _tueMelderLed;
  int _wutPin;

  unsigned int _melderTimeout = 5000;
  unsigned long _melderStartzeit;
  unsigned long _weckerStartzeit;

  boolean _tueMelderStatus;
  boolean _wutAktivierung;

public:
  /** Übergibt alle Pin die für die Zugtaste wichtig sind.
   * @param[in] melderName Name des Melder. Dieser Wird über den seriellen Monitor angezeigt, wenn der Melder auslöst.
   * @param[in] tueMelderLed Gibt die Led an, über die der Melder u.a ausschlägt.
   * @param[in] weckerPin Gibt den Pin an, über die der Wecker für den Melder ausschlägt.
   * @param[in] wutPin Eingabe des Pins der Weckerunterbrechertaaste.
   * @param[in] registerPin Die Pins aus dem Array werden an die Oberklasse actors übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
   * @see actors(int anzahl, int sh, int st, int ds)
   */
  Melder(String melderName, int tueMelderLed, int weckerPin, int wutPin, int registerPins[4]); // definieren der Pins von Meldern
  void tueMelder(ZugtastenControl ZugtastenControl);                                           /**<Löst nach 5 sec drücken einen optischen Melder und nach weiteren 5 den Wecker aus.*/
};
#endif