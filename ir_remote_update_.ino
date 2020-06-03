#include <ESP8266WiFi.h>
#include <WiFiClient.h>
extern "C" {
#include "user_interface.h"
}

const char* ssid = "Tenda";
const char* password = "46213228";

WiFiServer server(254);
unsigned int irSignal[] = {8900, 4450, 550, 600, 500, 600, 550, 1650, 550, 600, 550, 550, 550, 550, 550, 600, 550, 550, 550, 1700, 500, 1700, 550, 550, 550, 1700, 500, 1700, 550, 1650, 550, 1700, 500, 1700, 550, 1650, 550, 1650, 550, 600, 550, 550, 550, 600, 500, 600, 550, 1650, 550, 600, 550, 550, 550, 600, 500, 1700, 550, 1650, 550, 1700, 500, 1700, 550, 550, 550, 1650, 550}; //AnalysIR Batch Export (IRremote) - RAW


void setup(void) {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  Serial.println("setup complete");
  IPAddress staticIP(192, 168, 0, 125);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  WiFi.config(staticIP, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  WiFi.config(staticIP, gateway, subnet);
  server.begin();
  digitalWrite(2, LOW);
}


void transmit()
{
  Serial.println("transmitting_init");
  for (int i = 0; i < (sizeof(irSignal) / sizeof(irSignal[0])); i++)
  {
    if (!(i & 1))
      digitalWrite(2, HIGH);
    else
      digitalWrite(2, LOW);
    delayMicroseconds(irSignal[i] + 50);
  }
  Serial.println("transmission_complete");
  digitalWrite(2, LOW);
}

void analysis(String code)
{
  Serial.println("Analysis");
  int j = -1;
  for (int i = 0; i < 67; i++)
  {
    irSignal[i] = 0;
    while (code[++j] != ',')
    {
      irSignal[i] = irSignal[i] * 10 + (int)(code[j] - '0');
    }
  }
  transmit();
}


void loop(void) {
  WiFiClient client = server.available();
  if (!client) {
    delay(1);
    return;
  }
  String request = client.readStringUntil('|');
  //Serial.println(request);
  String code = (request.substring(request.indexOf("<start>") + 7, request.indexOf("<end>")));
  delay(1);
  Serial.println("bingo");
  analysis(code);
}
