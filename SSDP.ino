void SSDP_init(void) {
  // SSDP дескриптор
    server.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(server.client());
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
 // SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(SSDP_Name);
  SSDP.setSerialNumber("001788102201");
  SSDP.setURL("/");
  SSDP.setModelName("SSDP-Start");
  SSDP.setModelNumber("000000000001");
  SSDP.setModelURL("https://xn--80acmmon9a.xn--p1ai/");
  SSDP.setManufacturer("Abelint");
  SSDP.setManufacturerURL("https://xn--80acmmon9a.xn--p1ai/");
  SSDP.begin();
}