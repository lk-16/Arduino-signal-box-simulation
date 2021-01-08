/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include "Hauptsignal.h"
#include "Signal.h"

//Methoden der Klasse Hauptsignale, Unterklassen der Klasse actors und signale
Hauptsignal::Hauptsignal(int rotPin, int gelbPin, int gruenPin, int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4])
    : Signal(signaltaste, sperrmelder, allgSignaltasten, registerPin)
{
  _rotPin = rotPin;     //Halt Pin(rot)................................................................................................
  _gelbPin = gelbPin;   //Fahrt Pin(grün)
  _gruenPin = gruenPin; //langsamfahrt (grün und gelb)

  pinMode(_rotPin, OUTPUT);
  pinMode(_gelbPin, OUTPUT);
  pinMode(_gruenPin, OUTPUT);
}

void Hauptsignal::setSignalHp0()
{
  Signal::digitalSchalten(_gruenPin, LOW);
  Signal::digitalSchalten(_gelbPin, LOW);
  //timer?
  Signal::digitalSchalten(_rotPin, HIGH); //alle Lichter werden auf Low gestellt und dann Halt (rot) auf HIGH
}

void Hauptsignal::setSignalHp1()
{
  Signal::digitalSchalten(_rotPin, LOW);
  Signal::digitalSchalten(_gelbPin, LOW);
  Signal::digitalSchalten(_gruenPin, HIGH);
}

void Hauptsignal::setSignalHp2()
{
  Signal::digitalSchalten(_rotPin, LOW); //.. nur Halt also rot geht aus, alle anderen bleiben an
  Signal::digitalSchalten(_gelbPin, HIGH);
  Signal::digitalSchalten(_gruenPin, HIGH);
}

void Hauptsignal::hauptsignalSchalten(int newStatus) //funktion stellt das Signal
{

  if (Signal::getSignalsperre() == false) //kontrolle der Signalsperre und anschließende Kontrolle, ob sich der Wert geändert hat, spart Zeit im Programmcode
  {
    Signal::setSignalstatus(newStatus);
    if (Signal::getSignalstatus() == 0)
      Hauptsignal::setSignalHp0();
    if (Signal::getSignalstatus() == 1)
      Hauptsignal::setSignalHp1();
    if (Signal::getSignalstatus() == 2)
      Hauptsignal::setSignalHp2();
  }
}

void Hauptsignal::hauptsignalHp0Manuell() //Funktion sorgt dafür, dass mit signalgruppensperrtaste und signaltaste ein signal in hp0 versetzt werden kann
{
  if (digitalRead(Signal::getSignalhaltgruppentaste()) == HIGH && digitalRead(Signal::getSignaltaste()) == HIGH)
  {
    Hauptsignal::hauptsignalSchalten(0); //bei drücken von Signal und Signalhalttaste schalte signal auf hp0
  }
}
