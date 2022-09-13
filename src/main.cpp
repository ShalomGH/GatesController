// Import required libraries
#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include "pass.h"


uint32_t timer1, timer2;
bool flag1, flag2;
#define gate1 4
#define gate2 5

const char* PARAM_INPUT_1 = "number";


/************ Wi-Fi Settings **********/
IPAddress ip(10,1,1,71);
IPAddress gateway(10,1,1,1);
IPAddress subnet(255, 255, 255, 0);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="ru">
<head>
    <style>
        body {
            text-align: center;
            font-family: "Century Gothic";
            height: 100%;
            width: 100%;
            background: #eff0d1;
        }

        html {
            width: 100%;
            height: 100%;
        }

        h1 {
            font-size: 90px;
        }

        .container-1 {
            width: 100%;
            height: 100%;
            align-items: center;
            padding: 5%;
            display: grid;
            grid-template-rows: 1fr 5fr 1fr 5fr;
            grid-row-gap: 1em;
        }

        .flat-2 {
            width: 100%;
            height: 100%;
            display: grid;
            grid-template-columns: 1fr;
            grid-gap: 4em;
        }

        input {
            width: 100%;
            height: 100%;
            background: #77ba99;
            border: 0;
            font-size: 80px;
            border-radius: 30px;
        }

    </style>
    <meta charset="UTF-8">
    <title>PoolLights</title>
</head>
<body>
<div class="container-1">
    <div class="flat-1">
        <h1>Ворота 1</h1>
    </div>
    <div class="flat-2">
        <input type="button" value="On" class="r-" onclick="fetch('/gates?number=1')">
    </div>
    <div class="flat-1">
        <h1>Ворота 2</h1>
    </div>
    <div class="flat-2">
        <input type="button" value="On" class="r-" onclick="fetch('/gates?number=2')">
    </div>
</div>
</body>
</html>
)rawliteral";


void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);


  pinMode(gate1, OUTPUT);
  pinMode(gate2, OUTPUT);
  digitalWrite(gate1, LOW);
  digitalWrite(gate2, LOW);




  // Connect to Wi-Fi
  /****** Wi-Fi connection ******/
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/gates", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      if (inputMessage == "1") flag1 = true, timer1 = millis(), Serial.println("gate1");
      if (inputMessage == "2") flag2 = true, timer2 = millis(), Serial.println("gate2");
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  server.begin();
}

void loop() {
  if (flag1 == true){
    digitalWrite(gate1, HIGH);
    if (millis() - timer1 >= 500) {
      digitalWrite(gate1, LOW);
      flag1 = false;
    }
  }
  if (flag2 == true){
    digitalWrite(gate2, HIGH);
    if (millis() - timer2 >= 500) {
      digitalWrite(gate2, LOW);
      flag2 = false;
    }
  }
}