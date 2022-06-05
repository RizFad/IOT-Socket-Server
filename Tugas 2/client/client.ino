#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#define DHTTYPE DHT11
#define LAMPU1 D2  
#define LAMPU2 D4
#define PIN_LDR A0
#define trigger_pin D5 //6
#define Echo_pin D6 //5

long duration;
int distance;

DHT dht(D1, DHTTYPE);

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
    client.print("DATA ANALOG 4 VARIABEL");

    Serial.println("[Response:]");
    String line = client.readStringUntil('\n');
    Serial.println(line);
    client.stop();
    Serial.println("\n[Disconnected]");    
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();    
  }
  delay(3000);
}

void setup()
{  
  Serial.begin(115200);
  pinMode(Echo_pin, INPUT); 
  pinMode(trigger_pin, OUTPUT); 
  pinMode(LAMPU1, OUTPUT);
  pinMode(LAMPU2, OUTPUT);
  connect_wifi(); 
  Serial.print("DATA ANALOG 4 VARIABEL");
  dht.begin();
}

void dhtsensor(){
   delay(2000);
  int a = analogRead(A0);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  digitalWrite(trigger_pin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);          
  digitalWrite(trigger_pin, LOW); 
  duration = pulseIn(Echo_pin, HIGH);
  distance= duration*0.034/2;

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  Serial.print("Nilai Sensor Cahaya : ");
  Serial.print(a);
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (a > 50 && t > 17 && distance < 4) { 
    //  Hidupkan LED
        digitalWrite(LAMPU1, HIGH);
        digitalWrite(LAMPU2, LOW);
        delay(100);
//    //  Padamkan LED
//        digitalWrite(LAMPU1, LOW);
//        digitalWrite(LAMPU2, LOW);
//        delay(100);
    
    }
      else {
        digitalWrite(LAMPU2, HIGH);
        digitalWrite(LAMPU1, LOW);
    }
}

void loop()
{
  connect_server();  
  dhtsensor();
}
