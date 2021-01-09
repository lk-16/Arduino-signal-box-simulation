#include <Arduino.h>
#include "WeichenControl.h"
#include "ZugtastenControl.h"
#include "BesetztmeldungControl.h"
#include "HauptsignalControl.h"
#include "Melder.h"

//Pinbelegung
int schieberegisterPins[4] = {2, 8, 9, 10};
int w1g = 22; //w=Weiche  bzw. Weichenrelais g=Relais für gerade
int w1k = 23; //g=Relais für gerade
int w2g = 24;
int w2k = 25;
int w3g = 26;
int w3k = 27;

int wt1 = 33; //Pinbelegung Weichentasten
int wt2 = 32;

int ledw1g = 102; //Pinbelegung der WeichenLEDs
int ledw1k = 103;
int ledw2g = 104;
int ledw2k = 105;
int weichentimeout = 1000; //dauer des Schaltvorgangs bei den Weichen
int wgt = 53;              //Weichengruppentaste

int zta1 = 2; // Zugstraßentaste 1 an Pin 2
int zta2 = 3; // Zugstraßentaste 2 an Pin 3
int zta3 = 4; // Zugstraßentaste 3 an Pin 4
int zta4 = 5;

//Melder
int ftueMelderLed = 101; //Fahrstraßentastenüberwachung
int ftueMelderWut = 34;
int weckerPin = 28;
String ftueMelderName = "ftueMelder";

//Signale
int signalsperrtaste = ftueMelderWut;                                    //signalsperrtaste, zu testzwecken auf weichengruppentaste gestellt
int Signalentsperrtaste = wt1;                                           //s.o
int shgt = wgt;                                                          //s.o
int allgSignaltasten[3] = {signalsperrtaste, Signalentsperrtaste, shgt}; //Signalsperrtaste, Signalentsperrtaste, Signalhaltgruppentaste, Array speichert für jedes Signal wichtige informationen, hält den schreibaufwand geringer

//Signal1
int rot1 = 106;   //Hp0 (rot) vom Signal 1
int gruen1 = 107; //Hp1 (grün)  vom Signal 1
int gelb1 = 108;  //Hp2 (langsamfahrt) vom Signal 1
int sperrmelder1 = 109;
int signaltaste1 = zta3; //Signaltaste, zu tastzwecken weichentaste 1

const int hauptsignalanzahl = 1;
int rot[hauptsignalanzahl] = {rot1};
int gelb[hauptsignalanzahl] = {gelb1};
int gruen[hauptsignalanzahl] = {gruen1};
int signaltasten[hauptsignalanzahl] = {signaltaste1};
int sperrmelder[hauptsignalanzahl] = {sperrmelder1};
HauptsignalControl hauptsignale(hauptsignalanzahl, rot, gelb, gruen, signaltasten, sperrmelder, allgSignaltasten, schieberegisterPins);
//int rotPin, int gelbPin, int gruenPin,  int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4]

//Gleisbesetztmelder
const int besetztmelderAnzahl = 6;
int weichenbesetztmelder[besetztmelderAnzahl] ={0,1,0,0,2,0};//übergeben der Weichennr
int besetztmelderEingaenge[besetztmelderAnzahl] = {42, 41, 40, 39, 38, 37}; //an gnd angeschlossen
int besetztmelderLedsGelb[besetztmelderAnzahl] = {0, 0, 0, 110, 0, 112};
int besetztmelderLedsRot[besetztmelderAnzahl] = {0, 0, ftueMelderLed, 111, 0, 113};
BesetztmeldungControl besetztmeldung(besetztmelderEingaenge, besetztmelderLedsGelb, besetztmelderLedsRot, weichenbesetztmelder, besetztmelderAnzahl, schieberegisterPins);

