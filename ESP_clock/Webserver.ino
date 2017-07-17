void createServer() {
  server.on("/", Home);
  server.on("/settings", Settings);
  if (ENABLE_SENSOR) {
    server.on("/sensorData", sensorData);
  }
  server.on("/wifiSet", saveWifi);
  server.on("/nameSet", saveDeviceName);
  if (ENABLE_MATRIX) {
    server.on("/intensitySet", saveIntensity);
  }
  if (ENABLE_MATRIX) {
    server.on("/ledsOn", [](){setMatrixStatus(true);});
    server.on("/ledsOff", [](){setMatrixStatus(false);});
    server.on("/ledsToggle", [](){setMatrixStatus(!MatrixStatus);});
  }
  if (ENABLE_SOCKETS) {
    server.on("/socketSet", saveSocketSet);
    for(int i=0; i<(sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      String pathOn = "/socket"+String(i)+"On";
      const char* pathOnChar = pathOn.c_str();
      String pathOff = "/socket"+String(i)+"Off";
      const char* pathOffChar = pathOff.c_str();
      String pathToggle = "/socket"+String(i)+"Toggle";
      const char* pathToggleChar = pathToggle.c_str();
  
      server.on(pathOnChar, [i](){SocketSwitch(i, true);});
      server.on(pathOffChar, [i](){SocketSwitch(i, false);});
      server.on(pathToggleChar, [i](){SocketToggle(i);});
    }
  }
}

void returnTo(String path) {
  WifiLEDOn();
  server.sendHeader("Location", path, true);
  server.send(302, "text/plain", "");
  WifiLEDOff();
}

void sendResponse(String content) {
  WifiLEDOn();
  String finalContent = "<!DOCTYPE html><html><head><title>" + deviceName + "</title><meta name='viewport' content='initial-scale=1'><style>";
  finalContent += "container,input,nav{border-radius:2px;box-sizing:border-box}.switch,container,input,nav{box-sizing:border-box}body{background:#1F2237;color:#f9f9f9;font-family:-apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,Oxygen,Ubuntu,Cantarell,\"Fira Sans\",\"Droid Sans\",\"Helvetica Neue\",sans-serif;font-size:16px;margin:0}a{color:inherit}container,nav{color:#1F2237;background:#f9f9f9;width:95%;max-width:800px;display:block;margin:1em auto;padding:.5em 1em}nav{display:flex;align-items:baseline;margin-top:0}nav h2{flex-grow:2}nav ul{padding:0;list-style:none}nav li{display:inline-block;margin-right:1em}input{padding:1em .8em;margin-bottom:.8em;margin-right:.5em;width:100%;max-width:300px;border:1px solid gray;outline:0;font-size:.8rem}input[type=submit]{background-color:#64a97c;color:#fff;border-color:#64a97c}label{display:block;margin-bottom:.2em;font-size:.9em;color:#656565}#l0{color: #008000;}#l1{color: #FFA500;}#l2{color: #FF8040;}#l3{color: #FF0000;}#l4{color: #800000;}.switch{display:inline-block;padding:1.5em 0;width:30%;background:#cc5e5e;text-align:center;margin-right:5%;color:#fff;font-size:1.4em;font-weight:700;text-decoration:none;border-radius:3px}.switch:last-child{margin-right:0}.sensorData{display:flex;justify-content:space-around}";
  finalContent += "</style></head><body><nav><h2>"+ deviceName + "</h2>";
  finalContent += "<ul><li><a href='/'>Home</a></li>";
  finalContent += "<li><a href='/settings'>Settings</a></li></ul></nav>";
  finalContent += "<container>";
  finalContent += content;
  finalContent += "</container></body></html>";
  server.send(200, "text/html", finalContent);
  WifiLEDOff();
}

void Home() {
  if (!auth()) { return; }
  content = "";
  if (ENABLE_SOCKETS) {
    content += "<h3>Control your home:</h3>";
    for(int i=0; i<(sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      content += "<p><label class='title'>" + configSocketSets[i][1] + "</label><a class='switch' href=\"socket" + String(i) + "On\">ON</a><a class='switch' href=\"socket" + String(i) + "Off\">OFF</a><a class='switch' href=\"socket" + String(i) + "Toggle\">Toggle</a></p>";
    }
  }
  if (ENABLE_SENSOR) {
    content += "<h3>Sensor information</h3>";
    content += "<div class=\"sensorData\">";
    content += "<p><label class='title'>Temperature:</label>" + String(temperature) + " &deg;C</p>";
    content += "<p><label class='title'>Humidity:</label>" + String(humidity) + " %</p>";
    content += "<p id='l" + String(get_heat_index_level()) + "'><label class='title'>Heat-Index:</label>" + String(heatindex) + "</p>";
    content += "</div>";
  }
  if (ENABLE_MATRIX) {
    content += "<h3>LED Matrix</h3>";
    content += "<p><a class='switch' href=\"ledsOn\">ON</a><a class='switch' href=\"ledsOff\">OFF</a><a class='switch' href=\"ledsToggle\">Toggle</a></p>";
  }
  sendResponse(content);
}

void Settings() {
  if (!auth()) { return; }
  //wifi settings
  content = "<h3>WiFi Settings</h3><form action='/wifiSet' method='GET'>";
  content += "<label for='ssid'>Wifi SSID</label><input id='ssid' type='text' name='ssid' placeholder='Your Wifi SSID' value='"+WifiSsid+"' autofocus><br>";
  content += "<label for='pw'>Wifi Password</label><input id='pw' type='password' name='pw' placeholder='Your Wifi Password'><br>";
  content += "<input type='submit' value='Speichern'>";
  content += "</form>";
  //name settings
  content += "<h3>Change Name</h3><form action='/nameSet' method='GET'>";
  content += "<p>Your Interface is visible at $name.local, Allowed characters: A-Z,a-z,0-9,'-'</p>";
  content += "<label for='name'>New Name</label><input type='text' id='name' name='name' placeholder='Your Device Name' value='"+deviceName+"'><br>";
  content += "<input type='submit' value='Speichern'>";
  content += "</form>";
  //intensity settings
  if (ENABLE_MATRIX) {
    content += "<h3>Display Intensity</h3><form action='/intensitySet' method='GET'>";
    content += "<label for='intensity'>Intensity</label><input type='range' min='0' max='15' id='intensity' name='intensity' placeholder='Intensity' value='"+String(led_matrix_intensity)+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
  }
  if (ENABLE_SENSOR) {
    content += "<h3>Sensor API</h3>";
    content += "<p>The JSON API endpoint is at: <a href=\"/sensorData\">/sensorData</a></p>";
  }
  
  //sockets
  if (ENABLE_SOCKETS) {
    content += "<h3>Sockets</h3>";
    for(int i=0; i<(sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      String socketSet[5] = configSocketSets[i];
      content += "<form action='/socketSet' method='GET'><span><p><b>Socket ";
      content +=  String(i+1);
      content += "</b></p><br>";
      content += "<input type='hidden' name='socketID' value='"+String(i)+"'>";
      if(socketSet[0].equals(String("on"))) {
        content += "<label for='isV3'>Is Protocol 3</label><input type='checkbox' name='isV3' placeholder='is Protocol 3' checked><br>";
      } else {
        content += "<label for='isV3'>Is Protocol 3</label><input type='checkbox' name='isV3' placeholder='is Protocol 3'><br>";
      }
      content += "<label for='name'>Name of socket</label><input id='name' type='text' name='name' placeholder='Name for socket' value='"+socketSet[1]+"'><br>";
      content += "<label for='houseCode'>Housecode</label><input type='text' id='houseCode' name='houseCode' placeholder='Housecode' value='"+socketSet[2]+"'><br>";
      content += "<label for='groupCode'>Groupcode</label><input type='number' id='groupCode' name='groupCode' placeholder='Gruppe (optional)' value='"+socketSet[3]+"'><br>";
      content += "<label for='socketCode'>Socketcode</label><input type='text' id='socketCode' name='socketCode' placeholder='Device Code' value='"+socketSet[4]+"'><br>";
      content += "<input type='submit' value='Speichern'>";
      content += "</form><br>";
    }
  }
  sendResponse(content);
}

bool auth() {
  if (ENABLE_LOGIN) {
    if (ENABLE_LOCAL_NO_LOGIN) {
      String clientIp = server.client().remoteIP().toString();
      String net = "192.168";
      if (clientIp.startsWith(net)) {
        //print("local");
        return true;
      }
      //print("external");
    }
    if (!server.authenticate(WWW_USERNAME, WWW_PASSWORD)) {
      server.requestAuthentication();
      return false;
    }
  }
  return true;
}

/*
 * endpoint to output sensor data as json
 */
void sensorData() {
  if (!auth()) { return; }
  String json = "{ \"temperatur\": \""+String(temperature)+"\",";
  json += " \"humidity\": \""+String(humidity)+"\",";
  json += " \"timestamp\": \""+String(timeClient.getFormattedTime())+"\"}";
  server.send(200, "application/json", json);
}

