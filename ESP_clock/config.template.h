//PERSONAL-CONFIG-FILE

//DEBUG
const bool    DEBUG = true;
const int     REACTION_TIME = 500;  //less power consumption or less responsive

//SERVICES
const bool    ENABLE_SOCKETS = true;
const bool    ENABLE_SENSOR = true;
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
const int     NTP_OFFSET = 3600;
const int     NTP_INTERVAL = 60000;

//LED-Matrix
const int     DEFAULT_LED_MATRIX_INTENSITY = 15;
const int     LED_MATRIX_PORT_DATA = 13; //D7
const int     LED_MATRIX_PORT_CLK = 14; //D5
const int     LED_MATRIX_PORT_CHIP_SELECT = 4; //D2
const int     LED_MATRIX_PORT_AMOUNT = 3;

//REMOTE-SOCKET
const int     SOCKET_PORT = 2; //D4
const int     SOCKET_AMOUNT = 3;

//TEMPERATURE AND HUMIDITY
const int     SENSOR_PORT = 12; //D6
const long    SENSOR_INTERVAL = 2000;

//DYNDNS
const long    DYNDNS_INTERVAL = 600000; // every 10 minutes
const String  DYNDNS_URL = "http://freedns.afraid.org/dynamic/update.php?asdf...";

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

