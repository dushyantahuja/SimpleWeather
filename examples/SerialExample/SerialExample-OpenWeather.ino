#include <ESP8266WiFi.h>
#include "SimpleWeather.h"


// Replace with your wifi credentials
const char * ssid = "********";
const char * password = "********";

// Get and API Key by registering on
// https://openweathermap.org/api



String Key = "********";

weatherData w;
OpenWeather weather(Key, "London,uk");


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed");
      while(1) {
          delay(1000);
      }
  }
}

void loop() {
  weather.updateStatus(&w);
  Serial.println();
  Serial.println(w.weather);
  Serial.println(w.description);
  Serial.println(w.id);
  Serial.println(w.current_Temp);
  Serial.println(w.min_temp);
  Serial.println(w.max_temp);
  Serial.println(w.humidity);
  Serial.println(w.rain);
  delay(600000);       // Wait for 600 seconds
  // put your main code here, to run repeatedly:
}