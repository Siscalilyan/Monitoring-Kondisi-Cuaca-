#define BLYNK_TEMPLATE_ID "TMPL6aes9bhSo"
#define BLYNK_TEMPLATE_NAME "Weather"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SFE_BMP180.h>

// char auth[] = "NMgejQRP0IaPI4G30o0pSlsUi8Zny7S7"; // Masukkan token autentikasi Blynk Anda
char auth[] = "jhft-TtRLefaYjMCy_Ke2pfAePRBqY-S"; // Masukkan token autentikasi Blynk Anda
char ssid[] = "exe"; // Masukkan nama jaringan WiFi Anda
char pass[] = "22222222"; // Masukkan kata sandi WiFi Anda

#define DHTTYPE DHT22  // Tipe sensor DHT (DHT22)
// #define DHTPIN D1   // Pin yang digunakan untuk sensor DHT22
#define DHTPIN D3   // Pin yang digunakan untuk sensor DHT22

#define RAIN_PIN A0  // Pin yang digunakan untuk sensor hujan

SFE_BMP180 bmp;
double T, P;
char status;

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  dht.begin();

  timer.setInterval(1000L, sendSensorData);
}

void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int rainValue = analogRead(RAIN_PIN);
  rainValue = map(rainValue, 0, 1024, 0, 100);

  status =  bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);

    status = bmp.startPressure(3);// 0 to 3
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(P, T);
      if (status != 0) {
        Blynk.virtualWrite(V3, P);
      }
    }
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, rainValue);
}

void loop() {
  Blynk.run();
  timer.run();
}
