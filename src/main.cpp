/*Libary für die Steuerung eines SpDrS60 Gleisbildstellpults
 *  mit dieser Library können weichen Signale etc. gesteuert werden
 *
 * Implemetiert ist ein Stellwerk mit drei Weichen und entsprechenden Signalen
 * Schema des Stellpults:
 *         /-----
 * ------><------
 * -----<--------
 * 
 *  Lennart Klüner 10.04.2022
 */


#include <Arduino.h>
#include "WeichenControl.h"
#include "ZugtastenControl.h"
#include "BesetztmeldungControl.h"
#include "HauptsignalControl.h"
#include "Melder.h"
#include "Graph.h"
#include "List.h"

// Pinbelegung
int schieberegisterPins[4] = {4, 8, 9, 10};
int w1g = 24; // w=Weiche  bzw. Weichenrelais g=Relais für gerade
int w1k = 25; // g=Relais für gerade
int w2g = 22;
int w2k = 23;
int w3g = 26;
int w3k = 27;

int wt1 = 11; // Pinbelegung Weichentasten
int wt2 = 20;
int wt3 = 2;

// Pinbelegung der WeichenLEDs
int ledw1g = 109; 
int ledw1k = 108;
int ledw2g = 50;
int ledw2k = 39;
int ledw3g = 106;
int ledw3k = 104;

int ledw1gRot = 110; 
int ledw1kRot = 51;
int ledw2gRot = 101;
int ledw2kRot = 38;
int ledw3gRot = 107;
int ledw3kRot = 105;


int weichentimeout = 500; // dauer des Schaltvorgangs bei den Weichen
int wgt = 18;             // Weichengruppentaste

int zta1 = 5;  // Zugstraßentaste 1 an Pin 2
int zta2 = 21; // Zugstraßentaste 2 an Pin 3
int zta3 = 12; // Zugstraßentaste 3 an Pin 4
int zta4 = 3;
int zta5 = 6;
int zta6;
int zta7 = 13;
int zta8 = 4;
int zta9 = 7;

// Melder
int ftueMelderLed = 124; // Fahrstraßentastenüberwachung
int ftueMelderWut = 19;
int weckerPin = 28;
String ftueMelderName = "ftueMelder";

// Signale
int signalsperrtaste = 16;                                               // signalsperrtaste, zu testzwecken auf weichengruppentaste gestellt
int Signalentsperrtaste = 17;                                            // s.o
int shgt = 15;                                                           // s.o
int allgSignaltasten[3] = {signalsperrtaste, Signalentsperrtaste, shgt}; // Signalsperrtaste, Signalentsperrtaste, Signalhaltgruppentaste, Array speichert für jedes Signal wichtige informationen, hält den schreibaufwand geringer

// Signal1
int rot1 = 52;    // Hp0 (rot) vom Signal 1
int gruen1 = 125; // Hp1 (grün)  vom Signal 1
int gelb1;        // Hp2 (langsamfahrt) vom Signal 1
int sperrmelder1 = 126;
int signaltaste1 = zta3; // Signaltaste, zu tastzwecken weichentaste 1

// Signal2
int rot2 = 123;  // Hp0 (rot) vom Signal 1
int gruen2 = 46; // Hp1 (grün)  vom Signal 1
int gelb2;       // Hp2 (langsamfahrt) vom Signal 1
int sperrmelder2 = 49;
int signaltaste2 = zta4; // Signaltaste, zu tastzwecken weichentaste 1

// Signal3
int rot3 = 121;   // Hp0 (rot) vom Signal 1
int gruen3 = 122; // Hp1 (grün)  vom Signal 1
int gelb3;        // Hp2 (langsamfahrt) vom Signal 1
int sperrmelder3 = 127;
int signaltaste3 = zta7; // Signaltaste, zu tastzwecken weichentaste 1

// Signal4
int rot4 = 47;    // Hp0 (rot) vom Signal 1
int gruen4 = 111; // Hp1 (grün)  vom Signal 1
int gelb4;        // Hp2 (langsamfahrt) vom Signal 1
int sperrmelder4 = 48;
int signaltaste4 = zta8; // Signaltaste, zu tastzwecken weichentaste 1

