#include "webserver_mgr.h"

WebserverMgr::WebserverMgr(DHTMgr &dhtmgr) : server(80), dhtmgr(dhtmgr) {}

void WebserverMgr::handleRoot() {
  String out;
  out += "ESP32 up\n";
  server.send(200, "text/plain; charset=utf-8", out);
}

void WebserverMgr::handleSensors() {
	float temp = dhtmgr.getTempCels();
	float humidity = dhtmgr.getHumidity();
	String out = "";
	out += "{ \"tempC\": ";
	out += String(temp) + ",";
	out += " \"humidity\": " + String(humidity);
	out += "}\n";
  server.send(200, "text/plain; charset=utf-8", out);
}

void WebserverMgr::handleNotFound() {
  server.send(404, "text/plain; charset=utf-8", "not found\n");
}

void WebserverMgr::handlePostMessage() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method not allowed\n");
    return;
  }

  if (!server.hasArg("message")) {
    server.send(400, "text/plain", "Missing 'message'\n");
    return;
  }

  String msg = server.arg("message");
  server.send(200, "text/plain; charset=utf-8", "OK\n");
}

void WebserverMgr::setup() {
  server.on("/", HTTP_GET, [this]() { this->handleRoot(); });
  server.on("/sensors", HTTP_GET, [this]() { this->handleSensors(); });
  server.on("/post", HTTP_POST, [this]() { this->handlePostMessage(); });
  server.onNotFound([this]() { this->handleNotFound(); });

  server.begin();
}

void WebserverMgr::loop() { server.handleClient(); }
