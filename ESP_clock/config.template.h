//PERSONAL-CONFIG-FILE

//DEBUG
const bool    DEBUG = false;
const unsigned int     REACTION_TIME = 500;

//SERVICES
const bool    ENABLE_SOCKETS = true;
const bool    ENABLE_SENSOR = true;
const bool    ENABLE_POST_SENSOR_DATA = false;
const bool    ENABLE_MATRIX = true;
const bool    ENABLE_DYNDNS = false;
const bool    ENABLE_MDNS = false;
const bool    ENABLE_LOGIN = true;
const bool    ENABLE_LOCAL_NO_LOGIN = true;
const bool    ENABLE_OTA = true;
const bool    ENABLE_WEATHER = true;
const bool    ENABLE_VICTRON = false;

//DEVICE-NAME
const String  DEFAULT_DEVICE_NAME = "ESP8266";

//LOGIN
const char    WWW_USERNAME[] = "admin";
const char    WWW_PASSWORD[] = "password";

//FIRMWARE
const char    OTA_USERNAME[] = "admin";
const char    OTA_PASSWORD[] = "admin";

//NTP
const char    NTP_SERVER[] = "pool.ntp.org";
const int     NTP_TIMEZONE = 1;     // Central European Time
//const int     NTP_TIMEZONE = -5;  // Eastern Standard Time (USA)
//const int     NTP_TIMEZONE = -4;  // Eastern Daylight Time (USA)
//const int     NTP_TIMEZONE = -8;  // Pacific Standard Time (USA)
//const int     NTP_TIMEZONE = -7;  // Pacific Daylight Time (USA)
const int     NTP_INTERVAL = 600000; // every 10 minutes
const bool    NTP_DST = true; //Daylight saving time

//LED-Matrix
const int     LED_MATRIX_INTENSITY = 15;
const int     LED_MATRIX_DIRECTION = 0; // 0=bottom, 1=right, 2=top, 3=left
const int     LED_MATRIX_PORT_DATA = D3;
const int     LED_MATRIX_PORT_CHIP_SELECT = D2;
const int     LED_MATRIX_PORT_CLK = D1;
const int     LED_MATRIX_PORT_AMOUNT = 3;

//REMOTE-SOCKET
const int     SOCKET_PORT = D4;
const int     SOCKET_AMOUNT = 3;

//TEMPERATURE AND HUMIDITY
const int     SENSOR_PORT = D5;
const int     SENSOR_TYPE = DHT11;
const long    SENSOR_INTERVAL = 2000; //in ms
const String  POST_SENSOR_DATA_ENDPOINT = "someurl";
const String  POST_SENSOR_DATA_TOKEN = "sometoken";
const long    POST_SENSOR_INTERVAL = 1800000; // in ms (every 30 minutes)

//DYNDNS
const long    DYNDNS_INTERVAL = 60000; // in ms (every 10 minutes)
const String  DYNDNS_URL = "http://freedns.afraid.org/dynamic/update.php?asdf...";

//WEATHER
const long    GET_WEATHER_INTERVAL = 1800000; // in ms (every 30 minutes)
const String  WEATHER_CITY = "London,uk";
const String  WEATHER_API_TOKEN = "asdasdewqrsv..."; // api.openweathermap.org
const int     WEATHER_3HOUR_FUTURE = 2; // gives the weather time in the future: 1 current quarter, 2 next quarter, ..

//VICTRON
const int     VICTRON_RX = D7;
const int     VICTRON_TX = D8;

//################################################################
// DO NOT CHANGE THINGS BELOW, UNLESS YOU KNOW WHAT YOU ARE DOING

//STORAGE
/* File structure:
   /NAME_CONFIG_PATH:
      name
   /WIFI_CONFIG_PATH:
      ssid \n
      pw
   /sockets/$id:
      protocol (v3 = true; v1 = false)
      socketName;
      houseCode;
      groupCode;
      socketCode;
*/
const String  WIFI_CONFIG_PATH = "/wifi.txt";
const String  NAME_CONFIG_PATH = "/name.txt";
const String  LED_CONFIG_PATH = "/led.txt";
const String  SOCKET_CONFIG_PATH = "/sockets/";
