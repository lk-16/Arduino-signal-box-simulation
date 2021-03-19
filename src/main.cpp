#include <Arduino.h>
#include "WeichenControl.h"
#include "ZugtastenControl.h"
#include "BesetztmeldungControl.h"
#include "HauptsignalControl.h"
#include "Melder.h"

//megaatmega2560
//Pinbelegung
int schieberegisterPins[4] = {4, 8, 9, 10};
int w1g = 24; //w=Weiche  bzw. Weichenrelais g=Relais für gerade
int w1k = 25; //g=Relais für gerade
int w2g = 22;
int w2k = 23;
int w3g = 26;
int w3k = 27;

int wt1 = 11; //Pinbelegung Weichentasten
int wt2 = 20;
int wt3 = 2;

int ledw1g = 109; //Pinbelegung der WeichenLEDs
int ledw1k = 108;
int ledw2g = 50;
int ledw2k = 39;
int ledw3g = 106;
int ledw3k = 104;

int weichentimeout = 500; //dauer des Schaltvorgangs bei den Weichen
int wgt = 18;             //Weichengruppentaste

int zta1 = 5;  // Zugstraßentaste 1 an Pin 2
int zta2 = 21; // Zugstraßentaste 2 an Pin 3
int zta3 = 12; // Zugstraßentaste 3 an Pin 4
int zta4 = 3;
int zta5 = 6;
int zta6;
int zta7 = 13;
int zta8 = 4;
int zta9 = 7;

//Melder
int ftueMelderLed = 124; //Fahrstraßentastenüberwachung
int ftueMelderWut = 19;
int weckerPin = 28;
String ftueMelderName = "ftueMelder";

//Signale
int signalsperrtaste = 16;                                               //signalsperrtaste, zu testzwecken auf weichengruppentaste gestellt
int Signalentsperrtaste = 17;                                            //s.o
int shgt = 15;                                                           //s.o
int allgSignaltasten[3] = {signalsperrtaste, Signalentsperrtaste, shgt}; //Signalsperrtaste, Signalentsperrtaste, Signalhaltgruppentaste, Array speichert für jedes Signal wichtige informationen, hält den schreibaufwand geringer

//Signal1
int rot1 = 52;    //Hp0 (rot) vom Signal 1
int gruen1 = 125; //Hp1 (grün)  vom Signal 1
int gelb1;        //Hp2 (langsamfahrt) vom Signal 1
int sperrmelder1 = 126;
int signaltaste1 = zta3; //Signaltaste, zu tastzwecken weichentaste 1

//Signal2
int rot2 = 123;  //Hp0 (rot) vom Signal 1
int gruen2 = 46; //Hp1 (grün)  vom Signal 1
int gelb2;       //Hp2 (langsamfahrt) vom Signal 1
int sperrmelder2 = 49;
int signaltaste2 = zta4; //Signaltaste, zu tastzwecken weichentaste 1

//Signal3
int rot3 = 121;   //Hp0 (rot) vom Signal 1
int gruen3 = 122; //Hp1 (grün)  vom Signal 1
int gelb3;        //Hp2 (langsamfahrt) vom Signal 1
int sperrmelder3 = 127;
int signaltaste3 = zta7; //Signaltaste, zu tastzwecken weichentaste 1

//Signal4
int rot4 = 47;    //Hp0 (rot) vom Signal 1
int gruen4 = 111; //Hp1 (grün)  vom Signal 1
int gelb4;        //Hp2 (langsamfahrt) vom Signal 1
int sperrmelder4 = 48;
int signaltaste4 = zta8; //Signaltaste, zu tastzwecken weichentaste 1

