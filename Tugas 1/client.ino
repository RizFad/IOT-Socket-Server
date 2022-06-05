#include <Arduino.h>
#include <ESP8266WiFi.h>

#define LED D4


const char *ssid = "Kel Sri"; // nama SSID untuk koneksi Anda
const char *password = "satu2tiga45"; // password akses point WIFI Anda
const uint16_t port = 88; // diganti dengan port serve Anda
const char *host = "192.168.1.7";//diganti dengan host server Anda, bisa ip ataupun domain

void connect_wifi()
{
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void connect_server()
{
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print("Lampu LED Menyala");

    Serial.println("[Response:]");
    String line = client.readStringUntil('\n');
    Serial.println(line);
    client.stop();
    Serial.println("\n[Disconnected]");
    digitalWrite(LED, HIGH);
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
    digitalWrite(LED, LOW);
  }
  delay(3000);
}

void setup()
{
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
  connect_wifi();
}

void loop()
{
  connect_server();
}
