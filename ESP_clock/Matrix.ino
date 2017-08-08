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

void displayRow(int addr, int line, byte value) {
  switch (LED_MATRIX_DIRECTION) {
    case 0:
      lc.setRow(addr, line, value);
      break;
    case 1:
      lc.setColumn(addr, 7-line, value);
      break;
    case 2:
      lc.setRow(addr, 7-line, value);
      break;
    case 3:
      lc.setColumn(addr, line, value);
      break;
    default:
      print("ERROR: wrong direction of leds set.");
    break;
  }
}

void displayColumn(int addr, int line, byte value) {
  switch (LED_MATRIX_DIRECTION) {
    case 0:
      lc.setColumn(addr, line, value);
      break;
    case 1:
      lc.setRow(addr, 7-line, value);
      break;
    case 2:
      lc.setColumn(addr, 7-line, value);
      break;
    case 3:
      lc.setRow(addr, line, value);
      break;
    default:
      print("ERROR: wrong direction of leds set.");
    break;
  }
}

void displayLed(int addr, int row, int col, boolean state){
  switch (LED_MATRIX_DIRECTION) {
    case 0:
      lc.setLed(addr, row, col, state);
      break;
    case 1:
      lc.setLed(addr, col, 7-row, state);
      break;
    case 2:
      lc.setLed(addr, 7-row, 7-col, state);
      break;
    case 3:
      lc.setLed(addr, 7-col, row, state);
      break;
    default:
      print("ERROR: wrong direction of leds set.");
    break;
  }
}

//add progress in percent
void setProgress(float progress) {
  int amount = lc.getDeviceCount() * 8;
  int amountOfOn = progress * amount;
  for(int k = 0; k<lc.getDeviceCount(); k++) {
    for(int i = 0; i<amountOfOn; i++) {
      displayRow(k, i, B11111111);
    }
    amountOfOn = amountOfOn - 8;
  }
  if (progress == 1.0) {
    delay(200);
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
    return LED_MATRIX_INTENSITY;
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

void setMatrixStatus(boolean newStatus) {
  MatrixStatus = newStatus;
  if (!newStatus) {
    clearMatrix();
  }
  returnTo("/");
}

void yolo() {
  for(int k = 0; k<lc.getDeviceCount(); k++) {
    for(int i = 0; i<8; i++) {
      displayRow(k, i, B11111111);
      delay(200);
    }
  }
  for(int k = 0; k<lc.getDeviceCount(); k++) {
    for(int i = 0; i<8; i++) {
      displayRow(k, i, B00000000);
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


/* should be written again */
void drawSecondsGraph() {
  //scale seconds from 0 to 7
  int secPixels = second() / 8;
  // loop through last line of display
  for(int i = 0; i<8; i++) {
    if(i<=secPixels) {
      displayLed(LED_MATRIX_PORT_AMOUNT-2, 4, i, true);
    }
    else {
      displayLed(LED_MATRIX_PORT_AMOUNT-2, 4, i, false);
    }
  }
}

/*
 * draws 1 pixel wide spacing
 */
void drawSpacing(int row, int dis) {
  displayRow(dis, row, B00000000);
}

/*
 * draws 1 pixel wide seperator
 */
void drawSeperator(int row, int dis) {
  displayRow(dis, row, B00101000);
}

/*
 * draws 3 pixel number from numbers data
 */
void drawNumber(int number, int row, int dis) {
    /*each row of the number*/
    for(int a=0; a<3; a++) {
        displayRow(dis, row, numbers[number][a]);
        row++;
        /*works for me*/
        if(row==8) { row = 0; dis = dis+1; }
    }
}

/*
 * draws seperator with 2 leds
 */
void matrixBlinkSeperator() {
  unsigned long currentMillis = millis();
  if(currentMillis - matrixBlinkPreviousMillis >= 1000) {
    matrixBlinkPreviousMillis = currentMillis;
    seperatorBlink = !seperatorBlink;
  }
  if (!seperatorBlink) {
    drawSeperator(0, 1);
  } else {
    drawSpacing(0, 1);
  }
}

byte weather[13][8] = {
  {B10000001,B01011010,B00111100,B01111110,B01111110,B00111100,B01011010,B10000001},//clear sky day
  {B01100000,B00011000,B00001100,B00001110,B00001110,B00001100,B00011000,B01100000},//clear sky night
  {B00000000,B00011100,B01100010,B10000001,B10000001,B01111110,B00000000,B00000000},//few clouds day
  {B11111111,B11100011,B10011101,B01111110,B01111110,B10000001,B11111111,B11111111},//few clouds night
  {B00000000,B00011100,B01100010,B10001001,B11111111,B01111110,B00000000,B00000000},//scattered clouds day/night
  {B00011100,B01111110,B11111111,B11111111,B01111110,B00000000,B00000000,B00000000},//broken clouds day/night
  {B00011100,B01111110,B11111111,B11111111,B01111110,B00001000,B01010010,B10000100},//shower rain day/night       TODO
  {B00011100,B01111110,B11111111,B11111111,B01111110,B10001001,B00100000,B10000101},//rain day                    TODO
  {B00011100,B01111110,B11111111,B11111111,B01111110,B10001001,B00100000,B10000101},//rain night                  TODO
  {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000},//thunderstorm day/night      TODO
  {B01000010,B00100100,B10011001,B01111110,B10011001,B00100100,B01000010,B00000000},//snow day/night
  {B01010101,B10101010,B01010101,B10101010,B01010101,B10101010,B01010101,B10101010},//mist day/night
  {B00111000,B01000100,B01000100,B00100000,B00010000,B00010000,B00000000,B00010000}//??
};

void drawWeather(int number, int addr) {
  if (number == -1) {
    return;
  }
  /*each row of the number*/
  for(int a=0; a<8; a++) {
    displayColumn(addr, a, weather[number][a]);
  }
}

/*splits up time string to numbers: buggy not finsished at all!*/
void drawTime() {
  int hour1 = hour() / 10;
  int hour2 = hour() % 10;
  int minute1 = minute() / 10;
  int minute2 = minute() % 10;
  // each number is 3 pixel wide, wich adds up to 4 pixels including spacing
  drawNumber(hour1, 0, 0);
  drawSpacing(3, 0);
  drawNumber(hour2, 4, 0);
  drawSpacing(7, 0);
  matrixBlinkSeperator();
  drawSpacing(1, 1);
  drawNumber(minute1, 2, 1);
  drawSpacing(5, 1);
  drawNumber(minute2, 6, 1);
}

