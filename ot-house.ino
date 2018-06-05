/*  ArduinoIDE 1.6.12 compiled on Linux Mint 17.3
    Sketch uses 245,617 bytes (23%) of program storage space. Maximum is 1,044,464 bytes.
    Global variables use 37,048 bytes (45%) of dynamic memory, leaving 44,804 bytes for local variables. Maximum is 81,920 bytes.
*/

#include "./functions.h"
#include <ESP8266mDNS.h>


const char* ssid     = "MT";      
const char* password = "harchiharchi";
//const char* ssid     = "MT/iphone"; 
//const char* password = "yektahaft"; 


unsigned long previousMillis = 0;
const long interval = 200;
int clients=0;
int LED = 2;                            // NodeMCU by LoLin
int lastButtonState;

#define relay1  D1
#define relay2  D2
#define input1  D3
#define relay3  D8
WiFiClient client;
MDNSResponder mdns;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
 // Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // Send the current LED status
      if (clients < num+1) clients = num+1;
      updatePages();
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);
      if(((char*) payload)[0] == '#'){
        String s = "";
        for(int i=1; i<=strlen((char*) payload);i++)
          s += ((char*) payload)[i];  
          int relaynum = (s.substring(0,s.indexOf(","))).toInt();
          String stateToGoString =  s.substring(s.indexOf(",")+1);
          if(stateToGoString == "OFF") relaysVal[relaynum] =(relaysVal[relaynum] + 1)%2;
          if(stateToGoString == "ON") relaysVal[relaynum] = (relaysVal[relaynum] + 1)%2;
          updateRelays();
      }
      else{
        Serial.println("Unknown command");
      }
      // send data to all connected clients
      webSocket.broadcastTXT(payload, length);
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);
      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void updateRelays(){
  digitalWrite(relay1, relaysVal[0]);
  digitalWrite(relay2, relaysVal[1]);
  digitalWrite(input1, relaysVal[2]);
  digitalWrite(relay3, relaysVal[3]);
}
void updatePages(){
  for (int i=0;i<relays;i++){
    char sendingString[5];
    sprintf(sendingString, "#%d,%d", i,relaysVal[i]);
    webSocket.broadcastTXT(sendingString,strlen(sendingString));
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(input1, INPUT_PULLUP);
  lastButtonState = digitalRead(input1);
  pinMode(relay3, OUTPUT);

  Serial.begin(115200); delay(100);
  
  Serial.print("\n\nconnecting to ");
  WiFi.begin(ssid, password);
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  Serial.print("\n\nconnected to ");Serial.print(ssid);
  Serial.print("\nIpAddress: ");Serial.print(WiFi.localIP());

  if (mdns.begin("ot", WiFi.localIP())) {
    Serial.println("MDNS responder started");
    mdns.addService("http", "tcp", 80);
    mdns.addService("ws", "tcp", 81);
  }
  else {
    Serial.println("MDNS.begin failed");
  }
  Serial.print("Connect to http://ot.local or http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleWebsite);
  server.onNotFound(handleNotFound);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);  
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    if(clients >= 1){
      updatePages();
    }
    previousMillis = currentMillis;
  }
  if(digitalRead(input1) == LOW && lastButtonState != LOW){
    relaysVal[0] = (relaysVal[0] + 1)%2;
    lastButtonState = digitalRead(input1);
    updateRelays();
  }else if(digitalRead(input1) == HIGH && lastButtonState != HIGH){
    lastButtonState = digitalRead(input1);
  }
  webSocket.loop();
  server.handleClient();
}

