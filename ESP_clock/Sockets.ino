void saveSocketSet() {
  String socketID = server.arg("socketID");
  String isv3 = server.arg("isV3");
  if (isv3 != "on") {
    isv3 = "off";
  }
  String socketName = server.arg("name");
  String houseCode = server.arg("houseCode");
  String groupCode = server.arg("groupCode");
  String socketCode = server.arg("socketCode");

  String path = SOCKET_CONFIG_PATH + socketID;
  File f = SPIFFS.open(path, "w");
  
  f.println(isv3);
  f.println(socketName);
  f.println(houseCode);
  f.println(groupCode);
  f.println(socketCode);
  //save Timer Values (empty values are saved as an empty string)
  if(ENABLE_TIMER) {
    String timerOnTime = server.arg("timerOnTime");
    String timerOffTime = server.arg("timerOffTime");
    f.println(timerOnTime);
    f.println(timerOffTime);
  }
  f.close();
  loadSocketSets();
  returnTo("/settings");
}

void loadSocketSets() {
  Dir socketFolder = SPIFFS.openDir(SOCKET_CONFIG_PATH);
  while(socketFolder.next()){
    File entry = socketFolder.openFile("r");
    if (entry) {
      String socketId = entry.name();
      String isv3;
      String socketName;
      String houseCode;
      String groupCode;
      String socketCode;
      //load timer values every time
      String timerOnTime;
      String timerOffTime;
      int n = 0;
      while (entry.available()) {
        switch (n){
          case 0:
            isv3 = entry.readStringUntil('\n');
            break;
          case 1:
            socketName = entry.readStringUntil('\n');
            break;
          case 2:
            houseCode = entry.readStringUntil('\n');
            break;
          case 3:
            groupCode = entry.readStringUntil('\n');
            break;
          case 4:
            socketCode = entry.readStringUntil('\n');
            break;
          case 5:
            timerOnTime = entry.readStringUntil('\n');
            break;
          case 6:
            timerOffTime = entry.readStringUntil('\n');
            break;
          default:
            print("Error too many lines in reading sockets... deleting file");
            Serial.println(entry.readStringUntil('\n'));
            SPIFFS.remove(entry.name());
            break;
        }
        n ++;
      }
      socketId.replace(SOCKET_CONFIG_PATH, "");
      int i = socketId.toInt();
      if (i >= (sizeof configSocketSets / sizeof configSocketSets[0])) {
        continue;
      }
      isv3.trim();
      socketName.trim();
      houseCode.trim();
      groupCode.trim();
      socketCode.trim();
      timerOnTime.trim();
      timerOffTime.trim();
      configSocketSets[i][0] = isv3;
      configSocketSets[i][1] = socketName;
      configSocketSets[i][2] = houseCode;
      configSocketSets[i][3] = groupCode;
      configSocketSets[i][4] = socketCode;
      configSocketSets[i][5] = timerOnTime;
      configSocketSets[i][6] = timerOffTime;
      print("Cache socket { ID: "+socketId+\
            ", isv3: "+isv3+\
            ", Name: "+socketName+\
            ", HouseC: "+houseCode+\
            ", GroupC: "+groupCode+\
            ", SocketC: "+socketCode+\
            ", Timer On: "+timerOnTime+\
            ", Timer Off: "+timerOffTime+\
            " }");
      entry.close();
    } else {
      print("Error in reading sockets");
    }
  }
  //re-init timer with new data
  initTimer();
}

void SocketSwitch(int i, bool state) {
  //if (!auth()) { return; }
  String isv3String = configSocketSets[i][0];
  bool isv3;
  isv3 = isv3String.equals(String("on"));
  String houseC_String = configSocketSets[i][2];
  int groupC = configSocketSets[i][3].toInt();
  int socketC = configSocketSets[i][4].toInt();
  String socketC_String = configSocketSets[i][4];
  if (state) {
    print("On: " + configSocketSets[i][1]);
  } else {
    print("Off: " + configSocketSets[i][1]);
  }
  
  if (isv3) {
    char houseC = houseC_String.charAt(0);
    if (state) {
      mySwitch.switchOn(houseC, groupC, socketC);
    } else {
      mySwitch.switchOff(houseC, groupC, socketC);
    }
  } else {
    //hacky way - I really should learn more about C
    int len = houseC_String.length() + 1;
    char houseC[len];
    houseC_String.toCharArray(houseC, len);

    int lenSocket = socketC_String.length() +1;
    char socketCS[lenSocket];
    socketC_String.toCharArray(socketCS, lenSocket);

    if (state) {
      mySwitch.switchOn(houseC, socketCS);
    } else {
      mySwitch.switchOff(houseC, socketCS);
    }
  }
  statusSocketSets[i] = state;
}

void SocketToggle(int i) {
  Serial.print("Toggle-");
  SocketSwitch(i, !statusSocketSets[i]);
}

