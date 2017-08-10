bool inSummerTime(int hours, int currentDay, int currentMonth, int currentYear) {
  if (currentDay<0 || currentMonth<0 || currentYear<0) return false;
  if ((currentMonth>=3)&&(currentMonth<=10)) { // March to October inclusive
    if ((currentMonth>3)&&(currentMonth<10)) { // Definitely
       return true;
    }
    if (currentMonth==3) { // Have we passed the last SuncurrentDay of March, 1am UT ?
      if (currentDay>=25) { // It's getting interesting
       int dw = dayOfWeek(now())-1;
       // When is the next suncurrentDay ?
       int dts = 6-dw; // Number of currentDays before the next suncurrentDay
       if (dts==0) dts=7; // We are really speaking of the next suncurrentDay, not the current one
       if ((currentDay+dts)>31) { // The next suncurrentDay is next currentMonth !
        if (dw!=6 || hours>0) // We finally check that we are not on the currentDay of the change before the time of the change
         return true;
       }
      }
    }
    if (currentMonth==10) { // Have we passed the last SuncurrentDay of October 1am UT ?
      if (currentDay>=25) { // It's getting interesting
       int dw = dayOfWeek(now())-1;
       // When is the next suncurrentDay ?
       int dts = 6-dw; // Number of currentDays before the next suncurrentDay
       if (dts==0) dts=7; // We are really speaking of the next suncurrentDay, not the current one
       if ((currentDay+dts)>31) { // The next suncurrentDay is next currentMonth !
        if (dw!=6 || hours>0) // We finally check that we are not on the currentDay of the change before the time of the change
         return false; // We have passed the change
        else return true;
       } else return true;
      } else return true;
    }
  }
  return false;
}

void ajdustSummerTime() {
  bool isNewSummerTime = inSummerTime(hour(), day(), month(), year());
  if ( isNewSummerTime && !isSummerTime) {
    setTime(UnixStamp());
    adjustTime(3600);
    isSummerTime = true;
  } else if (!isNewSummerTime && isSummerTime){
    setTime(UnixStamp());
    isSummerTime = false;
  }
}

time_t UnixStamp() {
  return timeClient.getEpochTime();
}

