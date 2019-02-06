
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
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
    delayTime = (6000);

    Serial.println();
}


void loop() 
{ 

    Serial.print("Temperatur  ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Luftdruck  ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Luftfeuchtigkeit  ");
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

    Serial.println(" ");
    Serial.println(" ");
    delay(delayTime);
}
