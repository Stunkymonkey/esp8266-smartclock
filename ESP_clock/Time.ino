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
time_t calculate_offset() {
  static unsigned int previous_offset = NTP_TIMEZONE;
  if (!isAPMode) {
    static unsigned long timeoffset_PreviousMillis;
    unsigned long currentMillis = millis();
    if (currentMillis - timeoffset_PreviousMillis >= 1000 || timeoffset_PreviousMillis == 0) {
      timeoffset_PreviousMillis = currentMillis;
      int offset = NTP_TIMEZONE;
      if (NTP_DST) {
        if (isSummerTime(hour(timeClient.getEpochTime()), day(timeClient.getEpochTime()), month(timeClient.getEpochTime()), year(timeClient.getEpochTime()), NTP_TIMEZONE)) {
          offset += 1;
        }
      }
      previous_offset = offset;
    }
  }
  return previous_offset * SECS_PER_HOUR;
}
