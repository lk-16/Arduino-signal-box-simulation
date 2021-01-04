/*Libary zum testen:
 *  Ich möchte Weicehn über diese Libary schalten und
 *  vielleicht auch Signale für die Modelleisenbahn
 *  Lennart Klüner 05.09.2020
 *  gesamt: ca. 12.000 Zeichen
 *  ca.  1.600 Worte
 * 
 * Auto-Layout: Strg k, Strg f
*/

/**
 * Mit dieser Datei und entsprechenden .cpp-Dateien lässt sich ein Gleisbildstellpult auf einem Arduino zu simulieren.
 * Diese Software soll es ermöglichen Weichen und Signale, per Start-Zieltasten nach dem Vorbild eines SpDrs60 Stellwerks auf der Modellbahn zu steuern.
 * @author Lennart Klüner
*/

#ifndef actors_h
#define actors_h
#include <Arduino.h>
#include <Schieberegister.h>
#include "actors.h"

/**
 * Die Klasse actors enthält die Grundfunktionen jeder Klasse. Jede Klase nutzt diese Klasse, von hier wird der Quellcode für das Steuern des Schieberegisters abgerufen, auch die Funktion zum Blinken einer Led ist hier gespeichert.
*/
class actors
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
  actors(int anzahl, int sh, int st, int ds);
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