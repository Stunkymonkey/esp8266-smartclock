/*
   checks if we are in summer time
   source: https://github.com/gmag11/NtpClient/blob/master/src/NTPClientLib.cpp
*/

bool isSummerTime(int hour, int day, int month, int year, int tzHours) {
  if ((month < 3) || (month > 10)) return false; // no summertime in Jan, Feb, Nov, Dez
  if ((month > 3) && (month < 10)) return true; // summertime in Apr, May, Jun, Jul, Aug, Sep
  if ((month == 3 && ((hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7)))) || ((month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7)))))
    return true;
  else
    return false;
}

/*
   returns unixtime-stmap with DST correction
*/
time_t UnixStamp() {
  int offset = NTP_TIMEZONE;
  if (NTP_DST) {
    if (timeInit && inSummerTime(hour(), day(), month(), year())) {
      offset += 1;
    }
  }
  return timeClient.getEpochTime() + (offset * SECS_PER_HOUR);
}