//Weichen
const int anzahlWeichen = 2;
int weichenPinsGerade[anzahlWeichen] = {w1g, w2g};
int weichenPinsKurve[anzahlWeichen] = {w1k, w2k};
int weichenLedPinsGerade[anzahlWeichen] = {ledw1g, ledw2g};
int weichenLedPinsGeradeRot[anzahlWeichen] = {};
int weichenLedPinsKurve[anzahlWeichen] = {ledw1k, ledw2k};
int weichenLedPinsKurveRot[anzahlWeichen] = {};
int adressWeichenpositionen[anzahlWeichen] = {1, 5};
int weichentasten[anzahlWeichen] = {wt1, wt2};
WeichenControl weichen(anzahlWeichen, weichenPinsGerade, weichenPinsKurve, weichenLedPinsGerade, weichenLedPinsGeradeRot, weichenLedPinsKurve, weichenLedPinsKurveRot, adressWeichenpositionen, weichentimeout, weichentasten, wgt, schieberegisterPins);

//Melder
Melder ftueMelder(ftueMelderName, ftueMelderLed, weckerPin, ftueMelderWut, schieberegisterPins); //Ausgabe von FTÜ im Seriellen Monitior funktioniert nicht

//Zugtasten
const int zugtastenanzahl = 4;
int zugtastenPins[zugtastenanzahl] = {zta1, zta2, zta3, zta4};
ZugtastenControl zugtastenC(zugtastenanzahl, zugtastenPins);

//Fahrstraßensteuerung
const int fahrstrassenanzahl = 3;
int zugtastenspeicher[2];                      //es werden die Zugtasten gespeichert, die gedrückt wurden.
int anzahl = 0;                                //zählt wie viele zugtasten gedrückt wurden
boolean fahrstrassenstati[fahrstrassenanzahl]; //Beinhaltet, welche Fahrstrassen aktiv sind.
boolean fahrstrassenkontolle[fahrstrassenanzahl];//wenn auf true, dann sind alle Besetztmelder frei und für die Fahrstrasse festgelegt und die Stellaufträge für die Weichen sind gegeben.
int const felderAnzahl = 18;
int fahrstrasse;                                      //zeigt an welche Fahrstrasse kurzzeitig aktiv ist.
int fahrstrassenspeicher[felderAnzahl][felderAnzahl]; //1 variable = nummer des Tischfeldes, 2. variable = nummer des Zweiten Tischfeldes ergibt die zu betätigende Fahrstrasse
int freigabe[fahrstrassenanzahl] = {1, 1, 1};         //zählt wie viele halbe Besetztmelder schon freigegeben wurden
int besetztmelderposition[fahrstrassenanzahl + 1][/*Reihen(Bestztm.*/ 5][5] = {
    {{}}, //erster belibt leer, es gibt keine Fahrstrasse null, da sie in einem Array nicht gespeichert werden kann. Alle Ziffern sind automatisch null.
    {{3, 1}, {3}, {4, 1, 1 /*, 2, 1*/}, {5}},
    {{4, 1}, {3}, {4, 1, 0}, {1, 2, 0}, {2}},
    {{3, 1}, {0}, {1, 2, 1 /*, 2, 1*/}, {2}},
};                                               //0. ebene Besetztmelder anzahl, in den nächsten ebenen sind die Besetztmelder in Reihenfolge gespeichert. Die nächste Dimeinsion speichert, ob das Feld auf dem Besetztmelder eine Weiche ist, und darunter steht in welche richtung die geschaltet werden muss. Folgend steht, ob es eine Falnkenschutz weiche gibt                                                                            //anhand dieser Zahl lässt sich der Wert von Freigabe berechnen, enthält bis zu welchem Besetztmelder die Fahrstrasse aufgelöst ist.
int besetztmelderzahl[fahrstrassenanzahl] = {0}; //die anzahl der Besetztmelder die schon freigegeben wurde


