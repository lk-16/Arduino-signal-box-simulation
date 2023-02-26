/* Libary: Schieberegisters

    Die Funktionen sollen einfacher abgebildet und genutzt werden können.
    Lennart Klüner 14.10.2020
*/



#ifndef Schieberegister_h
#define Schieberegister_h
#include "Arduino.h"

class schieberegister
{
  public:

    schieberegister(int anzahl, int sh, int st, int ds);
    void setPin(int pinNr, boolean pinStatus);              /*<Pins können einzeln angesteuert werden*/
    void printPinStatus();                                  /*<Status der gesamten Pins wird über den Seriellen Monitor ausgegeben*/
    boolean getPinStatus(int pinNr);                        /*<Gibt einzelne Pinstati aus*/

  private:
    int _sh;
    int _st;
    int _ds;
    int _anzahlPins;                                        //anzahl der Pins im Register
    boolean *_daten;                                        //flexibles array zum übergeben der Daten an das Register


};

#endif
