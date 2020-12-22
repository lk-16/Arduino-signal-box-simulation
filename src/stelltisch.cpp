/* Libary zum testen:
    ich möchte Weicehn über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Gleisbild.h"

//Methoden der Klasse actors
stelltisch::stelltisch(int registerPins[4])
:actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{

}

stelltisch::~stelltisch()
{

}

boolean stelltisch::getWeichenausleuchtung()
{
    return weichenausleuchtung;
}

void stelltisch::weichenausleuchtungEinstellen(int tasterEin, int ledEin, int tasterAus, int ledAus)                                           //Die für diese Funktion wichtigen Pins werden nicht im Konstruktor deklariert, so bleibt die übersicht bei weiteren Funktionen
{
    if(tasterEin == HIGH)
    {
        weichenausleuchtung = true;
        actors::digitalSchalten(ledAus, LOW);
        actors::digitalSchalten(ledEin, HIGH);
        //led anschalten(entweder wird der Pin jedes Mal durch die Methode übertragen, oder der Wert existiert im Zugriff schon und wird nur übernommen)
    }
    else if (tasterAus == HIGH)
    {
        weichenausleuchtung = false;
        actors::digitalSchalten(ledEin, LOW);
        actors::digitalSchalten(ledAus, HIGH);
    }
}