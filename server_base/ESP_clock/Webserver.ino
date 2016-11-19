void sendResponse(String content) {
  String finalContent = "<!DOCTYPE html><html><head><title>" + deviceName + "</title><meta name='viewport' content='initial-scale=1'><style>";
  finalContent += ".switch,input{box-sizing:border-box}body{background:#1F2237;color:#f9f9f9;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Oxygen,Ubuntu,Cantarell,'Fira Sans','Droid Sans','Helvetica Neue',sans-serif;font-size:16px}a{color:inherit}container,nav{color:#1F2237;background:#f9f9f9;border-radius:2px;width:95%;max-width:800px;display:block;margin:1em auto;padding:.5em 1em}nav{display:flex;align-items:baseline}nav h2{flex-grow:2}nav ul{padding:0;list-style:none}nav li{display:inline-block;margin-right:1em}input{padding:1em .8em;margin-bottom:.8em;margin-right:.5em;width:100%;max-width:300px;border-radius:2px;border:1px solid gray;outline:0;font-size:.8rem}input[type=submit]{background-color:#64a97c;color:#fff;border-color:#64a97c}label{display:block;margin-bottom:.2em;font-size:.8em;color:#656565}.switch{display:inline-block;padding:1.5em 0;width:30%;background:#cc5e5e;text-align:center;margin-right:2%;color:#fff;font-size:1.4em;font-weight:700;text-decoration:none;border-radius:3px;}";
  finalContent += "</style></head><body><nav><h2>"+ deviceName +"</h2>";
  finalContent += "<ul><li><a href='/'>Home</a></li>";
  finalContent += "<li><a href='/settings'>Settings</a></li></ul></nav>";
  finalContent += "<container>";
  finalContent += content;
  finalContent += "</container></body></html>";
  server.send(200, "text/html", finalContent);
}

