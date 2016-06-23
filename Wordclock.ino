// DECLARATIONS ********************************************************************************************************************************************************************************************************************

//Librairie Température
#include "DHT.h"
DHT dht;

//Librairie récupération de l'heure
#include <Process.h>

// Initialisation des LED ("grid")
#include <Adafruit_NeoPixel.h>
#define RGBLEDPIN 6
#define N_LEDS 114 // 10 lignes de 11 LED
#define BRIGHTNESSDAY 150 // full on
#define BRIGHTNESSNIGHT 55 // half on
Adafruit_NeoPixel grid = Adafruit_NeoPixel(N_LEDS, RGBLEDPIN, NEO_GRB + NEO_KHZ800);

// Variables du temps
int delayval = 500;
Process date;                 // process used to get the date
int hour, minute, second;  // for the results
int lastSecond = -1;          // need an impossible value for comparison
int lastMinute = -1;

// Variables de couleur des LED
int intBrightness = BRIGHTNESSDAY; // the brightness of the clock (0 = off and 255 = 100%)
uint32_t colorWhite = grid.Color(80, 80, 60);
uint32_t colorBlack = grid.Color(0, 0, 0);
uint32_t colorRed = grid.Color(255, 0, 0);
uint32_t colorGreen = grid.Color(0, 255, 0);
uint32_t colorBlue = grid.Color(0, 0, 255);
uint32_t colorJGreen = grid.Color(50, 179, 30);

// Variables de la température
int TableauTemperature[2];
int TemperatureDizaine = -1 ; //variable temporaire pour éviter le clignotement lors de la mise à jour de la temperature
int TemperatureUnite = - 1; //variable temporaire pour éviter le clignotement lors de la mise à jour de la temperature

// Variables et constantes d'état des switchs
const int SwitchHTpin = 2;     // le switch Heure/Température sur le pin 2
int SwitchHTstatus = 0;
int CaaChangeOuPas = HIGH;


// Les mots
int arrIL[] = {1,2,-1};
int arrEST[] = {4,5,6,-1};
int arrET[] = {88,89,-1};
int arrQUART[] = {81,82,83,84,85,-1};
int arrET_DEMIE[] = {105,106,107,108,109,111,112,-1};
int arrMOINS[] = {68,69,70,71,72,-1};
int arrLE[] = {74,75,-1};
int arrVINGT_MINUTES[] = {90,91,92,93,94,-1};
int arrCINQ_MINUTES[] = {96,97,98,99,-1};
int arrVINGT_CINQ[] = {90,91,92,93,94,95,96,97,98,99,-1};
int arrDIX_MINUTES[] = {76,77,78,-1};
int arrHEURE[] = {58,59,60,61,62,-1}; 
int arrHEURES[] = {57,58,59,60,61,62,-1};

int arrUNE[] = {28,29,30,-1};
int arrDEUX[] = {8,9,10,11,-1};
int arrTROIS[] = {13,14,15,16,17,-1};
int arrQUATRE[] = {18,19,20,21,22,23,-1};
int arrCINQ[] = {35,36,37,38,-1};
int arrSIX[] = {39,40,41,-1};
int arrSEPT[] = {31,32,33,34,-1};
int arrHUIT[] = {42,43,44,45,-1};
int arrNEUF[] = {24,25,26,27,-1};
int arrDIX_HEURES[] = {48,49,50,-1};
int arrONZE[] = {64,65,66,67,-1};
int arrMIDI[] = {46,47,48,49,-1};
int arrMINUIT[] = {51,52,53,54,55,56,-1};



