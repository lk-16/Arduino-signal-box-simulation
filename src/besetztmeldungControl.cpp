/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Gleisbild.h"

//Methoden der Klasse besetztmeldungControl
besetztmeldungControl::besetztmeldungControl(int gleisPins[], int ledsGelb[], int ledsRot[], int anzahlMelder, int registerPins[4])
    : actors(registerPins[0], registerPins[1], registerPins[2], registerPins[3])
{
    _anzahlMelder = anzahlMelder;
    _gleisPins = (int *)calloc(_anzahlMelder, (sizeof(int)));
    _ledsGelb = (int *)calloc(_anzahlMelder, (sizeof(int)));
    _ledsRot = (int *)calloc(_anzahlMelder, (sizeof(int)));

    _gleisPins = gleisPins; //Register werden in die Klasse übertragen
    _ledsGelb = ledsGelb;
    _ledsRot = ledsRot;

    /*
    _besetztmelderArray = (besetztmelder *)calloc(anzahlMelder, (sizeof(besetztmelder)));
    if (_besetztmelderArray == NULL) //wenn kein speicher mehr frei ist, soll eine Fehlermeldung ausgegeben werden
    {
        Serial.println("Error: Daten der Objekte der Klasse Besetztmelder können nicht gespeichert werden. Source: besetztmeldungControl.cpp besetztmeldungControl::besetztmeldungControl");
        return;
    }

    for (int i = 0; i < anzahlMelder; i++)
    {
        String besetztmeldername = "Besetztmelder " + i;
        _besetztmelderArray[i] = besetztmelder(besetztmeldername, gleisPins[i], ledsGelb[i], ledsRot[i], registerPins);
    }*/
}

boolean besetztmeldungControl::getBesetztmelderstatus(int besetztmelder)
{
    return digitalRead(_gleisPins[besetztmelder]);
}
