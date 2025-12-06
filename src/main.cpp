// --- KREDENSIAL BLYNK & WIFI ---
#define BLYNK_TEMPLATE_ID "TMPL6votdTynk"
#define BLYNK_TEMPLATE_NAME "Monitoring Kualitas Lingkungan"
#define BLYNK_AUTH_TOKEN "3UfXQeBXbURlW04h1Lu-vtRVne4fkaSK"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

char ssid[] = "TSRD Family";
char pass[] = "TSRD22!!";

// --- DEFINISI PIN ---
#define DHTPIN 23
#define DHTTYPE DHT11

// --- OBJEK SENSOR ---
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
BlynkTimer timer;

// --- FUNGSI PEMBACAAN DATA ---
void sendSensor() {
  // 1. Baca DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // 2. Baca BH1750
  float lux = lightMeter.readLightLevel();

  // Validasi data DHT
  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca sensor DHT!");
  } else {
    // Kirim ke Blynk
    Blynk.virtualWrite(V0, t); // Suhu
    Blynk.virtualWrite(V1, h); // Kelembapan
    
    // Debugging Serial Monitor
    Serial.print("Suhu: "); Serial.print(t); Serial.print("C | ");
    Serial.print("Kelembapan: "); Serial.print(h); Serial.print("% | ");
  }

  // Validasi data BH1750
  if (lux >= 0) {
    Blynk.virtualWrite(V2, lux); // Cahaya
    Serial.print("Cahaya: "); Serial.print(lux); Serial.println(" lx");
  }
}

void setup() {
  Serial.begin(115200);

  // Inisialisasi I2C (SDA=21, SCL=22)
  Wire.begin();

  // Mulai Sensor
  dht.begin();
  if (lightMeter.begin()) {
    Serial.println("BH1750 siap!");
  } else {
    Serial.println("BH1750 error - Cek kabel!");
  }

  // Koneksi ke Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Atur interval pengiriman data setiap 1 detik (1000L)
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}