const int hauptsignalanzahl = 4;
int rot[hauptsignalanzahl] = {rot1, rot2, rot3, rot4};
int gelb[hauptsignalanzahl] = {gelb1, gelb2, gelb3, gelb4};
int gruen[hauptsignalanzahl] = {gruen1, gruen2, gruen3, gruen4};
int signaltasten[hauptsignalanzahl] = {signaltaste1, signaltaste2, signaltaste3, signaltaste4};
int sperrmelder[hauptsignalanzahl] = {sperrmelder1, sperrmelder2, sperrmelder3, sperrmelder4};
HauptsignalControl hauptsignale(hauptsignalanzahl, rot, gelb, gruen, signaltasten, sperrmelder, allgSignaltasten, schieberegisterPins);
//int rotPin, int gelbPin, int gruenPin,  int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4]

//Gleisbesetztmelder
//jedes Feld muss als einzelner Gleisbesetztmelder definiert sein
const int besetztmelderAnzahl = 19;
int weichenbesetztmelder[besetztmelderAnzahl] = {0, 0,
                                                 0, 0, 0, 0, 1, 0, 2, 0, 0,
                                                 0, 0, 0, 0, 3, 0, 0, 0};                                                       //Weichennummern werden übergeben
int besetztmelderEingaenge[besetztmelderAnzahl] = {69, 69, 59, 59, 63, 65, 65, 68, 68, 67, 67, 60, 60, 64, 64, 62, 62, 61, 61}; //an gnd angeschlossen
int besetztmelderLedsGelb[besetztmelderAnzahl] = {33, 45, 130, 116, 117, 114, 0, 102, 0, 31, 43, 121, 128, 133, 112, 0, 37, 35, 41};
int besetztmelderLedsRot[besetztmelderAnzahl] = {32, 44, 58, 0, 118, 115, 0, 103, 0, 30, 42, 132, 129, 120, 113, 0, 36, 34, 40};
BesetztmeldungControl besetztmeldung(besetztmelderEingaenge, besetztmelderLedsGelb, besetztmelderLedsRot, weichenbesetztmelder, besetztmelderAnzahl, schieberegisterPins);

//Weichen
const int anzahlWeichen = 3;
int weichenPinsGerade[anzahlWeichen] = {w1g, w2g, w3g};
int weichenPinsKurve[anzahlWeichen] = {w1k, w2k, w3k};
int weichenLedPinsGerade[anzahlWeichen] = {ledw1g, ledw2g, ledw3g};
int weichenLedPinsGeradeRot[anzahlWeichen] = {110, 101, 107};
int weichenLedPinsKurve[anzahlWeichen] = {ledw1k, ledw2k, ledw3k};
int weichenLedPinsKurveRot[anzahlWeichen] = {51, 38, 105};
int adressWeichenpositionen[anzahlWeichen] = {1, 5, 9};
int weichentasten[anzahlWeichen] = {wt1, wt2, wt3};
WeichenControl weichen(anzahlWeichen, weichenPinsGerade, weichenPinsKurve, weichenLedPinsGerade, weichenLedPinsGeradeRot, weichenLedPinsKurve, weichenLedPinsKurveRot, adressWeichenpositionen, weichentimeout, weichentasten, wgt, schieberegisterPins);

//Melder
Melder ftueMelder(ftueMelderName, ftueMelderLed, weckerPin, ftueMelderWut, schieberegisterPins); //Ausgabe von FTÜ im Seriellen Monitior funktioniert nicht

//Zugtasten
const int zugtastenanzahl = 9;
int zugtastenPins[zugtastenanzahl] = {zta1, zta2, zta3, zta4, zta5, zta6, zta7, zta8, zta9};
ZugtastenControl zugtastenC(zugtastenanzahl, zugtastenPins);

