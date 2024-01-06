
// функции обработки событий с веб страницы


void HTTP_init(void) {
  server.on("/restart", handle_Restart); // Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
  server.on("/configs.json", handle_ConfigJSON);

  // получение данных
  server.on("/ssdp", handle_set_ssdp);
  server.on("/ssidap", handle_set_ssid);
  // Запускаем HTTP сервер
  server.begin();
}

// передача данных со страницы в ЕСП get запросами
void handle_set_ssdp(){
  SSDP_Name = server.arg("ssdp");
  saveConfig();
  server.send(200,"text/plain", "OK");
}
void handle_set_ssid(){
  ssid = server.arg("ssid");
  password = server.arg("password");
   Serial.println("");
    Serial.print("handle_set_ssid  ");
    Serial.print(ssid);
    Serial.print(" ");
    Serial.print(password);
  saveConfig();
  server.send(200,"text/plain", "OK");
}


// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = server.arg("device");
  if (restart == "ok") ESP.restart();
  server.send(200, "text/plain", "OK");
}

// работа с фалом конфигурации
void handle_ConfigJSON() {
  String root = "{}";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(root);
  // Заполняем поля json
  json["SSDP"] = SSDP_Name;
  json["ssidAP"] = _ssidAP;
  json["passwordAP"] = _passwordAP;
  json["ssid"] = ssid;
  json["password"] = password;
  json["timezone"] = timezone;
  json["ip"] = WiFi.localIP().toString();
  json["time"] = GetTime();
  json["date"] = GetDate();
  // Помещаем созданный json в переменную root
  root = "";
  json.printTo(root);
  server.send(200, "text/json", root);
}

