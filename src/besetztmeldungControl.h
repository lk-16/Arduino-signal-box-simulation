/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
*/

/**
 * Diese Datei enthält die Klasse besetztmelderControl.
 * @author Lennart Klüner
 * @file BesetztmeldungControl.h
*/

#ifndef BesetztmeldungControl_h
#define BesetztmeldungControl_h
#include "Arduino.h"

/**
 * Über diese Klasse wird die Besetztmeldung gesteuert.
 * Die Klasse erstellt Besetztmelder und verwaltet diese, über diese Klasse werden die einzelen Besetztmelder abgefragt. Die Besetztmelder sind von 0 an nummeriert
**/
class BesetztmeldungControl
{
private:
  int _anzahlMelder;                     //anzahl an Meldern, für die Objekte erstllt wurden
  class Besetztmelder **_pbesetztmelder; //dynamisches Array von Objekten der Klasse besetztmelder, zur steuerung und abfrage von besetztmeldern

public:
  /** Erstellt die Angegebene Anzahl an Besetztmelder und initialisiert diese. Die Initialisierung passiert mit Hilfe der drei Übergebenen Array gleisPins, ledsGleb und ledsRot.
 * Aus der Klasse BesetztmeldungControl werden die entgegengenommenen Befehle auf die einzelnen Besetztmelder verteilt.
 * 
 * @param[in] gleisPins Speichert in einem Array die Anschlüsse, über, die die Besetztmelder den Status des Melderabschnitts einlesen können. (0 = nicht belegt, 1 = belegt)
 * @param[in] ledsGelb Dieses Array speichert die Anschlüsse, über, die die Besetztmelder ausgeben sollen, wenn der überwachte Gleisabschnitt frei ist.
 * @param[in] ledsRot Dieses Array speichert die Anschlüsse, über, die die Besetztmelder ausgeben sollen wenn der überwachte Gleisabschnitt besetzt ist.
 * @param[in] anzahlMelder Gibt an wie viele Besetztmelder erstellt werden sollen.
 * @param[in] registerPins Die Pins aus dem Array werden an die Oberklasse Actors übergeben. Das Array besteht aus der Anzahl der Register, dem Pin SH_CP, ST_CP, DS  in dieser Reihenfolge.
 * @see Actors(int anzahl, int sh, int st, int ds)
 * @see Besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4]);
*/
  BesetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int weichenbesetztmelder[], int anzahlMelder, int registerPins[4]); //Im Konstruktor wird ein Array von Objekten der Klasse Besetztmelder erstellt und initialisiert

  boolean getBesetztmelderstatus(int besetztmelder, boolean besetztmelderBeleuchtung, class WeichenControl WeichenControl);              /**<Gibt den Status des angegebenen Besetztmelders aus*/
  void setBesetztmelderBeleuchtung(int besetztmelder, boolean besetztmelderLicht);                  /**<Die leds, die den Status des Besetztmelders anzeigen können an und aus geschaltet werden. Sie zeigen dennoch immer an, wenn ein Gleis besetzt ist. Die Freimeldung wird nicht mehr angezeigt*/
  void setFahrstrassenelement(int besetztmelderNr, int fahrstrassenNr, boolean fahrstrassenstatus); /**<Ein Besetztmelder wird zu einem Fahrstraßenelement gemacht, somit verschwindet die Besetztmeldung auch bei frei sein des Gleises nicht. Der Besetztmelder zeigt auch die Stellung an, wenn die Beleuchtung ausgeschaltet ist.*/
  boolean getFahrstrassenelement(int besetztmelderNr);                                              /**<Es wird ausgegeben, ob ein Besetztmeldr von einer Fahrstrasse beansprucht ist.*/
  void besetztmelderAusfuehren(class WeichenControl WeichenControl);                                                                   /**<Führt für alle Besetztmelder Besetztmelder auslesen aus, somit wird wenn erforderlich bei jedem Besetztmelder der Status angezeigt*/
  //void setWeichenLeds();
};
#endif