// Les chiffres des températures
int arrHELLO[] = {25,27,29,32,37,40,42,44,47,48,49,51,54,62,64,66,69,71,73,76,-1};
int arrTEMPDIZAINE1[] = {27,42,43,49,64,71,-1};
int arrTEMPDIZAINE2[] = {25,26,27,42,47,48,49,66,69,70,71,-1};
int arrTEMPDIZAINE3[] = {25,26,27,42,48,49,64,69,70,71,-1};
int arrTEMP0[] = {29,30,31,38,40,51,53,60,62,73,74,75,-1};
int arrTEMP1[] = {30,39,40,52,61,74,-1};
int arrTEMP2[] = {29,30,31,38,51,52,53,62,73,74,75,-1};
int arrTEMP3[] = {29,30,31,38,52,53,60,73,74,75,-1};
int arrTEMP4[] = {29,31,38,40,51,52,53,60,75,-1};
int arrTEMP5[] = {29,30,31,40,51,52,53,60,73,74,75,-1};
int arrTEMP6[] = {29,30,31,40,51,52,53,60,62,73,74,75,-1};
int arrTEMP7[] = {29,30,31,38,53,60,75,-1};
int arrTEMP8[] = {29,30,31,38,40,51,52,53,60,62,73,74,75,-1};
int arrTEMP9[] = {29,30,31,38,40,51,52,53,60,73,74,75,-1};

// Variables pour le détecteur de mouvements
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;

// SETUP *******************************************************************************************************************************************************************************************************************

void setup() {

  
// lancement des LEDS
grid.begin();
grid.show();
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);


// lancement du thermomètre
Serial.println();
Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");

dht.setup(10); // data de température sur le pin 10

pinMode(SwitchHTpin, INPUT);

// Message de bienvenue
spellWord(arrHELLO, colorWhite);

//Calibrage du détecteur de mouvements
/*
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
*/

//première récupération du temps
  Bridge.begin();        // initialize Bridge
  
  //Serial.begin(9600);    // initialize serial
  //while (!Serial);              // wait for Serial Monitor to open
  //Serial.println("Time Check");  // Title of sketch

  // run an initial date process. Should return:
  // hh:mm:ss :
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%T");
    date.run();
    Serial.println("Date");
  }
delay(delayval);

// la grid est prête, on éteint le Hello, et affiche le temps.
paintWord(arrHELLO, colorBlack);
displayTime();
}



// LOOP ********************************************************************************************************************************************************************************************************************
void loop(){

delay(delayval);
grid.show();

SwitchHTstatus = digitalRead(SwitchHTpin);
//Serial.println(SwitchHTstatus);

if (SwitchHTstatus != CaaChangeOuPas){
        for(int i = 0; i < grid.numPixels() + 1; i++){
        grid.setPixelColor(i, colorBlack);
        } 
       } 
if (SwitchHTstatus == HIGH){
        displayTime();
        }
        else if (SwitchHTstatus == LOW){
             displayTemperature();  
           }
        
CaaChangeOuPas = SwitchHTstatus;
   
}


