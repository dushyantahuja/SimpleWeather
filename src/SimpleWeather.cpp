#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "SimpleWeather.h"

//const char fingerprint[] PROGMEM = "AC 06 70 3C 86 04 60 22 06 BE E5 11 A5 37 DB 7D 86 92 4E 1C"; // fingerprint

const size_t capacity = 2*JSON_ARRAY_SIZE(1) + 3*JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 3*JSON_OBJECT_SIZE(8) + 350;
const char *openweather = "api.openweathermap.org";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
WiFiClientSecure httpsClient;

OpenWeather::OpenWeather(String Key, String City){
  _Key = Key;
  _City = City;
  _forecast = false;
  _url = "/data/2.5/weather?q=" + _City + "&appid=" + _Key +"&units=metric";
}

OpenWeather::OpenWeather(String Key, String City, boolean forecast){
  _Key = Key;
  _City = City;
  _forecast = forecast;
  if(!forecast)
    _url = "/data/2.5/weather?q=" + _City + "&appid=" + _Key +"&units=metric&cnt=1";
  else
    _url = "/data/2.5/forecast?q=" + _City + "&appid=" + _Key +"&units=metric&cnt=1";
}

void OpenWeather::updateStatus(weatherData *w){
  //httpsClient.setFingerprint(fingerprint);
  httpsClient.setInsecure();
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);

  //Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(openweather, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }

  Serial.print("requesting URL: ");
  Serial.println(openweather+_url);

  httpsClient.print(String("GET ") + _url + " HTTP/1.1\r\n" +
               "Host: " + openweather + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      Serial.println(line);
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing, hence an extra read
  while(httpsClient.connected()){
    _Response = httpsClient.readString();
    Serial.println(_Response); //Print response
  }
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc,_Response);
  if(!_forecast){
    w->description = doc["weather"][0]["description"].as<String>();
    w->weather = doc["weather"][0]["main"].as<String>();
    w->id = doc["weather"][0]["id"].as<int>();
    w->current_Temp = doc["main"]["temp"].as<int>();
    w->min_temp = doc["main"]["temp_min"].as<int>();
    w->max_temp = doc["main"]["temp_max"].as<int>();
    w->humidity = doc["main"]["temp_max"].as<int>();
    if(w->id <700) w->rain = doc["rain"]["1h"].as<int>();
    else w->rain = 0;
  } else
  {
    w->description = doc["list"][0]["weather"]["description"].as<String>();
    w->weather = doc["list"][0]["weather"]["main"].as<String>();
    w->id = doc["list"][0]["weather"]["id"].as<int>();
    w->current_Temp = doc["list"][0]["main"]["temp"].as<int>();
    w->min_temp = doc["list"][0]["main"]["temp_min"].as<int>();
    w->max_temp = doc["list"][0]["main"]["temp_max"].as<int>();
    w->humidity = doc["list"][0]["main"]["temp_max"].as<int>();
    if(w->id <700) w->rain = doc["rain"]["1h"].as<int>();
    else w->rain = 0;
  }
}


// Replace with your wifi credentials
const char * ssid = "DUSHYANT";
const char * password = "ahuja987";

// Get and API Key by registering on
// https://openweathermap.org/api



String Key = "15121d64b58b9086439fed7f00050c04";

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