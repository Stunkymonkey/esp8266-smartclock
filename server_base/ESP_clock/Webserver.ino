void sendResponse(String content) {
  String finalContent = "<!DOCTYPE html><html><head><title>" + deviceName + "</title><meta name='viewport' content='initial-scale=1'><style>";
  finalContent += "body{background:#1F2237;color:#f9f9f9;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Oxygen,Ubuntu,Cantarell,'Fira Sans','Droid Sans','Helvetica Neue',sans-serif;font-size:16px}a{color:inherit}container,nav{color:#1F2237;background:#f9f9f9;border-radius:2px;width:95%;max-width:800px;display:block;margin:1em auto;padding:.5em 1em}nav{display:flex;align-items:baseline}nav h2{flex-grow:2}nav ul{padding:0;list-style:none}nav li{display:inline-block;margin-right:1em}input{padding:1em .8em;margin-bottom:.8em;margin-right:.5em;width:100%;max-width:300px;box-sizing:border-box;border-radius:2px;border:1px solid gray;outline:0;font-size:.8rem}input[type=submit]{background-color:#64a97c;color:#fff;border-color:#64a97c}";
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
      content += "<p><span class='title'>" + configSocketSets[i][0] + "</span><a class='switch' href=\"socket" + String(i) + "On\">ON</a><a class='switch' href=\"socket" + String(i) + "Off\">OFF</a><a class='switch' href=\"socket" + String(i) + "Off\">Toggle</a></p>";
    }
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
    content += "<p>Your Interface is visible at $name.local</p>";
    content += "<label for='name'>New Name</label><input type='text' id='name' name='name' placeholder='Your Device Name' value='"+deviceName+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    //sockets
    content += "<h3>Sockets</h3>";
    
    for(int i=0; i<(sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
      String socketSet[5] = configSocketSets[i];
      content += "<form action='/socketSet' method='GET'><span>Socket ";
      content +=  String(i+1);
      content += "</span><br>";
      content += "<input type='hidden' name='socketID' value='"+String(i)+"'>";
      content += "<label for='isV3'>Is Protocol 3</label><input type='checkbox' name='isV3' placeholder='is Techno Version'>(Currently:"+socketSet[0]+")<br>";
      content += "<label for='name'>Name of socket</label><input id='name' type='text' name='name' placeholder='Name for socket' value='"+socketSet[1]+"'>";
      content += "<label for='houseCode'Housecode</label><input type='text' id='houseCode' name='houseCode' placeholder='Housecode' value='"+socketSet[2]+"'>";
      content += "<label for='groupCode'Housecode</label><input type='number' id='groupCode' name='groupCode' placeholder='Gruppe (optional)' value='"+socketSet[3]+"'>";
      content += "<label for='socketCode'Socketcode</label><input type='number' id='socketCode' name='socketCode' placeholder='Device Code' value='"+socketSet[4]+"'>";
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
    saveSocketSet(socketSet);
    returnTo("/settings");
  });
  for(int i=0; i<(sizeof configSocketSets / sizeof configSocketSets[0]); i++) {
    String pathOn = "/socket"+String(i)+"On";
    const char* pathOnChar = pathOn.c_str();
    String pathOff = "/socket"+String(i)+"Off";
    const char* pathOffChar = pathOff.c_str();

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

    server.on(pathOnChar, [pathOn, isv3, houseC_String, groupC, socketC](){
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
      returnTo("/");
    });
    
    server.on(pathOffChar, [pathOff, isv3, houseC_String, groupC, socketC](){
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
      
      returnTo("/");
    });
  }
}

void returnTo(String path) {
  server.sendHeader("Location", path, true);
  server.send(302, "text/plain", "");
}