void createServer() {
  server.on("/", []() {
    content = "<h3>";
    content += "Controll your home:";
    content += "</h3>";
    for(int i=0; i<(sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      content += "<p><label class='title'>" + configSocketSets[i][1] + "</label><a class='switch' href=\"socket" + String(i) + "On\">ON</a><a class='switch' href=\"socket" + String(i) + "Off\">OFF</a><a class='switch' href=\"socket" + String(i) + "Toggle\">Toggle</a></p>";
    }
    content += "<h3>";
    content += "<p> Temperature: " + String(temperature) + " &deg;C</p>";
    content += "<p> Humidity: " + String(humidity) + " %</p>";
    content += "<p> Heat-Index: " + String(heatindex) + "</p>";
    content += "<p> Time: " + String(timeClient.getFormattedTime()) + "</p>";
    content += "</h3>";
    sendResponse(content);
  });
  server.on("/settings", []() {
    //wifi settings
    content = "<h3>WiFi Settings</h3><form action='/wifiSet' method='GET'>";
    content += "<label for='ssid'>Wifi SSID</label><input id='ssid' type='text' name='ssid' placeholder='Your Wifi SSID' value='"+configSsid+"' autofocus><br>";
    content += "<label for='pw'>Wifi Password</label><input id='pw' type='password' name='pw' placeholder='Your Wifi Password' value='"+configPw+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    //name settings
    content += "<h3>Change Name</h3><form action='/nameSet' method='GET'>";
    content += "<p>Your Interface is visible at $name.local, Allowed characters: A-Z,a-z,0-9,'-'</p>";
    content += "<label for='name'>New Name</label><input type='text' id='name' name='name' placeholder='Your Device Name' value='"+deviceName+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    //name settings
    content += "<h3>Display Intensity</h3><form action='/intensitySet' method='GET'>";
    content += "<label for='intensity'>Intensity</label><input type='range' min='0' max='15' id='intensity' name='intensity' placeholder='Intensity' value='"+String(led_matrix_intensity)+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    //sockets
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
      content += "<label for='socketCode'>Socketcode</label><input type='number' id='socketCode' name='socketCode' placeholder='Device Code' value='"+socketSet[4]+"'><br>";
      content += "<input type='submit' value='Speichern'>";
      content += "</form><br>";
    }
    sendResponse(content);
  });
  server.on("/wifiSet", []() {
    String ssid = server.arg("ssid");
    String pw = server.arg("pw");
    print(ssid);
    print(pw);
    File f = SPIFFS.open(WIFI_CONFIG_PATH, "w");
    f.println(ssid);
    f.println(pw);
    f.close();
    sendResponse("Please restart the module");
  });
   server.on("/nameSet", []() {
    String name = server.arg("name");
    name = parseName(name);
    File f = SPIFFS.open(NAME_CONFIG_PATH, "w");
    f.println(name);
    f.close();
    deviceName = name;
    sendResponse("Saved!");
  });
  server.on("/intensitySet", []() {
    String intensity_String = server.arg("intensity");
    int intensity = intensity_String.toInt();
    File f = SPIFFS.open(LED_CONFIG_PATH, "w");
    f.println(intensity);
    f.close();
    led_matrix_intensity = intensity;
    sendResponse("Saved!");
    initMatrix(led_matrix_intensity);
  });
  server.on("/socketSet", []() {
    String socketID = server.arg("socketID");
    String isv3 = server.arg("isV3");
    if (isv3 != "on") {
      isv3 = "off";
    }
    String socketName = server.arg("name");
    String houseCode = server.arg("houseCode");
    String groupCode = server.arg("groupCode");
    String socketCode = server.arg("socketCode");

    String socketSet[6] = {socketID, isv3, socketName, houseCode, groupCode, socketCode};
    int array_size = sizeof(socketSet) / sizeof(socketSet[0]);
    saveSocketSet(socketSet, array_size);
    returnTo("/settings");
  });
  for(int i=0; i<(sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
    String pathOn = "/socket"+String(i)+"On";
    const char* pathOnChar = pathOn.c_str();
    String pathOff = "/socket"+String(i)+"Off";
    const char* pathOffChar = pathOff.c_str();
    String pathToggle = "/socket"+String(i)+"Toggle";
    const char* pathToggleChar = pathToggle.c_str();

    String isv3String = configSocketSets[i][0];
    bool isv3;
    if (isv3String.equals(String("on"))) {
      isv3 = true;
    } else {
      isv3 = false;
    }
    String houseC_String = configSocketSets[i][2];
    int groupC = configSocketSets[i][3].toInt();
    int socketC = configSocketSets[i][4].toInt();

    server.on(pathOnChar, [i, pathOn, isv3, houseC_String, groupC, socketC](){
      print(pathOn);
      if (isv3) {
        char houseC = houseC_String.charAt(0);
        mySwitch.switchOn(houseC, groupC, socketC);
      } else {
        //@TODO: friedls scheiß funktioniert einfach nicht
        int len = houseC_String.length() + 1;
        char houseC[len];
        houseC_String.toCharArray(houseC, len);

        Serial.println(houseC);
        char* yolo = "00100";
        
        mySwitch.switchOn(&houseC[0], socketC);
      }
      statusSocketSets[i] = true;
      returnTo("/");
    });
    
    server.on(pathOffChar, [i, pathOff, isv3, houseC_String, groupC, socketC](){
      print(pathOff);
      if (isv3) {
        char houseC = houseC_String.charAt(0);
        mySwitch.switchOff(houseC, groupC, socketC);
      } else {
        //@TODO: friedls scheiß funktioniert einfach nicht
        int len = houseC_String.length() + 1;
        char houseC[len];
        houseC_String.toCharArray(houseC, len);
        
        Serial.println(houseC);
        char* yolo = "00100";
        
        mySwitch.switchOff(&houseC[0], socketC);
      }
      statusSocketSets[i] = false;
      returnTo("/");
    });
    
    server.on(pathToggleChar, [i, pathToggle, isv3, houseC_String, groupC, socketC](){
      print(pathToggle);
      if (isv3) {
        char houseC = houseC_String.charAt(0);
        if (statusSocketSets[i]) {
          mySwitch.switchOff(houseC, groupC, socketC);
        } else {
          mySwitch.switchOn(houseC, groupC, socketC);
        }
      } else {
        //@TODO: friedls scheiß funktioniert einfach nicht
        int len = houseC_String.length() + 1;
        char houseC[len];
        houseC_String.toCharArray(houseC, len);
        
        Serial.println(houseC);
        char* yolo = "00100";
        
        mySwitch.switchOff(&houseC[0], socketC);
      }
      statusSocketSets[i] = !statusSocketSets[i];
      Serial.println(statusSocketSets[i]);
      returnTo("/");
    });
  }
}

void returnTo(String path) {
  server.sendHeader("Location", path, true);
  server.send(302, "text/plain", "");
}

