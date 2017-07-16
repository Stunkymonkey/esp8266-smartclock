void getWeatherInfo() {
  if(ENABLE_WEATHER) {
    unsigned long currentMillis = millis();
    if(currentMillis - postWeatherPreviousMillis >= GET_WEATHER_INTERVAL) {
      http.begin(WEATHER_URL);
      int httpCode = http.GET();
      if(httpCode > 0) {
        if(httpCode == HTTP_CODE_OK) {
          postSensorPreviousMillis = currentMillis;
          String payload = http.getString();
          String searchString = "\"icon\":\"";
          int index = 0;
          index = payload.indexOf(searchString);
          String weatherIcon = "";
          weatherIcon = payload.substring(index+searchString.length(),index+searchString.length()+3);
          weatherStatus = weatherIconToIndex(weatherIcon);
          print("weathericon: " + weatherStatus);
        }
      }
      http.end();
    }
  }
}

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

