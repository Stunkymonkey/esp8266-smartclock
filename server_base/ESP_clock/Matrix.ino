// init all matrixes
void initMatrix(int k) {
  for (int i = 0; i < lc.getDeviceCount(); i++) {
    // Wake up display
    lc.shutdown(i, false);
    // Set intensity levels
    lc.setIntensity(i,k);
    // Clear Displays
    lc.clearDisplay(i);
  }
}

//add progress in percent
void setProgress(float progress) {
  int amount = lc.getDeviceCount() * 8;
  int amountOfOn = progress * amount;
  for(int k = 0; k<lc.getDeviceCount(); k++) {
    for(int i = 0; i<amountOfOn; i++) {
      lc.setRow(k, i, B11111111);
    }
    amountOfOn = amountOfOn - 8;
  }
  if (progress == 1.0) {
    delay(1000);
    clearMatrix();
  }
}


// clear all matrixes
void clearMatrix() {
  for (int i = 0; i < lc.getDeviceCount(); i++) {
    lc.clearDisplay(i);
  }
}

int loadIntensity() {
  File configFile = SPIFFS.open(LED_CONFIG_PATH, "r");
  if (configFile) {
    String intensity;
    while (configFile.available()) {
      intensity = configFile.readStringUntil('\n');
    }
    intensity.trim();
    configFile.close();
    return intensity.toInt();
  } else {
    configFile.close();
    return led_matrix_intensity;
  }
}

void saveIntensity() {
  String intensity_String = server.arg("intensity");
  int intensity = intensity_String.toInt();
  File f = SPIFFS.open(LED_CONFIG_PATH, "w");
  f.println(intensity);
  f.close();
  led_matrix_intensity = intensity;
  sendResponse("Saved!");
  initMatrix(led_matrix_intensity);
}

void yolo() {
  for(int k = 0; k<lc.getDeviceCount(); k++) {
    for(int i = 0; i<8; i++) {
      lc.setRow(k, i, B11111111);
      delay(200);
    }
  }
  for(int k = 0; k<lc.getDeviceCount(); k++) {
    for(int i = 0; i<8; i++) {
      lc.setRow(k, i, B00000000);
      delay(200);
    }
  }
}

