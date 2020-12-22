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
 * @file Gleisbild.h
 * @brief Mit dieser Datei und entsprechenden .cpp-Dateien lässt sich ein Gleisbildstellpult auf einem Arduino zu simulieren.
 * Diese Software soll es ermöglichen Weichen und Signale, per Start-Zieltasten nach dem Vorbild eines SpDrs60 Stellwerks auf der Modellbahn zu steuern.
 **/



#ifndef Gleisbild_h
#define Gleisbild_h
#include <Arduino.h>
#include <EEPROM.h>
#include <Schieberegister.h>

/**
 * @class actors
 * @brief Die Klasse actors enthält die Grundfunktionen jeder Klasse.
 * Jede Klase nutzt diese Klasse, von hier wird der Quellcode für das Steuern des Schieberegisters abgerufen, auch die Funktion zum Blinken einer Led ist hier gespeichert.
 **/
class actors
{
private:
  int _registerAnzahl;
  int _shPin;
  int _stPin;
  int _dsPin;

public:
  actors(int anzahl, int sh, int st, int ds);
  void blinken(int LedPin);                              //led blinkt
  void digitalSchalten(int PinNr, boolean newPinStatus); //leds können geschaltet werden, Differenzierung zwischen Led an normalen und leds an Schieberegisteroutputs
  void setRegisterPins(int anzahl, int sh, int st, int ds);
};

//evtl. später, zurzeit nicht wichtig, zu kompliziert
class stelltisch: public actors
{
private:
  boolean weichenausleuchtung;                            //true, Ausleuchtung an, false Ausleuchtung aus

public:
  stelltisch(int registerPins[4]);                                           //Die für diese Funktion wichtigen Pins werden nicht im Konstruktor deklariert, so bleibt die übersicht bei weiteren Funktionen
  ~stelltisch();

  boolean getWeichenausleuchtung();                       //in dieser Methode wird der Status der Weichenausleuchtung ausgegeben. So können sich andere Klassen an diese Einstllungen halten und sie kontrollieren
  void weichenausleuchtungEinstellen(int tasterEin, int ledein, int tasterAus, int ledAus);
};


/**
 * @class weichen
 * @brief In dieser Klasse werden Weichen gesteuert.
 * Die Klasse kann Weichen schalten und speichert deren Status im EEPROM und kann ihn abrufen, somit vergisst das 
 * Programm den Status einer Weiche nicht.
**/
class weichen : public actors
{
private:                 //private Variablen für die Klasse Weichen
  int weichenstatus = 0; //1gerade, 2kurve
  int _wnr;              //speichert die Weichennummer des Objekts
  int _weichenPinGerade;
  int _weichenPinKurve;
  int _weichenLedPinGerade;
  int _weichenLedPinKurve;
  int _adressWeichenposition;   //adresse zu speicherung der Weichenpostion auf dem EEPROM des Arduinos
  unsigned int _weichentimeout; //wie lange soll die Weiche schalten (unsigend ist nötig, da sonst unsigned und singed typen miteinander verglichen werden)
  int _wt;
  int _wgt;
  int _weckerPin;
  unsigned long wTime;       //nicht verwendet stimmt
  unsigned long _wStartzeit; //Startzeit der Pause
  boolean _weichenposition;  //true gerade und false kurve

public:
  weichen(int wnr, int weichenPinGerade, int weichenPinKurve, int weichenLedPinGerade, int weichenLedPinKurve, int adressWeichenposition, int weichentimeout, int wt, int wgt, int registerPins[4]); // definieren von für alle Methoden wichtige Informationen Pins etc.

  void weicheWechsel();         //..............................................................dont exist Funktion zum Wechseln der Weiche mit Weichengruppentaste und Weichentaste in Kombination
  void weicheGerade();          //Funktion um die Weiche in Geradelage zu versetzen
  void weicheKurve();           //Funktion um die Weiche in Kurvenlage zu versetzten
  void weichenBlinken();        //die Leds blinken
  void weicheRelaisHIGH();      //Alle Relais werden auf HIGH gesetzt
  void weicheRelaisLOW();       //alle Relais werden auf LOW also schalten gesetzt
  void weichenpositionEEPROM(); //die Weichenposition wird dauerhaft gespeichert
  void weichenSchalten();       //beendet das schalten des Relais: Methode muss für jedes Objekt der Klasse einmal im Loop vorhanden sein
};


/**
 * @class zugtasten
 * @brief Die Klasse gibt weiter ob zugtasten gedrückt wurden
 * Die Klasse speichert den Pin der Zugtaste, und gibt ihren Status aus
 **/
class zugtasten : public actors
{
public:
  zugtasten(int zugtastenPin, int registerPins[4]); // definieren von für alle Methoden wichtige Informationen: Pins, Input/Output, etc.
  boolean getzugtastenstatus();                     //Herausgabe des Zugtastenstatus

private:
  //Pins
  int _weckerPin;
  int _zugtastenPin; //,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,??
};

/**
 * @class melder
 * @brief In der Klasse Melder werden Melder und deren Funktionen erstellt.
 * Die Klasse enthält zur Zeit nur den Programmcode für Tastenüberwachung.
**/
class melder : public actors
{
public:
  melder(String melderName, int tueMelderLed, int weckerPin, int wutPin, int registerPins[4]); //definieren der Pins von Meldern
  void tueMelder();                                                            //Löst nach 5 sec drücken einen optischen melder und nach weiteren 5 ein akustisches Signal aus