// Fonction d'affichage de la température ********************************************************************************************************************************************************************************
void displayTemperature(){
  
  //delay(dht.getMinimumSamplingPeriod());

  int temperature = dht.getTemperature();

  //Serial.print(dht.getStatusString());
  //Serial.print("\t\t");
  //Serial.print(temperature, 1);
  //Serial.print("\t\t");
  TableauTemperature[0] = temperature % 10;
  TableauTemperature[1] = temperature / 10;
  //Serial.print(TableauTemperature[1], 1);
  //Serial.print("\t\t");
  //Serial.println(TableauTemperature[0], 1);


// on affiche la température sur la grid
// Affichage du degré
grid.setPixelColor(33, colorWhite);

//affichage des unités
if (TableauTemperature[0] != TemperatureUnite) {
  paintWord(arrTEMP0, colorBlack);
  paintWord(arrTEMP1, colorBlack);
  paintWord(arrTEMP2, colorBlack);
  paintWord(arrTEMP3, colorBlack);
  paintWord(arrTEMP4, colorBlack);
  paintWord(arrTEMP5, colorBlack);
  paintWord(arrTEMP6, colorBlack);
  paintWord(arrTEMP7, colorBlack);
  paintWord(arrTEMP8, colorBlack);
  paintWord(arrTEMP9, colorBlack);
}

  switch(TableauTemperature[0]){
    case 0:paintWord(arrTEMP0, colorWhite);
    break;
    case 1:paintWord(arrTEMP1, colorWhite);
    break;
    case 2:paintWord(arrTEMP2, colorWhite);
    break;
    case 3:paintWord(arrTEMP3, colorWhite);
    break;
    case 4:paintWord(arrTEMP4, colorWhite);
    break;
    case 5:paintWord(arrTEMP5, colorWhite);
    break;
    case 6:paintWord(arrTEMP6, colorWhite);
    break;
    case 7:paintWord(arrTEMP7, colorWhite);
    break;
    case 8:paintWord(arrTEMP8, colorWhite);
    break;
    case 9:paintWord(arrTEMP9, colorWhite);
    break;
    }
  
TemperatureUnite = TableauTemperature[0];

//affichage des dizaines
if (TableauTemperature[1] != TemperatureDizaine) {

  paintWord(arrTEMPDIZAINE1, colorBlack);
  paintWord(arrTEMPDIZAINE2, colorBlack);
  paintWord(arrTEMPDIZAINE3, colorBlack);
}
  if(TableauTemperature[1]==1){
    paintWord(arrTEMPDIZAINE1, colorWhite);
     }
      else if(TableauTemperature[1]==2){
      paintWord(arrTEMPDIZAINE2, colorWhite);
        }
         else if(TableauTemperature[1]==3){
        paintWord(arrTEMPDIZAINE3, colorWhite);
          }

TemperatureDizaine = TableauTemperature[1];

}



