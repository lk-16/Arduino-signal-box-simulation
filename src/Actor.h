/*Libary für die Steuerung einews Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *  Lennart Klüner 05.09.2020
*/

/**
 * Diese Datei enthält die Basisklasse actors, alle Klassen sind von dieser Klasse abhängig.
 * @author Lennart Klüner
 * @file Actor.h
*/

#ifndef Actor_h
#define Actor_h
#include <Arduino.h>
#include <Schieberegister.h>


/**
 * Die Klasse actors enthält die Grundfunktionen jeder Klasse. Jede Klase nutzt diese Klasse, von hier wird der Quellcode für das Steuern des Schieberegisters abgerufen, auch die Funktion zum Blinken einer Led ist hier gespeichert.
*/
class Actor
{
private:
  int _registerAnzahl;
  int _shPin;
  int _stPin;
  int _dsPin;

public:
  /**
   * Erstellt die Menge an Schieberegistern die angeschlossen sind, die im konstruktor vermerkt wurden.
   * @param[in] anzahl Die Anzahl der angeschlossenen Register
   * @param[in] sh Der Pin an dem SH_CP angeschlossen ist.
   * @param[in] st Der Pin an dem ST_CP angeschlossen ist.
   * @param[in] ds Der Pin an dem DS_CP angeschlossen ist.
  */
  Actor(int anzahl, int sh, int st, int ds);
  void blinken(int LedPin);                                             /**<Standard-Operator, lässt die angegebene Led blinken. Geschwindigkeit ist nicht einstellbar*/
  
  /**
   * Digitale Ausgänge können geschaltet werden. Differenzierung zwischen Verbrauchern am Controller und am Schiebregister.
   * @param[in] PinNr Pin der geschaltet werden soll. Schiebregister Pins = PinNr. 1,2,... +100
   * @param[in] newPinStatus Der neue Status des Pins.
   *   */
  void digitalSchalten(int PinNr, boolean newPinStatus);                
  void setRegisterPins(int anzahl, int sh, int st, int ds);             /**<die Pins des Registers lassen sich verändern (Pinvergabe genau wie beim Kontruktor der Methode actors)*/
};
#endif