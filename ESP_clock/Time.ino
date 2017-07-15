int dayOfWeek(int d, int m, int y)    /* 1 <= m <= 12,  y > 1752*/
{
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  // 0 Sunday, 1 Monday, etc...
  return (y + y/4 - y/100 + y/400 + t[m-1] + d + 6) % 7;
}

bool inSummerTime(int hours, int day, int month, int year) {
  if (day<0 || month<0 || year<0) return false;
  if ((month>=3)&&(month<=10)) { // March to October inclusive
    if ((month>3)&&(month<10)) { // Definitely
       return true;
    }
    if (month==3) { // Have we passed the last Sunday of March, 1am UT ?
      if (day>=25) { // It's getting interesting
       int dw = dayOfWeek(day,month,year);
       // When is the next sunday ?
       int dts = 6-dw; // Number of days before the next sunday
       if (dts==0) dts=7; // We are really speaking of the next sunday, not the current one
       if ((day+dts)>31) { // The next sunday is next month !
        if (dw!=6 || hours>0) // We finally check that we are not on the day of the change before the time of the change
         return true;
       }
      }
    }
    if (month==10) { // Have we passed the last Sunday of October 1am UT ?
      if (day>=25) { // It's getting interesting
       int dw = dayOfWeek(day,month,year);
       // When is the next sunday ?
       int dts = 6-dw; // Number of days before the next sunday
       if (dts==0) dts=7; // We are really speaking of the next sunday, not the current one
       if ((day+dts)>31) { // The next sunday is next month !
        if (dw!=6 || hours>0) // We finally check that we are not on the day of the change before the time of the change
         return false; // We have passed the change
        else return true;
       } else return true;
      } else return true;
    }
  }
  return false;
}

void ajdustSummerTime() {
  unsigned long time = timeClient.getEpochTime();
  // TODO
  // extrakt exact year, month, date and hour and pass it to inSummerTime()
  // maybe use the timelib
  // if it is true then setTimeOffset(int timeOffset) to 7200
  // if false to 3600
}

