#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
// #include <SoftwareSerial.h>

// // PINS AND CONSTANTS
// #define DHTPIN 7  // Replace with the digital pin connected to the DHT11 sensor
// #define DHTTYPE DHT11
// #define SEALEVELPRESSURE_HPA (1013.25)

// // interfaces
// SoftwareSerial bluetooth(8, 9);  // Definește pini pentru conexiunea Bluetooth (TX, RX)
// DHT dht(DHTPIN, DHTTYPE);
// Adafruit_BMP280 bmp;
// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// // variables
// float temperature;
// float humidity;
// float pressure;
// float altitude;
// volatile bool type;
// String current_command;
// String commands[4] = {"TEMPERATURE", "HUMIDITY", "PRESSURE", "ALTITUDE"};

// void handleBTInterrupt() {
//   if(current_command == commands[0]) {
//     temperature = bmp.readTemperature();
//     Serial.print("Temperature: ");
//     Serial.print(temperature);
//     Serial.print("C");
//     bluetooth.print("Temperature: ");
//     bluetooth.print(temperature);
//     bluetooth.print("C");
//   }
//   else if(current_command == commands[1]) {
//     humidity = dht.readHumidity();
//     Serial.print("Humidity: ");
//     Serial.print(humidity);
//     Serial.print("%");
//     bluetooth.print("Humidity: ");
//     bluetooth.print(humidity);
//     bluetooth.print("%");
//   }
//   else if(current_command == commands[2]) {
//     pressure = bmp.readPressure() / 100.0F;
//     Serial.print("Pressure: ");
//     Serial.print(pressure);
//     Serial.print("hPa");
//     bluetooth.print("Pressure: ");
//     bluetooth.print(pressure);
//     bluetooth.print("hPa");
//   }
//   else if(current_command == commands[3]) {
//     temperature = bmp.readAltitude(SEALEVELPRESSURE_HPA);
//     Serial.print("Altitude: ");
//     Serial.print(altitude);
//     Serial.print("m");
//     bluetooth.print("Altitude: ");
//     bluetooth.print(altitude);
//     bluetooth.print("m");
//   }
// }

// void setup() {
//   Serial.begin(9600);
//   bluetooth.begin(9600);
//   bmp.begin(0x76);
//   lcd.begin(16, 2);
//   dht.begin();

//   attachInterrupt(digitalPinToInterrupt(8), handleBTInterrupt, CHANGE);

//   current_command = "";
//   type = 0;
// }


// void loop() {

//   // // reads bluetooth data
//   if(bluetooth.available())
//   {
//     String data = bluetooth.readString();
//     data.trim();
//     if(data == commands[0] 
//     || data == commands[1]
//     || data == commands[2]
//     || data == commands[3]
//     ) {
//       current_command = data;
//       handleBTInterrupt();
//     }

//   }

//   lcd.clear();

//   // Read temperature and humidity from DHT11
//   // Display temperature and humidity on the LCD
//   if (type == 0) {
//     temperature = dht.readTemperature();
//     humidity = dht.readHumidity();
//     lcd.setCursor(0, 0);
//     lcd.print("Temp: ");
//     lcd.print(temperature);
//     lcd.print("C");

//     lcd.setCursor(0, 1);
//     lcd.print("Humidity: ");
//     lcd.print(humidity);
//     lcd.print("%");

//     delay(3000);
//   } else {
//     temperature = bmp.readTemperature();
//     pressure = bmp.readPressure() / 100.0F;
//     altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
//     lcd.setCursor(0, 0);
//     lcd.print("Temp: ");
//     lcd.print(temperature);
//     lcd.print("C");

//     lcd.setCursor(0, 1);
//     lcd.print("Height: ");
//     lcd.print(altitude);
//     lcd.print("m");

//     delay(3000);
//     lcd.clear();

//     lcd.setCursor(0, 0);
//     lcd.print("Pres: ");
//     lcd.print(pressure);
//     lcd.print("hPa");

//     delay(3000);
//   }

//   type = !type;
// }
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>

// PINS AND CONSTANTS
#define DHTPIN 7  // Pinul digital conectat la senzorul DHT11
#define DHTTYPE DHT11
#define SEALEVELPRESSURE_HPA (1013.25)

// Interfaces
SoftwareSerial bluetooth(8, 9);  // Definește pini pentru conexiunea Bluetooth (TX, RX)
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Variables
float temperature;
float humidity;
float pressure;
float altitude;
volatile bool type;
String current_command;
String commands[4] = {"TEMPERATURE", "HUMIDITY", "PRESSURE", "ALTITUDE"};

void handleBTInterrupt() {
  if (current_command == commands[0]) {
    temperature = bmp.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("C");
    bluetooth.print("Temperature: ");
    bluetooth.print(temperature);
    bluetooth.println("C");
  }
  else if (current_command == commands[1]) {
    humidity = dht.readHumidity();
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    bluetooth.print("Humidity: ");
    bluetooth.print(humidity);
    bluetooth.println("%");
  }
  else if (current_command == commands[2]) {
    pressure = bmp.readPressure() / 100.0F;
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println("hPa");
    bluetooth.print("Pressure: ");
    bluetooth.print(pressure);
    bluetooth.println("hPa");
  }
  else if (current_command == commands[3]) {
    altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println("m");
    bluetooth.print("Altitude: ");
    bluetooth.print(altitude);
    bluetooth.println("m");
  }
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  bmp.begin(0x76);
  lcd.begin(16, 2);
  dht.begin();

  attachInterrupt(digitalPinToInterrupt(8), handleBTInterrupt, CHANGE);

  current_command = "";
  type = false;
}

void displayTemperatureAndHumidity() {
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

void displayPressureAndAltitude() {
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

void loop() {
  if (bluetooth.available()) {
    String data = bluetooth.readString();
    data.trim();
    for (int i = 0; i < 4; i++) {
      if (data == commands[i]) {
        current_command = data;
        handleBTInterrupt();
        break;
      }
    }
  }

  lcd.clear();

  if (type == false) {
    displayTemperatureAndHumidity();
  } else {
    displayPressureAndAltitude();
  }

  type = !type;
}