const int hauptsignalanzahl = 4;
int rot[hauptsignalanzahl] = {rot1, rot2, rot3, rot4};
int gelb[hauptsignalanzahl] = {gelb1, gelb2, gelb3, gelb4};
int gruen[hauptsignalanzahl] = {gruen1, gruen2, gruen3, gruen4};
int signaltasten[hauptsignalanzahl] = {signaltaste1, signaltaste2, signaltaste3, signaltaste4};
int sperrmelder[hauptsignalanzahl] = {sperrmelder1, sperrmelder2, sperrmelder3, sperrmelder4};
boolean richtung[hauptsignalanzahl] = {true, false, true, false};
HauptsignalControl hauptsignale(hauptsignalanzahl, rot, gelb, gruen, signaltasten, sperrmelder, allgSignaltasten, schieberegisterPins, richtung);
// int rotPin, int gelbPin, int gruenPin,  int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4]

// Gleisbesetztmelder
// jedes Feld muss als einzelner Gleisbesetztmelder definiert sein
const int besetztmelderAnzahl = 19;
int weichenbesetztmelder[besetztmelderAnzahl] = {0, 0,
                                                 0, 0, 0, 0, 1, 0, 2, 0, 0,
                                                 0, 0, 0, 0, 3, 0, 0, 0};                                                       // Weichennummern werden übergeben
int besetztmelderEingaenge[besetztmelderAnzahl] = {69, 69, 59, 59, 63, 65, 65, 68, 68, 67, 67, 60, 60, 64, 64, 62, 62, 61, 61}; // an gnd angeschlossen
int besetztmelderLedsGelb[besetztmelderAnzahl] = {33, 45, 130, 116, 117, 114, 0, 102, 0, 31, 43, 121, 128, 133, 112, 0, 37, 35, 41};
int besetztmelderLedsRot[besetztmelderAnzahl] = {32, 44, 58, 0, 118, 115, 0, 103, 0, 30, 42, 132, 129, 120, 113, 0, 36, 34, 40};
BesetztmeldungControl besetztmeldung(besetztmelderEingaenge, besetztmelderLedsGelb, besetztmelderLedsRot, weichenbesetztmelder, besetztmelderAnzahl, schieberegisterPins);

// Weichen
const int anzahlWeichen = 3;
int weichenPinsGerade[anzahlWeichen] = {w1g, w2g, w3g};
int weichenPinsKurve[anzahlWeichen] = {w1k, w2k, w3k};
int weichenLedPinsGerade[anzahlWeichen] = {ledw1g, ledw2g, ledw3g};
int weichenLedPinsGeradeRot[anzahlWeichen] = {ledw1gRot, ledw2gRot, ledw3gRot};
int weichenLedPinsKurve[anzahlWeichen] = {ledw1k, ledw2k, ledw3k};
int weichenLedPinsKurveRot[anzahlWeichen] = {ledw1kRot, ledw2kRot, ledw3kRot};
int adressWeichenpositionen[anzahlWeichen] = {1, 5, 9};
int weichentasten[anzahlWeichen] = {wt1, wt2, wt3};
WeichenControl weichen(anzahlWeichen, weichenPinsGerade, weichenPinsKurve, weichenLedPinsGerade, weichenLedPinsGeradeRot, weichenLedPinsKurve, weichenLedPinsKurveRot, adressWeichenpositionen, weichentimeout, weichentasten, wgt, schieberegisterPins);

// Melder
Melder ftueMelder(ftueMelderName, ftueMelderLed, weckerPin, ftueMelderWut, schieberegisterPins); // Ausgabe von FTÜ im Seriellen Monitior funktioniert nicht

// Zugtasten
const int zugtastenanzahl = 9;
int zugtastenPins[zugtastenanzahl] = {zta1, zta2, zta3, zta4, zta5, zta6, zta7, zta8, zta9};
boolean zugtastenRichtung[zugtastenanzahl] = {1, 0, 1, 0, 1, 0, 1, 0, 1};
ZugtastenControl zugtastenC(zugtastenanzahl, zugtastenPins, zugtastenRichtung);

// Fahrstraßensteuerung
int zugtastenspeicher[2];                                    // es werden die Zugtasten gespeichert, die gedrückt wurden.
int anzahl = 0;                                              // zählt wie viele zugtasten gedrückt wurden

