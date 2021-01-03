#include <Arduino.h>
#include "Gleisbild.h"

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

//SpeicherPositionen
int adressWeichenposition1 = 1; //Speicheradresse für die Weichenposition von Weiche 1 für den EEPROM
int adressWeichenposition2 = 5; //Speicheradresse für die Weichenposition von Weiche 1 für den EEPROM

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

//Besetztmelder
const int besetztmelderAnzahl = 6;
int besetztmelderEingaenge[besetztmelderAnzahl] = {42, 41, 40, 39, 38, 37}; //an gnd angeschlossen
int besetztmelderLedsGelb[besetztmelderAnzahl] = {0, 0, 0, 0, 110, 112};
int besetztmelderLedsRot[besetztmelderAnzahl] = {0, 0, ftueMelderLed, 111, 0, 113};

//Objektedefinitonen
//Gleisbesetztmelder
besetztmeldungControl besetztmeldung(besetztmelderEingaenge, besetztmelderLedsGelb, besetztmelderLedsRot, besetztmelderAnzahl, schieberegisterPins);
//Weichen
weichen weiche1(1, w1g, w1k, ledw1g, ledw1k, adressWeichenposition1, weichentimeout, wt1, wgt, schieberegisterPins); //Objekt Weiche1 aus der Klasse Weichen inkl. aller relevanten Informationen zu Weiche
weichen weiche2(2, w2g, w2k, ledw2g, ledw2k, adressWeichenposition2, weichentimeout, wt2, wgt, schieberegisterPins); //Objekt Weiche1 aus der Klasse Weichen inkl. aller relevanten Informationen zu Weiche

//Melder
melder ftueMelder(ftueMelderName, ftueMelderLed, weckerPin, ftueMelderWut, schieberegisterPins); //Ausgabe von FTÜ im Seriellen Monitior funktioniert nicht

//Zugtasten
zugtasten zugtaste1(zta1, schieberegisterPins); //Objekt zugtaste1 mit Pin
zugtasten zugtaste8(zta2, schieberegisterPins);
zugtasten zugtaste10(zta3, schieberegisterPins);
zugtasten zugtaste18(zta4, schieberegisterPins);

//Signale
hauptsignale hauptsignal1(rot1, gruen1, gelb1, signaltaste1, sperrmelder1, allgSignaltasten, schieberegisterPins);
//int rotPin, int gelbPin, int gruenPin,  int signaltaste, int sperrmelder, int allgSignaltasten[3], int registerPin[4]

//Fahrstraßensteuerung
int zugtastenspeicher[2]; //es werden die Zugtasten gespeichert, die gedrückt wurden.
int anzahl = 0;           //zählt wie viele zugtasten gedrückt wurden
boolean fahrstrassenstati[2];

//Fahrstraßensteuerung
const int felderAnzahl = 18;
//feld1 und feld2
int fahrstrassenspeicher[felderAnzahl][felderAnzahl]; //1 variable = nummer des Tischfeldes, 2. variable = nummer des Zweiten Tischfeldes
int freigabe2 = 1;
int besetztmelderposition2[] = {3, 4, 1, 2};
int besetztmelderzahl = 0;
/*Warum wird keine Unterschiedung zwischen links und rechts benötigt? 
 * Da Züge nur mit den Signalen fahren, und nicht gegen,
 * gibt es für jede Tasten kombi von eine Richtung, für die Fahrt in die andere Richtung, muss ein anderes Signal verwendet werden.
*/

