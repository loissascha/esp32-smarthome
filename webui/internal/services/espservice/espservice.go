package espservice

import (
	"encoding/json"
	"fmt"
	"net/http"
	"time"
)

type ESPSensorData struct {
	TempC                float64
	Humidity             float64
	VoiceLevel           float64
	TischlampeManualMode bool
	TischlampeStatus     bool
	IsDaytime            bool
	PCOnline             bool
}

type ESPService struct {
	Sensors ESPSensorData
}

func New() *ESPService {
	return &ESPService{}
}

func (e *ESPService) RunSync() {
	for {
		err := e.sync()
		if err != nil {
			fmt.Println("sync error:", err)
		}
		time.Sleep(3 * time.Second)
	}
}

func (e *ESPService) sync() error {
	type SensorResponse struct {
		TempC                float64 `json:"tempC"`
		Humidity             float64 `json:"humidity"`
		VoiceLevel           float64 `json:"voice_level"`
		TischlampeManualMode int     `json:"tischlampe_manual_mode"`
		TischlampeStatus     int     `json:"tischlampe_status"`
		IsDaytime            int     `json:"is_daytime"`
		PCOnline             int     `json:"pc_online"`
	}

	client := &http.Client{
		Timeout: 10 * time.Second,
	}

	resp, err := client.Get("http://192.168.0.72/sensors")
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return fmt.Errorf("unexpected status: %s", resp.Status)
	}

	var result SensorResponse
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		return err
	}

	fmt.Println("sync result:", result)

	e.Sensors.TempC = result.TempC
	e.Sensors.Humidity = result.Humidity
	e.Sensors.VoiceLevel = result.VoiceLevel

	if result.TischlampeManualMode == 1 {
		e.Sensors.TischlampeManualMode = true
	} else {
		e.Sensors.TischlampeManualMode = false
	}

	if result.TischlampeStatus == 1 {
		e.Sensors.TischlampeStatus = true
	} else {
		e.Sensors.TischlampeStatus = false
	}

	if result.IsDaytime == 1 {
		e.Sensors.IsDaytime = true
	} else {
		e.Sensors.IsDaytime = false
	}

	if result.PCOnline == 1 {
		e.Sensors.PCOnline = true
	} else {
		e.Sensors.PCOnline = false
	}

	return nil
}
