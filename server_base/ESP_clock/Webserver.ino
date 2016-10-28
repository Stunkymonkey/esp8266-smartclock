void sendResponse(String content) {
  String finalContent = "<!DOCTYPE html><html><head><title>" + deviceName + "</title></head><body><container>";
  finalContent += content;
  finalContent += "</container></body></html>";
  server.send(200, "text/html", finalContent);
}

void createServer() {
  server.on("/", []() {
    content = "<h3>";
    content += "Welcome";
    content += "<br><a href='/settings'>Settings</a>";
    content += "</h3>";
    sendResponse(content);
  });
  server.on("/settings", []() {
    content = "<h3>WiFi Settings</h3><form action='/wifiSet' method='GET'>";
    content += "<input type='text' name='ssid' placeholder='Your Wifi SSID' value='"+configSsid+"' autofocus><br>";
    content += "<input type='password' name='pw' placeholder='Your Wifi Password' value='"+configPw+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    content += "<h3>Change Name</h3><form action='/nameSet' method='GET'>";
    content += "<input type='text' name='name' placeholder='Your Device Name' value='"+deviceName+"' autofocus><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
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
}

