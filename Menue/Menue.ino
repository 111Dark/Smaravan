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
float Gasflasche;

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
Gasflasche = 10;

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
if (digitalRead (MENUE) == HIGH)
delay (1000);

if (digitalRead (MENUE) == LOW)               
Serial.println ("Füllstände vorlesen");

else if (digitalRead (MENUE) == HIGH)
{for 100 Serial.print ("Gasflasche ");
Serial.print (Gasflasche);
Serial.println (" l");}

}
