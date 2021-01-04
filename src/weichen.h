/*Libary zum testen:
 *  Ich möchte Weicehn über diese Libary schalten und
 *  vielleicht auch Signale für die Modelleisenbahn
 *  Lennart Klüner 05.09.2020
*/

/**
 * Diese Datei enthält die Klasse weichen.
 * @author Lennart Klüner
 * @file weichen.h
*/

#ifndef weichen_h
#define weichen_h
#include <Arduino.h>
#include "actors.h"

/**
 * In dieser Klasse werden Weichen gesteuert.
 * Die Klasse kann Weichen schalten und speichert deren Status im EEPROM und kann ihn abrufen, somit vergisst das 
 * Programm den Status einer Weiche auch nach dem Reset des Microcontrollers nicht.
**/
class weichen : public actors
{
private:                 //private Variablen für die Klasse Weichen
  int weichenstatus = 0; //1gerade, 2kurve
  int _wnr;              ///speichert die Weichennummer des Objekts
  int _weichenPinGerade;
  int _weichenPinKurve;
  int _weichenLedPinGerade;
  int _weichenLedPinKurve;
  int _adressWeichenposition;   //adresse zu speicherung der Weichenpostion auf dem EEPROM des Arduinos
  unsigned int _weichentimeout; //wie lange soll die Weiche schalten (unsigend ist nötig, da sonst unsigned und singed typen miteinander verglichen werden)
  int _wt;
  int _wgt;
  int _weckerPin;
  unsigned long wTime;                 //nicht verwendet stimmt
  unsigned long _wStartzeit;           //Startzeit der Pause
  boolean _weichenposition;            //true gerade und false kurve
  boolean _weichenausleuchtung = true; //true an, die Stellungsmelder der Weiche sind angeschaltet, false die Stellungsmelder der Weiche sind ausgeschaltet
  boolean _weichenfestlegung = false;  //wenn aus true, kann die Weiche nicht mehr gestellt werden

public:
  weichen(int wnr, int weichenPinGerade, int weichenPinKurve, int weichenLedPinGerade, int weichenLedPinKurve, int adressWeichenposition, int weichentimeout, int wt, int wgt, int registerPins[4]); // definieren von für alle Methoden wichtige Informationen Pins etc.

  void weicheWechsel();         /**<Funktion zum Wechseln der Weiche mit Weichengruppentaste und Weichentaste in Kombination. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weicheGerade();          /**<Funktion um die Weiche in Geradeweichenlage zu versetzen. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weicheKurve();           /**<Funktion um die Weiche in Kurvenlage zu versetzten. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weichenBlinken();        /**<Die Weichen-Leds blinken je nach Lage der Weiche. Dies funktioniert nur, wenn die Weiche auch schaltet.*/
  void weicheRelaisHIGH();      /**<Alle Weichenrelais werden auf HIGH gesetzt. Die Relais sind alle inaktiv.*/
  void weicheRelaisLOW();       /**<Alle Weichenrelais werden auf LOW gesetzt. Die Relais sind alle aktiv.*/
  void weichenpositionEEPROM(); /**<Die Weichenposition wird dauerhaft im EEPROM gespeichert. Dafür wird sie geupdatet, wenn eine Weiche geschaltet wurde*/
  void weichenSchalten();       /**<Beendet nach der zuvor im Konstruktor definierten Zeit das Schalten des Relais.(je nach Weichenantrieb und Schaltdauer) Methode muss für jedes Objekt der Klasse einmal im Loop vorhanden sein.*/

  void setWeichenfestlegung(boolean festlegestatus, int fahrstrassennr); /**<Kann die Festlegung der Weichen aktivieren. Die Weichenlage kann nicht mehr verändert werden, bis die Festlegung durch die Fahrstraße ausfgelöst wird.*/
  boolean getWeichenfestlegung();                                        /**<Es wird ausgegeben, ob die Weiche festgelegt ist, oder nicht*/
};



#endif
