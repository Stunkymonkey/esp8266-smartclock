/*
   read temperature from sensor
*/
void getTemperature() {
  if (ENABLE_SENSOR) {
    static unsigned long sensorPreviousMillis;
    unsigned long currentMillis = millis();
    if (currentMillis - sensorPreviousMillis >= SENSOR_INTERVAL || sensorPreviousMillis == 0) {
      sensorPreviousMillis = currentMillis;
      float temp;
      temp = dht.readTemperature();
      if (!isnan(temp)) {
        temperature = temp;
      } else {
        print("Failed to read temperature from DHT sensor!");
      }
      temp = dht.readHumidity();
      if (!isnan(temp)) {
        humidity = temp;
      } else {
        print("Failed to read humidity from DHT sensor!");
      }
      temp = dht.computeHeatIndex(temperature, humidity);
      if (!isnan(temp)) {
        heatindex = temp;
      } else {
        print("Failed to calculate heatindex!");
      }
    }
  }
}

/*
   calculate if it hits critical values => changes color
*/
int get_heat_index_level() {
  if (heatindex < 27) {
    return 0;
  } else if (heatindex >= 27 && heatindex < 32) {
    return 1;
  } else if (heatindex >= 32 && heatindex < 40) {
    return 2;
  } else if (heatindex >= 40 && heatindex < 54) {
    return 3;
  } else if (heatindex >= 54) {
    return 4;
  }
  return -1;
}

/*
   sending your data via json request
*/
void sendSensorData() {
  if (ENABLE_POST_SENSOR_DATA && !isAPMode) {
    static unsigned long postSensorPreviousMillis;
    unsigned long currentMillis = millis();
    if (currentMillis - postSensorPreviousMillis >= POST_SENSOR_INTERVAL || postSensorPreviousMillis == 0) {
      postSensorPreviousMillis = currentMillis;

      String message = "{\"temp\":\"" + String(temperature) + "\", \"humidity\":\"" + String(humidity) + "\"}";
      if (POST_SENSOR_DATA_ENDPOINT.startsWith("https")) {
        http.begin(*client_secure, POST_SENSOR_DATA_ENDPOINT);
      } else {
        http.begin(client, POST_SENSOR_DATA_ENDPOINT);
      }
      http.addHeader("Content-Type", "application/json");
      http.addHeader("token", POST_SENSOR_DATA_TOKEN);
      int httpCode = http.POST(message);
      if (httpCode > 0) {
        if (DEBUG && (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)) {
          http.writeToStream(&Serial);
          print("");
        }
      } else {
        Serial.printf("HTTP Sensor-POST failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    }
  }
}
