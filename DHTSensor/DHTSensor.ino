#include "U8glib.h"
#include <DHT.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define RX 3
#define TX 2
SoftwareSerial BT(TX, RX);


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

DHT dht(6,DHT11);


void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  BT.println(t);
  BT.println(h);
  u8g.firstPage();
  do{
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(5,10);
    u8g.print("Humi: ");
    u8g.setPrintPos(45,10);
    u8g.print(h);
    u8g.drawStr(5,30, "temp: ");
    u8g.setPrintPos(45,30);
    u8g.print(t);
  }while(u8g.nextPage());
  delay(2000);
}
