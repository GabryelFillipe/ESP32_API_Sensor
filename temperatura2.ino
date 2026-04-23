#include <WiFi.h>
#include <HTTPClient.h>

#include "AdafruitIO_WiFi.h"
#include <DHT.h>

//-------------------- WIFI & ADAFRUIT --------------------
#define WIFI_SSID ""
#define WIFI_PASS ""

#define IO_USERNAME ""
#define IO_KEY ""

//-------------------- API RENDER --------------------
String serverName = "https://esp32-api-sensor.onrender.com/sensor";

// ----------------------------- DHT11 ------------------
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Configuração da conexão com Adafruit IO
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *temperaturaFeed = io.feed("temperatura");
AdafruitIO_Feed *umidadeFeed = io.feed("umidade");

// ------------------------- Variaveis -------------------------------
float ultimaTemp = -1000;
float ultimaUmidade = -1000;

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  Serial.print("Conectando ao Adafruit IO e WiFi");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  io.run();

  if (WiFi.status() == WL_CONNECTED) {

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("Erro ao ler o DHT11");
      delay(2000);
      return;
    }

    if (abs(temp - ultimaTemp) >= 0.2 || abs(hum - ultimaUmidade) >= 1.0) {
      
      Serial.print("\nTemperatura: ");
      Serial.print(temp);
      Serial.println(" °C");

      Serial.print("Umidade: ");
      Serial.print(hum);
      Serial.println(" %");

      Serial.println("-> Enviando para Adafruit IO...");
      temperaturaFeed->save(temp);
      umidadeFeed->save(hum);

      Serial.println("-> Enviando para API Render...");
      HTTPClient http;
      
      String serverPath = serverName + "?temp=" + String(temp) + "&hum=" + String(hum);
      http.begin(serverPath);

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("Código Render: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println("Resposta API: " + payload);
      } else {
        Serial.print("Erro HTTP Render: ");
        Serial.println(httpResponseCode);
      }

      http.end();

      ultimaTemp = temp;
      ultimaUmidade = hum;
    }
  } else {
    Serial.println("WiFi Desconectado. Aguardando reconexão do Adafruit...");
  }

  delay(3000);
}