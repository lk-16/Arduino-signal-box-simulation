/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include "melder.h"
#include "actors.h"


//Methoden der Klasse Melder
melder::melder(String melderName, int tueMelderLed, int weckerPin, int wutPin, int registerPins[4])
  : actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])   //Led des Melders, Name des Melders(für Serielle Kommunitation z.B. FTÜ,GTÜ)................doesn't works
{
  Serial.begin(9600);                               //Die Serielle Kommunikation wird gestartet

  //Anschlussdefinition
  _melderName = melderName;
  _tueMelderLed = tueMelderLed;                     //übernehmen der Variablen in die Klasse
  _wutPin = wutPin;
  _weckerPin= weckerPin;
  //definieren ob Anschlüsse Aus- oder Eingang sind
  pinMode(_tueMelderLed, OUTPUT);                   //Tastenüberwachung led Melder
  pinMode(_wutPin, INPUT_PULLUP);                          //Tastenüberwachung Weckerunterbrechertaste
  pinMode(_weckerPin, OUTPUT); 
}

void melder::tueMelder()                                                                //tastenüberwachung
{
  if ((millis() - _melderStartzeit >= _melderTimeout))
  {
    _tueMelderStatus = true;                                                             //FTÜ-Melder ist false keine Warnung, true melder und ggf. wecker schlägt an
    melder::blinken(_tueMelderLed);

    if (millis() - _melderStartzeit >= (_melderTimeout * 2) && _wutAktivierung == LOW)      //Wenn 10 s lang eine Taste gedrückt wurde und nicht die Wut gedrückt wurde
    {
      digitalSchalten(_weckerPin, LOW);                                           //Relais schaltet --> Wecker klingelt

    }
    if (melder::getWutStatus() == HIGH)                                                 //wenn Weckerunterbrechertaste gedrückt
    {
      _wutAktivierung = HIGH;                                                            //die Weckerunterbrechertaste wurde gedrückt
      digitalSchalten(_weckerPin, HIGH);                                          //Relais fällt zurück --> Wecker wird gestoppt
    }
    Serial.println(_melderName);                                                        //Meldername und Status wird über den seriellen Monitor angezeigt............................... Ausgabe der Meldernamen funktioniert noch nicht
  }
}


int melder::getTueLedPin()                 //Herausgabe des Melder LED-Pin
{
  return _tueMelderLed;                    //Pin wird zurückgegeben
}

boolean melder::getWutStatus()                        //gib den aktuellen Status der Weckerunterbrechertaste aus
{
  boolean wutstatus = digitalRead(_wutPin);
  return wutstatus;
}

void melder::setWutAktivierung(boolean aktivierung)   //verändere den Status der WUT-Aktivierung, ist die WUT aktiv oder nicht
{
  _wutAktivierung = aktivierung;
}

void melder::setTueMelderStatus(boolean newTueMelderStatus)         //verändere den Status der Tastenüberwachung, aktiviert oder nicht aktiviert
{
  _tueMelderStatus = newTueMelderStatus;
}

void melder::setMelderStartzeit(unsigned long newMelderStartzeit)   //verändere die Startzeit des melders um die Länge des Tastendrucks zu messen
{
  _melderStartzeit = newMelderStartzeit;
}

int melder::getWecker()
{
  return _weckerPin;
}