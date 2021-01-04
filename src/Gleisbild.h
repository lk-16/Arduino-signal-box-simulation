/*Libary zum testen:
 *  Ich möchte Weicehn über diese Libary schalten und
 *  vielleicht auch Signale für die Modelleisenbahn
 *  Lennart Klüner 05.09.2020
 *  gesamt: ca. 12.000 Zeichen
 *  ca.  1.600 Worte
 * 
 * Auto-Layout: Strg k, Strg f
*/

/*Die Klasse Signale  muss noch genauer Dokumnetiert werden. Verstehen!!
 *  Klasse Stelltisch wurde erstellt, für das erstellen des Fahrstraßenrechners fehlt noch eine funktionierende Besetztmeldung
 * in der Klasse Stelltisch sollen einstellungen für den gesmaten Stelltisch gespeichert werden.
 * Angefangen habe ich mit dem ein und ausschalten der Weichenausleuchtung s.h. S. 13 (noch nicht viel fertig)
*/

/**
 * Mit dieser Datei und entsprechenden .cpp-Dateien lässt sich ein Gleisbildstellpult auf einem Arduino zu simulieren.
 * Diese Software soll es ermöglichen Weichen und Signale, per Start-Zieltasten nach dem Vorbild eines SpDrs60 Stellwerks auf der Modellbahn zu steuern.
 * @author Lennart Klüner
 * @file Gleisbild.h
 *  **/

#ifndef Gleisbild_h
#define Gleisbild_h
#include <Arduino.h>
#include <EEPROM.h>
#include <Schieberegister.h>
#include "actors.h"
#include "zugtasten.h"

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


/**In der Klasse Melder werden Melder und deren Funktionen erstellt. Die Klasse enthält zur Zeit nur den Programmcode für die Tastenüberwachung.
**/
class melder : public actors
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
  melder(String melderName, int tueMelderLed, int weckerPin, int wutPin, int registerPins[4]); //definieren der Pins von Meldern
  void tueMelder();                                          /**<Löst nach 5 sec drücken einen optischen Melder und nach weiteren 5 den Wecker aus.*/
  int getTueLedPin();                                        /**<Herausgabe des LedPins. Wird in der main benötigt, so werden bleibt der Programmcode für die TÜ-Melder egal, wie der Pin der Led benannt ist, immer gleich und nur die Objektdefinitionen müssen angepasst werden.*/
  boolean getWutStatus();                                    /**<Herausgabe des Status der Weckerunterbrechertaste.*/
  void setWutAktivierung(boolean newWutAktivierung);         /**<Veränderung des Status der WUT: aktiv/inaktiv.*/
  void setTueMelderStatus(boolean newTueMelderStatus);       /**<Veränderung des Status des gesamten Melders.*/
  void setMelderStartzeit(unsigned long newMelderStartzeit); /**<Setzt den Melder zurück (Abfragen in der main.cpp werden benötigt).*/
  int getWecker();                                           /**<Der Pin des Weckers wird ausgegeben. Wird in der main benötigt, so werden bleibt der Programmcode für die TÜ-Melder egal, wie der Pin des Weckers benannt ist, immer gleich und nur die Objektdefinitionen müssen angepasst werden.*/
  

};

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

/**
 * Über diese Klasse wird die Besetztmeldung gesteuert.
 * Die Klasse erstellt Besetztmelder und verwaltet diese, über diese Klasse werden die einzelen Besetztmelder abgefragt. Die Besetztmelder sind von 0 an nummeriert
**/
class besetztmeldungControl : public actors
{
private:
  int _anzahlMelder;                    //anzahl an Meldern, für die Objekte erstllt wurden
  class besetztmelder **pbesetztmelder; //dynamisches Array von Objekten der Klasse besetztmelder, zur steuerung und abfrage von besetztmeldern

public:
/** Erstellt die Angegebene Anzahl an besetztmelder und initialisiert diese. Die Initialisierung passiert mit Hilfe der drei Übergebenen Array gleisPins, ledsGleb und ledsRot.
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
  besetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int anzahlMelder, int registerPins[4]); //Im Konstruktor wird ein Array von Objekten der Klasse Besetztmelder erstellt und initialisiert

  boolean getBesetztmelderstatus(int besetztmelder, boolean besetztmelderBeleuchtung); /**<Gibt den Status des angegebenen Besetztmelders aus*/
  void setBesetztmelderBeleuchtung(int besetztmelder, boolean besetztmelderLicht);     /**<Die leds, die den Status des Besetztmelders anzeigen können an und aus geschaltet werden. Sie zeigen dennoch immer an, wenn ein Gleis besetzt ist. Die Freimeldung wird nicht mehr angezeigt*/
  void setFahrstrassenelement(int besetztmelderNr, int fahrstrassenNr, boolean fahrstrassenstatus);        /**<Ein Besetztmelder wird zu einem Fahrstraßenelement gemacht, somit verschwindet die Besetztmeldung auch bei frei sein des Gleises nicht. Der Besetztmelder zeigt auch die Stellung an, wenn die Beleuchtung ausgeschaltet ist.*/
  boolean getFahrstrassenelement(int besetztmelderNr);                                                                        /**<Es wird ausgegeben, ob ein Besetztmeldr von einer Fahrstrasse beansprucht ist.*/
};

/**
 * Die Klasse Besetztmelder erstellt einzelne Besetztmelder.
 * Die Klasse kann Besetztmelder abfragen. Objekte der Klasse werden von der Klasse Besetztmelder Control erstellt.
**/
class besetztmelder : public actors
{
private:
  int _gleisPin;                        //hier sind die Pins gespeichert über, die die Besetztmelder ausgelesen werden könne. sie werden über die Klasse BesetzmlderControl bestimmt.
  int _ledGelb;                         //s.o.
  int _ledRot;                          //s.o.
  boolean _besetztmelderstatus;         //hier wirde der aktuelle Status des Besetztmelders gespeichert(0 nicht besetzt, 1 besetzt)
  boolean _besetztmelderLicht = false;  //es wird gespeichert, ob der Besetzmelder auf dem Stellpult zu sehen sein soll(an/aus)
  boolean _fahrstrassenelement = false; //sind die Melder Teil einer Fahrstraße, sollen die immer an sein und der boolean ist true

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
  besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4]); //Konstruktor der Klasse Besetztmelder
  boolean besetztmelderAuslesen(boolean besetztmelderBeleuchtung);          /**<Auslesen des Besetztmelders, aktueller Status wird zurückgegeben und die Anzeige je nach Einstellung in besetztmelderLicht und fahrstrassenelement geschaltet*/
  void setBesetztmelderLicht(boolean newBesetztmelderStatus);               /**<Die Beleuchtung des Besetztmelder kann an und aus geschaltet werden.  Sie zeigen dennoch immer an, wenn ein Gleis besetzt ist. Die Freimeldung wird nicht mehr angezeigt*/
  void setFahrstrassenelement(int fahrstrassennr, boolean Fahrstrassenelement);                 /**<Der Besetztmelder wird zu einem Fahrstraßenelement gemacht, somit verschwindet die Besetztmeldung auch bei frei sein des Gleises nicht. Der Besetztmelder zeigt auch Frei oder belegt sein an, wenn besetztmelderLicht ausgeschaltet ist. Die festlegung und beanspruchung durch eine Fahrstrasse kann nur durch die selbige wieder aufgehoben werden*/
  boolean getFahrstrassenelement();                                                             /**<Die Methode gibt aus, ob der Besetztmelder durch eine Fahrstraße beansprucht ist.*/
};
#endif
