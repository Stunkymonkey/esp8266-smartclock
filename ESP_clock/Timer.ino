/* compare Time to valid Timers */
void checkTimer(int hours, int minutes) {
    for(int i=0; i<(sizeof validTimersOn / sizeof validTimersOn[0]); i++) {
      int onTimers[3] = validTimersOn[i];
      if(hours == onTimers[0] && minutes == onTimers[1]) {
        SocketSwitch(onTimers[2], true);
      }
      int offTimers[3] = validTimersOff[i];
      if(hours == offTimers[0] && minutes == offTimers[1]) {
        SocketSwitch(offTimers[2], false);
      }
      memset(onTimers,0,sizeof(onTimers));
      memset(offTimers,0,sizeof(offTimers));
    }
}

/* initalize Timer */
/* sockets already loaded and stored in: configSocketSets */
void initTimer() {
  for (int i = 0; i < (sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
    String socketSet[7] = configSocketSets[i];
    if (socketSet[5].length() == 5) {
      String hour = getValue(socketSet[5], ':', 0);
      String minute = getValue(socketSet[5], ':', 1);
      validTimersOn[i][0] = hour.toInt();
      validTimersOn[i][1] = minute.toInt();
      validTimersOn[i][2] = socketSet[0].toInt();
    }
    if (socketSet[6].length() == 5) {
      String hour = getValue(socketSet[6], ':', 0);
      String minute = getValue(socketSet[6], ':', 1);
      validTimersOff[i][0] = hour.toInt();
      validTimersOff[i][1] = minute.toInt();
      validTimersOff[i][2] = socketSet[0].toInt();
    }
  }
}

