/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

/**
 * Diese Datei enthält die Klasse besetztmelderControl.
 * @author Lennart Klüner
 * @file HauptsignalControl.h
 */

#ifndef HauptsignalControl_h
#define HauptsignalControl_h
#include "Hauptsignal.h"

/**
 * Über diese Klasse werden die Hauptsignale gesteuert.
 * Die Klasse erstellt Besetztmelder und verwaltet diese, über diese Klasse werden die einzelen Hauptsignale abgefragt. Die Hauptsignale sind von 0 an nummeriert
 **/
class HauptsignalControl
{
private:
  int _anzahlHauptsignale;            // anzahl an Meldern, für die Objekte erstellt wurden
  class Hauptsignal **_pHauptsignale; // dynamisches Array von Objekten der Klasse Hauptsignale, zur steuerung und abfrage von Hauptsignalen

public:
  /** Erstellt die angegebene Anzahl an Objekten der Klasse Hauptsignale und initialisiert diese. Die Initialisierung passiert mit Hilfe der drei Übergebenen Array.
   * Aus der Klasse HauptsignalControl werden die entgegengenommenen Befehle auf die einzelnen Hauptsignale verteilt. Zusätzlich werden alle Signale auf Rot gestellt, um Unfällen vorzugbeugen.
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
  HauptsignalControl(int anzahlHauptsignale, int ledPinsRot[], int ledPinsGelb[], int ledPinsGruen[], int signaltasten[], int sperrmelder[], int allgSignaltasten[3], int registerPin[4], boolean richtung[] = nullptr); // Im Konstruktor wird ein Array von Objekten der Klasse Besetztmelder erstellt und initialisiert
  Hauptsignal *getHauptsignal(int hauptsignalNr);
  void hauptsignalSchalten(int hauptsignalNr, int newStatus); /**<Schaltet das angegebene Signal in die angegebene Postion. (newStatus = 0(Halt), 1(Fahrt), 2(Langsamfahrt) Wenn das Signal gesperrt ist, ist kein schalten möglich*/
  void hauptsignaleHp0Manuell();                              /**<Die Hauptsignale wird mit der Signalhalttaste und der dem Signal zugeortneten Zugstraßentaste auf hp0 (rot) gestellt.*/
  void hauptsignaleSperren();                                 /**<Alle erstellten Hauptsignale können über diese Methode in der main.cpp über Signalsperr/entsperrtaste und Signaltaste, wenn sie in Haltstellung sind gegen umstellen gesperrt werden.*/
  boolean getHauptsignalSperren(int hauptsignalNr);           /**<Gibt aus, ob das angegebene Hauptsignal gesperrt ist.*/
};
#endif