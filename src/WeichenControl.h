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
#include "Actor.h"
#include "Weiche.h"


/**
 * Über diese Klasse werden die Weichen gesteuert.
 * Die Klasse erstellt Weichen und verwaltet diese, über diese Klasse werden die einzelen weichen geschaltet und abgefragt werden. Die Besetztmelder sind von 0 an nummeriert
*/
class WeichenControl : public Actor
{
private:
  int _anzahlWeichen;                    //anzahl an Meldern, für die Objekte erstllt wurden
  class Weiche **_pWeiche; //dynamisches Array von Objekten der Klasse besetztmelder, zur steuerung und abfrage von besetztmeldern

public:
/**Erstellt die Angegebene Anzahl an besetztmelder und initialisiert diese. Die Initialisierung passiert mit Hilfe der drei Übergebenen Array gleisPins, ledsGleb und ledsRot.
 * Aus der Klasse besetztmeldungControl werden die entgegengenommenen Befehle auf die einzelnen Besetztmelder verteilt.
 * 
 * @param[in] gleisPins Speichert in einem Array die Anschlüsse, über, die die Besetztmelder den Status des Melderabschnitts einlesen können. (0 = nicht belegt, 1 = belegt)
 * @param[in] ledsGelb Dieses Array speichert die Anschlüsse, über, die die Besetztmelder ausgeben sollen, wenn der überwachte Gleisabschnitt frei ist.
 * @param[in] ledsRot Dieses Array speichert die Anschlüsse, über, die die Besetztmelder ausgeben sollen wenn der überwachte Gleisabschnitt besetzt ist.
 * @param[in] anzahlMelder Gibt an wie viele Besetztmelder erstellt werden sollen.
 * @param[in] registerPins Die Pins aus dem Array werden an die Oberklasse actors übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
 * @see actors(int anzahl, int sh, int st, int ds)
 * @see besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4]);
*/
  WeichenControl(int anzahlweichen, int weichenPinGerade[], int weichenPinKurve[], int weichenLedPinGerade[], int weichenLedPinKurve[], int adressWeichenposition[], int weichentimeout, int wt[], int wgt, int registerPins[4]); //Im Konstruktor wird ein Array von Objekten der Klasse Weichen erstellt und initialisiert
  void weichenWechseln();                     /**<Funktion zum Wechseln der Weiche mit Weichengruppentaste und Weichentaste in Kombination. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weichenGerade(int weichenNr);          /**<Funktion um die Weiche in Geradeweichenlage zu versetzen. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weichenKurve(int weichenNr);           /**<Funktion um die Weiche in Kurvenlage zu versetzten. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  //void weichenBlinken(int weichennr);        /**<Die Weichen-Leds blinken je nach Lage der Weiche. Dies funktioniert nur, wenn die Weiche auch schaltet.*/
  void weichenRelaisHIGH();      /**<Alle Weichenrelais werden auf HIGH gesetzt. Die Relais sind alle inaktiv.*/
  void weichenRelaisLOW();       /**<Alle Weichenrelais werden auf LOW gesetzt. Die Relais sind alle aktiv.*/
  void weichenpositionenEEPROM(); /**<Die Weichenposition wird dauerhaft im EEPROM gespeichert. Dafür wird sie geupdatet, wenn eine Weiche geschaltet wurde*/
  void weichenSchalten();       /**<Beendet nach der zuvor im Konstruktor definierten Zeit das Schalten des Relais.(je nach Weichenantrieb und Schaltdauer) Methode muss für jedes Objekt der Klasse einmal im Loop vorhanden sein.*/

  void setWeichenfestlegung(int weichenNr, boolean festlegestatus, int fahrstrassenNr); /**<Kann die Festlegung der Weichen aktivieren. Die Weichenlage kann nicht mehr verändert werden, bis die Festlegung durch die Fahrstraße ausfgelöst wird.*/
  boolean getWeichenfestlegung(int weichenNr);                                        /**<Es wird ausgegeben, ob die Weiche festgelegt ist, oder nicht*/
};

#endif