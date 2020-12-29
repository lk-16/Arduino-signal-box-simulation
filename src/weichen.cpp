/* Libary zum testen:
    ich möchte Weicehn über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Gleisbild.h"

//Methoden der Klasse Weichen

//definieren von für alle Methoden wichtige Informationen Pins etc.
weichen::weichen(int wnr, int weichenPinGerade, int weichenPinKurve, int weichenLedPinGerade, int weichenLedPinKurve, int adressWeichenposition, int weichentimeout, int wt, int wgt, int registerPins[4])
    : actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
  Serial.begin(9600);
  _wnr = wnr; //öffentliche Variablen der Klasse Weichen werden zu privaten Variablen der Klasse Weichen
  _weichenPinGerade = weichenPinGerade;
  _weichenPinKurve = weichenPinKurve;
  _weichenLedPinGerade = weichenLedPinGerade;
  _weichenLedPinKurve = weichenLedPinKurve;
  _adressWeichenposition = adressWeichenposition;
  _weichentimeout = weichentimeout;
  _wt = wt;
  _wgt = wgt;

  pinMode(_weichenPinGerade, OUTPUT); //define that the pins on the board are outputs
  pinMode(_weichenPinKurve, OUTPUT);
  pinMode(_weichenLedPinGerade, OUTPUT);
  pinMode(_weichenLedPinKurve, OUTPUT);
  pinMode(wt, INPUT_PULLUP);
  pinMode(wgt, INPUT_PULLUP);
}

//die Weiche wechselt ihre Position
void weichen::weicheWechsel()
{
  boolean _wtstatus = digitalRead(_wt);
  boolean _wgtstatus = digitalRead(_wgt);

  if (_wtstatus == HIGH && _wgtstatus == HIGH) //wenn die wt und wgt gedrückt werden
  {
    if (_weichenposition == true) //wenn die Weiche auf gerade steht schalte auf gerade
    {
      weichen::weicheKurve(); //stelle die Weiche auf Kurve
    }
    if (_weichenposition == false) //wenn die Weiche auf Kurve steht schalte auf Kurve
    {
      weichen::weicheGerade(); //stelle die Weiche auf Gerade
    }
  }
}

void weichen::weicheGerade() //die Weiche wird in gerade Lage vesetzt
{

  if (_weichenposition == false && weichenstatus == 0 && _weichenfestlegung == false) //wenn die Weiche nicht schaltet und in der Kurve steht
  {
    _wStartzeit = millis();
    actors::digitalSchalten(_weichenPinKurve, HIGH); //Relais werden geschaltet
    actors::digitalSchalten(_weichenPinGerade, LOW);

    if (_weichenausleuchtung == true)
    {
      actors::digitalSchalten(_weichenLedPinKurve, LOW); //Leds werden geschaltet
      actors::digitalSchalten(_weichenLedPinGerade, HIGH);
    }
    else
    {
      actors::digitalSchalten(_weichenLedPinKurve, LOW); //Leds werden geschaltet
      actors::digitalSchalten(_weichenLedPinGerade, LOW);
    }

    weichenstatus = 1;
    _weichenposition = true;                                 //die Weiche liegt gerade
    EEPROM.update(_adressWeichenposition, _weichenposition); //die Weichenposition wird gespeichert
  }
}

void weichen::weicheKurve() //die Weiche wird in Kurvenlage versetzt
{
  if (_weichenposition == true && weichenstatus == 0 && _weichenfestlegung == false)
  {
    _wStartzeit = millis();
    actors::digitalSchalten(_weichenPinGerade, HIGH); //Relais werden geschaltet
    actors::digitalSchalten(_weichenPinKurve, LOW);

    if (_weichenausleuchtung == true)
    {
      actors::digitalSchalten(_weichenLedPinGerade, LOW); //Leds werden geschaltet
      actors::digitalSchalten(_weichenLedPinKurve, HIGH);
    }
    else
    {
      actors::digitalSchalten(_weichenLedPinKurve, LOW); //Leds werden geschaltet
      actors::digitalSchalten(_weichenLedPinGerade, LOW);
    }
    weichenstatus = 2;                                       //der Weichenstatus ist 2 also kurve. blinken und timer werden abgerufen
    _weichenposition = false;                                //die Weiche steht auf Kurvenlage
    EEPROM.update(_adressWeichenposition, _weichenposition); //die Weichenposition wird gespeichert
  }
}

void weichen::weichenBlinken()
{ //eine Weichenled kann Blinken

  //festlegen, welche Led benötigt wird.
  int _ledPin;
  if (weichenstatus == 1)
  {
    _ledPin = _weichenLedPinGerade;
  }
  if (weichenstatus == 2)
  {
    _ledPin = _weichenLedPinKurve;
  }

  //blinken
  actors::blinken(_ledPin);
}

void weichen::weicheRelaisHIGH() //Alle Relais werden auf HIGH gesetzt
{
  actors::digitalSchalten(_weichenPinKurve, HIGH);
  actors::digitalSchalten(_weichenPinGerade, HIGH);
}

void weichen::weicheRelaisLOW() //Alle Relais werden auf HIGH gesetzt
{
  actors::digitalSchalten(_weichenPinKurve, LOW);
  actors::digitalSchalten(_weichenPinGerade, LOW);
}

void weichen::weichenpositionEEPROM() //die weichenposition wird abgerufen und auf die Variablen übertragen
{
  _weichenposition = EEPROM.read(_adressWeichenposition); //lese die Weichenposition ein
  if (_weichenposition == true)
  {
    _weichenposition = false; //die Weichenposition wird auf false gesetzt, damit die Weichen erneut geschaltet werden kann
    weichen::weicheGerade();
  }
  else
  {
    _weichenposition = true; //die Weichenposition wird auf false gesetzt, damit die Weichen erneut geschaltet werden kann
    weichen::weicheKurve();
  }
}

void weichen::weichenSchalten() //lässt die Relais wieder in die unaktiv position zurückfallen
{
  unsigned long currentmillis = millis();
  if (!weichen::weichenstatus == 0)
  {
    if (_weichenausleuchtung == true)
      weichen::weichenBlinken();
    if (currentmillis - _wStartzeit >= _weichentimeout)
    {
      weichen::weichenstatus = 0;
      weichen::weicheRelaisHIGH();

      if (_weichenausleuchtung == true)
      {
        if (_weichenposition == true)
          actors::digitalSchalten(_weichenLedPinGerade, HIGH);
        if (_weichenposition == false)
          actors::digitalSchalten(_weichenLedPinKurve, HIGH);
      }
      else
      {
        actors::digitalSchalten(_weichenLedPinKurve, LOW);
        actors::digitalSchalten(_weichenLedPinGerade, LOW);
      }

      Serial.print("Weiche ");
      Serial.print(_wnr);
      Serial.print("     "); //Ausgabe der Weichenstellung an den Seriellen Monitor
      if (_weichenposition == true)
        Serial.println("gerade");
      if (_weichenposition == false)
        Serial.println("kurve");
    }
  }
}

void weichen::setWeichenfestlegung(boolean festlegestatus, int fahrstrassennr) //kann die Festlegung der Weichen aktivieren, die Weichen können nicht mehr verändert werden, bis die Fahrstraße ausfgelöst ist
{
  static int _fahrstrassefestgelegt = 0; //speichert von welcher Fahrstraße die Weiche festgelegt wurde, und nur diese kann die Festlegung auch wieder lösen
  if (_fahrstrassefestgelegt == 0)       //wenn die Weiche nicht festgelgt ist
  {
    _weichenfestlegung = festlegestatus;     //kann sie von einer anderen Fahrstraße festgelgt werden, diese wird gespeichert
    _fahrstrassefestgelegt = fahrstrassennr; //und ide Fahrstraße entsprechend festgelegt
  }
  else if (fahrstrassennr == _fahrstrassefestgelegt && _fahrstrassefestgelegt != 0) //|| _fahrstrassenfestlegung == 0)
  {
    _weichenfestlegung = festlegestatus;
    _fahrstrassefestgelegt = 0;
  }
}

boolean weichen::getWeichenfestlegung()
{
  return _weichenfestlegung;
}