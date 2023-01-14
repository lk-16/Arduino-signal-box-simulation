/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 */

/**
 * Diese Datei enthält die Klasse weichen.
 * @author Lennart Klüner
 * @file Weiche.h
 */

#ifndef Weiche_h
#define Weiche_h
#include <Arduino.h>
#include "Actor.h"

/**
 * In dieser Klasse werden Weichen gesteuert.
 * Die Klasse kann Weichen schalten und speichert deren Status im EEPROM und kann ihn abrufen, somit vergisst das
 * Programm den Status einer Weiche auch nach dem Reset des Microcontrollers nicht.
 **/
class Weiche : public Actor
{
private:                 // private Variablen für die Klasse Weichen
  int weichenstatus = 0; // 1gerade, 2kurve
  int _wnr;              /// speichert die Weichennummer des Objekts
  int _weichenPinGerade;
  int _weichenPinKurve;
  int _weichenLedPinGerade;
  int _weichenLedPinKurve;
  int _weichenLedPinGeradeRot;  // Für besetztmelder
  int _weichenLedPinKurveRot;   // s.o.
  boolean _besetzt = false;     // wenn true, sollen die roten Leds verwendet werden.
  int _adressWeichenposition;   // adresse zu speicherung der Weichenpostion auf dem EEPROM des Arduinos
  unsigned int _weichentimeout; // wie lange soll die Weiche schalten (unsigend ist nötig, da sonst unsigned und singed typen miteinander verglichen werden)
  int _wt;
  int _wgt;
  int _weckerPin;
  unsigned long wTime;                      // nicht verwendet stimmt
  unsigned long _wStartzeit;                // Startzeit der Pause
  boolean _weichenposition;                 // true gerade und false kurve
  boolean _weichenausleuchtung = true;      // true an, die Stellungsmelder der Weiche sind angeschaltet, false die Stellungsmelder der Weiche sind ausgeschaltet
  boolean _weichenfestlegung = false;       // wenn aus true, kann die Weiche nicht mehr gestellt werden
  int _fahrstrassefestgelegt = 0;           // speichert von welcher Fahrstraße die Weiche festgelegt wurde, und nur diese kann die Festlegung auch wieder lösen
  boolean _flankenschutzWeiche = false;     //true, wenn eine Weiche eine Flankenschutzweiche wird
  int _flankenschutzfestgelegt = 0;         //Speichert die Nummer der Fahrstraße, zu der der Flankenschutz gehört
  void setWeichenLeds(boolean weichenlage); /**<kurve oder gerade und Pinstatus, schaltet je nach Besetztmelderstatus und beleuchtungsstatus die passenden Weichenspitzenmelder*/
  void weichenBlinken();                    /**<Die Weichen-Leds blinken je nach Lage der Weiche. Dies funktioniert nur, wenn die Weiche auch schaltet.*/

public:
  /** Erstellt Weichen.
   * @param[in] wnr Nummer der Weiche.
   * @param[in] weichenPinGerade Schaltet das Relais, welches einen Wechsel Geradelage auslöst.
   * @param[in] weichenPinKurve Schaltet das Relais, welches einen Wechsel in Kurvenlage auslöst.
   * @param[in] weichenLedPinGerade Speichert die Ledanschlüsse, über, die die Weiche, wenn sie gerade ist anzeigen kann, das sie nicht besetzt ist.
   * @param[in] weichenLedPinGeradeRot Speichert die Anschlüsse, über, die die Weiche, wenn sie gerade ist die Besetztmeldung anzeigen kann.
   * @param[in] weichenLedPinKurve Speichert die Anschlüsse, über, die die Weiche, wenn sie in Kurvenlage anzeigen kann, das sie nicht besetzt ist.
   * @param[in] weichenLedPinKurveRot Speichert die Anschlüsse, über, die die Weiche, wenn sie in Kurvenlage ist die Besetztmeldung anzeigen kann.
   * @param[in] adressWeichenposition An dieser Speicherposition wird die Weichenlage gspeichert
   * @param[in] wt Speichert die Anschlüsse, an der die Weichentaste angeschlossen ist.
   * @param[in] wgt Speichert die Anschlüsse, an der die Weichengruppentaste angeschlossen ist.
   * @param[in] registerPins Die Pins aus dem Array werden an die Oberklasse besetztmeldungControl/actors übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
   * @see actors(int anzahl, int sh, int st, int ds)
   * @see besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4]);
   */
  Weiche(int wnr, int weichenPinGerade, int weichenPinKurve, int weichenLedPinGerade, int weichenLedPinGeradeRot, int weichenLedPinKurve, int weichenLedPinKurveRot, int adressWeichenposition, int weichentimeout, int wt, int wgt, int registerPins[4]); // definieren von für alle Methoden wichtige Informationen Pins etc.

  void weicheWechsel();         /**<Funktion zum Wechseln der Weiche mit Weichengruppentaste und Weichentaste in Kombination. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weicheGerade();          /**<Funktion um die Weiche in Geradeweichenlage zu versetzen. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weicheKurve();           /**<Funktion um die Weiche in Kurvenlage zu versetzten. Dies ist nur möglich wenn die Weiche nicht von einer Fahrstraße beansprucht wird.*/
  void weicheRelaisHIGH();      /**<Alle Weichenrelais werden auf HIGH gesetzt. Die Relais sind alle inaktiv.*/
  void weicheRelaisLOW();       /**<Alle Weichenrelais werden auf LOW gesetzt. Die Relais sind alle aktiv.*/
  void weichenpositionEEPROM(); /**<Die Weichenposition wird aus dem EEPROM abgerufen und ausgeführt. Die Methode wird am Anfang benötigt, so wird sicher gestellt, das alle Weichen in der vom Stellpult ange-nommenen Lage sind*/
  void weicheSchalten();        /**<Beendet nach der zuvor im Konstruktor definierten Zeit das Schalten des Relais.(je nach Weichenantrieb und Schaltdauer) Methode muss für jedes Objekt der Klasse einmal im Loop vorhanden sein.*/

  void setWeichenfestlegung(boolean festlegestatus, int fahrstrassennr); /**<Kann die Festlegung der Weichen aktivieren. Die Weichenlage kann nicht mehr verändert werden, bis die Festlegung durch die Fahrstraße ausfgelöst wird.*/
  boolean getWeichenfestlegung();                                        /**<Es wird ausgegeben, ob die Weiche festgelegt ist, oder nicht*/
  void setFlankenschutz(boolean flankenschutzstatus, int fahrstrassennr); /**<Weichen werden zu Flankenschutzweichen und können bis zur auslösung der Fahrstraße nicht mehr geschaltet werden.*/
  boolean getFlankenschutz();                                        /**<Es wird ausgegeben, ob die Weiche eine Flankenschutzweiche ist, oder nicht*/
  boolean getWeichenposition();                                          /**<Gibt die Weichenposition aus, wenn die Weiche umgelaufen ist.*/
  void setWeichenposition(boolean position);                             /**<veränderung der Weichenposition, weiche Kurve und WeicheGerade wird so aufgelöst*/
  void setWeichebesetzt(boolean besetztmelderstatus);                    /**<Ändert die Weichenausleuchtung auf die RotenLeds um die Besetztmeldung anzuzeigen. Da die Weichen mit WeichenControl erstellt werden, benötigt es keine Zugriffseinschränkung, diese findet sich in WeichenControl, nur Besetztmelder können von dort auf diese Methode von Weiche zugreifen.*/
  int getNr();
};

#endif
