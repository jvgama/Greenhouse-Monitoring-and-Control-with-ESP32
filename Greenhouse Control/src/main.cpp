/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#define BLYNK_TEMPLATE_ID "TMPL2bvO4Lvmq"
#define BLYNK_TEMPLATE_NAME "Monitor de Temperatura"
#define BLYNK_AUTH_TOKEN "XlIODiiuw5dEEerisr_ubOBJZkRJlJCV"
#define LED_EMBUTIDO 2
#define DHTPIN 4 // pino 2 do sensor temp
#define DHTTYPE DHT22 // modelo do sensor temp


#include <Arduino.h>
#include "DHT.h" 
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>

char ssid[] = "Gama";
char pass[] = "123456789";


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5)
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Serial.print("\nTemperatura: ");
  Serial.print(t);
  Serial.print("\nUmidade: ");
  Serial.print(h);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void ligaLed(){
  digitalWrite(LED_EMBUTIDO, HIGH);
}

void desligaLed(){
  digitalWrite(LED_EMBUTIDO, LOW);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();
  pinMode(LED_EMBUTIDO, OUTPUT);
  // Setup a function to be called every second
  timer.setInterval(150L, sendSensor);
  timer.setInterval(500L, ligaLed);
  timer.setInterval(200L, desligaLed);
}

void loop()
{
    Blynk.run();
  timer.run();
}