//Fahrstraßensteuerung
const int fahrstrassenanzahl = 8;
int zugtastenspeicher[2];                                    //es werden die Zugtasten gespeichert, die gedrückt wurden.
int anzahl = 0;                                              //zählt wie viele zugtasten gedrückt wurden
boolean fahrstrassenstati[fahrstrassenanzahl];               //Beinhaltet, welche Fahrstrassen aktiv sind.
boolean fahrstrassenkontolle[fahrstrassenanzahl];            //wenn auf true, dann sind alle Besetztmelder frei und für die Fahrstrasse festgelegt und die Stellaufträge für die Weichen sind gegeben.
int const felderAnzahl = 9;                                  //Anzahl der Felder auf dem Feld
int fahrstrasse;                                             //zeigt an welche Fahrstrasse kurzzeitig aktiv ist.
int fahrstrassenspeicher[felderAnzahl][felderAnzahl];        //1 variable = nummer des Tischfeldes, 2. variable = nummer des Zweiten Tischfeldes ergibt die zu betätigende Fahrstrasse
int freigabe[fahrstrassenanzahl] = {1, 1, 1, 1, 1, 1, 1, 1}; //zählt wie viele halbe Besetztmelder schon freigegeben wurden
int besetztmelderposition[fahrstrassenanzahl + 1][/*Reihen(Bestztm.)*/ 9][5 /*Tiefe*/] = {
    {{}},
    {{7, 1}, {4}, {5}, {6, 1, 1}, {7}, {8, 2, 1}, {9}, {10}},    //3-5 ->
    {{7, 1}, {4}, {5}, {6, 1, 1}, {7}, {8, 2, 0}, {0}, {1}},     //3-1 ->
    {{7, 1}, {4}, {5}, {6, 1, 0}, {15, 3, 0}, {16}, {17}, {18}}, //3-9 ->
    {{6, 3}, {13}, {14}, {15, 3, 1}, {16}, {17}, {18}},          //7-9 ->
    {{7, 2}, {8, 2, 1}, {7}, {6, 1, 1}, {5}, {4}, {3}, {2}},     //4-2 <-
    {{6, 4}, {16}, {15, 3, 1}, {14}, {13}, {12}, {11}},          //8-6 <-
    {{7, 4}, {16}, {15, 3, 0}, {6, 1, 0}, {5}, {4}, {3}, {2}},   //8-2 <-
    {{7, 0}, {8, 2, 0}, {7}, {6, 1, 1}, {5}, {4}, {3}, {2}}      //1-2 <-
};                                                               //0. ebene Besetztmelder anzahl, in den nächsten ebenen sind die Besetztmelder in Reihenfolge gespeichert. Die nächste Dimeinsion speichert, ob das Feld auf dem Besetztmelder eine Weiche ist, und darunter steht in welche richtung die geschaltet werden muss. Folgend steht, ob es eine Falnkenschutz weiche gibt                                                                            //anhand dieser Zahl lässt sich der Wert von Freigabe berechnen, enthält bis zu welchem Besetztmelder die Fahrstrasse aufgelöst ist.
int besetztmelderzahl[fahrstrassenanzahl] = {0};                 //die anzahl der Besetztmelder die schon freigegeben wurde

//Fahrstraßenfindung:
const int breite = 9;
const int hoehe = 3;
const char stellpult[hoehe][breite] = {{'0', '0', '0', '0', '0', '0', '0', '/', ')'},
                                       {'(', '-', '-', '-', '<', '-', '<', '-', ')'},
                                       {'0', '(', '-', '-', '-', '>', '-', '-', ')'}};
int verbindungen[besetztmelderAnzahl][besetztmelderAnzahl];

