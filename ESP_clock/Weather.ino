/*
 * get weather-icon/status from openweathermap.org
 */
void getWeatherInfo() {
  if(ENABLE_WEATHER && !isAPMode) {
    static unsigned long weatherPreviousMillis;
    unsigned long currentMillis = millis();
    if(currentMillis - weatherPreviousMillis >= GET_WEATHER_INTERVAL || weatherPreviousMillis == 0) {
      WiFiClient client;
      client.flush();
      const char* host = "api.openweathermap.org";
      if (!client.connect(host, 80)) {
        print("weather: connection failed");
        weatherPreviousMillis = currentMillis - (GET_WEATHER_INTERVAL/10);
        weatherStatus = weatherIconToIndex("not defined");
        return;
      }
      
      // We now create a URI for the request
      String url = "/data/2.5/forecast?q=";
      url += WEATHER_CITY;
      url += "&appid=";
      url += WEATHER_API_TOKEN;
      
      // This will send the request to the server
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n\r\n");
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 3000) {
          print("weather: Client Timeout !");
          weatherPreviousMillis = currentMillis - (GET_WEATHER_INTERVAL/10);
          weatherStatus = weatherIconToIndex("not defined");
          client.stop();
          return;
        }
      }
      
      String payload = "";
      if(client.available()){
        payload = client.readStringUntil('\n');
        // we want to get the second icon, because the first shows the weather right now
        payload = client.readStringUntil(']');
        payload = client.readStringUntil(']');
      }

      client.flush();
      client.stop();

      payload.trim();
      const String searchString = "\"icon\":\"";
      int index = 0;
      index = payload.indexOf(searchString);
      if (index != -1) {
        String weatherIcon = "";
        weatherIcon = payload.substring(index+searchString.length(),index+searchString.length()+3);
        weatherStatus = weatherIconToIndex(weatherIcon);
        print("Weather-Icon: " + String(weatherStatus));
      } else {
        print("Error: updating weather");
      }
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
