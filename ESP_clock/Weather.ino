/*
   get weather-icon/status from openweathermap.org
*/
void getWeatherInfo() {
  if (ENABLE_WEATHER && !isAPMode) {
    static unsigned long weatherPreviousMillis;
    unsigned long currentMillis = millis();
    if (currentMillis - weatherPreviousMillis >= GET_WEATHER_INTERVAL || weatherPreviousMillis == 0) {
      if (!http.begin(client, "http://api.openweathermap.org/data/2.5/forecast?q=" + WEATHER_CITY + "&appid=" + WEATHER_API_TOKEN)) {
        print("weather: connection failed");
        weatherPreviousMillis = currentMillis - (GET_WEATHER_INTERVAL / 10);
        weatherStatus = weatherIconToIndex("not defined");
        return;
      }

      int httpCode = http.GET();
      String weatherIcon;
      if (httpCode > 0 && (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)) {
        for (int i = 1; i <= WEATHER_3HOUR_FUTURE; i++) {
          weatherIcon = getHTTPValue(http, "icon", 3, 3);
          //print(String(i) + ":" + weatherIcon);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();

      weatherStatus = weatherIconToIndex(weatherIcon);
      print("Weather-Icon: " + String(weatherStatus));
      weatherPreviousMillis = currentMillis;
    }
  }
}

/*
   returns icon-index from weather status
*/
int weatherIconToIndex(String icon) {
  switch (icon[0]) {
    case '0':
      switch (icon[1]) {
        case '1': //clear
          switch (icon[2]) {
            case 'd': //clear sky day
              return 0;
              break;
            case 'n': //clear sky night
              return 1;
              break;
            default:    // ??
              return 12;
              break;
          }
        case '2': //few clouds
          switch (icon[2]) {
            case 'd': //few clouds day
              return 2;
              break;
            case 'n': //few clouds night
              return 3;
              break;
            default:    // ??
              return 12;
              break;
          }
        case '3': //scattered clouds day/night
          return 4;
          break;
        case '4': //broken clouds day/night
          return 5;
          break;
        case '9': //shower rain day/night
          return 6;
          break;
        default:    // ??
          return 12;
          break;
      }
    case '1':
      switch (icon[1]) {
        case '0': //rain
          switch (icon[2]) {
            case 'd': //rain day
              return 7;
              break;
            case 'n': //rain night
              return 8;
              break;
            default:    // ??
              return 12;
              break;
          }
        case '1': //thunderstorm day/night
          return 9;
          break;
        case '3': //snow day/night
          return 10;
          break;
        default:    // ??
          return 12;
          break;
      }
    case '5': //mist day/night
      return 11;
      break;
    default:    // ??
      return 12;
      break;
  }
}
