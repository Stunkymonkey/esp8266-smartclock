/*
   main Webserver method with routing definitions
*/
void createServer() {
  server.on("/", Home);
  server.on("/settings", Settings);
  server.on("/restart", restart);
  if (ENABLE_SENSOR) {
    server.on("/sensorData", sensorData);
  }
  server.on("/wifiSet", saveWifi);
  server.on("/nameSet", saveDeviceName);
  if (ENABLE_MATRIX) {
    server.on("/intensitySet", saveIntensity);
  }
  if (ENABLE_MATRIX) {
    server.on("/ledsOn", []() {
      setMatrixStatus(true);
    });
    server.on("/ledsOff", []() {
      setMatrixStatus(false);
    });
    server.on("/ledsToggle", []() {
      setMatrixStatus(!MatrixStatus);
    });
  }
  if (ENABLE_SOCKETS) {
    server.on("/socketSet", saveSocketSet);
    server.on("/socketMasterOn", SocketMasterOn);
    server.on("/socketMasterOff", SocketMasterOff);
    for (size_t i = 0; i < (sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      String pathOn = "/socket" + String(i) + "On";
      const char* pathOnChar = pathOn.c_str();
      String pathOff = "/socket" + String(i) + "Off";
      const char* pathOffChar = pathOff.c_str();
      String pathToggle = "/socket" + String(i) + "Toggle";
      const char* pathToggleChar = pathToggle.c_str();

      server.on(pathOnChar, [i]() {
        SocketSwitch(i, true);
      });
      server.on(pathOffChar, [i]() {
        SocketSwitch(i, false);
      });
      server.on(pathToggleChar, [i]() {
        SocketToggle(i);
      });
    }
  }
  server.on("/disco", disco);
  server.on("/disco-toggle", disco_toggle);
  server.onNotFound(handleNotFound);
}

void returnTo(String path) {
  WifiLEDOn();
  server.sendHeader("Location", path, true);
  server.send(302, "text/plain", "");
  WifiLEDOff();
}

/*
   main response helper that wraps the content into the html markup including stylesheets
*/
void sendResponse(String content) {
  WifiLEDOn();
  String finalContent = "<!DOCTYPE html><html><head><title>" + deviceName + "</title><meta name='viewport' content='initial-scale=1'><style>";
  finalContent += ".navContainer h2 a,.switch a{text-decoration:none}container,input{box-sizing:border-box}body{background:#191919;color:#f3f3f3;font-family:'Fira Mono',monospace;font-size:16px;margin:0}a{color:inherit}.switch,.switch.off .onLabel,nav{color:#191919}container{width:95%;max-width:800px;display:block;margin:0 auto;padding:0 1em}.small{max-width:500px}nav{width:100%;background:#f3f3f3;padding:.3em 0}.navContainer{display:flex;align-items:center;flex-wrap:wrap}.navContainer h2{flex-grow:2;margin:.5em 0}.navContainer .icon{margin-left:1em;display:inline-block;width:28px;line-height:28px;height:28px;background-size:cover}.bulbIcon{background-image:url(data:image/svg+xml,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%2224%22%20height%3D%2224%22%20viewBox%3D%220%200%2024%2024%22%3E%3Cpath%20d%3D%22M9%2021c0%20.55.45%201%201%201h4c.55%200%201-.45%201-1v-1H9v1zm3-19C8.14%202%205%205.14%205%209c0%202.38%201.19%204.47%203%205.74V17c0%20.55.45%201%201%201h6c.55%200%201-.45%201-1v-2.26c1.81-1.27%203-3.36%203-5.74%200-3.86-3.14-7-7-7zm2.85%2011.1l-.85.6V16h-4v-2.3l-.85-.6C7.8%2012.16%207%2010.63%207%209c0-2.76%202.24-5%205-5s5%202.24%205%205c0%201.63-.8%203.16-2.15%204.1z%22%2F%3E%3C%2Fsvg%3E)}.settingsIcon{background-image:url(data:image/svg+xml,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%2224%22%20height%3D%2224%22%20viewBox%3D%220%200%2024%2024%22%3E%3Cpath%20d%3D%22M19.43%2012.98c.04-.32.07-.64.07-.98s-.03-.66-.07-.98l2.11-1.65c.19-.15.24-.42.12-.64l-2-3.46c-.12-.22-.39-.3-.61-.22l-2.49%201c-.52-.4-1.08-.73-1.69-.98l-.38-2.65C14.46%202.18%2014.25%202%2014%202h-4c-.25%200-.46.18-.49.42l-.38%202.65c-.61.25-1.17.59-1.69.98l-2.49-1c-.23-.09-.49%200-.61.22l-2%203.46c-.13.22-.07.49.12.64l2.11%201.65c-.04.32-.07.65-.07.98s.03.66.07.98l-2.11%201.65c-.19.15-.24.42-.12.64l2%203.46c.12.22.39.3.61.22l2.49-1c.52.4%201.08.73%201.69.98l.38%202.65c.03.24.24.42.49.42h4c.25%200%20.46-.18.49-.42l.38-2.65c.61-.25%201.17-.59%201.69-.98l2.49%201c.23.09.49%200%20.61-.22l2-3.46c.12-.22.07-.49-.12-.64l-2.11-1.65zM12%2015.5c-1.93%200-3.5-1.57-3.5-3.5s1.57-3.5%203.5-3.5%203.5%201.57%203.5%203.5-1.57%203.5-3.5%203.5z%22%2F%3E%3C%2Fsvg%3E)}.navContainer span{margin-right:.8em;font-weight:600}@media only screen and (max-width:500px){.sensorContainer{order:2;text-align:center;flex-grow:2;margin:.5em 0}}.switchContainer{margin:2em auto;width:98%;max-width:500px}.switchContainer.small{max-width:400px}.switchContainer h3,.switchContainer h4{font-weight:400;margin:.6em 0}.switch{overflow:hidden;height:120px;line-height:120px;width:100%;background:#f3f3f3;border-radius:4px;display:flex;position:relative}.switchContainer.small .switch{height:100px;line-height:100px}.switch a{z-index:10;font-size:60px;font-weight:700;flex-grow:2;text-align:center;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Oxygen,Ubuntu,Cantarell,'Fira Sans','Droid Sans','Helvetica Neue',sans-serif}.switchContainer.small .switch a{font-size:50px}.switch.off .offLabel,.switch.on .onLabel{color:#f3f3f3}.switch.on .offLabel{color:#191919}.toggler{position:absolute;width:50%;height:100%;bottom:0;z-index:0;transition:all ease .4s}.switch.on .toggler{left:0;background:#115C24}.switch.off .toggler{left:50%;background:#802516}input{padding:1em .8em;margin-bottom:.8em;margin-right:.5em;width:100%;border-radius:2px;border:1px solid gray;outline:0;font-size:.8rem}input[type=submit]{background-color:#115C24;color:#fff;border-color:#115C24;text-transform:uppercase;font-weight:700}input[type=checkbox]{width:auto}label{display:block;margin-bottom:.7em;font-size:.8em}#l0{color:green}#l1{color:orange}#l2{color:#FF8040}#l3{color:red}#l4{color:maroon}";
  finalContent += "</style>";
  finalContent += "</head><body><nav><container class=\"navContainer\"><h2><a href=\"/\">" + deviceName + "</a></h2>";
  if (ENABLE_SENSOR) {
    finalContent += "<div class=\"sensorContainer\">";
    finalContent += "<span>" + String(temperature) + "&deg;C</span><span>" + String(humidity) + "%</span><span id='l" + String(get_heat_index_level()) + "'>" + String(heatindex) + "HT</span>";
    finalContent += "</div>";
  }
  if (ENABLE_MATRIX) {
    finalContent += "<a href=\"/ledsToggle\" class=\"icon bulbIcon\"></a>";
  }
  finalContent += "<a href=\"/settings\" class=\"icon settingsIcon\"></a>";
  finalContent += "</container>";
  finalContent += "</nav>";
  finalContent += content;
  finalContent += "</body></html>";
  server.send(HTTP_CODE_OK, "text/html", finalContent);
  WifiLEDOff();
}

/*
   route /
   markup for the homepage
*/
void Home() {
  if (!auth()) {
    return;
  }
  content = "";
  content += "<container>";
  if (ENABLE_SOCKETS) {
    content += "<div class=\"switchContainer\">";
    content += "<h3>Master</h3>";
    content += "<div class=\"switch ";
    bool allFalse = true;
    bool allTrue = true;
    for (size_t i = 0; i < sizeof(statusSocketSets); i++) {
      if (statusSocketSets[i]) {
        allFalse = false;
      } else {
        allTrue = false;
      }
    }
    if (allTrue) {
      content += "on";
    } else if (allFalse) {
      content += "off";
    }
    content += "\">";
    content += "<a href=\"socketMasterOn\" class=\"onLabel\">ON</a>";
    content += "<a href=\"socketMasterOff\" class=\"offLabel\">OFF</a>";
    content += "<div class=\"toggler\"></div>";
    content += "</div>";
    content += "</div>";
    for (size_t i = 0; i < (sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      content += "<div class=\"switchContainer small\">";
      content += "<h4>" + configSocketSets[i][1] + "</h4>";
      content += "<div class=\"switch ";
      if (statusSocketSets[i]) {
        content += "on";
      } else {
        content += "off";
      }
      content += "\">";
      content += "<a href=\"socket" + String(i) + "On\" class=\"onLabel\">ON</a>";
      content += "<a href=\"socket" + String(i) + "Off\" class=\"offLabel\">OFF</a>";
      //content += "<a href=\"socket" + String(i) + "Toggle\" class=\"toggleLabel\">ASDF</a>";
      content += "<div class=\"toggler\"></div>";
      content += "</div>";
      content += "</div>";
    }
  }
  content += "</div>";
  if (DEBUG_STRING != "") {
    content += "<h3>" + DEBUG_STRING + "</h3>";
  }
  content += "</container>";
  sendResponse(content);
}

/*
   route /settings
   markup for the settings page
*/
void Settings() {
  content = "";
  content += "<container class=\"small\">";
  if (!auth()) {
    return;
  }
  //wifi settings
  content += "<h3>WiFi Settings</h3><form action='/wifiSet' method='GET'>";
  content += "<label for='ssid'>Wifi SSID</label><input id='ssid' type='text' name='ssid' placeholder='Your Wifi SSID' value='" + WifiSsid + "' autofocus><br>";
  content += "<label for='pw'>Wifi Password</label><input id='pw' type='password' name='pw' placeholder='Your Wifi Password'><br>";
  content += "<input type='submit' value='Speichern'>";
  content += "</form>";
  //name settings
  content += "<h3>Change Name</h3><form action='/nameSet' method='GET'>";
  content += "<p>Your Interface is visible at $name.local, Allowed characters: A-Z,a-z,0-9,'-'</p>";
  content += "<label for='name'>New Name</label><input type='text' id='name' name='name' placeholder='Your Device Name' value='" + deviceName + "'><br>";
  content += "<input type='submit' value='Speichern'>";
  content += "</form>";
  //intensity settings
  if (ENABLE_MATRIX) {
    content += "<h3>Display Intensity</h3><form action='/intensitySet' method='GET'>";
    content += "<label for='intensity'>Intensity</label><input type='range' min='0' max='15' id='intensity' name='intensity' placeholder='Intensity' value='" + String(led_matrix_intensity) + "'><br>";
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
    for (size_t i = 0; i < (sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      String socketSet[5] = configSocketSets[i];
      content += "<form action='/socketSet' method='GET'><span><p><b>Socket ";
      content +=  String(i + 1);
      content += "</b></p><br>";
      content += "<input type='hidden' name='socketID' value='" + String(i) + "'>";
      if (socketSet[0].equals(String("on"))) {
        content += "<label for='isV3'>Is Protocol 3</label><input type='checkbox' name='isV3' placeholder='is Protocol 3' checked><br>";
      } else {
        content += "<label for='isV3'>Is Protocol 3</label><input type='checkbox' name='isV3' placeholder='is Protocol 3'><br>";
      }
      content += "<label for='name'>Name of socket</label><input id='name' type='text' name='name' placeholder='Name for socket' value='" + socketSet[1] + "'><br>";
      content += "<label for='houseCode'>Housecode</label><input type='text' id='houseCode' name='houseCode' placeholder='Housecode' value='" + socketSet[2] + "'><br>";
      content += "<label for='groupCode'>Groupcode</label><input type='number' id='groupCode' name='groupCode' placeholder='Gruppe (optional)' value='" + socketSet[3] + "'><br>";
      content += "<label for='socketCode'>Socketcode</label><input type='text' id='socketCode' name='socketCode' placeholder='Device Code' value='" + socketSet[4] + "'><br>";
      content += "<input type='submit' value='Speichern'>";
      content += "</form><br>";
    }
  }
  content += "<h3>RESTART</h3><form action='/restart' method='GET'>";
  content += "<input type='submit' value='RESTART'>";
  content += "</form>";
  content += "</container>";
  sendResponse(content);
}

/*
   returns true if authenticated
*/
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
   endpoint to output sensor data as json
*/
void sensorData() {
  if (!auth()) {
    return;
  }
  String json = "{ \"temperatur\": \"" + String(temperature) + "\",";
  json += " \"humidity\": \"" + String(humidity) + "\",";
  json += " \"timestamp\": \"" + String(timeClient.getFormattedTime()) + "\"}";
  server.send(HTTP_CODE_OK, "application/json", json);
}

/*
   404-site
*/
void handleNotFound() {
  content = "<h1 style=\"text-align:center;\">404 - Not Found</h1>";
  sendResponse(content);
}
