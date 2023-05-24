/**
 * @file
 * @brief Proiect pentru citirea datelor de la senzorii DHT11 și BMP280 și afișarea acestora pe un LCD și prin conexiune Bluetooth.
 *
 * Acest proiect utilizează un Arduino și modulele DHT11, BMP280 și un modul Bluetooth HC-05 pentru a citi datele de temperatură, umiditate, presiune și altitudine. Aceste date sunt afișate pe un LCD și pot fi transmise prin Bluetooth către o aplicație mobilă.
 *
 * @note Este necesară instalarea librăriilor Adafruit_Sensor, Adafruit_BMP280, DHT și LiquidCrystal înainte de compilare.
 */

#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>

/**
 * @brief Pinul digital conectat la senzorul DHT11.
 *
 * Define-ul DHTPIN specifică numărul pinului digital la care este conectat senzorul DHT11.
 */
#define DHTPIN 7

/**
 * @brief Tipul de senzor DHT utilizat.
 *
 * Define-ul DHTTYPE specifică tipul de senzor DHT utilizat, în acest caz DHT11.
 */
#define DHTTYPE DHT11

/**
 * @brief Valoarea presiunii la nivelul mării în unități hPa.
 *
 * Define-ul SEALEVELPRESSURE_HPA specifică valoarea presiunii la nivelul mării în unități hectoPascali (hPa).
 * Această valoare este utilizată în calculul altitudinii în funcție de presiunea citită de la senzorul BMP280.
 */
#define SEALEVELPRESSURE_HPA (1013.25)


/**
 * @brief Obiect pentru comunicarea serială Bluetooth.
 *
 * Obiectul bluetooth de tip SoftwareSerial este utilizat pentru a realiza comunicarea cu modulul Bluetooth conectat.
 * Piniile TX și RX sunt specificate ca argumente în constructorul obiectului.
 */
SoftwareSerial bluetooth(8, 9);

/**
 * @brief Obiect pentru citirea datelor de la senzorul DHT11.
 *
 * Obiectul dht de tip DHT este utilizat pentru a citi temperatura și umiditatea de la senzorul DHT11.
 * Pinul digital conectat la senzor este specificat ca primul argument în constructorul obiectului,
 * iar tipul senzorului (DHT11) este specificat ca al doilea argument.
 */
DHT dht(DHTPIN, DHTTYPE);

/**
 * @brief Obiect pentru citirea datelor de la senzorul BMP280.
 *
 * Obiectul bmp de tip Adafruit_BMP280 este utilizat pentru a citi temperatura, presiunea și altitudinea de la senzorul BMP280.
 * Adresa I2C implicită a senzorului (0x76) este folosită în constructorul obiectului.
 */
Adafruit_BMP280 bmp;

/**
 * @brief Obiect pentru controlul LCD-ului.
 *
 * Obiectul lcd de tip LiquidCrystal este utilizat pentru a controla afișajul LCD conectat.
 * Pinii pentru controlul LCD (RS, E, D4, D5, D6, D7) sunt specificați ca argumente în constructorul obiectului.
 */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Variables
/**
 * @brief Variabila pentru temperatura.
 *
 * Variabila temperature stochează valoarea temperaturii citite de la senzorul BMP280.
 */
float temperature;

/**
 * @brief Variabila pentru umiditate.
 *
 * Variabila humidity stochează valoarea umidității citite de la senzorul DHT11.
 */
float humidity;

/**
 * @brief Variabila pentru presiune.
 *
 * Variabila pressure stochează valoarea presiunii citite de la senzorul BMP280.
 */
float pressure;

/**
 * @brief Variabila pentru altitudine.
 *
 * Variabila altitude stochează valoarea altitudinii calculate pe baza presiunii citite de la senzorul BMP280.
 */
float altitude;

/**
 * @brief Variabila pentru ciclarea afisarii.
 *
 * Variabila type este folosita pentru ciclarea intre modurile de afisare a diferitilor senzori.
 */
volatile bool type;

