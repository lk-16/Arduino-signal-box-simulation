/* Libary zum testen:
    ich möchte Weicehn über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Weiche.h"

//Methoden der Klasse Weichen

//definieren von für alle Methoden wichtige Informationen Pins etc.
Weiche::Weiche(int wnr, int weichenPinGerade, int weichenPinKurve, int weichenLedPinGerade, int weichenLedPinGeradeRot, int weichenLedPinKurve, int weichenLedPinKurveRot, int adressWeichenposition, int weichentimeout, int wt, int wgt, int registerPins[4])
    : Actor(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
  Serial.begin(9600);
  _wnr = wnr; //öffentliche Variablen der Klasse Weichen werden zu privaten Variablen der Klasse Weichen
  _weichenPinGerade = weichenPinGerade;
  _weichenPinKurve = weichenPinKurve;
  _weichenLedPinGerade = weichenLedPinGerade;
  _weichenLedPinGeradeRot = weichenLedPinGeradeRot;
  _weichenLedPinKurve = weichenLedPinKurve;
  _weichenLedPinKurveRot = weichenLedPinKurveRot;
  _adressWeichenposition = adressWeichenposition;
  _weichentimeout = weichentimeout;
  _wt = wt;
  _wgt = wgt;

  pinMode(_weichenPinGerade, OUTPUT); //define that the pins on the board are outputs
  pinMode(_weichenPinKurve, OUTPUT);
  pinMode(_weichenLedPinGerade, OUTPUT);
  pinMode(_weichenLedPinGeradeRot, OUTPUT);
  pinMode(_weichenLedPinKurve, OUTPUT);
  pinMode(_weichenLedPinKurveRot, OUTPUT);
  pinMode(wt, INPUT_PULLUP);
  pinMode(wgt, INPUT_PULLUP);
}

//die Weiche wechselt ihre Position
void Weiche::weicheWechsel()
{
  boolean _wtstatus = digitalRead(_wt);
  boolean _wgtstatus = digitalRead(_wgt);

  if (_wtstatus == HIGH && _wgtstatus == HIGH) //wenn die wt und wgt gedrückt werden
  {
    if (_weichenposition == true) //wenn die Weiche auf gerade steht schalte auf gerade
    {
      Weiche::weicheKurve(); //stelle die Weiche auf Kurve
    }
    if (_weichenposition == false) //wenn die Weiche auf Kurve steht schalte auf Kurve
    {
      Weiche::weicheGerade(); //stelle die Weiche auf Gerade
    }
  }
}

void Weiche::weicheGerade() //die Weiche wird in gerade Lage vesetzt
{

  if (_weichenposition == false && weichenstatus == 0 && _weichenfestlegung == false) //wenn die Weiche nicht schaltet und in der Kurve steht
  {
    _wStartzeit = millis();
    Actor::digitalSchalten(_weichenPinKurve, HIGH); //Relais werden geschaltet
    Actor::digitalSchalten(_weichenPinGerade, LOW);

    if (_weichenausleuchtung == true)
    {
      Actor::digitalSchalten(_weichenLedPinKurve, LOW); //Leds werden geschaltet
      Actor::digitalSchalten(_weichenLedPinGerade, HIGH);
    }
    else
    {
      Actor::digitalSchalten(_weichenLedPinKurve, LOW); //Leds werden geschaltet
      Actor::digitalSchalten(_weichenLedPinGerade, LOW);
    }

    weichenstatus = 1;
    EEPROM.update(_adressWeichenposition, _weichenposition); //die Weichenposition wird gespeichert
  }
}

void Weiche::weicheKurve() //die Weiche wird in Kurvenlage versetzt
{
  if (_weichenposition == true && weichenstatus == 0 && _weichenfestlegung == false)
  {
    _wStartzeit = millis();
    Actor::digitalSchalten(_weichenPinGerade, HIGH); //Relais werden geschaltet
    Actor::digitalSchalten(_weichenPinKurve, LOW);

    if (_weichenausleuchtung == true)
    {
      Actor::digitalSchalten(_weichenLedPinGerade, LOW); //Leds werden geschaltet
      Actor::digitalSchalten(_weichenLedPinKurve, HIGH);
    }
    else
    {
      Actor::digitalSchalten(_weichenLedPinKurve, LOW); //Leds werden geschaltet
      Actor::digitalSchalten(_weichenLedPinGerade, LOW);
    }
    weichenstatus = 2;                                       //der Weichenstatus ist 2 also kurve. blinken und timer werden abgerufen
    EEPROM.update(_adressWeichenposition, _weichenposition); //die Weichenposition wird gespeichert
  }
}

void Weiche::weichenBlinken()
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
  Actor::blinken(_ledPin);
}

void Weiche::weicheRelaisHIGH() //Alle Relais werden auf HIGH gesetzt
{
  Actor::digitalSchalten(_weichenPinKurve, HIGH);
  Actor::digitalSchalten(_weichenPinGerade, HIGH);
}

void Weiche::weicheRelaisLOW() //Alle Relais werden auf HIGH gesetzt
{
  Actor::digitalSchalten(_weichenPinKurve, LOW);
  Actor::digitalSchalten(_weichenPinGerade, LOW);
}

void Weiche::weichenpositionEEPROM() //die weichenposition wird abgerufen und auf die Variablen übertragen
{
  _weichenposition = EEPROM.read(_adressWeichenposition); //lese die Weichenposition ein
  if (_weichenposition == true)
  {
    _weichenposition = false; //die Weichenposition wird auf false gesetzt, damit die Weichen erneut geschaltet werden kann
  Weiche::weicheGerade();
  }
  else
  {
    _weichenposition = true; //die Weichenposition wird auf false gesetzt, damit die Weichen erneut geschaltet werden kann
    Weiche::weicheKurve();
  }
}

void Weiche::weicheSchalten() //lässt die Relais wieder in die unaktiv position zurückfallen
{
  unsigned long currentmillis = millis();
  if (!Weiche::weichenstatus == 0)
  {
    if (_weichenausleuchtung == true)
      Weiche::weichenBlinken();
    if (currentmillis - _wStartzeit >= _weichentimeout)
    {
      if(weichenstatus== 1) _weichenposition = true;                   //die Weiche steht auf Kurvenlage
      else if(weichenstatus == 2) _weichenposition = false;            //die Weiche liegt gerade
      weichenstatus = 0;
      Weiche::weicheRelaisHIGH();

      if (_weichenausleuchtung == true)
      {
        if (_weichenposition == true)
          Actor::digitalSchalten(_weichenLedPinGerade, HIGH);
        if (_weichenposition == false)
          Weiche::setWeichenLeds(false);
          //Actor::digitalSchalten(_weichenLedPinKurve, HIGH);
      }
      else
      {
        Actor::digitalSchalten(_weichenLedPinKurve, LOW);
        Actor::digitalSchalten(_weichenLedPinGerade, LOW);
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

void Weiche::setWeichenfestlegung(boolean festlegestatus, int fahrstrassennr) //kann die Festlegung der Weichen aktivieren, die Weichen können nicht mehr verändert werden, bis die Fahrstraße ausfgelöst ist
{
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

boolean Weiche::getWeichenfestlegung()
{
  return _weichenfestlegung;
}

boolean Weiche::getWeichenposition()
{
  return _weichenposition;
}
void Weiche::setWeichebesetzt(boolean besetztmelderstatus)
{
  _besetzt=besetztmelderstatus;
}

//private
void Weiche::setWeichenLeds(boolean weichenlage)
{
  if(_besetzt == true)
  {
    if(weichenlage == true)
    {
      Actor::digitalSchalten(_weichenLedPinKurve, LOW);
      Actor::digitalSchalten(_weichenLedPinKurveRot, LOW);
      Actor::digitalSchalten(_weichenLedPinGerade, LOW);
      Actor::digitalSchalten(_weichenLedPinGeradeRot, HIGH);
    }
    else
    {
      Actor::digitalSchalten(_weichenLedPinGerade, LOW);
      Actor::digitalSchalten(_weichenLedPinGeradeRot, LOW);
      Actor::digitalSchalten(_weichenLedPinKurve, LOW);
      Actor::digitalSchalten(_weichenLedPinKurveRot, HIGH);
    }
    
  }
  else//wenn nicht besetzt
  {
    if(weichenlage == true)
    {
      Actor::digitalSchalten(_weichenLedPinKurve, LOW);
      Actor::digitalSchalten(_weichenLedPinKurveRot, LOW);
      Actor::digitalSchalten(_weichenLedPinGeradeRot, LOW);
      Actor::digitalSchalten(_weichenLedPinGerade, HIGH);
    }
    else
    {
      Actor::digitalSchalten(_weichenLedPinGerade, LOW);
      Actor::digitalSchalten(_weichenLedPinGeradeRot, LOW);
      Actor::digitalSchalten(_weichenLedPinKurveRot, LOW);
      Actor::digitalSchalten(_weichenLedPinKurve, HIGH);
    }
  }
}