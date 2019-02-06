//Wasserwaage
#define MOOVER 3                              // DigitalPIN3 zur Überwachung des Moverbetrieb ###### Hilfsweise ######
#define QUERACHSE A0                          // AnalogPIN1 zur Überwachung Querache
#define LAENGSACHSE A1                        // AnalogPIN1 zur Überwachung Längsache

//Menuetasten
#define MENUE 2                               // DigitalPIN2 ist Menuetaste
#define PLUS 3                                // DigitalPIN3 ist Plustaste
#define MINUS 4                               // DigitalPIN4 ist Minustaste

//Sprachausgabe
//#define MP3STEUERUNG 0                      //Digilpin 0 ist MP3 Steuerung
//#define MP3ANTWORT 1                        //Digitalpin 1 ist MP Antwort
//#define STUMM 13                            // DigitalPIN13 ist Stummschaltung MP3

void setup() 
{
Serial.begin (9600);                           // Start der seriellen Verbindung für den serial monitor.

pinMode(MOOVER, INPUT);                        // definiert Moover als Input

pinMode (MENUE, INPUT);                        // definiert Menuetaste als Input
pinMode (PLUS, INPUT);                         // definiert Plustaste als Input
pinMode (MINUS, INPUT);                        // definiert Minustaste als Input

//pinMode (STUMM, OUTPUT);                     // definiert Stumm als Output

Serial.println ("SMAR-avan 1.0");
delay (5000); 
}

void loop() 
{
  
if (digitalRead (MOOVER) == LOW)                // wenn Moover ausgeschaltet ist
    {Serial.println ("Wetterbericht");}         // schreibe Wetterbericht

                                                //########### Beginn Ausrichichtungsfunktion ################
    
else if (digitalRead (MOOVER) == HIGH)          // wenn Moover eingeschaltet ist
    {Serial.println ("Wohnwagen ausrichten");   // dann schreibe Wohnwagen ausrichten und
    delay (2000);                               // warte 2 Sekunden

    while (digitalRead (MOOVER) == HIGH)        // solange Moover eingeschaltet ist
    if (analogRead (QUERACHSE) > 330)           // wenn Auslesen der Querachse mehr als 330
    Serial.println ("linkes Rad auf");          // dann Ausgabe linkes Rad auf und
    else if (analogRead (QUERACHSE) < 329)      // sonst wenn Auslesen der Querachse weniger als 326
    Serial.println ("rechtes Rad auf");         // dann Ausgabe rechts Rad auf und
    else                                        // sonst
    Serial.println ("Querachse ausgerichtet");  // Ausgabe Querachse ausgerichtet

                                                //########### Wechsel Querachse zu Längsachse #####################
      
    while (digitalRead (MENUE) == LOW)          // solange Menü nicht gedrückt wird
    if (analogRead (LAENGSACHSE) > 330)         // wenn Auslesen der Längsachse mehr als 330
    Serial.println ("Stützrad auf");            // dann Ausgabe Stützrad auf und
    else if (analogRead (LAENGSACHSE) < 329)    // sonst wenn Auslesen der Längsachse weniger als 326
    Serial.println ("Stützradad ab");           // dann Ausgabe Stützrad ab und
    else                                        // sonst
    Serial.println ("Längsachse ausgerichtet"); // Ausgabe Längsachse ausgerichtet
    
    Serial.println ("Wohnwagen ausgerichtet");  // dann schreibe Wohnwagen ausgerichtet und
    delay (2000);                               // warte 2 Sekunden
    Serial.println ("Stützen absenken");        // dann schreibe Stützen absenken und
    delay (2000);}                              // warte 2 Sekunden

}                                               //########### Ende Ausrichtungsfunktion #####################
