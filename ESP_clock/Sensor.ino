void gettemperature() {
  if (ENABLE_SENSOR) {
    unsigned long currentMillis = millis();
    if(currentMillis - sensorPreviousMillis >= SENSOR_INTERVAL) {
      sensorPreviousMillis = currentMillis;
      temperature = dht.readTemperature();
      humidity = dht.readHumidity();
      heatindex = dht.computeHeatIndex(temperature, humidity);
      if (isnan(temperature) || isnan(humidity) || isnan(heatindex)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
      if(ENABLE_POST_SENSOR_DATA) {
        char celsiusTemp[7];
        char humidityTemp[7];
        dtostrf(temperature, 6, 2, celsiusTemp);
        dtostrf(humidity, 6, 2, humidityTemp);
        String tstr(temperature);
        String hstr(humidity);
        String postMessage = "{\"tmp\":\""+ tstr + "\", \"humidity\":\"" + hstr + "\"}";
        sendSensorData(postMessage);
      }
    }
  }
}

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

void sendSensorData(String message) {
  http.begin(POST_SENSOR_DATA_ENDPOINT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("token", POST_SENSOR_DATA_TOKEN);
  int httpCode = http.POST(message);
  if(DEBUG) {
    Serial.print("http result:");
    Serial.println(httpCode);
    http.writeToStream(&Serial); 
  }
  http.end();
}

