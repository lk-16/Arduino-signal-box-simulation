/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include <Arduino.h>
#include "actors.h"
#include "signale.h"

//Methoden der Klasse signal, Unterklasse der Klasse actors
signale::signale(int signaltastenPin, int sperrmelderPin, int allgSignaltasten[3], int registerPins[4])      //Konstuktor: allgemeine Signaltasten beinhalten die Signalsperr- und entsperrtaste und Signalhaltgruppentaste
    : actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
  _signalsperrtaste = allgSignaltasten[0];
  _signalentsperrtaste = allgSignaltasten[1];
  _signalhaltgruppentaste = allgSignaltasten[2];
  _signaltaste = signaltastenPin;
  _signalSperrmelder = sperrmelderPin;

  pinMode(_signalsperrtaste, INPUT);
  pinMode(_signalentsperrtaste, INPUT);
  pinMode(_signalhaltgruppentaste, INPUT);
  pinMode(_signaltaste, INPUT);
  pinMode(_signalSperrmelder, OUTPUT);
}

int signale::getSignalstatus()
{
  return _signalstatus;
}

void signale::setSignalstatus(int newSignalstatus)
{
  _signalstatus = newSignalstatus;
}

int signale::getSignaltaste()
{
  return _signaltaste;
}

int signale::getSignalhaltgruppentaste()
{
  return _signalhaltgruppentaste;
}

int signale::getSignalsperrmelder()
{
  return _signalhaltgruppentaste;
}

int signale::getSignalsperrtaste()
{
  return _signalsperrtaste;
}

boolean signale::getSignalsperre()
{
  return _signalsperre;
}



void signale::signalSperren()
{
  if (_signalsperre == true) //wenn das Signal gesperrt ist.
  {
    //Serial.println("gesperrt");
    if (digitalRead(_signalentsperrtaste) == HIGH && digitalRead(_signaltaste) == HIGH) //wenn die Signalsperr- und Signaltaste gedrückt, das Signal ist ja Rot
    {
      _signalsperre = false;                                //Signalsperrre ist Inaktiv
      Serial.println("entsperrt");
      digitalSchalten(_signalSperrmelder, LOW); //stelle den Sperrmelder aus
    }
    else
    {                               
      digitalSchalten(_signalSperrmelder, HIGH);
    }
  }
  else                                                    //wenn das Signal nicht gesperrt ist
  {
    //Serial.println("entsperrt");
    if (digitalRead(_signalsperrtaste) == HIGH && digitalRead(_signaltaste) == HIGH && _signalstatus == 0) //wenn die Signalsperr- und Signaltaste gedrückt und das Signal Rot ist
    {
      _signalsperre = true;                                //Signalsperrre ist Akitv
      Serial.println("gesperrt");
      digitalSchalten(_signalSperrmelder, HIGH); //stelle den Sperrmelder an
    }
    else
    {                               
      digitalSchalten(_signalSperrmelder, LOW);
    }
  }
  
}