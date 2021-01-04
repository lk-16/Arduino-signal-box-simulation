/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include "hauptsignale.h"
#include "signale.h"

//Methoden der Klasse Hauptsignale, Unterklassen der Klasse actors und signale
hauptsignale::hauptsignale(int rotPin, int gelbPin, int gruenPin,  int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4])
    : signale(signaltaste, sperrmelder, allgSignaltasten, registerPin)
{
  _rotPin = rotPin;     //Halt Pin(rot)................................................................................................
  _gelbPin = gelbPin;   //Fahrt Pin(grün)
  _gruenPin = gruenPin; //langsamfahrt (grün und gelb)
  
  pinMode(_rotPin, OUTPUT);
  pinMode(_gelbPin, OUTPUT);
  pinMode(_gruenPin, OUTPUT);
  
}

void hauptsignale::setSignalHp0()
{
  signale::digitalSchalten(_gruenPin, LOW);
  signale::digitalSchalten(_gelbPin, LOW);
  //timer?
  signale::digitalSchalten(_rotPin, HIGH); //alle Lichter werden auf Low gestellt und dann Halt (rot) auf HIGH
}

void hauptsignale::setSignalHp1()
{
  signale::digitalSchalten(_rotPin, LOW);
  signale::digitalSchalten(_gelbPin, LOW);
  signale::digitalSchalten(_gruenPin, HIGH);
}

void hauptsignale::setSignalHp2()
{
  signale::digitalSchalten(_rotPin, LOW); //.. nur Halt also rot geht aus, alle anderen bleiben an
  signale::digitalSchalten(_gelbPin, HIGH);
  signale::digitalSchalten(_gruenPin, HIGH);
}

void hauptsignale::hpschalten(int newStatus) //funktion stellt das Signal
{
  
  
  if (signale::getSignalsperre() == false && newStatus != signale::getSignalstatus())              //kontrolle der Signalsperre und anschließende Kontrolle, ob sich der Wert geändert hat, spart Zeit im Programmcode
  {
    signale::setSignalstatus(newStatus);
    if (signale::getSignalstatus() == 0) hauptsignale::setSignalHp0();
    if (signale::getSignalstatus() == 1) hauptsignale::setSignalHp1();
    if (signale::getSignalstatus() == 2) hauptsignale::setSignalHp2();
  }
}

void hauptsignale::hauptsignalhp0manuell() //Funktion sorgt dafür, dass mit signalgruppensperrtaste und signaltaste ein signal in hp0 versetzt werden kann
{
  //boolean _signalhaltgruppentasteStatus = );
  //boolean _signaltasteStatus = );
//Serial.print("signaltaste ");Serial.println(digitalRead(signale::getSignaltaste()));
  //  Serial.print("Signalhalttaste ");Serial.println(digitalRead(signale::getSignalhaltgruppentaste()));

  if (digitalRead(signale::getSignalhaltgruppentaste()) == HIGH && digitalRead(signale::getSignaltaste()) == HIGH) 
  {
        hauptsignale::hpschalten(0); //bei drücken von Signal und Signalhalttaste schalte signal auf hp0
  }
  //wenn vorsignal am mast, dann dunkel weil hauptsignal rot..............................muss noch ergänzt werden
}
