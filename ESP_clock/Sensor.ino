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
}

