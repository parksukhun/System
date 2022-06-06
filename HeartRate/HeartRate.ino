#include "U8glib.h"
#include "MAX30100_PulseOximeter.h"
#include <Wire.h>
#include <SoftwareSerial.h>

#define REPORTING_PERIOD_MS     1000
#define RX 3
#define TX 2
SoftwareSerial BT(TX, RX);

PulseOximeter pox;
uint32_t tsLastReport = 0;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);  // 오류나면 9600으로 수정
  BT.begin(115200);
  Serial.begin(115200);
  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }

  // The default current for the IR LED is 50mA and it could be changed 
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options. --값 잘 안나오면 MAX30100_Registers.h 확인, 50MA를 11AM로 수정
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
   pox.update();

   //시리얼 출력용
   if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }

  //블루투스 값 전달
  BT.println(pox.getHeartRate());
  BT.println(pox.getSpO2());

  //화면 출력
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(5, 10); //setPrintPos(x좌표, y좌표) 글씨 겹치면 수정
    u8g.print("Heart rate:");
    u8g.setPrintPos(45, 10);
    u8g.print(pox.getHeartRate());
    u8g.drawStr(5, 30, "bpm / SpO2:");
    u8g.setPrintPos(45, 30);
    u8g.print(pox.getSpO2());
    // u8g.print("%"); // % 추가, 앞에 내용 정상출력되면 위치 적당히 변경
  } while (u8g.nextPage());
  delay(2000);
}
