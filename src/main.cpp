#include <Arduino.h>
#include <SoftwareSerial.h>
#include "./modbusrx.h"

SoftwareSerial swSer1(D2, D1, false, 256);

void setup() {
  Serial.begin(115200);
	Serial.println("\nOne Wire Half Duplex Serial Tester");
	swSer1.begin(115200);
}

long time1 = millis();

void loop() {


  modbustask(&swSer1);

  if (millis() - time1 > 5000) {
	  time1 = millis();
	  char *msg = "";
	  sprintf(msg, "%d,%d,%d,%d,%d,%d", 01, 05,00,00,00,00 );
	  sendsignal(&swSer1, msg);
  }
}