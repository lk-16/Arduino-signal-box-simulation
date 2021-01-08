/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
 *  gesamt: ca. 12.000 Zeichen
 *  ca.  1.600 Worte
*/

/**
 * Diese Datei enthält die Klasse besetztmelder.
 * @author Lennart Klüner
 * @file Besetztmelder.h
*/

#ifndef Besetztmelder_h
#define Besetztmelder_h
#include "Arduino.h"
#include "Actor.h"

/**
 * Die Klasse Besetztmelder erstellt einzelne Besetztmelder.
 * Die Klasse kann Besetztmelder abfragen. Objekte der Klasse werden von der Klasse Besetztmelder Control erstellt.
**/
class Besetztmelder : public Actor
{
private:
  int _gleisPin;                        //hier sind die Pins gespeichert über, die die Besetztmelder ausgelesen werden könne. sie werden über die Klasse BesetzmlderControl bestimmt.
  int _ledGelb;                         //s.o.
  int _ledRot;                          //s.o.
  boolean _besetztmelderstatus;         //hier wirde der aktuelle Status des Besetztmelders gespeichert(0 nicht besetzt, 1 besetzt)
  boolean _besetztmelderLicht = false;  //es wird gespeichert, ob der Besetzmelder auf dem Stellpult zu sehen sein soll(an/aus)
  boolean _fahrstrassenelement = false; //sind die Melder Teil einer Fahrstraße, sollen die immer an sein und der boolean ist true
  int _fahrstrasse = 0; //speichert von welcher Fahrstraße der Besetztmelder festgelegt wurde, und nur diese kann die Festlegung auch wieder lösen

public:
/** Erstellt die angegebene Anzahl an besetztmelder und initialisiert diese. Die Initialisierung passiert mit Hilfe der drei Übergebenen Array gleisPins, ledsGleb und ledsRot.
 * Aus der Klasse besetztmeldungControl werden die entgegengenommenen Befehle auf die einzelnen Besetztmelder verteilt.
 * 
 * @param[in] gleisPin Speichert die Anschlüsse, über, die der Besetztmelder den Status des Melderabschnitts einlesen kann. (0 = nicht belegt, 1 = belegt)
 * @param[in] ledGelb Speichert die Anschlüsse, über, die die Besetztmelder ausgeben sollen, wenn der überwachte Gleisabschnitt frei ist.
 * @param[in] ledRot Speichert die Anschlüsse, über, die die Besetztmelder ausgeben sollen wenn der überwachte Gleisabschnitt besetzt ist.
 * @param[in] registerPins Die Pins aus dem Array werden an die Oberklasse besetztmeldungControl/actors übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
 * @see actors(int anzahl, int sh, int st, int ds)
 * @see besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4]);
*/
  Besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4]); //Konstruktor der Klasse Besetztmelder
  boolean besetztmelderAuslesen(boolean besetztmelderBeleuchtung);          /**<Auslesen des Besetztmelders, aktueller Status wird zurückgegeben und die Anzeige je nach Einstellung in besetztmelderLicht und fahrstrassenelement geschaltet*/
  void setBesetztmelderLicht(boolean newBesetztmelderStatus);               /**<Die Beleuchtung des Besetztmelder kann an und aus geschaltet werden.  Sie zeigen dennoch immer an, wenn ein Gleis besetzt ist. Die Freimeldung wird nicht mehr angezeigt*/
  void setFahrstrassenelement(int fahrstrassennr, boolean Fahrstrassenelement);                 /**<Der Besetztmelder wird zu einem Fahrstraßenelement gemacht, somit verschwindet die Besetztmeldung auch bei frei sein des Gleises nicht. Der Besetztmelder zeigt auch Frei oder belegt sein an, wenn besetztmelderLicht ausgeschaltet ist. Die festlegung und beanspruchung durch eine Fahrstrasse kann nur durch die selbige wieder aufgehoben werden*/
  boolean getFahrstrassenelement();                                                             /**<Die Methode gibt aus, ob der Besetztmelder durch eine Fahrstraße beansprucht ist.*/
};


#endif