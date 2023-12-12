/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#define BLYNK_TEMPLATE_ID "TMPL2bvO4Lvmq"
#define BLYNK_TEMPLATE_NAME "Monitor de Temperatura"
#define BLYNK_AUTH_TOKEN "XlIODiiuw5dEEerisr_ubOBJZkRJlJCV"
#define LED_EMBUTIDO 2 // onboard led, used as status indicator
#define LED_OUTPUT 13 // external led, used for lighting
#define PUMP 26 // water pump output
#define FAN_OUTPUT 27 // external fan control
#define LDR 34 // analog input from LDR
#define MOISTURE 35 // soil moisture sensor
#define DHTPIN 4 // temperature sensor pin 2
#define DHTTYPE DHT22 // temperature sensor model


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

int LDR_Val = 0;

int SoilMoisture_Val = 0;
int SoilMoisture_Pct = 0;
int SoilDry = 4095;
int SoilWet = 2200;
int SoilDryPct = 0;
int SoilWetPct = 100;

void ligaLed(){
  digitalWrite(LED_EMBUTIDO, HIGH);
}

void desligaLed(){
  digitalWrite(LED_EMBUTIDO, LOW);
}

void sendSensor()
{
  // temperature and humidity values
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("\nTemperatura: ");
  Serial.print(t);
  Serial.print("\nUmidade: ");
  Serial.print(h);

  // Cooling fan
  if (t>30){
    digitalWrite(FAN_OUTPUT, LOW);
    Serial.print("\nFan on");
  }
  else{
    digitalWrite(FAN_OUTPUT, HIGH);
    Serial.print("\nFan off");
  }

  // LDR
  LDR_Val = analogRead(LDR);

  Serial.print("\nSaida do LDR: ");
  Serial.print(LDR_Val);

  // Lighting LEDs
  if(LDR_Val>50){ 
    digitalWrite(LED_OUTPUT, HIGH);
    Serial.print("\nLED off");
  }
  else{
    digitalWrite(LED_OUTPUT, LOW);
    Serial.print("\nLED on");
  }

  // Soil Moisture Sensor
  SoilMoisture_Val = analogRead(MOISTURE);
  SoilMoisture_Pct = map(SoilMoisture_Val, SoilWet, SoilDry, SoilWetPct, SoilDryPct);
  Serial.print("Umidade do solo: ");
  Serial.print(SoilMoisture_Pct);
  Serial.print("%");



  // writing read values to Blynk platform
  Blynk.virtualWrite(V2, SoilMoisture_Pct);
  Blynk.virtualWrite(V4, LDR_Val);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
    
}

  // Water pump activation
BLYNK_WRITE(V3){
    if(param.asInt()){
      digitalWrite(PUMP,LOW);
    }
    else{
      digitalWrite(PUMP,HIGH);
    }
  }


void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // blynk platform login

  dht.begin(); // dht sensor startup

  digitalWrite(PUMP,LOW);
  
  pinMode(LED_EMBUTIDO, OUTPUT);
  pinMode(LED_OUTPUT,OUTPUT);
  pinMode(FAN_OUTPUT, OUTPUT);
  pinMode(PUMP, OUTPUT);
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