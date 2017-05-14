/* compare Time to valid Timers */
/* is currently checking for exact hours&minute. so this function should be called at least every minute */
void checkTimer(int hours, int minutes) {
    for(int i=0; i<(sizeof validTimersOn / sizeof validTimersOn[0]); i++) {
      if(hours == validTimersOn[i][0] && minutes == validTimersOn[i][1]) {
        SocketSwitch(validTimersOn[i][2], true);
      }
      else if(hours == validTimersOff[i][0] && minutes == validTimersOff[i][1]) {
        SocketSwitch(validTimersOff[i][2], false);
      }
    }
}

/* sockets already loaded and stored in: configSocketSets */
/* converts all settings to integers and validates them. This allows an efficient checkTimer function */
void initTimer() {
  for (int i = 0; i < (sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
    String socketSet[7] = configSocketSets[i];
    if (socketSet[5].length() == 5) {
      String hour = getValue(socketSet[5], ':', 0);
      String minute = getValue(socketSet[5], ':', 1);
      validTimersOn[i][0] = hour.toInt();
      validTimersOn[i][1] = minute.toInt();
      validTimersOn[i][2] = i;
    }
    if (socketSet[6].length() == 5) {
      String hour = getValue(socketSet[6], ':', 0);
      String minute = getValue(socketSet[6], ':', 1);
      validTimersOff[i][0] = hour.toInt();
      validTimersOff[i][1] = minute.toInt();
      validTimersOff[i][2] = i;
    }
  }
}

