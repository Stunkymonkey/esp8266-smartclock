void sendResponse(String content) {
  String finalContent = "<!DOCTYPE html><html><head><title>" + deviceName + "</title></head><body><nav>"+ deviceName +"<br>";
  finalContent += "<ul><li><a href='/'>Home</a></li>";
  finalContent += "<li><a href='/settings'>Settings</a></li></ul></nav><hr>";
  finalContent += "<container>";
  finalContent += content;
  finalContent += "</container></body></html>";
  server.send(200, "text/html", finalContent);
}

void createServer() {
  server.on("/", []() {
    content = "<h3>";
    content += "Welcome!";
    content += "</h3>";
    sendResponse(content);
  });
  server.on("/settings", []() {
    //wifi settings
    content = "<h3>WiFi Settings</h3><form action='/wifiSet' method='GET'>";
    content += "<input type='text' name='ssid' placeholder='Your Wifi SSID' value='"+configSsid+"' autofocus><br>";
    content += "<input type='password' name='pw' placeholder='Your Wifi Password' value='"+configPw+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    //name settings
    content += "<h3>Change Name</h3><form action='/nameSet' method='GET'>";
    content += "<p>Your Interface is visible at $name.local</p>";
    content += "<input type='text' name='name' placeholder='Your Device Name' value='"+deviceName+"' autofocus><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    //sockets
    content += "<h3>Sockets</h3>";
    for(int i=0; i<3; i++) {
      Serial.println(configSocketSets[i][0]);
      String socketSet[4] = configSocketSets[i];
      content += "<form action='/socketSet' method='GET'><span>Socket ";
      content +=  String(i);
      content += "</span><br>";
      content += "<input type='text' name='name' placeholder='Name for socket' value='"+socketSet[0]+"'>";
      content += "<input type='text' name='houseCode' placeholder='Housecode' value='"+socketSet[1]+"'>";
      content += "<input type='hidden' name='socketID' value='"+String(i)+"'>";
      content += "<input type='number' name='groupCode' placeholder='Gruppe (optional)' value='"+socketSet[2]+"'>";
      content += "<input type='number' name='socketCode' placeholder='Device Code' value='"+socketSet[3]+"'>";
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
    if (!f) {
      print("File doesn't exist yet. Creating it");
    }
    f.println(ssid);
    f.println(pw);
    f.close();
    sendResponse("Please restart the module");
  });
   server.on("/nameSet", []() {
    String name = server.arg("name");
    File f = SPIFFS.open(NAME_CONFIG_PATH, "w");
    if (!f) {
      print("File doesn't exist yet. Creating it");
    }
    f.println(name);
    f.close();
    deviceName = name;
    sendResponse("Saved!");
  });
  server.on("/socketSet", []() {
    String socketID = server.arg("socketID");    
    String socketName = server.arg("name");
    String houseCode = server.arg("houseCode");
    String groupCode = server.arg("groupCode");
    String socketCode = server.arg("socketCode");

    String socketSet[5] = {socketID, socketName, houseCode, groupCode, socketCode};
    saveSocketSet(socketSet);
    sendResponse("Saved!");
  });
}