/**
 * @brief Variabila pentru retinerea comenzii curente.
 *
 * Variabila current_command este folosita pentru retinerea ultimii comenzi valide date de utilizator.
 */
String current_command;

/**
 * @brief Variabila pentru retinerea comenzii curente.
 *
 * Variabila commands este folosita pentru a determina comenzile valide pe care le poate da utilizatorul.
 */
String commands[4] = {"TEMPERATURE", "HUMIDITY", "PRESSURE", "ALTITUDE"};

/**
 * @brief Funcție pentru tratarea intreruperii Bluetooth.
 *
 * Această funcție este apelată atunci când se primește o comandă prin Bluetooth.
 * Verifică comanda primită și efectuează acțiunile corespunzătoare pentru fiecare comandă.
 */
void handleBTInterrupt()
{
    if (current_command == commands[0])
    {
        temperature = bmp.readTemperature();
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println("C");
        bluetooth.print("Temperature: ");
        bluetooth.print(temperature);
        bluetooth.println("C");
    }
    else if (current_command == commands[1])
    {
        humidity = dht.readHumidity();
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println("%");
        bluetooth.print("Humidity: ");
        bluetooth.print(humidity);
        bluetooth.println("%");
    }
    else if (current_command == commands[2])
    {
        pressure = bmp.readPressure() / 100.0F;
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println("hPa");
        bluetooth.print("Pressure: ");
        bluetooth.print(pressure);
        bluetooth.println("hPa");
    }
    else if (current_command == commands[3])
    {
        altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
        Serial.print("Altitude: ");
        Serial.print(altitude);
        Serial.println("m");
        bluetooth.print("Altitude: ");
        bluetooth.print(altitude);
        bluetooth.println("m");
    }
}

/**
 * @brief Inițializarea și configurarea componentelor.
 *
 * Această funcție se execută o singură dată la pornirea Arduino-ului.
 * Inițializează și configurează modulele DHT11, BMP280, LCD și modulul Bluetooth.
 * Setează pinii de intrare pentru citirea comenzilor Bluetooth.
 */
void setup()
{
    Serial.begin(9600);
    bluetooth.begin(9600);
    bmp.begin(0x76);
    lcd.begin(16, 2);
    dht.begin();

    attachInterrupt(digitalPinToInterrupt(8), handleBTInterrupt, CHANGE);

    current_command = "";
    type = false;
}

/**
 * @brief Afișarea temperaturii și umidității pe LCD.
 *
 * Această funcție afișează temperatura și umiditatea pe LCD timp de 3 secunde.
 */
void displayTemperatureAndHumidity()
{
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");

    delay(3000);
}

/**
 * @brief Afișarea presiunii și altitudinii pe LCD.
 *
 * Această funcție afișează presiunea și altitudinea pe LCD timp de 3 secunde.
 * După afișarea presiunii și altitudinii, se șterge ecranul LCD și se afișează temperatura.
 */
void displayPressureAndAltitude()
{
    temperature = bmp.readTemperature();
    pressure = bmp.readPressure() / 100.0F;
    altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Height: ");
    lcd.print(altitude);
    lcd.print("m");

    delay(3000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Pres: ");
    lcd.print(pressure);
    lcd.print("hPa");

    delay(3000);
}

/**
 * @brief Funcția principală a programului.
 *
 * Această funcție se execută într-un ciclu continuu.
 * Verifică dacă există date disponibile pe conexiunea Bluetooth.
 * Dacă este primită o comandă validă, se apelează funcția handleBTInterrupt().
 * Se alternează între afișarea temperaturii și umidității și afișarea presiunii și altitudinii pe LCD.
 */
void loop()
{
    if (bluetooth.available())
    {
        String data = bluetooth.readString();
        data.trim();
        for (int i = 0; i < 4; i++)
        {
            if (data == commands[i])
            {
                current_command = data;
                handleBTInterrupt();
                break;
            }
        }
    }

    lcd.clear();

    if (type == false)
    {
        displayTemperatureAndHumidity();
    }
    else
    {
        displayPressureAndAltitude();
    }

    type = !type;
}