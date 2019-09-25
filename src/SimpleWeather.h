/*
  TFLStatus.cpp - Library to get Geolocation data from IPGeolocation.io
  Created by Dushyant Ahuja, September 2019.
  Released into the public domain.
*/

#ifndef SimpleWeather_h
#define SimpleWeather_h

#include "Arduino.h"
#include <ESP8266WiFi.h>


struct weatherData{
  String weather;
  String description;
  int id;
  int current_Temp;
  int min_temp;
  int max_temp;
  int humidity;
  int rain;
};

class OpenWeather
{
  public:
    OpenWeather(String Key, String City); 
    OpenWeather(String Key, String City, boolean forecast);
    void updateStatus(weatherData *w);
  private:
    String _Key;
    String _City;
    String _TimeZone;
    int _offset;
    String _Response;
    String _url;
    boolean _forecast;
};

#endif
