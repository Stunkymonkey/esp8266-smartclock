
/*
 * enable/disable the disco mode (default = off)
 */
void disco() {
  DiscoStatus = !DiscoStatus;
  clearMatrix();
  returnTo("/");
}

/*
 * draw the current disco-mode
 */
void draw_disco(){
  Disco_Functions[DiscoState]();
}

/*
 * go to next disco mode
 */
void disco_toggle() {
  DiscoState = (DiscoState + 1) % sizeof(Disco_Functions);
  clearMatrix();
  DiscoTmp = 0;
  returnTo("/");
}

/*
 * random pixel beeing displayed
 */
void random_pixel() {
  int addr = random(0, lc.getDeviceCount());
  int index = random(0, 8);
  int pos = random(0, 8);
  int state = random(0, 2);
  lc.setLed(addr, index, pos, state);
}


/*
 * waterworld
 */
 byte fishes[1][8] = {
  {B00010000,B00111000,B01000100,B01000100,B01000100,B00111000,B00010000,B00111000}, // medium fish
};
const long WATERWORLD_SPEED = 200;
 void waterworld() {
    static unsigned long lastWaterworldDraw;
    unsigned long currentDraw = millis();
    if(currentDraw - lastWaterworldDraw >= WATERWORLD_SPEED || lastWaterworldDraw == 0) {
      lastWaterworldDraw = currentDraw;
      clearMatrix();
      drawWaterWorld();
    }
  
 }

int iconPosition = lc.getDeviceCount() * 8;
void drawWaterWorld() {
    int amount = lc.getDeviceCount() * 8;
    int currentRow = 0;
    for(int k = 0; k<lc.getDeviceCount(); k++) {
      for(int i=0; i<8; i++) {
        currentRow++;
        //currentRow is in the iconPosition Window
        if(currentRow >= iconPosition && currentRow < (iconPosition+8)) {
          print(currentRow);
          int rowInIconPosition = currentRow - iconPosition;
          //displayColumn(k, i, fishes[0][rowInIconPosition]);
           displayRow(k, i, fishes[0][rowInIconPosition]);
        } 
      }
    }
    iconPosition = iconPosition-1;
    if (iconPosition < -8) iconPosition = lc.getDeviceCount() * 8;
}
