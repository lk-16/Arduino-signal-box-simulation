/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
*/

/**
 * Diese Datei enthält die Klasse hauptsignale.
 * @author Lennart Klüner
 * @file hauptsignale.h
*/

#ifndef hauptsignale_h
#define hauptsignale_h
#include "signale.h"
/**
 * Die Klasse Hauptsignale erzeugt ein Hauptsignal.
 * In der Klasse Hauptsignale lassen sich Signale schalten und über Signaltaste und Signalgruppentaste auf Hp0 schalten.
**/
class hauptsignale : public signale
{
private:
  int _rotPin; //Anschlusspins des Signals
  int _gelbPin;
  int _gruenPin;

  void setSignalHp0(); /**<Die Stati der Signale werden visuell und die Variable über die Methode signale::setSignalstatus() verändert, in hpSchalten können sie von außerhalb der Klasse verändert werden*/
  void setSignalHp1();
  void setSignalHp2();

public:
/** Für die Erstellung eines Hauptsignals.
   * @param[in] rotPin Der Pin an dem die Signaltaste, bzw. die zugehörige Zugtaste (beides Identisch) angeschlossen ist.
   * @param[in] gelbPin Pin an dem eine Led angeschlossn ist, die anzeigt, ob das Signal gesperrt oder entsperrt ist.
   * @param[in] gruenPin Das Array besteht aus Signalsperr- und entsperrtaste und Signalhaltgruppentaste bzw. den dazugehörigen Anschlüssen.
   * @param[in] signaltaste Der Pin an dem die Signaltaste, bzw. die zugehörige Zugtaste (beides Identisch) angeschlossen ist. Wird an die Klasse Signale übergeben.
   * @param[in] sperrmelder Pin an dem eine Led angeschlossn ist, die anzeigt, ob das Signal gesperrt oder entsperrt ist. Wird an die Klasse Signale übergeben.
   * @param[in] allgSignaltasten Dieses Array wird für den Konstruktor der Oberklasse Signale benötigt. Das Array besteht aus Signalsperr- und entsperrtaste und Signalhaltgruppentaste bzw. den dazugehörigen Anschlüssen.
   * @param[in] registerPin Die Pins aus dem Array werden an die Oberklasse signale/actors übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
   * @see actors(int anzahl, int sh, int st, int ds)
   * @see signale(int signaltastenPin, int sperrmelderPin, int allgSignaltasten[3], int registerPins[4]);
  */
  hauptsignale(int rotPin, int gelbPin, int gruenPin, int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4]); //alle Informationen zur Ansteuereung eines Hauptsignals.
  void hpschalten(int newStatus);                             /**<Schaltet das Signal in die angegebene Postion. (newStatus = 0(Halt), 1(Fahrt), 2(Langsamfahrt) Wenn das Signal gesperrt ist, ist kein schalten möglich*/
  void hauptsignalhp0manuell();                               /**<Das Hauptsignal wird mit der Signalhalttaste und der dem Signal zugeortneten Zugstraßentaste auf hp0 (rot) gestellt.*/
};

#endif