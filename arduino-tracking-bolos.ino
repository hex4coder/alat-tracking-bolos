#include <ESP8266HTTPClient.h>

#define triggerPin D6
#define echoPin D5
#define buzzer D2
#define batas 70

byte status = 0;
#include <ESP8266WiFi.h>

/*Put your SSID & Password*/
const char* ssid = "(^o^)";  // Enter SSID here
const char* password = "";   //Enter Password here

/* Set GET link with channel ID */
const char* _getLink = "http://192.168.43.93/tracking-bolos/tracking.php";

WiFiClient client;
HTTPClient http;


void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("Mengkoneksikan jaringan WiFi ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Terkoneksi dengan WiFi");
}
void loop() {
  long duration, jarak;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration / 2) / 29.1;

  // cek jarak
  if (jarak > 0 && jarak <= batas) {
    // ada objek terdeteksi dalam batas jarak
    if (status != 1) {
      status = 1;
      digitalWrite(buzzer, HIGH);

      // panggil kode server dengan alamat http://ipserver/tracking-bolos/tracking.php
      http.begin(client, _getLink);
      int httpCode = http.GET();
      String payload = http.getString();
      Serial.println(payload);
      http.end();
    }
  } else {
    // tidak ada objek terdeteksi dalam jarak batas
    if (status != 0) {
      status = 0;
      digitalWrite(buzzer, LOW);
    }
  }

  Serial.println("jarak :");
  Serial.print(jarak);
  Serial.println(" cm");
  delay(100);
}