//Sketch steuert die Heizung eines Wohnwagen indem er über NTC die Raumtemperatur erfasst und umrechnet
//um dann per Optokoppler 12V auf eine 24V40W Heizpatrone zu geben und damit die Kapilarrohrsteuerung zu erhitzen um die Heizung abzuregeln.
//Tag und Nachttemperatur und Zeit ist vordefiniert kann jedoch über Menüführung verändert werden.

#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

#define FUEHLER A7                       //10k NTC auf 5V und Messung ANALOG7
#define HEIZPATRONE 9                    //Heizpatrone über Optokoppler an D9

#define SET 2                            //Settaste an D2
#define PLUS 3                           //Plustaste an D3
#define MINUS 4                          //Minustaste an D4

float RAUMTEMP = 0;                      //gemessene Raumtemperatur
int TAGSOLL = 0;                         //gewünschte Tagestemperatur
int NACHTSOLL  =  0;                     //gewünschte Nachtemperatur

int TAGEINSTUNDE  = 00;                  //Beginn Tagestemperatur
int TAGEINMINUTE  = 00;                  //Beginn Tagestemperatur

int TAGAUSSTUNDE = 00;                   //Ende Nachttemperatur
int TAGAUSMINUTE  = 00;                  //Beginn Tagestemperatur

int ABSENKUNG = 0;                       //per Mode angeforderte Nachttemperatur für x Stunden, 1x drücken = 1 Stunde

int MENUE;                               //Menuenummer
int D;                                   // Delay zum Entprellen

void setup() 
{                                        // ################# Startsequenz Beginn ################
  
Serial.begin (9600);                     // Start der seriellen Verbindung für den serial monitor.

pinMode (A7, INPUT);                     //Fühler ist Input
pinMode (9, OUTPUT);                     //Heizpatrone ist Output

pinMode (2, INPUT);                      //PIN2 ist INPUT SET
pinMode (3, INPUT);                      //PIN3 ist INPUT +
pinMode (4, INPUT);                      //PIN4 ist INPUT -

MENUE = 0;                               //Startmenue 0
D = 150;                                 //Delay zum Entprellen 120

TAGSOLL = 22;                            //vordefinierte Tagestemperatur 23°
NACHTSOLL = 18;                          //vordefinierte Nachttemperatur 18°

TAGEINSTUNDE = 07;                       //vordefinierter Beginn Tag 07:?? Uhr
TAGEINMINUTE = 00;                       //vordefinierter Beginn Tag ??:00 Uhr

TAGAUSSTUNDE = 21;                       //vordefiniertes Ende Tag 21:?? Uhr
TAGAUSMINUTE = 00;                       //vordefiniertes Ende Tag ??:00 Uhr

Serial.println ("    SMARavan    ");     //schreibe
Serial.println ("Heizungssteuerung");    //schreibe 
Serial.println ();                       //Leerzeile
delay (1000);

while (!Serial) ; // wait until Arduino Serial Monitor opens
setSyncProvider(RTC.get);   // the function to get the time from the RTC
if(timeStatus()!= timeSet) 
  Serial.println("Unable to sync with the RTC");
else
  Serial.println("RTC has set the system time"); 

Serial.print ("TAG: ");                  //schreibe Tag
Serial.print (TAGSOLL);                  //...Wunschtemperatur
Serial.println ("°C");                   //...°C
Serial.println ();                       //Leerzeile
delay (1000);                            //warte bis gelesen
Serial.print ("VON: ");                  //schreibe von
if (TAGEINSTUNDE < 10)                   //wenn Zeit einstellig
Serial.print("0");                       //schreibe zusätzlich eine Null
Serial.print (TAGEINSTUNDE);             //...Stunde
Serial.print (":");                      //...:
if (TAGEINMINUTE < 10)                   //wenn Zeit einstellig
Serial.print("0");                       //schreibe zusätzlich eine Null
Serial.print (TAGEINMINUTE);             //...Minute
Serial.println (" UHR");                 //...Uhr
delay (1000);                            //warte bis gelesen
Serial.print ("BIS: ");                  //schreibe bis
if (TAGAUSSTUNDE < 10)                   //wenn Zeit einstellig
Serial.print("0");                       //schreibe zusätzlich eine Null
Serial.print (TAGAUSSTUNDE);             //...Stunde
Serial.print (":");                      //...:
if (TAGAUSMINUTE < 10)                   //wenn Zeit einstellig
Serial.print("0");                       //schreibe zusätzlich eine Null
Serial.print (TAGAUSMINUTE);             //...Minute
Serial.println (" UHR");                 //...Uhr
Serial.println ();                       //Leerzeile
delay (1000);                            //warte bis gelesen
Serial.print ("NACHT: ");                //schreibe Nachht
Serial.print (NACHTSOLL);                //...Wunschtemperatur
Serial.println ("°C");                   //...°C
Serial.println ();                       //Leerzeile
delay (1000);

}                                        //################# Startsequenz Ende #######################

