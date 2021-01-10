/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
*/

/**
 * Diese Datei enthält die Klasse besetztmelderControl.
 * @author Lennart Klüner
 * @file WeichenControl.h
*/

#ifndef WeichenControl_h
#define WeichenControl_h
#include "Weiche.h"
#include "Besetztmelder.h"

/**
 * Über diese Klasse werden die Weichen gesteuert.
 * Die Klasse erstellt Weichen und verwaltet diese, über diese Klasse werden die einzelen weichen geschaltet und abgefragt werden. Die Besetztmelder sind von 0 an nummeriert
*/
class WeichenControl
{
private:
  int _anzahlWeichen;      //anzahl an Meldern, für die Objekte erstllt wurden
  class Weiche **_pWeiche; //dynamisches Array von Objekten der Klasse besetztmelder, zur steuerung und abfrage von besetztmeldern
  void setWeicheBesetzt(int WeichenNr, boolean besetztmelderstatus);

public:
  /**Diese Klasse erstellt nach der angegebenen Menge Objekte der Klasse Weiche und initialisiert diese. 
 * @param[in] anzahlWeichen Speichert in einem Array die Anzhal der Weichen die erstellt werden sollen.
 * @param[in] weichenPinGerade Das Array speichert die Anschlüsse des Relais, das die Weiche gerade stellt.
 * @param[in] weichenPinKurve Das Array speichert die Anschlüsse des Relais, das die Weiche in Kurvenlage stellt.
 * @param[in] weichenLedPinGerade Das Array speichert den Anschluss der Led, die die Gerade Lage der Weiche anzeigt.
 * @param[in] weichenLedPinKurve Das Array speichert den Anschluss der Led, die die Gerade Lage der Weiche anzeigt.
 * @param[in] adressWeichenposition Das Array bestimmt den speicherort an dem die Weichenposition gespeichert werden soll.
 * @param[in] weichentimeout Bestimmt wie lange die Weiche geschaltet wird. 
 * @param[in] wt Das Array deklariert den Eingang an dem die Weichentaste angeschlossen ist.
 * @param[in] wgt Übergibt den AnschlussPin der Weichengruppentaste.
 * @param[in] registerPins Die Pins aus dem Array werden an die Klasse Weiche und von dieser an die Oberklasse Actor übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
*/
  WeichenControl(int anzahlweichen, int weichenPinGerade[], int weichenPinKurve[], int weichenLedGerade[], int weichenLedGeradeRot[], int weichenLedKurve[], int weichenLedKurveRot[], int adressWeichenposition[], int weichentimeout, int wt[], int wgt, int registerPins[4]); //Im Konstruktor wird ein Array von Objekten der Klasse Weichen erstellt und initialisiert
  void weichenWechseln();                                                                                                                                                                                                         /**<Funktion zum Wechseln der Weiche mit Weichengruppentaste und Weichentaste in Kombination. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weichenStellen(int weichenNr, boolean weichenlage);                                                                                                                                                                        /**<Stellt die angegebene Weiche. true = gerade, false = kurve*/
  void weichenGerade(int weichenNr);                                                                                                                                                                                              /**<Funktion um die Weiche in Geradeweichenlage zu versetzen. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weichenKurve(int weichenNr);                                                                                                                                                                                               /**<Funktion um die Weiche in Kurvenlage zu versetzten. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weichenRelaisHIGH();                                                                                                                                                                                                       /**<Alle Weichenrelais werden auf HIGH gesetzt. Die Relais sind alle inaktiv.*/
  void weichenRelaisLOW();                                                                                                                                                                                                        /**<Alle Weichenrelais werden auf LOW gesetzt. Die Relais sind alle aktiv.*/
  void weichenpositionenEEPROM();                                                                                                                                                                                                 /**<Die Weichenposition wird dauerhaft im EEPROM gespeichert. Dafür wird sie geupdatet, wenn eine Weiche geschaltet wurde*/
  void weichenSchalten();                                                                                                                                                                                                         /**<Beendet nach der zuvor im Konstruktor definierten Zeit das Schalten des Relais.(je nach Weichenantrieb und Schaltdauer) Methode muss für jedes Objekt der Klasse einmal im Loop vorhanden sein.*/
  void setWeichenfestlegung(int weichenNr, int fahrstrassenNr, boolean festlegestatus); /**<Kann die Festlegung der Weichen aktivieren. Die Weichenlage kann nicht mehr verändert werden, bis die Festlegung durch die Fahrstraße ausfgelöst wird.*/
  boolean getWeichenfestlegung(int weichenNr);                                          /**<Es wird ausgegeben, ob die Weiche festgelegt ist, oder nicht*/
  boolean getWeichenposition(int weichenNr);                                            /**<Es wird der akuelle Status der angegebenen Weiche ausgegeben. (true = gerade, false = kurve)*/
  friend class Besetztmelder;/**<veränder den Status der Weiche und läasst die roten Leds die Stellung anzeigen*/
};

#endif