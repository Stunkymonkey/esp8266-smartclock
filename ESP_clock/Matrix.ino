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
    delay(500);
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
    return DEFAULT_LED_MATRIX_INTENSITY;
  }
}

void saveIntensity() {
  if (!auth()) { return; }
  String intensity_String = server.arg("intensity");
  int intensity = intensity_String.toInt();
  File f = SPIFFS.open(LED_CONFIG_PATH, "w");
  f.println(intensity);
  f.close();
  led_matrix_intensity = intensity;
  initMatrix(led_matrix_intensity);
  returnTo("/settings");
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

byte numbers[10][3] = {
  /*0*/{B11111110,B10000010,B11111110},
  /*1*/{B00001000,B00000100,B11111110},
  /*2*/{B11110010,B10010010,B10011110},
  /*3*/{B10010010,B10010010,B11111110},
  /*4*/{B00011110,B00010000,B11111110},
  /*5*/{B10011110,B10010010,B11110010},
  /*6*/{B11111110,B10010010,B11110010},
  /*7*/{B00000010,B00000010,B11111110},
  /*8*/{B11111110,B10010010,B11111110},
  /*9*/{B10011110,B10010010,B11111110}
};

/*draws array of for integers: 1_2_:_3_4*/
void drawNumber(int data[]) {
  int dis = 0;
  int row = 0;
  clearMatrix();
  /*each number to draw*/
  for(int i=0; i<sizeof(data); i++) {
    /*each row of the number*/
    for(int a=0; a<3; a++) {
        lc.setRow(dis, row, numbers[data[i]][a]);
        row++;
        /*works for me*/
        if(row==8) { row = 0; dis = dis+1; }
    }
    /*spacing*/
    lc.setRow(dis, row, B00000000);
    row++;
    if(row==8) { row = 0; dis = dis+1; }

    /*show : between 2 numbers: @todo better way*/
    if(i==1) {
          lc.setRow(dis, row, B00100100);
          row++;
          if(row==8) { row = 0; dis = dis+1; }
          lc.setRow(dis, row, B00000000);
          row++;
          if(row==8) { row = 0; dis = dis+1; }
    }
  }
}

/*splits up time string to numbers: buggy not finsished at all!*/
void drawTime(String time) {
  if (ENABLE_MATRIX) {
    int hour1 = time.charAt(0) - '0';
    int hour2 = time.charAt(1) - '0';
    int minute1 = time.charAt(3) - '0';
    int minute2 = time.charAt(4) - '0';
    int numbers[4] = { hour1, hour2, minute1, minute2 };
    drawNumber(numbers);
  }
}

