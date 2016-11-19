void gettemperature() {
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

