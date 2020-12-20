/* Libary zum testen:
    ich möchte Weichen über diese Libary schalten und
    vielleicht auch Signale für die Modelleisenbahn
    Lennart Klüner 05.09.2020
*/

#include "Arduino.h"
#include <EEPROM.h>
#include "Schieberegister.h"
#include "Gleisbild.h"

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

int besetztmelder::besetztmelderAuslesen()
{
    //der Status des Besetztmelders wird eingelesen
    boolean newBesetztmelderstatus = digitalRead(_gleisPin);
    if (_besetztmelderAktiv && !_besetztmelderstatus == newBesetztmelderstatus)                     //wenn die Lampen des Besetztmelders an sein sollen(_besetztmelderAktiv), und der Besetztmelderstatus sich vom alten unterscheidet
    {
        _besetztmelderstatus=newBesetztmelderstatus;                                                //erneuere den Status des besetztmelders
                                                                                                    //passe die Led amzeige an
            if (_besetztmelderstatus)                                                               //wenn das Gleis besetzt ist
            {
                digitalSchalten(_ledGelb, LOW);                                                     //schalte die gelbe aus
                digitalSchalten(_ledRot, HIGH);                                                     //und die rote Led ein
            }
            else                                                                                    //wenn es nicht besetzt ist(_besetztmeldestatus == 0)
            {
                digitalSchalten(_ledRot, LOW);                                                      //sonst schalte die rote aus
                digitalSchalten(_ledGelb, HIGH);                                                    //und die gelbe Led an
            }
        
    }
    else                                                                                            // wird keine anzeige benötigt,
    {   
        digitalSchalten(_ledRot, LOW);                                                              //schalte alle Leds aus
        digitalSchalten(_ledGelb, LOW);
    }

    return _besetztmelderstatus;                                                                    //gibt am ende den Status des Besetztmelder zurück
}

void besetztmelder::setBesetztmelderAktiv(boolean newbesetztmelderStatus) //ein und ausschalten der Beleuchtung der Besetztmelder
{
    _besetztmelderAktiv = newbesetztmelderStatus;
}