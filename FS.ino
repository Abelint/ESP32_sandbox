
void FI_init(){
   //вывод списка файлов
  server.on("/list", HTTP_GET, handleFileList);
  //load editor
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  //create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  //если страница не известна  
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  
  //вывод состояния всех пинов по критериям
  server.on("/all", HTTP_GET, []() {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(A0));
    json += ", \"gpio\":" + String((uint32_t)(0));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
}


//форматы данных
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

// тип файлов
String getContentType(String filename) {
  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

// проверка существования пути
bool exists(String path){
  bool yes = false;
  File file = FILESYSTEM.open(path, "r");
  if(!file.isDirectory()){
    yes = true;
  }
  file.close();
  return yes;
}

//считывание файла по адресу
bool handleFileRead(String path) {
  Serial.println("handleFileRead handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (exists(pathWithGz) || exists(path)) {
    if (exists(pathWithGz)) {
      path += ".gz";
    }
    File file = FILESYSTEM.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

//загрузка файла
void handleFileUpload() {
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = FILESYSTEM.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
    Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
  }
}

// удаление файла
void handleFileDelete() {
  if (server.args() == 0) {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg(0);
  Serial.println("handleFileDelete: " + path);
  if (path == "/") {
    return server.send(500, "text/plain", "BAD PATH");
  }
  if (!exists(path)) {
    return server.send(404, "text/plain", "FileNotFound");
  }
  FILESYSTEM.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

//создание файла
void handleFileCreate() {
  if (server.args() == 0) {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg(0);
  Serial.println("handleFileCreate: " + path);
  if (path == "/") {
    return server.send(500, "text/plain", "BAD PATH");
  }
  if (exists(path)) {
    return server.send(500, "text/plain", "FILE EXISTS");
  }
  File file = FILESYSTEM.open(path, "w");
  if (file) {
    file.close();
  } else {
    return server.send(500, "text/plain", "CREATE FAILED");
  }
  server.send(200, "text/plain", "");
  path = String();
}

//получение списка фалов видимо
void handleFileList() {
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("dir");
  Serial.println("handleFileList: " + path);


  File root = FILESYSTEM.open(path);
  path = String();

  String output = "[";
  if(root.isDirectory()){
      File file = root.openNextFile();
      while(file){
          if (output != "[") {
            output += ',';
          }
          output += "{\"type\":\"";
          output += (file.isDirectory()) ? "dir" : "file";
          output += "\",\"name\":\"";
          output += String(file.path()).substring(1);
          output += "\"}";
          file = root.openNextFile();
      }
  }
  output += "]";
  server.send(200, "text/json", output);
}