#include "init.h"
#include "updater.h"


// подключение к роутеру
const char* host = "esp32";
String  ssid = "TERRA";
String password = "ae2w4b9w";

//запуск сервера
WebServer server(80);

// для загрузки файлов
File fsUploadFile;

//для SSDP
IPAddress apIP(192, 168, 0, 200);
String _ssidAP = "WiFi_ESP_sandbox";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String SSDP_Name="Start"; // Имя SSDP

//для файла конфигурации
String jsonConfig = "{}";

//время
int timezone = 3;               // часовой пояс GTM

void setup() {
  Serial.begin(115200);

  int a =10;
  int* b = &a;
  test(b);

  Serial.println(a);
  delay(3000);

  // подключение файловой системы
  if (FORMAT_FILESYSTEM) FILESYSTEM.format();
  FILESYSTEM.begin();
  {
      File root = FILESYSTEM.open("/");
      File file = root.openNextFile();
      while(file){
          String fileName = file.name();
          size_t fileSize = file.size();
          Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
          file = root.openNextFile();
      }
      Serial.printf("\n");
  }
  ////////////////////////////////////

  //подгружаем настройки
  loadConfig();

  // подключение к вай фаю
  WIFIinit();
  /////////////////////////////////////
  

  // запуск http сервера
  MDNS.begin(host);
  Serial.print("Open http://");
  Serial.print(host);
  Serial.println(".local/edit to see the file browser");


  //ИНИЦИАЛИЗАЦИЯ HTTP СЕРВЕРА для файловой системы
  FI_init();


  // Запуск HTTP сервера
  HTTP_init();
  Serial.println("HTTP server started");

  ///////////////////////////////////////////////////////////////////

  // запуск прошивки по воздуху
  StartOTA();
  //////////////////////////////
}



void test(int* x){
 *x += 100;
}

void loop() {
  server.handleClient();
  delay(1);

}
