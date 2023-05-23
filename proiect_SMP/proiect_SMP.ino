#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define DHTPIN 7  // Replace with the digital pin connected to the DHT11 sensor
#define DHTTYPE DHT11
#define SEALEVELPRESSURE_HPA (1013.25)

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp; 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
	Serial.begin(9600);

  bmp.begin(0x76);
  delay(2000);

  lcd.begin(16, 2);
  delay(2000);
  dht.begin();
}

void loop() {
  // Read temperature and humidity from DHT11
  // float temperature = dht.readTemperature();
  // float humidity = dht.readHumidity();

  // // Display temperature and humidity on the LCD
  // lcd.setCursor(0, 0);
  // lcd.print("Temp: ");
  // lcd.print(temperature);
  // lcd.print(" C");

  // lcd.setCursor(0, 1);
  // lcd.print("Humidity: ");
  // lcd.print(humidity);
  // lcd.print(" %");

  	Serial.print("Temperature = ");
	Serial.print(bmp.readTemperature());
	Serial.println("*C");

	Serial.print("Pressure = ");
	Serial.print(bmp.readPressure() / 100.0F);
	Serial.println("hPa");

	Serial.print("Approx. Altitude = ");
	Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
	Serial.println("m");

	Serial.println();
	delay(1000);
}