// Fonction d'affichage de l'heure ****************************************************************************************************************************************************************************************
void displayTime(){


    // start the date process:
    if (!date.running()) {
      date.begin("date");
      date.addParameter("+%T");
      date.run();
  }

  //if there's a result from the date process, parse it:
  while (date.available() > 0) {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();

    // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");

    // get the substrings for hour, minute second:
    String hourString = timeString.substring(0, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    // convert to ints,saving the previous second:
    hour = hourString.toInt();
    minute = minString.toInt();
    lastSecond = second;          // save to do a time comparison
    second = secString.toInt();
  }
 
// *********************************************************************************  On allume "IL EST"


paintWord(arrIL, colorWhite);
paintWord(arrEST, colorWhite);


//********************************************************************************** on allume les coins selon la minute où nous nous trouvons
if((minute==0) || (minute==5) || (minute==10) || (minute==15) || (minute==20) || (minute==56) || (minute==30) || (minute==35) || (minute==40) || (minute==45) || (minute==50) || (minute==55)){
  grid.setPixelColor(0, colorBlack);
  grid.setPixelColor(12, colorBlack);
  grid.setPixelColor(101, colorBlack);
  grid.setPixelColor(113, colorBlack);
  grid.show(); 
}
else if((minute==1) || (minute==6) || (minute==11) || (minute==16) || (minute==21) || (minute==26) || (minute==31) || (minute==36) || (minute==41) || (minute==46) || (minute==51) || (minute==56)){
  grid.setPixelColor(0, colorBlack);
  grid.setPixelColor(12, colorWhite);
  grid.setPixelColor(101, colorBlack);
  grid.setPixelColor(113, colorBlack);
  grid.show(); 
}
else if((minute==2) || (minute==7) || (minute==12) || (minute==17) || (minute==22) || (minute==27) || (minute==32) || (minute==37) || (minute==42) || (minute==47) || (minute==52) || (minute==57)){
  grid.setPixelColor(0, colorBlack);
  grid.setPixelColor(12, colorWhite);
  grid.setPixelColor(101, colorWhite);
  grid.setPixelColor(113, colorBlack);
  grid.show(); 
}  
else if((minute==3) || (minute==8) || (minute==13) || (minute==18) || (minute==23) || (minute==28) || (minute==33) || (minute==38) || (minute==43) || (minute==48) || (minute==53) || (minute==58)){
  grid.setPixelColor(0, colorBlack);
  grid.setPixelColor(12, colorWhite);
  grid.setPixelColor(101, colorWhite);
  grid.setPixelColor(113, colorWhite);
  grid.show();
}
else if((minute==4) || (minute==9) || (minute==14) || (minute==19) || (minute==24) || (minute==29) || (minute==34) || (minute==39) || (minute==44) || (minute==49) || (minute==54) || (minute==59)){
  grid.setPixelColor(0, colorWhite);
  grid.setPixelColor(12, colorWhite);
  grid.setPixelColor(101, colorWhite);
  grid.setPixelColor(113, colorWhite);
  grid.show();
}
  
// ******************************************************************************** On allume les minutes (par pas de 5). Le "moins" de 35 à 55mn sera géré avec les heures
if((minute>=0) && (minute<5)){  
// PILE
paintWord(arrCINQ_MINUTES, colorBlack);  
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorBlack);
paintWord(arrVINGT_CINQ, colorBlack);
} 
if((minute>4) && (minute<10)){  
// FIVE MINUTES
paintWord(arrCINQ_MINUTES, colorWhite);  
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorBlack);
//paintWord(arrVINGT_CINQ, colorBlack);
} 
if((minute>9) && (minute<15)) { 
//TEN MINUTES;
paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorWhite);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorBlack);
paintWord(arrVINGT_CINQ, colorBlack);
}
if((minute>14) && (minute<20)) {
// QUARTER
paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorWhite);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorWhite);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorBlack);
paintWord(arrVINGT_CINQ, colorBlack);
}
if((minute>19) && (minute<25)) { 
//TWENTY MINUTES
//paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorWhite);
//paintWord(arrVINGT_CINQ, colorBlack);
}
if((minute>24) && (minute<30)) { 
//TWENTY FIVE
//paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
//paintWord(arrVINGT_MINUTES, colorBlack);
paintWord(arrVINGT_CINQ, colorWhite);
} 
if((minute>29) && (minute<35)) {
//THIRTY
paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorWhite);
paintWord(arrVINGT_MINUTES, colorBlack);
paintWord(arrVINGT_CINQ, colorBlack);
}
if((minute>34) && (minute<40)) { 
//MOINS VINGT-CINQ
//paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
//paintWord(arrVINGT_MINUTES, colorBlack);
paintWord(arrVINGT_CINQ, colorWhite);
} 
if((minute>39) && (minute<45)) {
paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorWhite);
//paintWord(arrVINGT_CINQ, colorBlack);
}
if((minute>44) && (minute<50)) {
paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorWhite);
paintWord(arrQUART, colorWhite);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorBlack);
paintWord(arrVINGT_CINQ, colorBlack);
}
if((minute>49) && (minute<55)){
paintWord(arrCINQ_MINUTES, colorBlack);
paintWord(arrDIX_MINUTES, colorWhite);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorBlack); 
paintWord(arrVINGT_CINQ, colorBlack);
} 
if(minute>54){
paintWord(arrCINQ_MINUTES, colorWhite);
paintWord(arrDIX_MINUTES, colorBlack);
paintWord(arrET, colorBlack);
paintWord(arrLE, colorBlack);
paintWord(arrQUART, colorBlack);
paintWord(arrET_DEMIE, colorBlack);
paintWord(arrVINGT_MINUTES, colorBlack);
//paintWord(arrVINGT_CINQ, colorBlack);
}

