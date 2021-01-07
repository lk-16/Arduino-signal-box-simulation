#include <Arduino.h>
#include "WeichenControl.h"
#include "ZugtastenControl.h"
#include "BesetztmeldungControl.h"
#include "Hauptsignal.h"
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

//Gleisbesetztmelder
const int besetztmelderAnzahl = 6;
int besetztmelderEingaenge[besetztmelderAnzahl] = {42, 41, 40, 39, 38, 37}; //an gnd angeschlossen
int besetztmelderLedsGelb[besetztmelderAnzahl] = {0, 0, 0, 110, 0, 112};
int besetztmelderLedsRot[besetztmelderAnzahl] = {0, 0, ftueMelderLed, 111, 0, 113};
BesetztmeldungControl besetztmeldung(besetztmelderEingaenge, besetztmelderLedsGelb, besetztmelderLedsRot, besetztmelderAnzahl, schieberegisterPins);

//Weichen
const int anzahlWeichen = 2;
int weichenPinsGerade[anzahlWeichen] = {w1g, w2g};
int weichenPinsKurve[anzahlWeichen] = {w1k, w2k};
int weichenLedPinsGerade[anzahlWeichen] = {ledw1g, ledw2g};
int weichenLedPinsKurve[anzahlWeichen] = {ledw1k, ledw2k};
int adressWeichenpositionen[anzahlWeichen] = {1, 5};
int weichentasten[anzahlWeichen] = {wt1, wt2};
WeichenControl weichen(anzahlWeichen, weichenPinsGerade, weichenPinsKurve, weichenLedPinsGerade, weichenLedPinsKurve, adressWeichenpositionen, weichentimeout, weichentasten, wgt, schieberegisterPins);

//Melder
Melder ftueMelder(ftueMelderName, ftueMelderLed, weckerPin, ftueMelderWut, schieberegisterPins); //Ausgabe von FTÜ im Seriellen Monitior funktioniert nicht

//Signale
Hauptsignal hauptsignal1(rot1, gruen1, gelb1, signaltaste1, sperrmelder1, allgSignaltasten, schieberegisterPins);
//int rotPin, int gelbPin, int gruenPin,  int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4]

//Zugtasten
const int zugtastenanzahl = 4;
int zugtastenPins[zugtastenanzahl] = {zta1, zta2, zta3, zta4};
ZugtastenControl zugtastenC(zugtastenanzahl, zugtastenPins);

//Fahrstraßensteuerung
const int fahrstrassenanzahl = 3;
int zugtastenspeicher[2]; //es werden die Zugtasten gespeichert, die gedrückt wurden.
int anzahl = 0;           //zählt wie viele zugtasten gedrückt wurden
boolean fahrstrassenstati[2];//Beinhaltet, welche Fahrstrassen aktiv sind.

int const felderAnzahl = 18;
const int maxlang = 4; //maximale Länge von 4 Besetztmeldern
int fahrstrasse;//zeigt an welche Fahrstrasse kurzzeitig aktiv ist.
int fahrstrassenspeicher[felderAnzahl][felderAnzahl]; //1 variable = nummer des Tischfeldes, 2. variable = nummer des Zweiten Tischfeldes
int freigabe2 = 1;
int besetztmelderposition[fahrstrassenanzahl][/*Reihen(Bestztm.*/ 5][3] = {{{}},
                                                                           {{3}, {3}, {4, 1, 1}, {5}},
                                                                           {{4}, {3}, {4, 1, 0}, {1, 2, 0}, 2}}; //0. ebene Besetztmelder anzahl, in den nächsten ebenen sind die Besetztmelder in Reihenfolge gespeichert. Die nächste Dimeinsion speichert, ob das Feld auf dem Besetztmelder eine Weiche ist, und darunter steht in welche richtung die geschaltet werden muss
int besetztmelderzahl = 0;                                                                             //anhand dieser Zahl lässt sich der Wert von Freigabe berechnen, enthält bis zu welchem Besetztmelder die Fahrstrasse aufgelöst ist.
/*int fahrstrassenspeicher[tischfeld][tischfeld][ebene] 
    1. Ebene: Fahrstrassennr
    2. Ebene: Anzahl der Besetztmelder
    3. bis ... Ebene: Nummern der Besetztmelder
*/
/*Warum wird keine Unterschiedung zwischen links und rechts benötigt? 
 * Da Züge nur mit den Signalen fahren, und nicht gegen,
 * gibt es für jede Tasten kombi von eine Richtung, für die Fahrt in die andere Richtung, muss ein anderes Signal verwendet werden.
*/