void setup()
{
  pinMode(ftueMelderLed, OUTPUT);
  ftueMelder.digitalSchalten(weckerPin, HIGH);
  weiche1.weicheRelaisHIGH(); //alle möglichen Eingaben an den Relais werdengelöscht
  weiche2.weicheRelaisHIGH();
  weiche1.weichenpositionEEPROM(); //gespeicherte Weichenposition wird angezeigt und ausgeführt
  weiche2.weichenpositionEEPROM();
  Serial.begin(9600);
  //weiche1.setRegisterPins(2,8,9,10);

  hauptsignal1.hpschalten(0); //....................................................................................
  //besetztmeldung.setBesetztmelderBeleuchtung(0,HIGH);                  //Der Status des Lichtes kann eingestellt werden

  fahrstrassenspeicher[10][8] = 2;
  fahrstrassenspeicher[8][10] = 2; //wenn die Tasten auf feld 10 und feld 8 gedrückt werden, soll die Fahrstraße 1 einlaufen::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

void loop()
{
  zugtastenspeicher[0] = 0;
  zugtastenspeicher[1] = 0;
  anzahl = 0;
  if (zugtaste10.getzugtastenstatus() == true && anzahl < 2)      //Wenn die Zugstrassentaste gedrückt wurde, und noch nicht zwei Tasten gedrückt wurden
  {
    zugtastenspeicher[anzahl] = 10;//speichere das Feld auf dem die taste gedrückt wurde
    anzahl++;//erhöhe die anzahl der gedrückten anzahl an tasten um 1
  }
  if (zugtaste8.getzugtastenstatus() == true && anzahl < 2)     //s.o.
  {
    zugtastenspeicher[anzahl] = 8;
    anzahl++;
  }
  if (zugtaste18.getzugtastenstatus() == true && anzahl < 2)
  {
    zugtastenspeicher[anzahl] = 18;
    anzahl++;
  }
  /*
es wird die 18 auch als 8 und 1 erkannt, liegt evtl an der größe des arrays 
evtl. sollen die Zahlen vorher sortiert werden, damit sie nicht zu groß für eine array position sind
*/


  if (fahrstrassenspeicher[zugtastenspeicher[0]][zugtastenspeicher[1]] == 2)
  {

    if ((besetztmeldung.getBesetztmelderstatus(2, LOW) == LOW) && besetztmeldung.getBesetztmelderstatus(1, LOW) == LOW //wenn die besetztmelder unbesetzt sind
        && besetztmeldung.getFahrstrassenelement(2) == false && besetztmeldung.getFahrstrassenelement(2) == false      //und sie nicht in eine Fahrstrasse eingebunden sind
        && weiche1.getWeichenfestlegung() == false && weiche2.getWeichenfestlegung() == false)                         //und wenn die Weichen nicht in eine Fahrstrasse eingebunden sind

    {
      //Besetztmeldung
      besetztmeldung.setFahrstrassenelement(1, 2, true); //binde die Besetztmelder in die Fahrstrasse ein
      besetztmeldung.setFahrstrassenelement(2, 2, true);
      //Weichen
      weiche1.weicheKurve();                 //Weiche wird geschaltet
      weiche1.setWeichenfestlegung(true, 2); //weiche wird festgelegt
      weiche2.weicheKurve();
      weiche2.setWeichenfestlegung(true, 2);

      hauptsignal1.hpschalten(1);
      fahrstrassenstati[1] = true; //die Fahrstrasse wird als aktiv deklariert
      Serial.println("Fahrstraße 2");
    }
  }
  /*if (fahrstrassenstati[1] == true)
  {

    if (besetztmeldung.getBesetztmelderstatus(2, LOW) == HIGH)
    {
      hauptsignal1.hpschalten(0);
      besetztmeldung.setFahrstrassenelement(1, 2, false);
    }
    if (besetztmeldung.getBesetztmelderstatus(1, LOW) == HIGH)
    {
      hauptsignal1.hpschalten(0);
      besetztmeldung.setFahrstrassenelement(1, 2, false);
    }

    //alle besetztmelder müssen in der Fahrstraße einmal rot gewesen sein und am ende wieder frei sein
  }
  int freigabe2 = 1;
  int besetztmelderposition2[] = {3,4,1,2};
  in besetztmelderzahl = 0;
  dreidimensionales array [tasterfeld1][tasterfeld1][1. ebene Fahrstrasse 2. bis weitere ebene besetztmelder]
  erster und letzter besetztmelder sind besonders
  int fahrstrassenspeicher[tischfeld][tischfeld][ebene] 
    1. Ebene: Fahrstrassennr
    2. Ebene: Anzahl der Besetztmelder
    3. bis ... Ebene: Nummern der Besetztmelder
  */
  if (fahrstrassenstati[1] == true)
  {
    //1. besetztmelder
    //auf einen Besetztmelder sind immer zwei zahlen festgelegt, die erste Zahl wird beim besetztsein des melder eingespeichert, die zweite, wird erst dnn eingespeichert, wenn die erste eingespeichert ist,
    //der nächste besetztmelder kann erst freigegeben werden, wenn der erste freigegeben ist, also die Zahl eine bestimmte höhe hat.

    if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition2[besetztmelderzahl], LOW) == HIGH && freigabe2 == besetztmelderzahl * 2 + 1) //wenn der Besetztmelder besetzt ist und die freigabe der Besetztmelder Zahl plus eins entspricht
    {
      hauptsignal1.hpschalten(0);
      freigabe2++; //beim ersten mal ist die freigabe1 und wird auf 2 erhöht. wenn dies passiert ist, und der Besetztmelder wieder frei geworden ist, wird der Besetztmelder als Fahrstrassenelement aufgelöst
    }

    if (besetztmeldung.getBesetztmelderstatus(besetztmelderposition2[besetztmelderzahl], LOW) == LOW && freigabe2 == besetztmelderzahl * 2 + 2)
    {
      besetztmeldung.setFahrstrassenelement(besetztmelderposition2[besetztmelderzahl], 2, false);
      freigabe2++;         //3
      besetztmelderzahl++; //es geht mit dem nächsten besetztmelder der Fahrstrasse weiter

      if (besetztmelderzahl == 4)
      {
        freigabe2 = 1;
        fahrstrassenstati[1] = false;
        weiche1.setWeichenfestlegung(false, 2);
        weiche2.setWeichenfestlegung(false, 2);
      }
    }
    Serial.println(freigabe2);
    //alle besetztmelder müssen in der Fahrstraße einmal rot gewesen sein und am ende wieder frei sein
  }

  delay(500);
  /*//Zugtastensteuerung
  if (zugtaste1.getzugtastenstatus() == true && zugtaste2.getzugtastenstatus() == true)
  {
    weiche1.weicheGerade();
    weiche2.weicheGerade();
  }
  if (zugtaste2.getzugtastenstatus() == true && zugtaste3.getzugtastenstatus() == true)
  {
    weiche1.weicheKurve();
    weiche2.weicheKurve();
  }
  if (zugtaste3.getzugtastenstatus() == true && zugtaste4.getzugtastenstatus() == true)
  {
    weiche1.weicheGerade();
    weiche2.weicheGerade();
  }*/

  //FTÜ-Melder
  if (zugtaste1.getzugtastenstatus() == HIGH || zugtaste8.getzugtastenstatus() == HIGH || zugtaste10.getzugtastenstatus() == HIGH || zugtaste18.getzugtastenstatus() == HIGH) //wenn eine Zugtaste gedrückt ist
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
  weiche1.weicheWechsel(); //WGT und WT können zum Umschalten einer Weiche benutzt werden
  weiche2.weicheWechsel();
  weiche1.weichenSchalten(); //lässt das Relais nach dem Schalten wieder zurückfallen
  weiche2.weichenSchalten();

  //Besetztmeldung
  besetztmeldung.getBesetztmelderstatus(0, LOW); //besetztmelder sollen immer wenn sie belget sind den Status anzeigen
  besetztmeldung.getBesetztmelderstatus(1, LOW);
  besetztmeldung.getBesetztmelderstatus(2, LOW);
  besetztmeldung.getBesetztmelderstatus(3, LOW);
}