  int getTueLedPin();                                        //Herausgabe des LedPins
  boolean getWutStatus();                                    //Herausgabe des Status der Weckerunterbrechertaste
  void setWutAktivierung(boolean newWutAktivierung);         //Veränderung des Status der WUT: aktiv/inaktiv
  void setTueMelderStatus(boolean newTueMelderStatus);       //Veränderung des Status des gesamten Melders
  void setMelderStartzeit(unsigned long newMelderStartzeit); //Veränderung des Beginns des Auslösen des Melders
  int getWecker();                                       //der Pin des Weckers wird ausgegeben..............................................................................?
//void setWecker(int wecker);                            //der Pin des Weckers wird verändert
  

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
};

/**
 * @class signale
 * @brief Die Klasse Signale enthält die Grundfunktionen jedes Signals.
 * Die Klasse speichert die Signalhaltgruppentaste und signalsperrtaste. Über sie läasst sich der Signalstatus ausgeben.
 * Das Singal kann auf Halt gestellt werden und gesperrt werden. 
**/
class signale : public actors
{
private:
  int _signalstatus;           //0 = Hp0, 1 = Hp1, 2= Hp2 (rot, Fahrt, Langsamfahrt), je nach Signal auch anders vergeben
  int _signalhaltgruppentaste; //in Kombination aus Zug oder Rangiertaste am Feld kann das Signal auf Halt gestellt werden
  int _signaltaste;            //Taste die auf dem Selben Tischfeld wie das Sginal ist
  int _signalSperrmelder;      //melder der Anzeig ob ein Signal gesperrt ist oder nicht
  int _signalsperrtaste;       //speichert den Pin der Signalsperrtaste
  int _signalentsperrtaste;    //speichert den Pin der Siganlentsperrtaste
  boolean _signalsperre;       //zeigt an ob das Signal gesperrt ist oder nicht(true Signalsperre aktiv)

protected:
  void setSignalstatus(int newSignalStatus); //Veränderung des Signalstatus

public:
  signale(int signaltastenPin, int sperrmelderPin, int allgSignaltasten[3], int registerPins[4]);      //Konstuktor: allgemeine Signaltasten beinhalten die Signalsperr- und entsperrtaste und Signalhaltgruppentaste
  int getSignalstatus();                     //Ausgabe des Signalstatus, 0, 1, 2(rot, Fahrt, Langsamfahrt)
  int getSignaltaste();                      //gibt den Pin der Signaltaste aus
  int getSignalhaltgruppentaste();           //gibt den Pin der Singalhaltgruppentaste aus
  int getSignalsperrmelder();                //gibt den Pin des Signalsperrmelders aus
  int getSignalsperrtaste();                 //gibt den Pin der Singalsperrtaste aus       
  boolean getSignalsperre();                 //gibt aus, ob das Singalgesperrt
  void signalSperren();                      //überprüft ob ein Singalgesperrt oder eintsperrt werden kann
};


/**
 * @class hauptsignale
 * @brief Die Klasse Hauptsignale erzwugt ein funktionierendes Hauptsignal.
 * In der Klass Hauptsignale lassen sich Signale schalten und über Signaltaste und Signalgruppentaste auf Hp0 schalten.
**/
class hauptsignale : public signale
{
private:
  int _rotPin; //Anschlusspins des Signals
  int _gelbPin;
  int _gruenPin;
  
  void setSignalHp0();            //Die Stati der Signale werden verändert
  void setSignalHp1();
  void setSignalHp2();

public:
  hauptsignale(int rotPin, int gelbPin, int gruenPin,  int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4]);
  void hpschalten(int newStatus); //schaltet das Signal........................................................funktioniert
  void hauptsignalhp0manuell();   //........................................
};

/**
 * @class besetztmeldungControl
 * @brief Über diese Klasse wird die Besetztmeldung gesteuert.
 * Die Klasse erstellt Besetztmelder und verwaltet diese, über diese Klasse sollen die einzelen Besetztmelder abgefragt werden.
 * Über diese Klasse könne einfach alle Besetztmelder abgefragt werden.
**/

class besetztmeldungControl : public actors
{
private:
  int _anzahlMelder; //anzahl an Meldern um das Objektarray im Konstruktor zu erstellen
  //besetztmelder *_besetztmelderArray;
  int *_gleisPins;
  int *_ledsGelb;
  int *_ledsRot;
  

public:
  besetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int anzahlMelder, int registerPins[4]);
  //~besetztmeldungControl(); //Destruktor
  boolean getBesetztmelderstatus(int besetztmelder);
  
};

/**
 * @brief Die Klasse Besetztmelder erstellt einzelne Besetztmelder.
 * Die Klasse kann ihren Besetztmelder abfragen
**/
class besetztmelder : public actors
{
private:
  int _gleisPin;                    //hier sind die Pins gespeichert über, die die Besetztmelder ausgelesen werden könne. sie werden über die Klasse BesetzmlderControl bestimmt(bisher noch nicht funktionsfähig)......................................................................................
  int _ledGelb;                     //s.o.
  int _ledRot;                      //s.o.
  boolean _besetztmelderstatus;     //hier wirde der aktuelle Status des Besetztmelders gespeichert(0 nicht besetzt, 1 besetzt)
  boolean _besetztmelderAktiv;      //es wird gespeichert, ob der Besetzmelder auf dem Stellpult zu sehen sein soll

public:
  besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4]);       //Konstruktor der Klasse Besetztmelder
  ~besetztmelder(); //Destruktor, keine Verwendung
  String getBesetztmelderName();
  int besetztmelderAuslesen();                                                    //auslesen des bestztmelders, aktueller Status wird zurückgegeben.................................namen überdenken
  void setBesetztmelderAktiv(boolean newBesetztmelderStatus);
};
#endif
