String loadDeviceName() {
  Dir socketFolder = SPIFFS.openDir(SOCKET_CONFIG_PATH);
  if (socketFolder) {
    String name;
    while(dir.next()){
      File entry = dir.openFile("r");
      if (entry) {
        String socketName;
        String houseCode;
        String groupCode;
        String socketCode;
        int n = 0;
        while (entry.available()) {
          switch (n){
            case 0:
              socketName = configFile.readStringUntil('\n').trim();
              break;
            case 1:
              houseCode = configFile.readStringUntil('\n').trim();
              break;
            case 2:
              groupCode = configFile.readStringUntil('\n').trim();
              break;
            case 3:
              socketCode = configFile.readStringUntil('\n').trim();
              break;
          }
          n ++;
          //TODO collect date and return
          entry.close();
        }
      } else {
        print("Error in reading sockets")
      }
    }
  }
}
