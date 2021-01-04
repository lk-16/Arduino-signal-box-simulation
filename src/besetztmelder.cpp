/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "besetztmelder.h"

//Methoden der Klasse besetztmelder
besetztmelder::besetztmelder(int gleisPin, int ledGelb, int ledRot, int registerPin[4])
    : actors(registerPin[0], registerPin[1], registerPin[2], registerPin[3])
{
    _gleisPin = gleisPin;
    _ledGelb = ledGelb;
    _ledRot = ledRot;

    pinMode(_gleisPin, INPUT);
    pinMode(_ledGelb, OUTPUT);
    pinMode(_ledRot, OUTPUT);
}
boolean besetztmelder::besetztmelderAuslesen(boolean besetztmelderBeleuchtung)
{
    //der Status des Besetztmelders wird eingelesen
    _besetztmelderLicht = besetztmelderBeleuchtung;
    _besetztmelderstatus = digitalRead(_gleisPin);

    if (_besetztmelderstatus == false && (_besetztmelderLicht || _fahrstrassenelement)) //wenn das Gleis  nicht besetzt ist und die Anzeige an ist, oder wenn der Besetztmelder Teil einer Fahrstraße ist
    {
        digitalSchalten(_ledRot, LOW);   //sonst schalte die rote aus
        digitalSchalten(_ledGelb, HIGH); //und die gelbe Led an
    }
    else if (!_besetztmelderstatus && !_besetztmelderLicht && !_fahrstrassenelement) // wird keine anzeige benötigt,
    {
        digitalSchalten(_ledRot, LOW); //schalte alle Leds aus
        digitalSchalten(_ledGelb, LOW);
    }
    else //wenn das Gleis besetzt ist(_besetztmeldestatus == 1)
    {
        digitalSchalten(_ledGelb, LOW); //schalte die gelbe aus
        digitalSchalten(_ledRot, HIGH); //und die rote Led ein
    }

    return _besetztmelderstatus; //_besetztmelderstatus; //gibt am ende den Status des Besetztmelder zurück
}

void besetztmelder::setBesetztmelderLicht(boolean newbesetztmelderLicht) //ein und ausschalten der Beleuchtung der Besetztmelder
{
    _besetztmelderLicht = newbesetztmelderLicht;
}

void besetztmelder::setFahrstrassenelement(int fahrstrassennr, boolean fahrstrassenelement)//nr der Fahrstraße, soll es zu einem Fahrstraßenelement werden oder nicht
{
    static int _fahrstrasse = 0; //speichert von welcher Fahrstraße der Besetztmelder festgelegt wurde, und nur diese kann die Festlegung auch wieder lösen
    if (_fahrstrasse == 0)       //wenn der Melder nicht beansprucht ist
    {
        _fahrstrassenelement = fahrstrassenelement;     //kann sie von einer anderen Fahrstraße festgelgt werden, diese wird gespeichert
        _fahrstrasse = fahrstrassennr; //und die Fahrstraße entsprechend festgelegen
    }
    else if (fahrstrassennr == _fahrstrasse && _fahrstrasse != 0) //|| _fahrstrassenfestlegung == 0)
    {
        _fahrstrassenelement = fahrstrassenelement;                 //der Status, ob die weiche festgelegt wurde wird geänder
        if(_fahrstrassenelement == 0) _fahrstrasse = 0;             //wenn die Fahrstraße nicht mehr beansprucht ist, kann sie von allen anderen Fahrstraßen verendet werden
    }
}
boolean besetztmelder::getFahrstrassenelement()
{
    return _fahrstrassenelement;
}