void setup()
{
  pinMode(ftueMelderLed, OUTPUT);
  ftueMelder.digitalSchalten(weckerPin, HIGH);
  weichen.weichenRelaisHIGH();       //alle möglichen Eingaben an den Relais werden gelöscht
  weichen.weichenpositionenEEPROM(); //gespeicherte Weichenposition wird angezeigt und ausgeführt
  Serial.begin(600);

  //Fahrstrassentastenkombinationen
  fahrstrassenspeicher[2][3] = 1;
  fahrstrassenspeicher[1][2] = 2; //wenn die Tasten auf feld 10 und feld 8 gedrückt werden, soll die Fahrstraße 1 einlaufen
  fahrstrassenspeicher[0][1] = 3;
}

void loop()
{
  //abfragen aller Zugtasten
  zugtastenspeicher[0] = 0;
  zugtastenspeicher[1] = 0;
  anzahl = 0;
  for (int j = 0; j < zugtastenC.getZugtastenAnzahl(); j++)
  {
    if (zugtastenC.getZugtastenstatus(j) == true && anzahl < 2) //Wenn die Zugstrassentaste gedrückt wurde, und noch nicht zwei Tasten gedrückt wurden
    {
      zugtastenspeicher[anzahl] = j; //speichere das Feld auf dem die taste gedrückt wurde
      anzahl++;                      //erhöhe die anzahl der gedrückten anzahl an tasten um 1
    }
  }

  //Kontrolle der Fahrstrasse und einstellen und Festlegen des Fahrwegs
  fahrstrasse = fahrstrassenspeicher[zugtastenspeicher[0]][zugtastenspeicher[1]];
  if (fahrstrasse != 0 && fahrstrassenstati[fahrstrasse - 1] == false)
  {
    int belegt = 0;
    for (int j = 0; j < besetztmelderposition[fahrstrasse][0][0]; j++) //wiederhole bis du alle besetztmelder überprüft hast(menge an besetztmeldern werden über besetztmelder position eingebracht)
    {
      if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[fahrstrasse][j + 1][0], LOW) == HIGH || besetztmeldung.getFahrstrassenelement(besetztmelderposition[fahrstrasse][j + 1][0]) == true //wenn der Besetztmelder belegt oder festgelegt ist.
          || (besetztmelderposition[fahrstrasse][j + 1][1] > 0 && weichen.getWeichenfestlegung(besetztmelderposition[fahrstrasse][j + 1][1] - 1) == true))                                                //Kontrolle ob Weichen schon in eine Fahrstraße eingebunden sind(weichennr aus Weichenpostion muss -1 genommen werden, da sonst die Weiche null nicht von den anderen zu unterschreiden wäre.
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
  for (int kontrollen = 1; kontrollen < fahrstrassenanzahl + 1; kontrollen++)//kontrolliere alle Fahrstrassen.
  {
    int weichenstati = 0;
    if (fahrstrassenkontolle[kontrollen - 1] == true) //wenn die untersuchte Fahrstrasse kontrolliert wurde und nur noch die Weichen einlaufen müssen,
    {

      for (int k = 0; k < besetztmelderposition[kontrollen][0][0]; k++) //wiederhole für jeden Besetztmelder dieser Fahrstraße,
      {

        if (weichen.getWeichenposition(besetztmelderposition[kontrollen][k + 1][1] - 1) != besetztmelderposition[kontrollen][k + 1][2]) //wenn die Weichenposition der Weiche, die beim Besetztmelder hinterlegt ist, mit der realen Weichenposition nicht übereinstimmt,
          weichenstati++;//erhöhe weichenstati um 1.
      }
      if (weichenstati == 0) //ist belegt gleich null, sind also alle Weichen in der richtigen Position.
      {
        hauptsignale.hauptsignalSchalten(besetztmelderposition[kontrollen][0][1] - 1, 1); //schalte das Hauptsignal
        fahrstrassenkontolle[kontrollen - 1] = false;                                     //deaktiviere Fahrstraßenkontrolle
        fahrstrassenstati[kontrollen - 1] = true;                                         //und aktiviere die Fahrstrasse, die Fahrstrasse ist somit komplett ausgeführt
        Serial.print("Fahrstraße ");                                                      //schreibe den Namen der Fahrstrasse
        Serial.println(kontrollen);
      }
    }

    if (fahrstrassenstati[kontrollen - 1] == true)
    {
      //auf einen Besetztmelder sind immer zwei zahlen festgelegt, die erste Zahl wird beim besetztsein des melder eingespeichert, die zweite, wird erst dnn eingespeichert, wenn die erste eingespeichert ist,
      //der nächste besetztmelder kann erst freigegeben werden, wenn der erste freigegeben ist, also die Zahl eine bestimmte höhe hat.
      if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][0], LOW) == HIGH) //wenn der Besetztmelder besetzt ist und die freigabe der Besetztmelder Zahl plus eins entspricht
      {
        hauptsignale.hauptsignalSchalten(besetztmelderposition[kontrollen][0][1] - 1, 0); //immer wenn ein Besetztmelder innerhalb einer fahrstrasse belegt wird, soll das zugehörige signal rot werden
        if (freigabe[kontrollen - 1] == besetztmelderzahl[kontrollen - 1] * 2 + 1)
          freigabe[kontrollen - 1]++; //beim ersten mal ist die freigabe1 und wird auf 2 erhöht. wenn dies passiert ist, und der Besetztmelder wieder frei geworden ist, wird der Besetztmelder als Fahrstrassenelement aufgelöst
      }

      if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[kontrollen][besetztmelderzahl[kontrollen - 1] + 1][0], LOW) == LOW && freigabe[kontrollen - 1] == besetztmelderzahl[kontrollen - 1] * 2 + 2)
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
          freigabe[kontrollen - 1] = 1;              //Der Freigabe Zähler wird wieder Zurückgesetzt.
          fahrstrassenstati[kontrollen - 1] = false; //die Fahrstrasse wird wieder als inaktiv gemeldet.
        }
      }
      //alle besetztmelder müssen in der Fahrstraße einmal rot gewesen sein und am ende wieder frei sein
    }
  }

  //FTÜ-Melder
  if (zugtastenC.zugtastenGedrueckt()) //wenn eine Zugtaste gedrückt ist
  {
    ftueMelder.tueMelder(); //kontrolliere wie lange die Tasten gesrückt wurden
  }
  else //wenn keine Mehr gedrückt wird,
  {
    ftueMelder.setMelderStartzeit(millis());                    //setze den Timer zurück
    ftueMelder.setTueMelderStatus(false);                       //den Status auf 0
    ftueMelder.digitalSchalten(ftueMelder.getTueLedPin(), LOW); //Schalte die Led aus
    ftueMelder.digitalSchalten(ftueMelder.getWecker(), HIGH);   //Mache den Wecker aus
    ftueMelder.setWutAktivierung(LOW);                          //beim Beenden der Störung wird die Unterbrechung wieder aufgehoben
  }

  //Signale
  hauptsignale.hauptsignaleHp0Manuell(); //über die Signalhaltgruppentaste und die Zugtaste auf dem Feld des Signals kann ein Signal auf hp0 gestellt werden
  hauptsignale.hauptsignaleSperren();    //über weichen  und weichensperrtaste kann ein Signal gesperrt werden

  //Weichen
  weichen.weichenWechseln(); //WGT und WT können zum Umschalten einer Weiche benutzt werden
  weichen.weichenSchalten(); //lässt das Relais nach dem Schalten wieder zurückfallen

  //Besetztmeldung
  //besetztmeldung.besetztmelderAusfuehren();//besetztmelder sollen immer wenn sie belegt sind den Status anzeigen.
  besetztmeldung.getBesetztmelderstatus(0, LOW);
  besetztmeldung.getBesetztmelderstatus(1, LOW);
  besetztmeldung.getBesetztmelderstatus(2, LOW);
  besetztmeldung.getBesetztmelderstatus(3, LOW);
  besetztmeldung.getBesetztmelderstatus(4, LOW);
  besetztmeldung.getBesetztmelderstatus(5, LOW);
}