// ************************************************************************************************************* pour de xx heur(s) 00 à 34 (affichage de l'heure normale et sans "moins")
if((minute < 35) && (minute >=0)){
paintWord(arrMOINS, colorBlack);
switch(hour){
case 1:
case 13:
paintWord(arrHEURE, colorWhite);
//paintWord(arrHEURES, colorBlack);
paintWord(arrUNE, colorWhite);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 2:
case 14:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorWhite);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 3: 
case 15:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorWhite);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 4: 
case 16:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorWhite);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 5: 
case 17:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorWhite);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 6: 
case 18:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorWhite);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 7: 
case 19:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorWhite);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 8: 
case 20:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorWhite);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 9: 
case 21:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorWhite);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 10:
case 22:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorWhite);
paintWord(arrONZE, colorBlack);
//paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 11:
case 23:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorWhite);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 12: 
paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorBlack);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
//paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorWhite);
paintWord(arrMINUIT, colorBlack);
break;
case 0:
case 24:
paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorBlack);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorWhite);
break;


}
}





//***************************************************************************************************************** pour de xx heure(s) 35 à 59 (affichage d'un "moins" et heure suivante)

else{
paintWord(arrMOINS, colorWhite);
switch (hour) {
case 1: 
case 13:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorWhite);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 14:
case 2:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorWhite);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 15:
case 3:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorWhite);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 4: 
case 16:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorWhite);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 5: 
case 17:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorWhite);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 6: 
case 18:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorWhite);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 7: 
case 19:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorWhite);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 8: 
case 20:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorWhite);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 9: 
case 21:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorWhite);
paintWord(arrONZE, colorBlack);
//paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 10: 
case 22:
//paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorWhite);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorWhite);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
case 11: 
paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorBlack);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
//paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorWhite);
paintWord(arrMINUIT, colorBlack);
break;
case 23:
paintWord(arrHEURE, colorBlack);
paintWord(arrHEURES, colorBlack);
paintWord(arrUNE, colorBlack);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorWhite);
break;
case 0:
case 12: 
case 24:
paintWord(arrHEURE, colorWhite);
//paintWord(arrHEURES, colorBlack);
paintWord(arrUNE, colorWhite);
paintWord(arrDEUX, colorBlack);
paintWord(arrTROIS, colorBlack);
paintWord(arrQUATRE, colorBlack);
paintWord(arrCINQ, colorBlack);
paintWord(arrSIX, colorBlack);
paintWord(arrSEPT, colorBlack);
paintWord(arrHUIT, colorBlack);
paintWord(arrNEUF, colorBlack);
paintWord(arrDIX_HEURES, colorBlack);
paintWord(arrONZE, colorBlack);
paintWord(arrMIDI, colorBlack);
paintWord(arrMINUIT, colorBlack);
break;
}
}
}
// Les autres fonctions utiles ******************************************************************************************************************************************************************************************** 

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, uint8_t wait) {
for(uint16_t i=0; i<grid.numPixels(); i++) {
grid.setPixelColor(i, color);
}
grid.show();
delay(wait);
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
if(WheelPos < 85) {
return grid.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
} else if(WheelPos < 170) {
WheelPos -= 85;
return grid.Color(255 - WheelPos * 3, 0, WheelPos * 3);
} else {
WheelPos -= 170;
return grid.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}
}
void paintWord(int arrWord[], uint32_t intColor){
for(int i = 0; i < grid.numPixels() + 1; i++){
if(arrWord[i] == -1){
grid.show();
break;
}else{
grid.setPixelColor(arrWord[i],intColor);
}
}
}
void spellWord(int arrWord[], uint32_t intColor){
for(int i = 0; i < grid.numPixels() + 1; i++){
if(arrWord[i] == -1){
break;
}else{
grid.setPixelColor(arrWord[i],intColor);
grid.show();
delay(25);
}
}
}

void fadeOut(int time){
for (int i = intBrightness; i > 0; --i){
grid.setBrightness(i);
grid.show();
delay(time);
}
}
void fadeIn(int time){
for(int i = 1; i < intBrightness; ++i){
grid.setBrightness(i);
grid.show();
delay(time); 
}
}


