/* Libary: Schieberegisters

    Die Funktionen eines Schieberegisters sollen einfacher genutzt werden können.
    - einfaches erstellen von Schieberegistern
    - einfaches Abrufen aller Daten
    - wenig sichtbarer Programmieraufwand --> weniger Fehler

    Lennart Klüner 14.10.2020
*/
#include "Arduino.h"
#include <Schieberegister.h>                         //einbeziehen der Bibliothek


schieberegister schieberegister1(1, 8, 9, 10);      //definieren eines Objekts innerhalb der Klasse Schieberegister:
                                                    // Name(Anzahl der Register innerhalb eines Verbundes, SH_CP, ST_CP, DS-Pin)

void  setup()
{
  Serial.begin(9600);

}
void loop()
{
  schieberegister1.setPin(1, HIGH);                   //Methode um einen Pin an- bzw. auszuschalten
  delay(250);
  schieberegister1.printPinStatus();                  //Methode um alle Werte eines Schieberegisterverbunds anzuzeigen
  schieberegister1.setPin(1, LOW);                    //....setPin(PinNr. , neuerWert)
  delay(250);
  schieberegister1.printPinStatus();                  //Methode um alle Werte eines Schieberegisterverbunds anzuzeigen
  
  Serial.println(schieberegister1.getPinStatus(2));   //Methode um Pins auszulesen
}