void loop() 
{                                                               //############ Programmanfang #######################

  if (timeStatus() == timeSet) {
    digitalClockDisplay();
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(4000);
  }
  delay(1000);
  
(RAUMTEMP) = (analogRead (FUEHLER)/22);                         //Lies den Fühler aus 24,4° = 1023,00

if ((RAUMTEMP < TAGSOLL) && (ABSENKUNG = 0))                    //wenn Raum kälter als gewollt und innerhalb Tag dann
digitalWrite (HEIZPATRONE, LOW);                                //lass die Heizung laufen
else if ((RAUMTEMP > TAGSOLL) && (ABSENKUNG = 0))               //sonst wenn Raum wärmer als gewollt und innerhalb Tag dann
analogWrite (HEIZPATRONE, 80);                               //schalte die Heizung ab

else if (RAUMTEMP < NACHTSOLL)                                  //wenn Raum kälter als nachts gewollt dann
digitalWrite (HEIZPATRONE, LOW);                                //lass die Heizung laufen
else if (RAUMTEMP > NACHTSOLL)                                  //sonst
analogWrite (HEIZPATRONE, 80);                               //schalte die Heizung ab


                                                                //################ Menüführung #####################
                                                                
 
while (MENUE == 0)                                              //### Raumtemperaturanzeige ### Nachtabsenkung per Hand ###
 
  {                                                             //Startmenue 
  Serial.print (RAUMTEMP);                                      //Schreibe die aktuelle Raumtemperatur
  Serial.println ("°C");                                        //Schreibe °C

    if (timeStatus() == timeSet) 
    digitalClockDisplay();
  
  if (digitalRead (SET) == HIGH)                                //wenn Menue gedrückt wird
  MENUE ++;                                                    //schalte ein Menue weiter und
  delay (D);                                                   //entprelle
  
  if ((digitalRead (PLUS) == HIGH) && (ABSENKUNG < 10))          //wenn Plus gedrückt und weniger als 10 Stunden
  {ABSENKUNG ++;                                                 //Absenkung + 1 Stunde
  Serial.print ("ABSENKUNG FÜR ");                               //schreibe Absenkung für
  Serial.print (ABSENKUNG);                                      //...wie lange...
  Serial.println (" Stunde(n)");                                 //...in Stunden
  delay (1000);}                                                 //entprelle 

  if ((digitalRead (MINUS) == HIGH) && (ABSENKUNG > 0))          //wenn Plus gedrückt und mehr als 0 Stunden
  {ABSENKUNG --;                                                 // Absenkung + 1 Stunde
  Serial.print ("ABSENKUNG FÜR ");                               //schreibe Absenkung für
  Serial.print (ABSENKUNG);                                      //...wie lange...
  Serial.println (" Stunde(n)");                                 //...in Stunden
  delay (1000);}                                                 //entprelle   
  }

                                                                 //### TAGSOLL ###
                                                                 
while (MENUE == 1)                                               //wenn Menue einmal gedrückt wurde
  {
  Serial.print ("TAG: ");                                        //schreibe Tag...
  Serial.print (TAGSOLL);                                        //...Solltemperatur...
  Serial.println ("°C");                                         //....°C
  
  if (digitalRead (SET) == HIGH)                                 //wenn Menue gedrückt wird
     MENUE ++;                                                   //schalte ein Menue weiter und
     delay (D);                                                  //entprelle
    
  if ((digitalRead (PLUS) == HIGH) && (TAGSOLL < 30))            //wenn + gedrückt und Soll unter 30
     TAGSOLL ++ ;                                                //Tagsoll +1°
     delay (D);                                                  //entprelle
    
  if ((digitalRead (MINUS) == HIGH) && (TAGSOLL > 10))           //wenn - gedrückt und Soll über 10
     TAGSOLL -- ;                                                //Tagsoll -1°      
     delay (D);                                                  //entprelle
  }
  
                                                                 //### NACHTSOLL ###  
                                                                    
while (MENUE == 2)                                               //wenn Menue noch einmal gedrückt wurde
  { 
  Serial.print ("NACHT: ");                                      //schreibe Tag...
  Serial.print (NACHTSOLL);                                      //...Solltemperatur...
  Serial.println ("°C");                                         //....°C
  
  if (digitalRead (SET) == HIGH)                                 //wenn Menue gedrückt wird
  MENUE ++;                                                      //schalte ein Menue weiter und
  delay (D);                                                     //warte 
    
   if ((digitalRead (PLUS) == HIGH)  && (NACHTSOLL < 25))        //wenn + gedrückt und Soll unter 25
      NACHTSOLL ++ ;                                             //Nachtsoll +1°
      delay (D);                                                 //entprelle

   if ((digitalRead (MINUS) == HIGH) && (NACHTSOLL > 5))         //wenn - gedrückt und Soll über 10
      NACHTSOLL -- ;                                             //Nachtsoll -1°
      delay (D);                                                 //entprelle
  }                               

                                                                 //### TAGBEGINN ####
                                                                 
while (MENUE == 3)                                               //wenn Menue noch einmal gedrückt wurde
  {                                  
  Serial.print ("TAG VON: *");                                   //schreibe Tag...
  if (TAGEINSTUNDE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.print (TAGEINSTUNDE);                                   //...Stunde...
  Serial.print (":");                                            //...:...
  Serial.print (TAGEINMINUTE);                                   //...Minute
  if (TAGEINMINUTE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.println (" Uhr");                                       //....Uhr

  if (digitalRead (SET) == HIGH)                                 //wenn Menue gedrückt wird
  MENUE ++;                                                      //schalte ein Menue weiter und
  delay (D);                                                     //warte 
    
  if ((digitalRead (PLUS) == HIGH) && (TAGEINSTUNDE <24))        //wenn + gedrückt und Stunde unter 24
      TAGEINSTUNDE ++ ;                                          //Tag Stunde +1
      delay (D);                                                 //entprelle
 
  if ((digitalRead (MINUS) == HIGH) && (TAGEINSTUNDE <0))        //wenn - gedrückt und Stunde über 0
      TAGEINSTUNDE -- ;                                          //Tag Stunde -1      
      delay (D);                                                 //entprelle
  }  

while (MENUE == 4)                                               //wenn Menue noch einmal gedrückt wurde
  {                                  
  Serial.print ("TAG VON: ");                                    //schreibe Tag...
  if (TAGEINSTUNDE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.print (TAGEINSTUNDE);                                   //...Stunde...
  Serial.print (":");                                            //...:...
  if (TAGEINMINUTE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.print (TAGEINMINUTE);                                   //...Minute
  Serial.println ("* Uhr");                                      //....Uhr

  if (digitalRead (SET) == HIGH)                                 //wenn Menue gedrückt wird
  MENUE ++;                                                      //schalte ein Menue weiter und
  delay (D);                                                     //warte 
    
  if ((digitalRead (PLUS) == HIGH) && (TAGEINMINUTE < 59))       //wenn + gedrückt und Minuten unter 59
      TAGEINMINUTE = TAGEINMINUTE + 15;                                          //Tag Minute +1
      delay (D);                                                 //entprelle
 
  if ((digitalRead (MINUS) == HIGH) && (TAGEINMINUTE > 0))       //wenn - gedrückt und Minuten über 0
      TAGEINMINUTE = TAGEINMINUTE - 15;                                          //Tag Minute -1      
      delay (D);                                                 //entprelle
  }
  
                                                                 //### TAGENDE ###

while (MENUE == 5)                                               //wenn Menue noch einmal gedrückt wurde
  {                                  
  Serial.print ("TAG VON: *");                                   //schreibe Tag...
  Serial.print (TAGAUSSTUNDE);                                   //...Stunde...
  if (TAGAUSSTUNDE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.print (":");                                            //...:...
  if (TAGAUSMINUTE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.print (TAGAUSMINUTE);                                   //...Minute
  Serial.println (" Uhr");                                       //....Uhr

  if (digitalRead (SET) == HIGH)                                 //wenn Menue gedrückt wird
  MENUE ++;                                                      //schalte ein Menue weiter und
  delay (D);                                                     //warte 
    
  if ((digitalRead (PLUS) == HIGH) && (TAGAUSSTUNDE < 24))       //wenn + gedrückt und Stunde unter 24
      TAGAUSSTUNDE ++;                                           //Tag Stunde +1
      delay (D);                                                 //entprelle
 
  if ((digitalRead (MINUS) == HIGH) && (TAGAUSSTUNDE > 0))       //wenn - gedrückt und Stunde über 0
      TAGAUSSTUNDE -- ;                                          //Tag Stunde -1      
      delay (D);                                                 //entprelle
  }  

while (MENUE == 6)                                               //wenn Menue noch einmal gedrückt wurde
  {                                  
  Serial.print ("TAG BIS: ");                                    //schreibe Tag...
  if (TAGAUSSTUNDE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.print (TAGAUSSTUNDE);                                   //...Stunde...
  Serial.print (":");                                            //...:...
  if (TAGAUSMINUTE < 10)                                         //wenn Zeit einstellig
  Serial.print("0");                                             //schreibe zusätzlich eine Null
  Serial.print (TAGAUSMINUTE);                                   //...Minute
  Serial.println ("* Uhr");                                      //....Uhr

  if (digitalRead (SET) == HIGH)                                 //wenn Menue gedrückt wird
  MENUE = 0;                                                     //schalte ein Menue weiter und
  delay (D);                                                     //warte 
    
  if ((digitalRead (PLUS) == HIGH) && (TAGAUSMINUTE <59))        //wenn + gedrückt und Minute unter 59
      TAGAUSMINUTE = TAGAUSMINUTE + 15;                          //Tag Stunde +1
      delay (D);                                                 //entprelle
 
  if ((digitalRead (MINUS) == HIGH) && (TAGAUSMINUTE >0))        //wenn - gedrückt und Minute über 0
      TAGAUSMINUTE = TAGAUSMINUTE - 15;                          //Tag Stunde -1      
      delay (D);                                                 //entprelle
  } 
}                                                                //Programmende

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
