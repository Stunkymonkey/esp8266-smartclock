
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
 * cosine wave
 */
void cosine_wave() {
  clearMatrix();
  if(DiscoTmp==32) {
    DiscoTmp = 0;
  }
  for(int k = 0; k<lc.getDeviceCount(); k++) {
    for(int i = 0; i<8; i++) {
      int x = (k*8)+i;
      int y = (int)(4*cos((float)x*0.3+DiscoTmp)+4+0.5);
      for(int a = 0; a<y; a++) {
        lc.setLed(k, a, 7-i, true);
      }
    }
  }
  DiscoTmp++;
}
