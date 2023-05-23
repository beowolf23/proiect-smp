#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>

#define DHTPIN 7  // Replace with the digital pin connected to the DHT11 sensor
#define DHTTYPE DHT11
#define SEALEVELPRESSURE_HPA (1013.25)

// interfaces
SoftwareSerial bluetooth(8, 9);  // Definește pini pentru conexiunea Bluetooth (TX, RX)
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// variables
float temperature;
float humidity;
float pressure;
float altitude;
volatile bool type;
String current_command;
String commands[2] = {"C1", "C2"};

void handleBTInterrupt() {
  Serial.println(current_command);
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  bmp.begin(0x76);
  lcd.begin(16, 2);
  dht.begin();

  attachInterrupt(digitalPinToInterrupt(8), handleBTInterrupt, CHANGE);

  current_command = "";
  type = 0;
}


void loop() {

  // // reads bluetooth data
  if(bluetooth.available())
  {
    String data = bluetooth.readString();
    if(data.equals('C1')) {
      current_command = data;
      handleBTInterrupt();
      Serial.print("AM primit C1");
    }

    // Serial.print(data);
  }

  lcd.clear();

  // Read temperature and humidity from DHT11
  // Display temperature and humidity on the LCD
  if (type == 0) {
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
  } else {
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

  type = !type;


  // 	Serial.print("Temperature = ");
  // Serial.print(bmp.readTemperature());
  // Serial.println("*C");

  // Serial.print("Pressure = ");
  // Serial.print(bmp.readPressure() / 100.0F);
  // Serial.println("hPa");

  // Serial.print("Approx. Altitude = ");
  // Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  // Serial.println("m");

  // Serial.println();
  // delay(1000);
}