void setup()
{
  pinMode(ftueMelderLed, OUTPUT);
  ftueMelder.digitalSchalten(weckerPin, HIGH);
  weichen.weichenRelaisHIGH();     //alle möglichen Eingaben an den Relais werden gelöscht
  weichen.weichenpositionenEEPROM(); //gespeicherte Weichenposition wird angezeigt und ausgeführt
  Serial.begin(9600);

  //besetztmeldung.setBesetztmelderBeleuchtung(0,HIGH);                  //Der Status des Lichtes kann eingestellt werden

  fahrstrassenspeicher[2][3] = 1;
  fahrstrassenspeicher[1][2] = 2; //wenn die Tasten auf feld 10 und feld 8 gedrückt werden, soll die Fahrstraße 1 einlaufen::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

void loop()
{
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

  /*es wird die 18 auch als 8 und 1 erkannt, liegt evtl an der größe des arrays 
evtl. sollen die Zahlen vorher sortiert werden, damit sie nicht zu groß für eine array position sind
um die weichen auch zu überwachen, muss weicehncontrol her.
*/

  fahrstrasse = fahrstrassenspeicher[zugtastenspeicher[0]][zugtastenspeicher[1]];
  if (fahrstrasse != 0)
  {

    int besetzt = 0;
    for (int j = 0; j < besetztmelderposition[fahrstrasse][0][0]; j++) //wiederhole bis du alle besetztmelder überprüft hast(menge an besetztmeldern werden über besetztmelder position eingebracht)
    {

      if(besetztmeldung.getBesetztmelderstatus(besetztmelderposition[fahrstrasse][j + 1][0], LOW) == HIGH)
        besetzt++;
      if(besetztmelderposition[fahrstrasse][j + 1][1] > 0 && weichen.getWeichenfestlegung(besetztmelderposition[fahrstrasse][j + 1][1]-1)== true)//kontrolle ob Weichen schon in eine Fahrstraße eingebunden sind(weichennr aus Weichenpostion muss -1 genommen werden, da sonst die Weiche null nicht von den anderen zu unterschreiden wäre)
        besetzt++;
    }
    if (besetzt == 0) /*besetztmeldung.getBesetztmelderstatus(2, LOW) == LOW && besetztmeldung.getBesetztmelderstatus(1, LOW) == LOW                                                                                                              //wenn die besetztmelder unbesetzt sind
        && besetztmeldung.getBesetztmelderstatus(3, LOW) == LOW && besetztmeldung.getBesetztmelderstatus(4, LOW) == LOW && besetztmeldung.getFahrstrassenelement(2) == false && besetztmeldung.getFahrstrassenelement(2) == false //und sie nicht in eine Fahrstrasse eingebunden sind
        && weiche1.getWeichenfestlegung() == false && weiche2.getWeichenfestlegung() == false)                                                                                                                                    //und wenn die Weichen nicht in eine Fahrstrasse eingebunden sind
*/
    {

      //Besetztmeldung
      for (int j = 0; j < besetztmelderposition[fahrstrasse][0][0]; j++)//einbinden der Besetztmelder in die Fahrstrasse
      {
        besetztmeldung.setFahrstrassenelement(j, fahrstrasse, true);
        if(besetztmelderposition[fahrstrasse][j][1] > 0 && weichen.getWeichenfestlegung(besetztmelderposition[fahrstrasse][j][1]-1) == false)
        {
          weichen.weichenStellen(besetztmelderposition[fahrstrasse][j][1]-1,besetztmelderposition[fahrstrasse][j][2]);
          weichen.setWeichenfestlegung(besetztmelderposition[fahrstrasse][j][1]-1, true, fahrstrasse);

        }
        
      }
      //Weichen
     /* weichen.weichenKurve(0);                 //Weiche wird geschaltet
      weichen.setWeichenfestlegung(0, true, 2); //weiche wird festgelegt
      weichen.weichenKurve(1);
      weichen.setWeichenfestlegung(1, true, 2);
*/
      hauptsignal1.hpschalten(1);
      fahrstrassenstati[fahrstrasse-1] = true; //die Fahrstrasse wird als aktiv deklariert
      Serial.print("Fahrstraße ");Serial.println(fahrstrasse);
    }
  }
  
for(int kontrollen=1;kontrollen<fahrstrassenanzahl; kontrollen++)
{
  if (fahrstrassenstati[kontrollen-1] == true)
  {
    //1. besetztmelder
    //auf einen Besetztmelder sind immer zwei zahlen festgelegt, die erste Zahl wird beim besetztsein des melder eingespeichert, die zweite, wird erst dnn eingespeichert, wenn die erste eingespeichert ist,
    //der nächste besetztmelder kann erst freigegeben werden, wenn der erste freigegeben ist, also die Zahl eine bestimmte höhe hat.
    if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[kontrollen][besetztmelderzahl + 1][0], LOW) == HIGH) //wenn der Besetztmelder besetzt ist und die freigabe der Besetztmelder Zahl plus eins entspricht
    {
      hauptsignal1.hpschalten(0);//immer wenn ein Besetztmelder innerhalb einer fahrstrasse belegt wird, soll das zugehörige signal rot werden
      if(freigabe2 == besetztmelderzahl * 2 + 1)
      freigabe2++; //beim ersten mal ist die freigabe1 und wird auf 2 erhöht. wenn dies passiert ist, und der Besetztmelder wieder frei geworden ist, wird der Besetztmelder als Fahrstrassenelement aufgelöst
    }

    if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition[kontrollen][besetztmelderzahl + 1][0], LOW) == LOW && freigabe2 == besetztmelderzahl * 2 + 2)
    {
      //Auflösung der Besetztmelderfestlegung
      besetztmeldung.setFahrstrassenelement(besetztmelderposition[kontrollen][besetztmelderzahl + 1][0], 2, false);
      //Auflösung der Weichenfestlegung
      if(besetztmelderposition[kontrollen][besetztmelderzahl+1][1] > 0 && weichen.getWeichenfestlegung(besetztmelderposition[kontrollen][besetztmelderzahl+1][1]-1) == true)//besetztmelderZahl+1, weil die erste Position des arrays besetztmelderposition nur die Anzhal der Besetztmelder enthält und die namen der melder erst im nächsten feld erscheinen
        {
          weichen.setWeichenfestlegung((besetztmelderposition[kontrollen][besetztmelderzahl+1][1])-1, false, kontrollen);//die WEichenzahl die Herauskommt muss um 1 verkleinert werden, sonst gäbe es keine WEiche 0
        }
      
      freigabe2++;         //3
      besetztmelderzahl++; //es geht mit dem nächsten besetztmelder der Fahrstrasse weiter
      
      if (besetztmelderzahl == besetztmelderposition[2][0][0])//wenn die gesamte Fahrstrße wieder freigegeben ist
      {
        besetztmelderzahl=0;
        freigabe2 = 1;                  //Der Freigabe Zähler wird wieder Zurückgesetzt.
        fahrstrassenstati[1] = false;   //die Fahrstrasse wird wieder als inaktiv gemeldet.
      }
    }
    Serial.println(freigabe2);
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
  hauptsignal1.hauptsignalhp0manuell(); //über die Signalhaltgruppentaste und die Zugtaste auf dem Feld des Signals kann ein Signal auf hp0 gestellt werden
  hauptsignal1.signalSperren();         //über weichen  und weichensperrtaste kann ein Signal gesperrt werden
  //if (digitalRead(zta2) == HIGH)
  //  hauptsignal1.hpschalten(1);

  //Weichen
  weichen.weichenWechseln(); //WGT und WT können zum Umschalten einer Weiche benutzt werden
  weichen.weichenSchalten(); //lässt das Relais nach dem Schalten wieder zurückfallen

  //Besetztmeldung
  besetztmeldung.getBesetztmelderstatus(0, LOW); //besetztmelder sollen immer wenn sie belget sind den Status anzeigen
  besetztmeldung.getBesetztmelderstatus(1, LOW);
  besetztmeldung.getBesetztmelderstatus(2, LOW);
  besetztmeldung.getBesetztmelderstatus(3, LOW);
  besetztmeldung.getBesetztmelderstatus(4, LOW);
  besetztmeldung.getBesetztmelderstatus(5, LOW);
}