void setup()
{
  //Die unbenutzten Felder(0) dürfen nicht mitgezählt werden, damit die FElder sich auf die Besetztmelder übertragen lassen.
  int reihenfolge0; // zählt wie viele nullen hintereinander liegen
  for (int h = 0; h < hoehe; h++)
  {
    for (int b = 0; b < breite; b++)
    {
      //unbelegtes
      if (stellpult[h][b] == '0')
      {
        reihenfolge0++;
      }
      //geraden
      if (stellpult[h][b] == '-')
      {
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 - 1] = 1; //Notiere die Verbindung davor und dahinter als Verbindung
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 + 1] = 1;
      }
      //ende rechts
      if (stellpult[h][b] == ')')
      {
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 - 1] = 1;
      }
      //ende links
      if (stellpult[h][b] == '(')
      {
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 + 1] = 1;
      }
      //Kurve
      if (stellpult[h][b] == '/')
      {
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 + 1] = 1; //Suche nach dem Feld links
        //suche das Feld schräg unterhalb der Kurve
        int reihenfolge01 = reihenfolge0;  //zum zählen der nullen, die es von der Start zur zielposition gibt
        for (int h3 = h; h3 < h - 1; h3++) //suche solange, bis du auf der Richtigen höhe bist
        {
          for (int b3 = b; b3 < b - 1; b3++) //suche solange bis zu in der Rictigen breite bist
          {
            if (b3 == b - 1 && h3 == h - 1)
              verbindungen[h * b - reihenfolge0][b3 * h3 - reihenfolge01] = 1; //1. Position Ausgangssymbol, 2. Position zielsymbol
            else if (stellpult[h3][b3] == '0')
              reihenfolge01++; //wenn in einem FEld vom Start zum ziel eine Null liegt, dann notiere das.
          }
        }
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 + 1] = 1;
      }

      //Weiche nach links
      if (stellpult[h][b] == '>') // wenn an der Stelle eine Weiche liegt
      {
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 - 1] = 1; //notiere, das die Schiene hinter dem Gleis verbunden ist
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 + 1] = 1; //notiere, das die Schiene gerade vor dem Gleis verbunden ist

        //oberhalb
        if (stellpult[h - 1][b - 1] == '<') //ist die verbundene Weiche oberhalb verbunden.
        {
          //Suche nach der Nummer des Besetztmelders
          int nullenb = 0; //zähler, wie viele Nullen bei der Suche nach dem passenden Gleis liegen
          for (int h2 = 0; h2 <= h - 1; h2++)
          {
            for (int k = 0; k <= b - 1; k++) //wiederhole bis du eine Position weiter rechts bist als die Unterliegende Weiche
            {
              if (stellpult[h2][k] == '0')
              {
                nullenb++; //Finde heraus, wie viele Nullen vor dem Besetztmelder lagen.
              }
              if (h2 == h - 1 && k == b - 1)
                verbindungen[h * b - reihenfolge0][h2 * k - nullenb] = 1; //notiere, dass die Schiene oberhalb des Gleises verbunden ist.
            }
          }
        }

        //unterhalb
        else if (stellpult[h + 1][b - 1] == '<') //ist die verbundene Weiche unterhalb verbunden.
        {
          //Suche nach der Nummer des Besetztmelders
          int nullenb = 0; //zähler, wie viele Nullen bei der Suche nach dem passenden
          for (int h2 = 0; h2 <= h + 1; h2++)
          {
            for (int k = 0; k <= b - 1; k++) //wiederhole bis du eine Position weiter rechts bist als die Unterliegende Weiche
            {
              if (stellpult[h2][k] == '0')
              {
                nullenb++; //Finde heraus, wie viele Nullen vor dem Besetztmelder lagen.
              }
              if (h2 == h + 1 && k == b - 1)
                verbindungen[h * b - reihenfolge0][h2 * k - nullenb] = 1; //notiere, dass die Schiene oberhalb des Gleises verbunden ist.
            }
          }
        }
      }
      //weiche nach rechts
      if (stellpult[h][b] == '<') // wenn an der Stelle eine Weiche liegt
      {
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 - 1] = 1; //notiere, das die Schiene hinter dem Gleis verbunden ist
        verbindungen[h * b - reihenfolge0][h * b - reihenfolge0 + 1] = 1; //notiere, das die Schiene gerade vor dem Gleis verbunden ist

        //oberhalb
        if (stellpult[h - 1][b + 1] == '/' || stellpult[h - 1][b + 1] == '>') //ist die verbundene Weiche oberhalb verbunden.
        {
          //Suche nach der Nummer des Besetztmelders
          int nullenb = 0; //zähler, wie viele Nullen bei der Suche nach dem passenden
          for (int h2 = 0; h2 <= h - 1; h2++)
          {
            for (int k = 0; k <= b + 1; k++) //wiederhole bis du eine Position weiter rechts bist als die Unterliegende Weiche
            {
              if (stellpult[h2][k] == '0')
              {
                nullenb++; //Finde heraus, wie viele Nullen vor dem Besetztmelder lagen.
              }
              if (h2 == h - 1 && k == b + 1)
                verbindungen[h * b - reihenfolge0][h2 * k - nullenb] = 1; //notiere, dass die Schiene oberhalb des Gleises verbunden ist.
            }
          }
        }
        else if (stellpult[h + 1][b + 1] == '>') //ist die verbundene Weiche unterhalb verbunden.
        {
          //Suche nach der Nummer des Besetztmelders
          int nullenb = 0; //zähler, wie viele Nullen bei der Suche nach dem passenden
          for (int h2 = 0; h2 <= h + 1; h2++)
          {
            for (int k = 0; k <= b + 1; k++) //wiederhole bis du eine Position weiter rechts bist als die überleigende Weiche
            {
              if (stellpult[h2][k] == '0')
              {
                nullenb++; //Finde heraus, wie viele Nullen vor dem Besetztmelder lagen.
              }
              if (h2 == h + 1 && k == b + 1)
              {
                verbindungen[h * b - reihenfolge0][h2 * k - nullenb] = 1; //notiere, dass die Schiene unterhalb des Gleises verbunden ist.
              }
            }
          }
        }
      }
    }
  }                                  //tiefen und breitensuche
  weichen.weichenRelaisHIGH();       //alle möglichen Eingaben an den Relais werden gelöscht
  weichen.weichenpositionenEEPROM(); //gespeicherte Weichenposition wird angezeigt und ausgeführt
  Serial.begin(9600);
  

  //Fahrstrassentastenkombinationen
  fahrstrassenspeicher[2][4] = 1; //minus 1 wegen array
  fahrstrassenspeicher[0][2] = 2;
  fahrstrassenspeicher[2][8] = 3;
  fahrstrassenspeicher[6][8] = 4;
  fahrstrassenspeicher[1][3] = 5;
  fahrstrassenspeicher[5][7] = 6;
  fahrstrassenspeicher[1][7] = 7;
  fahrstrassenspeicher[0][1] = 8; //wenn die Tasten auf feld 1 und feld 2 gedrückt werden, soll die Fahrstraße 8 einlaufen
}

