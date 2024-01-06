bool StartAPMode() {
  IPAddress apIP(192, 168, 0, 1);
  // Отключаем WIFI
  WiFi.disconnect();
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных _ssidAP _passwordAP
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
   Serial.println("");
    Serial.print("WiFi up AP");
    Serial.print(_ssidAP.c_str());
    Serial.print(" ");
    Serial.print(_ssidAP.c_str());
    Serial.println("");
  return true;
}

void WIFIinit() {
  // Попытка подключения к точке доступа
  WiFi.mode(WIFI_STA);
  byte tries = 30;
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("");
    Serial.print("WIFIinit  ");
    Serial.print(ssid.c_str());
    Serial.print(" ");
    Serial.print(password.c_str());
    Serial.println("");
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться запускаем в режиме AP
    Serial.println("");
    Serial.print("WiFi up AP");
    Serial.print(_ssidAP.c_str());
    Serial.print(" ");
    Serial.print(_ssidAP.c_str());
    Serial.println("");
    StartAPMode();
  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}