void saveSocketSet(String newSocketSet[]) {
  String path = SOCKET_CONFIG_PATH + newSocketSet[0];
  File f = SPIFFS.open(path, "w");
  for(int i=1; i<sizeof(newSocketSet); i++) {
    f.println(newSocketSet[i]);
  }
  f.close();  
}

void loadSocketSets() {
  Dir socketFolder = SPIFFS.openDir(SOCKET_CONFIG_PATH);
    while(socketFolder.next()){
      File entry = socketFolder.openFile("r");
      if (entry) {
        String socketId = entry.name();
        String socketName;
        String houseCode;
        String groupCode;
        String socketCode;
        int n = 0;
        while (entry.available()) {
          switch (n){
            case 0:
              socketName = entry.readStringUntil('\n');
              break;
            case 1:
              houseCode = entry.readStringUntil('\n');
              break;
            case 2:
              groupCode = entry.readStringUntil('\n');
              break;
            case 3:
              socketCode = entry.readStringUntil('\n');
              break;
          }
          n ++;
          socketId.replace(SOCKET_CONFIG_PATH, "");
          int i = socketId.toInt();
          socketName.trim();
          houseCode.trim();
          groupCode.trim();
          socketCode.trim();
          configSocketSets[i][0] = socketName;
          configSocketSets[i][1] = houseCode;
          configSocketSets[i][2] = groupCode;
          configSocketSets[i][3] = socketCode;
          print("Cache new socket "+socketId+" with name "+socketName);
          entry.close();
        }
      } else {
        print("Error in reading sockets");
      }
    }
}
