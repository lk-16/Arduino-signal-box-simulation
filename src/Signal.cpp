/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include <Arduino.h>
#include "Actor.h"
#include "Signal.h"

// Methoden der Klasse signal, Unterklasse der Klasse actors
Signal::Signal(int signaltastenPin, int sperrmelderPin, int allgSignaltasten[3], int registerPins[4], boolean richtung) // Konstuktor: allgemeine Signaltasten beinhalten die Signalsperr- und entsperrtaste und Signalhaltgruppentaste
    : Actor(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
  _signalsperrtaste = allgSignaltasten[0];
  _signalentsperrtaste = allgSignaltasten[1];
  _signalhaltgruppentaste = allgSignaltasten[2];
  _signaltaste = signaltastenPin;
  _signalSperrmelder = sperrmelderPin;
  _richtung = richtung;

  pinMode(_signalsperrtaste, INPUT_PULLUP);
  pinMode(_signalentsperrtaste, INPUT_PULLUP);
  pinMode(_signalhaltgruppentaste, INPUT_PULLUP);
  pinMode(_signaltaste, INPUT_PULLUP);
  pinMode(_signalSperrmelder, OUTPUT);
}

int Signal::getSignalstatus()
{
  return _signalstatus;
}

void Signal::setSignalstatus(int newSignalstatus)
{
  _signalstatus = newSignalstatus;
}

int Signal::getSignaltaste()
{
  return _signaltaste;
}

int Signal::getSignalhaltgruppentaste()
{
  return _signalhaltgruppentaste;
}

int Signal::getSignalsperrmelder()
{
  return _signalSperrmelder;
}

int Signal::getSignalsperrtaste()
{
  return _signalsperrtaste;
}

boolean Signal::getSignalsperre()
{
  return _signalsperre;
}

boolean Signal::getRichtung()
{
  return _richtung;
}

void Signal::signalSperren()
{
  if (_signalsperre == true) // wenn das Signal gesperrt ist.
  {
    // Serial.println("gesperrt");
    if (digitalRead(_signalentsperrtaste) == LOW && digitalRead(_signaltaste) == LOW) // wenn die Signalsperr- und Signaltaste gedrückt, das Signal ist ja Rot
    {
      _signalsperre = false; // Signalsperrre ist Inaktiv
      Serial.println("entsperrt");
      Actor::digitalSchalten(_signalSperrmelder, LOW); // stelle den Sperrmelder aus
    }
    else
    {
      Actor::digitalSchalten(_signalSperrmelder, HIGH);
    }
  }
  else // wenn das Signal nicht gesperrt ist
  {
    // Serial.println("entsperrt");
    if (digitalRead(_signalsperrtaste) == LOW && digitalRead(_signaltaste) == LOW && _signalstatus == 0) // wenn die Signalsperr- und Signaltaste gedrückt und das Signal Rot ist
    {
      _signalsperre = true; // Signalsperrre ist Akitv
      Serial.println("gesperrt");
      Actor::digitalSchalten(_signalSperrmelder, HIGH); // stelle den Sperrmelder an
    }
    else
    {
      digitalSchalten(_signalSperrmelder, LOW);
    }
  }
}