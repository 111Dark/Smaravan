#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc; 
#include <SPI.h> 
#include <Adafruit_Sensor.h> 
#include <Adafruit_BMP280.h> 

#define BMP_SCK 13 // SCK ist an PIN 13 angeschlossen... 
#define BMP_MISO 12 
#define BMP_MOSI 11 
#define BMP_CS 10 

Adafruit_BMP280 bmp;

#include <LiquidCrystal.h> 
LiquidCrystal lcd(2,3,4,5,6,7,8);

// Zeitintervalle setzen
int TeilerSeriell = 5; // Serielle Ausgabe alle 2 min
int TeilerTendenz = 15;


// Timer setzen
unsigned long currentMillis;
unsigned long currentDisplayMillis;
unsigned long SeriellMillis;
long SeriellIntervall = 1000;
unsigned long DisplayMillis;
long DisplayIntervall = 3000;
unsigned long TendenzMillis;
long TendenzIntervall = 1000;


// Variablen für Werteerfassung und Umrechnung deklarieren
int MesswertHell = 0;
int EnergiestromWpM = 0;
float MaxTemp = -1000;
float MinTemp = 1000;
float Temperatur;
float Luftdruck;
float LuftdruckhPa;
float LuftdruckWGT;
String Wetterstatus = "leer";
String Tendenz = "leer";


//Variable für Ausgabemodus am Display
int modus = 1; // Startet bei 1
boolean writeOK = false;

// Variablen für Tendenz P-Zeit in Minuten
float Pnow;
float P15;
float P30;
float P45;
float P60; //1h
float P75;
float P90;
float P105;
float P120; //2h
float P135;
float P150;
float P165;
float P180; //3h

void setup() 
{ 
 //Serial.begin(9600); 
 rtc.begin();
 Wire.begin();
 lcd.begin(16,2); 
 bmp.begin();
 
 //rtc.adjust(DateTime(__DATE__, __TIME__)); // EINMALIG: kalibrieren der Uhr mit PC 

 
 // Druck für Tendenz Startwerte
 Luftdruck = bmp.readPressure(); // Erfasse Luftdruck
 LuftdruckhPa = Luftdruck / 100; // Luftdruck von Pa in hPa umrechnen
 LuftdruckWGT = (LuftdruckhPa + ( 485 / 8.5 )); // für 485 m ü. NN ( Weingarten) anpassen
 Pnow = LuftdruckWGT;
 P15 = LuftdruckWGT;
 P30 = LuftdruckWGT;
 P45 = LuftdruckWGT;
 P60 = LuftdruckWGT; //1h
 P75 = LuftdruckWGT;
 P90 = LuftdruckWGT;
 P105 = LuftdruckWGT;
 P120 = LuftdruckWGT; //2h
 P135 = LuftdruckWGT;
 P150 = LuftdruckWGT;
 P165 = LuftdruckWGT;
 P180 = LuftdruckWGT; //3h
 Tendenz = "Erhebe Daten";
 
 
 //Serial.println(F("Datum Zeit; Temperatur; Sonnenenergie/m2; Luftdruck; Druckdiff/3h; AktuellesWetter; Tendenz")); // Beschriftung nur ein Mal

}


