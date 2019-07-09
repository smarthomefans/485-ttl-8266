#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "./modbusrx.h"

#define SERIAL0_TCP_PORT 8880 // Wifi Port UART0
#define MAX_NMEA_CLIENTS 4
#define bufferSize 1024

const char *ssid = "1";
const char *password = "monkey1234";
bool debug = false;

uint8_t buf1[bufferSize];
uint16_t i1=0;

uint8_t buf2[bufferSize];
uint16_t i2=0;

SoftwareSerial swSer1(D2, D1, false, 256);
WiFiServer server_0(SERIAL0_TCP_PORT);
WiFiServer *server = &server_0;
WiFiClient TCPClient[MAX_NMEA_CLIENTS];
WiFiClient wfclient;

void setup()
{
	Serial.begin(115200);
	Serial.println("\nOne Wire Half Duplex Serial Tester");
	delay(500);
	swSer1.begin(115200);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	server->begin(); // start TCP server
	server->setNoDelay(true);
}

long time1 = millis();

void loop()
{
	if (server->hasClient())
	{
		for (byte i = 0; i < MAX_NMEA_CLIENTS; i++)
		{
			//find free/disconnected spot
			if (!TCPClient[i] || !TCPClient[i].connected())
			{
				if (TCPClient[i])
					TCPClient[i].stop();
				TCPClient[i] = server->available();
				if (debug)
					Serial.print("New client for COM");
				if (debug)
					Serial.print(0);
				if (debug)
					Serial.println(i);
				continue;
			}
		}
		//no free/disconnected spot so reject
		WiFiClient TmpserverClient = server->available();
		TmpserverClient.stop();
	}

	for(byte cln = 0; cln < MAX_NMEA_CLIENTS; cln++) {               
      if(TCPClient[cln]) {
        while(TCPClient[cln].available()) {
          buf1[i1] = TCPClient[cln].read(); // read char from TCP port:8880
          if(i1<bufferSize-1) i1++;
        } 
        swSer1.write(buf1, i1); // now send to UART
        i1 = 0;
      }
    }
	modbustask(&swSer1, TCPClient);
}