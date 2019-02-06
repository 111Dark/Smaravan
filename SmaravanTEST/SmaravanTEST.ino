//######### Wetterstation #########
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
unsigned long delayTime;

//########## Wasserwage ###########
#define MOOVER 3                              // DigitalPIN3 zur Überwachung des Moverbetrieb ###### Hilfsweise ######
#define QUERACHSE A0                          // AnalogPIN0 zur Überwachung Querache
#define LAENGSACHSE A1                        // AnalogPIN1 zur Überwachung Längsache

//########## Menuetasten/Grundfunktionen ################
#define MENUE 2                               // DigitalPIN2 ist Menuetaste
#define PLUS 3                                // DigitalPIN3 ist Plustaste
#define MINUS 4                               // DigitalPIN4 ist Minustaste

#define PIEZO 9                               // DigitalPIN9 ist Piezo

//############## Sprachausgabe ##########
//#define MP3STEUERUNG 0                      //Digilpin 0 ist MP3 Steuerung
//#define MP3ANTWORT 1                        //Digitalpin 1 ist MP Antwort
//#define STUMM 13                            // DigitalPIN13 ist Stummschaltung MP3

void setup() 
{
Serial.begin (9600);                           // Start der seriellen Verbindung für den serial monitor.

//########## Wetterstation #########
Serial.println(F("BME280 test"));
    bool status;
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("-- Default Test --");
    delayTime = 1000;
    Serial.println();

//######## Wasserwaage ###############
pinMode(MOOVER, INPUT);                        // definiert Moover als Input

//#########Menue############
pinMode (MENUE, INPUT);                        // definiert Menuetaste als Input
pinMode (PLUS, INPUT);                         // definiert Plustaste als Input
pinMode (MINUS, INPUT);                        // definiert Minustaste als Input

pinMode (PIEZO, OUTPUT);                       // definiert Piezo als Output

//########## Sprachausgabe ##############
//pinMode (STUMM, OUTPUT);                     // definiert Stumm als Output

Serial.println ("SMAR-avan 1.0");
delay (2000); 
}

void loop() 
{
  
if (digitalRead (MOOVER) == LOW)                // wenn Moover ausgeschaltet ist
    {printValues();                             // schreibe Wetterbericht
    delay(delayTime);}

                                                //########### Beginn Ausrichichtungsfunktion ################
    
else if (digitalRead (MOOVER) == HIGH)          // wenn Moover eingeschaltet ist
    {Serial.println ("Wohnwagen ausrichten");   // dann schreibe Wohnwagen ausrichten und
    analogWrite (PIEZO, 128);                   // dann Statuston 50% und
    delay (500);                                // warte 1 Sekunden und 
    digitalWrite (PIEZO, LOW);                  // schate Statuston ab

    while (digitalRead (MOOVER) == HIGH)        // solange Moover eingeschaltet ist
    if (analogRead (QUERACHSE) > 330)           // wenn Auslesen der Querachse mehr als 330
    {Serial.println ("linkes Rad auf");         // dann Ausgabe linkes Rad auf und
    analogWrite (PIEZO, 128);                   // dann Statuston 50% und
    delay (QUERACHSE*50);                       // warte Frequenz und 
    digitalWrite (PIEZO, LOW);                  // schate Statuston ab
    delay (QUERACHSE*50);}                      // warte Frequenz und
    else if (analogRead (QUERACHSE) < 326)      // sonst wenn Auslesen der Querachse weniger als 326
    {Serial.println ("rechtes Rad auf");        // dann Ausgabe rechts Rad auf und
    analogWrite (PIEZO, 128);                   // dann Statuston 50% und
    delay (QUERACHSE*10);                       // warte Frequenz und 
    digitalWrite (PIEZO, LOW);                  // schate Statuston ab
    delay (QUERACHSE*10);}                      // warte Frequenz und    
    else                                        // sonst
    Serial.println ("Querachse ausgerichtet");  // Ausgabe Querachse ausgerichtet

                                                //########### Wechsel Querachse zu Längsachse #####################
      
    while (digitalRead (MENUE) == LOW)          // solange Menü nicht gedrückt wird
    if (analogRead (LAENGSACHSE) > 330)         // wenn Auslesen der Längsachse mehr als 330
    {Serial.println ("Stützrad auf");           // dann Ausgabe Stützrad auf und
    analogWrite (PIEZO, 126);                   // dann Statuston 50% und
    delay (LAENGSACHSE*50);                     // warte Frequenz und 
    digitalWrite (PIEZO, LOW);                  // schate Statuston ab
    delay (LAENGSACHSE*50);}                    // warte Frequenz und
    else if (analogRead (LAENGSACHSE) < 329)    // sonst wenn Auslesen der Längsachse weniger als 326
    {Serial.println ("Stützrad ab");            // dann Ausgabe Stützrad ab und
    analogWrite (PIEZO, 128);                   // dann Statuston 50% und
    delay (LAENGSACHSE*10);                     // warte Frequenz und 
    digitalWrite (PIEZO, LOW);                  // schate Statuston ab
    delay (LAENGSACHSE*10);}                    // warte Frequenz und 
    else                                        // sonst
    Serial.println ("Längsachse ausgerichtet"); // Ausgabe Längsachse ausgerichtet
    
    Serial.println ("Wohnwagen ausgerichtet");  // dann schreibe Wohnwagen ausgerichtet und
    delay (2000);                               // warte 2 Sekunden
    Serial.println ("Stützen absenken");        // dann schreibe Stützen absenken und
    delay (2000);}                              // warte 2 Sekunden

}                                               //########### Ende Ausrichtungsfunktion #####################

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.print("Wetterbericht  ");
    if (bme.readPressure() < 1040)
    Serial.print("sonnig");
    else if(bme.readPressure() < 1000)
    Serial.print ("wechselhaft");
    else if(bme.readPressure() < 980)
    Serial.print ("regnerisch");    
    else if (bme.readPressure() > 980)
    Serial.println ("stürmisch"); 

    Serial.println ("Hier fehlt noch eine Prognose anhand des Luftdrucks!!!");
    
    Serial.println();
}
