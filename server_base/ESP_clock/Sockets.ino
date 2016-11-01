void saveSocketSet(String newSocketSet[]) {
  if (sizeof(newSocketSet) == 4 ) {
    print("keine sinn, dass das 4 lang ist");
  } else {
    Serial.println(sizeof(newSocketSet));
  }
  String path = SOCKET_CONFIG_PATH + newSocketSet[0];
  File f = SPIFFS.open(path, "w");
  for(int i=1; i <= (sizeof(newSocketSet)+1); i++) {
    Serial.println(newSocketSet[i]);
    f.println(newSocketSet[i]);
  }
  f.close();
  loadSocketSets();
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
            default:
              print("Error too many lines in reading sockets");
          }
          n ++;
        }
        socketId.replace(SOCKET_CONFIG_PATH, "");
        int i = socketId.toInt();
        isv3.trim();
        socketName.trim();
        houseCode.trim();
        groupCode.trim();
        socketCode.trim();
        configSocketSets[i][0] = isv3;
        configSocketSets[i][1] = socketName;
        configSocketSets[i][2] = houseCode;
        configSocketSets[i][3] = groupCode;
        configSocketSets[i][4] = socketCode;
        print("Cache socket { ID: "+socketId+\
              ", isv3: "+isv3+\
              ", Name: "+socketName+\
              ", HouseC: "+houseCode+\
              ", GroupC: "+groupCode+\
              ", SocketC: "+socketCode+\
              " }");
        entry.close();
      } else {
        print("Error in reading sockets");
      }
    }
}