void loop() 
{
 // Teil 1 Berechnung aller Werte...
 // 
 
 MesswertHell = analogRead(A1);
 EnergiestromWpM = map(MesswertHell, 0, 940, 8, 199); 
 Luftdruck = bmp.readPressure(); // Erfasse Luftdruck
 Temperatur = bmp.readTemperature(); // Erfasse Temperatur
 LuftdruckhPa = Luftdruck / 100; // Luftdruck von Pa in hPa umrechnen
 LuftdruckWGT = (LuftdruckhPa + ( 485 / 8.5 )); // für 485 m ü. NN ( Weingarten) anpassen
 
 if(Temperatur < MinTemp) {MinTemp = Temperatur;}
 if(Temperatur > MaxTemp) {MaxTemp = Temperatur;}
 
 // WetterAktuell:
 
 if ((LuftdruckWGT <= 980)&& (Temperatur >= 0)){Wetterstatus = "Sturm, Regen";}
 if ((LuftdruckWGT <= 980)&& (Temperatur < 0)){Wetterstatus = "Sturm, Schnee";}
 if ((LuftdruckWGT > 980) && (LuftdruckWGT <= 1000) && (Temperatur >= 0)) {Wetterstatus = "Regnerisch";} 
 if ((LuftdruckWGT > 980) && (LuftdruckWGT <= 1000) && (Temperatur < 0)) {Wetterstatus = "Schneeschauer";} 
 if ((LuftdruckWGT > 1000) && (LuftdruckWGT <= 1020)){Wetterstatus = "Wechselhaft";} 
 if ((LuftdruckWGT > 1020) && (LuftdruckWGT <= 1040)){Wetterstatus = "Sonnig,bestaendig";} 
 if (LuftdruckWGT > 1040){Wetterstatus = "Trocken,Gewitter";} 
 
 currentMillis = millis(); // Timer für jeden Durchgang setzten.
 DateTime aktuell = rtc.now();
 
 // Teil 2 Tendenzen berechen..
 //
 
 if ((aktuell.second() == 0 ) && (aktuell.minute()%TeilerTendenz == 0)) // Wenn die aktuelle Minute durch 15 teilbar ist, dann.... 
 {
 if(currentMillis - TendenzMillis > TendenzIntervall)
 { 
 // Register eins weiterschieben
 P180 = P165;
 P165 = P150;
 P150 = P135;
 P135 = P120;
 P120 = P105;
 P105 = P90;
 P90 = P75;
 P75 = P60;
 P60 = P45;
 P45 = P30;
 P30 = P15;
 P15 = Pnow;
 Pnow = LuftdruckWGT; 

 
 // Tendenz berechnen
 if( Pnow-P180 <= -8 ){Tendenz ="Sturm mit Hagel";} 
 if((Pnow-P180 <= -5 ) && (Pnow-P180 > -8 )){Tendenz ="Regen/Unwetter";} 
 if((Pnow-P180 <= -3 ) && (Pnow-P180 > -5 )){Tendenz ="Regnerisch";} 
 if((Pnow-P180 <= -1.3) && (Pnow-P180 > -3 )){Tendenz ="baldiger Regen";} 
 if((Pnow-P180 <= 1.3 ) && (Pnow-P180 > -1.3)){Tendenz ="gleichbleibend";} 
 if((Pnow-P180 <= 3 ) && (Pnow-P180 >= 1.3)){Tendenz ="lange Schoen";}
 if((Pnow-P180 <= 5 ) && (Pnow-P180 >= 3 )){Tendenz ="Schoen & labil";}
 if( Pnow-P180 > 5 ){Tendenz = "Sturmwarnung";}
 TendenzMillis = currentMillis;
 }
 } 
 
 
 // Teil 3 Ausgabe Dispaly

 if(currentMillis - DisplayMillis > DisplayIntervall)
 {
 if (modus >= 6) { modus = 1; }
 else
 {
 modus = modus + 1;
 }
 DisplayMillis = currentMillis;
 }
 
 switch(modus)
 {
 case 1: //Wetterstatus
 DisplayIntervall = 5000;
 lcd.setCursor(0,0); 
 lcd.print(F("Wetterntan: ")); 
 lcd.setCursor(0,1); 
 lcd.print(Wetterstatus); 
 lcd.print(" "); 
 break;
 
 case 2: //WetterTendenz
 DisplayIntervall = 5000;
 lcd.setCursor(0,0); 
 lcd.print(F("Wetterprognose; 
 lcd.setCursor(0,1); 
 lcd.print(Tendenz); 
 lcd.print(" "); 
 break; 

 case 3: // Datum und Uhrzeit
 DisplayIntervall = 3500;
 lcd.setCursor(0,0); 
 lcd.print(F("Datum 
 if (aktuell.day() < 10){lcd.print(F("0 
 lcd.print(aktuell.day());
 lcd.print("/");
 if (aktuell.month() < 10){lcd.print(F("0
 lcd.print(aktuell.month());
 lcd.print(F("/"));
 lcd.print(aktuell.year());
 lcd.print(F(" "));
 lcd.setCursor(0,1); 
 lcd.print(F("Zeit 
 if (aktuell.hour() < 10){lcd.print(F("0
 lcd.print(aktuell.hour());
 lcd.print(F(":"));
 if (aktuell.minute() < 10){lcd.print(F("0
 lcd.print(aktuell.minute());
 lcd.print(F(":"));
 if (aktuell.second() < 10){lcd.print(F("0 
 lcd.print(aktuell.second()); 
 lcd.print(F(" "));
 break;
 
 case 4: //Temperatur und Luftdruck
 DisplayIntervall = 3500;
 lcd.setCursor(0,0); 
 lcd.print(F("Temp)); 
 lcd.print(Temperatur,2); // NEU : ,1 heißt eine Nachkommazahl!
 lcd.print(F(" \337C "));
 lcd.setCursor(0,1); 
 lcd.print(F("Druck); 
 lcd.print(LuftdruckWGT,0);
 lcd.print(F(" hPa ")); 
 break;

 case 5: //Beleuchtung und Luftdrucktendez
 DisplayIntervall = 3500;
 lcd.setCursor(0,0); 
 lcd.print(F("Bel); 
 lcd.print(EnergiestromWpM); // NEU : ,1 heißt eine Nachkommazahl!
 lcd.print(F(" W/m2 "));
 lcd.setCursor(0,1); 
 lcd.print(F("dP); 
 lcd.print(Pnow -P180,1);
 lcd.print(F(" /3h ")); 
 break; 
 
 case 6: //Beleuchtung und Luftdrucktendez
 DisplayIntervall = 3500;
 lcd.setCursor(0,0); 
 lcd.print(F("T ")); 
 lcd.print(MinTemp); 
 lcd.print(F(" \337C "));
 lcd.setCursor(0,1); 
 lcd.print(F("T ")); 
 lcd.print(MaxTemp);
 lcd.print(F(" \337C ")); 
 break; 

 }
 
 
 // Teil 4 Ausgabe Serielle Schnittstelle

 if ((aktuell.second() == 0 ) && (aktuell.minute()%TeilerSeriell == 0))
 {
 if(currentMillis - SeriellMillis > SeriellIntervall)
 {
 if (aktuell.day() < 10){Serial.print(F("0 // EXCEL DATUM: "TT.MM.JJJJ HH:MM:SS"
 Serial.print(aktuell.day());
 Serial.print(F("."));
 if (aktuell.month() < 10){Serial.print(F("0
 Serial.print(aktuell.month());
 Serial.print(F("."));
 Serial.print(aktuell.year());
 Serial.print(F(" ")); 
 if (aktuell.hour() < 10){Serial.print(F("0
 Serial.print(aktuell.hour());
 Serial.print(F(":"));
 if (aktuell.minute() < 10){Serial.print(F("0
 Serial.print(aktuell.minute());
 Serial.print(F(":"));
 if (aktuell.second() < 10){Serial.print(F("0 
 Serial.print(aktuell.second());
 Serial.print(F(";")); // Trennzeichen für EXCEL nächster Datensatz
 Serial.print(Temperatur); 
 Serial.print(F(";")); // Trennzeichen für EXCEL nächster Datensatz 
 Serial.print(EnergiestromWpM); 
 Serial.print(F(";")); // Trennzeichen für EXCEL nächster Datensatz 
 Serial.print(LuftdruckWGT); 
 Serial.print(F(";")); // Trennzeichen für EXCEL nächster Datensatz 
 Serial.print(Pnow-P180); 
 Serial.print(F(";")); // Trennzeichen für EXCEL nächster Datensatz 
 Serial.print(Wetterstatus); 
 Serial.print(F(";")); // Trennzeichen für EXCEL nächster Datensatz 
 Serial.println(Tendenz); // println für neue Zeile 
 
 SeriellMillis = currentMillis;
 }
 }
 

 // Teil 6 LCD Refresh alle 10 min
 // weil sich das LCD manchmal hängt

 if ((aktuell.second() == 0 ) && (aktuell.minute()%10 == 0))
 {
 lcd.begin(16,2); 
 }

} //ENDE LOOP