void loop()
{
  Serial.println("Hallo");
  /* int kai = 122;
  ftueMelder.digitalSchalten(kai,HIGH);
  delay(100);
  ftueMelder.digitalSchalten(kai,LOW);
  delay(100);
*/

  //abfragen aller Zugtasten
  zugtastenspeicher[0] = 0;
  zugtastenspeicher[1] = 0;
  anzahl = 0;
  for (int j = 0; j < zugtastenC.getZugtastenAnzahl(); j++)
  {
    if (zugtastenC.getZugtastenstatus(j) == false && anzahl < 2) //Wenn die Zugstrassentaste gedrückt wurde...
    {                                                            //...und noch nicht zwei Tasten gedrückt wurden
      zugtastenspeicher[anzahl] = j;                             //speichere das Feld auf dem die taste gedrückt wurde
      anzahl++;                                                  //erhöhe die anzahl der gedrückten anzahl an tasten um 1
    }
  }

  //Kontrolle der Fahrstrasse und einstellen und Festlegen des Fahrwegs
  fahrstrasse = fahrstrassenspeicher[zugtastenspeicher[0]][zugtastenspeicher[1]];
  if (fahrstrasse != 0 && fahrstrassenstati[fahrstrasse - 1] == false)
  {
    int belegt = 0;
    for (int j = 0; j < besetztmelderposition[fahrstrasse][0][0]; j++) //wiederhole bis du alle besetztmelder überprüft hast(menge an besetztmeldern werden über besetztmelder position eingebracht)
    {
      if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[fahrstrasse][j + 1][0], LOW, weichen) == HIGH || besetztmeldung.getFahrstrassenelement(besetztmelderposition[fahrstrasse][j + 1][0]) == true //wenn der Besetztmelder belegt oder festgelegt ist.
          || (besetztmelderposition[fahrstrasse][j + 1][1] > 0 && weichen.getWeichenfestlegung(besetztmelderposition[fahrstrasse][j + 1][1] - 1) == true))                                                         //Kontrolle ob Weichen schon in eine Fahrstraße eingebunden sind(weichennr aus Weichenpostion muss -1 genommen werden, da sonst die Weiche null nicht von den anderen zu unterschreiden wäre.
        belegt++;
    }
    if (hauptsignale.getHauptsignalSperren(besetztmelderposition[fahrstrasse][0][1] - 1) == true) // wenn das Signal gesperrt ist kann die Fahrstrasse nicht gestellt werden.
    {
      belegt++;
    }

    if (belegt == 0)
    {
      //Besetztmeldung
      for (int j = 0; j < besetztmelderposition[fahrstrasse][0][0]; j++) //einbinden der Besetztmelder in die Fahrstrasse
      {

        besetztmeldung.setFahrstrassenelement(besetztmelderposition[fahrstrasse][j + 1][0], fahrstrasse, true);
        if (besetztmelderposition[fahrstrasse][j + 1][1] > 0) //wenn der Besetztmelder auf einer Weiche liegt
        {
          weichen.weichenStellen(besetztmelderposition[fahrstrasse][j + 1][1] - 1, besetztmelderposition[fahrstrasse][j + 1][2]); //stelle die Weiche
          weichen.setWeichenfestlegung(besetztmelderposition[fahrstrasse][j + 1][1] - 1, fahrstrasse, true);                      //lege die Weiche fest.
        }
      }
      fahrstrassenkontolle[fahrstrasse - 1] = true;
    }
  }

  //Fahrstrassenauflösung/Fahrtstellen des Hauptsignals
  for (int kontrollen = 1; kontrollen < fahrstrassenanzahl + 1; kontrollen++) //kontrolliere alle Fahrstrassen.
  {
    int weichenstati = 0;
    if (fahrstrassenkontolle[kontrollen - 1] == true) //wenn die untersuchte Fahrstrasse kontrolliert wurde und nur noch die Weichen einlaufen müssen,
    {

      for (int k = 0; k < besetztmelderposition[kontrollen][0][0]; k++) //wiederhole für jeden Besetztmelder dieser Fahrstraße,
      {

        if (besetztmelderposition[kontrollen][k + 1][1] > 0 && weichen.getWeichenposition(besetztmelderposition[kontrollen][k + 1][1] - 1) != besetztmelderposition[kontrollen][k + 1][2]) //wenn die Weichenposition der Weiche, die beim Besetztmelder hinterlegt ist, mit der realen Weichenposition nicht übereinstimmt,
        {
          weichenstati++; //erhöhe weichenstati um 1.
        }
      }
      if (weichenstati == 0) //ist belegt gleich null, sind also alle Weichen in der richtigen Position.
      {
        hauptsignale.hauptsignalSchalten(besetztmelderposition[kontrollen][0][1] - 1, 2); //schalte das Hauptsignal
        fahrstrassenkontolle[kontrollen - 1] = false;                                     //deaktiviere Fahrstraßenkontrolle
        fahrstrassenstati[kontrollen - 1] = true;                                         //und aktiviere die Fahrstrasse, die Fahrstrasse ist somit komplett ausgeführt
        Serial.print("Fahrstraße ");                                                      //schreibe den Namen der Fahrstrasse
        Serial.println(kontrollen);
      }
    }

    if (fahrstrassenstati[kontrollen - 1] == true)
    {
      //auf einen Besetztmelder sind immer zwei zahlen festgelegt, die erste Zahl wird beim besetztsein des melder eingespeichert, die zweite, wird erst dann eingespeichert, wenn die erste eingespeichert ist,
      //der nächste besetztmelder kann erst freigegeben werden, wenn der erste freigegeben ist, also die Zahl eine bestimmte höhe hat.
      if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][0], LOW, weichen) == HIGH) //wenn der Besetztmelder besetzt ist und die freigabe der Besetztmelder Zahl plus eins entspricht
      {
        hauptsignale.hauptsignalSchalten(besetztmelderposition[kontrollen][0][1] - 1, 0); //immer wenn ein Besetztmelder innerhalb einer fahrstrasse belegt wird, soll das zugehörige signal rot werden
        if (freigabe[kontrollen - 1] == besetztmelderzahl[kontrollen - 1] * 2 + 1)
          freigabe[kontrollen - 1]++; //beim ersten mal ist die freigabe1 und wird auf 2 erhöht. wenn dies passiert ist, und der Besetztmelder wieder frei geworden ist, wird der Besetztmelder als Fahrstrassenelement aufgelöst
      }

      if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][0], LOW, weichen) == LOW && freigabe[kontrollen - 1] == besetztmelderzahl[kontrollen - 1] * 2 + 2)
      {
        //Auflösung der Besetztmelderfestlegung
        besetztmeldung.setFahrstrassenelement(besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][0], kontrollen, false);
        //Auflösung der Weichenfestlegung
        if (besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][1] > 0 && weichen.getWeichenfestlegung(besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][1] - 1) == true) //besetztmelderZahl+1, weil die erste Position des arrays besetztmelderposition nur die Anzhal der Besetztmelder enthält und die namen der melder erst im nächsten feld erscheinen
        {
          weichen.setWeichenfestlegung((besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][1]) - 1, kontrollen, false); //die Weichenzahl die Herauskommt muss um 1 verkleinert werden, sonst gäbe es keine WEiche 0
        }

        freigabe[kontrollen - 1]++;
        besetztmelderzahl[kontrollen - 1]++; //es geht mit dem nächsten besetztmelder der Fahrstrasse weiter

        if (besetztmelderzahl[kontrollen - 1] == besetztmelderposition[kontrollen][0][0]) //wenn die gesamte Fahrstrße wieder freigegeben ist
        {
          besetztmelderzahl[kontrollen - 1] = 0;
          freigabe[kontrollen - 1] = 1;              //Der Freigabe Zähler wird wieder zurückgesetzt.
          fahrstrassenstati[kontrollen - 1] = false; //die Fahrstrasse wird wieder als inaktiv gemeldet.
        }
      }
      //alle besetztmelder müssen in der Fahrstraße einmal rot gewesen sein und am ende wieder frei sein
    }
  }

  ftueMelder.tueMelder(zugtastenC);
  //Signale
  //hauptsignale.hauptsignaleHp0Manuell(); //über die Signalhaltgruppentaste und die Zugtaste auf dem Feld des Signals kann ein Signal auf hp0 gestellt werden
  hauptsignale.hauptsignaleSperren(); //über weichen  und weichensperrtaste kann ein Signal gesperrt werden

  //Weichen
  weichen.weichenWechseln(); //WGT und WT können zum Umschalten einer Weiche benutzt werden
  weichen.weichenSchalten(); //lässt das Relais nach dem Schalten wieder zurückfallen

  //Besetztmeldung
  //besetztmelder sollen immer wenn sie belegt sind den Status anzeigen.
  for (int h = 0; h < 19; h++)
  {
    besetztmeldung.getBesetztmelderstatus(h, LOW, weichen);
  }
  /*besetztmeldung.getBesetztmelderstatus(1, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(2, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(3, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(4, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(5, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(6, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(7, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(8, LOW, weichen);
  besetztmeldung.getBesetztmelderstatus(9, LOW, weichen);
  

if(digitalRead(shgt)==LOW)
{
  for(int o=0;o<9;o++)
  Zugtaste.setFahrstraßenelement
}*/
}
