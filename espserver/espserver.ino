#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define ANALOG_PIN 34

#define SSID "batata"
#define PASSWORD "fritacombacon"

#define DATA_SIZE 500

WebServer server(80);

const int led = 13;

uint16_t analogData[DATA_SIZE];

void sendData()
{
  int buff = 0;
  for (uint16_t i = 0; i < DATA_SIZE; i++)
    buff += analogData[i];
#define SIZE_TEXT 100
  char text[SIZE_TEXT];
  snprintf(text, SIZE_TEXT, "{\"v\": %d}", buff / DATA_SIZE);
  server.send(200, "application/plaintext", text);
}

void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }
  server.enableCORS();
  server.on("/", sendData);
  server.begin();
  Serial.println("HTTP server started");
}

uint16_t i = 0;

void loop(void)
{
  server.handleClient();
  if (i == DATA_SIZE)
    i = 0;
  i++;
  analogData[i] = analogRead(ANALOG_PIN);
  delay(2);
}