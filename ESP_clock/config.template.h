//PERSONAL-CONFIG-FILE

//DEBUG
const bool    DEBUG = true;
const int     REACTION_TIME = 500;  //less power consumption or less responsive

//SERVICES
const bool    ENABLE_SOCKETS = true;
const bool    ENABLE_SENSOR = true;
const bool    ENABLE_POST_SENSOR_DATA = false;
const bool    ENABLE_MATRIX = true;
const bool    ENABLE_DYNDNS = false;
const bool    ENABLE_LOGIN = true;
const bool    ENABLE_LOCAL_NO_LOGIN = true;
const bool    ENABLE_OTA = true;

//DEVICE-NAME
const String  DEFAULT_DEVICE_NAME= "ESP8266";

//LOGIN
const char    WWW_USERNAME[] = "admin";
const char    WWW_PASSWORD[] = "password";

//FIRMWARE
const char    OTA_USERNAME[] = "admin";
const char    OTA_PASSWORD[] = "admin";

//NTP
const char    NTP_SERVER[] = "de.pool.ntp.org";
const int     NTP_TIMEZONE = 1;     // Central European Time
//const int     NTP_TIMEZONE = -5;  // Eastern Standard Time (USA)
//const int     NTP_TIMEZONE = -4;  // Eastern Daylight Time (USA)
//const int     NTP_TIMEZONE = -8;  // Pacific Standard Time (USA)
//const int     NTP_TIMEZONE = -7;  // Pacific Daylight Time (USA)
const int     NTP_INTERVAL = 60000; // every 10 minutes

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
const long    SENSOR_INTERVAL = 2000;
const String  POST_SENSOR_DATA_ENDPOINT = "someurl";
const String  POST_SENSOR_DATA_TOKEN = "sometoken";
const long    POST_SENSOR_INTERVAL = 1800000; // every 30 minutes

//DYNDNS
const long    DYNDNS_INTERVAL = 60000; // every 10 minutes
const String  DYNDNS_URL = "http://freedns.afraid.org/dynamic/update.php?asdf...";

//WEATHER
const long    GET_WEATHER_INTERVAL = 1800000; // every 30 minutes
const String  WEATHER_CITY = "London,uk";
const String  WEATHER_API_TOKEN = "asdasdewqrsv..."; // api.openweathermap.org

//############################################

//STORAGE
/* File structure:
 * /NAME_CONFIG_PATH:
 *    name
 * /WIFI_CONFIG_PATH:
 *    ssid \n
 *    pw
 * /sockets/$id:
 *    protocol (v3 = true; v1 = false)
 *    socketName;
 *    houseCode;
 *    groupCode;
 *    socketCode;
 */
const String  WIFI_CONFIG_PATH = "/wifi.txt";
const String  NAME_CONFIG_PATH = "/name.txt";
const String  LED_CONFIG_PATH = "/led.txt";
const String  SOCKET_CONFIG_PATH = "/sockets/";