List list;
Graph *graph = nullptr;
int nachbarn[besetztmelderAnzahl][3] = { // Nachbarn ders einzelnen Gleisymbole. Nach nummerierung in einem 2d Array gespeichert. Leere Felder müssen mit einem Wert außerhalb der Wertebereichs gefüllt werden z.B. -1, da dies kleiner als 0 ist.
    {1, 8, -1},
    {0, -1, -1},
    {3, -1, -1},
    {2, 4, -1},
    {3, 5, -1},
    {4, 6, -1},
    {5, 7, 15},
    {6, 8, -1},
    {7, 9, 0},
    {8, 10, -1},
    {9, -1, -1},
    {12, -1, -1},
    {11, 13, -1},
    {12, 14, -1},
    {13, 15, -1},
    {16, 14, 6},
    {15, 17, -1},
    {16, 18, -1},
    {17, -1, -1}};
char gleissymboltyp[besetztmelderAnzahl] = {'-', '+', // wird für die Zuweisung der Aktoren zu den Gleisbildsymbolen benötigt, in Kombination mit Control
                                            '+', 's', '-', '-', '<', '-', '<', 's', '+',
                                            '+', 's', '-', '-', '<', '-', 's', '+'}; //- normales Gleis, + Zugtaste, < weiche, s signal und zugtaste (lesart wie die besetztmelder von links oben nach rechts unten)

void setup()
{
  weichen.weichenRelaisHIGH(); // wird benötigt, da der Graph erst später erstellt wird und die Relais sonst Schalten würden.
  Serial.begin(9600);

  // Der folgende Algorithmus fügt den Knoten Attribute, wie Besetztmelder, Weichen, Signale hinzu.
  Gleissymbol knoten[besetztmelderAnzahl]; // Speichert die Knoten, des Graphen

  int zta = 0;                                  // zugtastenzähler
  int signal = 0;                               // signalzähler
  int weiche = 0;                               // weichenzähler
  for (int i = 0; i < besetztmelderAnzahl; i++) // erstellen der Gleissymbole bezüglich des Array gleissymboltyp
  {
    if (gleissymboltyp[i] == '-') // wenn normales Gleis
      knoten[i] = Gleissymbol(besetztmeldung.getBesetztmelder(i));
    else if (gleissymboltyp[i] == '+') // zugtaste
    {
      knoten[i] = Gleissymbol(besetztmeldung.getBesetztmelder(i)); //
      zugtastenC.setGleissymbol(zta, &knoten[i]);                  // weise Gleissymbol einer Zugtaste zu
      zta++;
    }
    else if (gleissymboltyp[i] == 's') // signal und zugtaste
    {
      knoten[i] = Gleissymbol(besetztmeldung.getBesetztmelder(i), nullptr, hauptsignale.getHauptsignal(signal));
      zugtastenC.setGleissymbol(zta, &knoten[i]);
      zta++;
      signal++;
    }
    else if (gleissymboltyp[i] == '<') // weiche
    {
      knoten[i] = Gleissymbol(besetztmeldung.getBesetztmelder(i), weichen.getWeiche(weiche));
      weiche++;
    }
  }

  graph->prepare();                                         // alle Eingaben an den Relais werden gelöscht und gespeicherte Weichenposition wird angezeigt und ausgeführt
  graph = new Graph(besetztmelderAnzahl, knoten, nachbarn); // erstelle den Graphen
}

void loop()
{
  zugtastenspeicher[0] = 0;
  zugtastenspeicher[1] = 0;
  anzahl = 0;
  for (int j = 0; j < zugtastenC.getZugtastenAnzahl(); j++)
  {
    if (zugtastenC.getZugtastenstatus(j) == false && anzahl < 2) // Wenn die Zugstrassentaste gedrückt wurde...
    {                                                            //...und noch nicht zwei Tasten gedrückt wurden
      zugtastenspeicher[anzahl] = j;                             // speichere das Feld auf dem die taste gedrückt wurde
      anzahl++;                                                  // erhöhe die anzahl der gedrückten anzahl an tasten um 1
    }
  }
  if (anzahl > 1)
  {
    graph->fahrstrasseEinstellen(zugtastenC.getZugtaste(zugtastenspeicher[0]), zugtastenC.getZugtaste(zugtastenspeicher[1]));
  }

  ftueMelder.tueMelder(zugtastenC);
  graph->updateSymbole();
}