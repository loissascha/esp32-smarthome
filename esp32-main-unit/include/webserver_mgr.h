#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include "dht_mgr.h"
#include "time_mgr.h"
#include "pconline_mgr.h"
#include "settings_mgr.h"
#include "lightcontrol_mgr.h"
#include "mic_volume_nb.h"

class WebserverMgr {
public:
	WebserverMgr(DHTMgr &dhtmgr, TimeMgr &timemgr, PCOnlineMgr &pconlinemgr, SettingsMgr &settingsmgr, LightcontrolMgr &lightcontrolmgr, MicVolumeNB &mic);
	void setup();
	void loop();

private:
	WebServer server;
	DHTMgr &dhtmgr;
	TimeMgr &timemgr;
	PCOnlineMgr &pconlinemgr;
	SettingsMgr &settingsmgr;
	LightcontrolMgr &lightcontrolmgr;
	MicVolumeNB &mic;
	void handleRoot();
	void handleSensors();
	void handleNotFound();
	void handlePostMessage();
	void handleTischlampeModeSwitch(bool manual);
	void handleTischlampeManualStatusSwitch(bool status);
	void handleDaytimeStartUpdate();
	void handleDaytimeEndUpdate();
};
