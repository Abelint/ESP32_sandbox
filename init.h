#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

//для SSDP, не входжит в стандартный пакет
#include <ESP32SSDP.h>        //https://github.com/luc-github/ESP32SSDP

//для работы с файлом конфигурации
#include <ArduinoJson.h>

// настройка работы с файловой системой
#define FILESYSTEM SPIFFS
// You only need to format the filesystem once
#define FORMAT_FILESYSTEM false

#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif