// ========================================================================
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include "./js_css.h"

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void buildWebsite() {
  buildJavascript();
  buildCss();
  webSite = "<!DOCTYPE HTML>\n";
  webSite += "<html>\n";
  webSite += "<title>nodeMCU</title>\n";
  webSite += "<meta charset='UTF-8'>\n";
  webSite += "<meta name='viewport' content='width=device-width, initial-scale=1'>\n";
  webSite += css;
  webSite += "<body>\n";
  webSite += "  <div class='display-middle', style='width:90%; text-align:center;' id='middle-div'></div>\n";
   webSite += javaScript;
  webSite += "</body>\n";
  webSite += "</html>\n";
}

void handleWebsite() {
  buildWebsite();
  server.send(200, "text/html", webSite);
}


