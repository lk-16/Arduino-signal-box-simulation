/* Libary: Schieberegisters

    Die Funktionen sollen einfacher abgebildet und genutzt werden können.
    Lennart Klüner 14.10.2020
*/


#include "Arduino.h"
#include "Schieberegister.h"


schieberegister::schieberegister(int anzahl, int sh, int st, int ds)
{
  Serial.begin(9600);
  _anzahlPins = anzahl * 8;
  _sh = sh;                                       //Übergabepins für das Register werden festgelegt
  _st = st;
  _ds = ds;
  pinMode(_sh, OUTPUT);                           // Pins für Schieberegister als Ausgang definieren
  pinMode(_st, OUTPUT);
  pinMode(_ds, OUTPUT);

  digitalWrite(_sh, LOW);
  digitalWrite(_st, LOW);
  digitalWrite(_ds, LOW);


  //definieren der Größe des dynamischen Arrays
  _daten = (boolean*)calloc(_anzahlPins, (sizeof(boolean)));
  if (_daten == NULL)                                         //wenn kein speicer mehr frei ist, soll eine Fehlermeldung ausgegeben werden
  {
    Serial.println("Error: Daten des Schieberegisters können nicht gespeichert werden. Source: Schieberegister.h schieberegister::setPin");
    return;
  }
}


void schieberegister::setPin(int pinNr, boolean pinStatus)
{
  digitalWrite(_st, LOW);
  _daten[pinNr - 1] = pinStatus;

  for (int index = _anzahlPins - 1; index >= 0; index --)    // Die Reihenfolge der leuchtenden LEDs
  { // durch die for- Schleife einfügen

    digitalWrite(_ds, 0);                        // DS Pin SH_CP Pin auf "0" (LOW) stellen
    digitalWrite(_sh, 0);                        //
    digitalWrite(_ds, _daten[index]);             // In DS Pin schreiben, ob LED an oder aus (0 für LOW, also aus, und 1 für HIGH, also an)
    digitalWrite(_sh, 1);                        // SH_CP Pin auf 1 setzen (HIGH) damit die Information am DS Pin gespeichert wird
  }
      digitalWrite(_st, HIGH);                           // Wenn for- Schleife beendet wurde, ST_CP Pin auf 1 (HIGH) setzen damit die Datenübertragen werden
}

void schieberegister::printPinStatus()
{
  int auslesen = 0;
  for (int a = 1; a <= _anzahlPins / 8; a++)
  {
    for (int j = 1; j <= 8; j ++)
    {
      Serial.print(_daten[auslesen]);
      auslesen++;
    }
    Serial.println();

  }
}

boolean schieberegister::getPinStatus(int pinNr)
{
  return _daten[pinNr];
}
