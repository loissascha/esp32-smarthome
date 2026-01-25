package espservice

import (
	"encoding/json"
	"fmt"
	"net/http"
	"time"
)

type ESPService struct {
	TempC    float64
	Humidity float64
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
		time.Sleep(10 * time.Second)
	}
}

func (e *ESPService) sync() error {
	type SensorResponse struct {
		TempC    float64 `json:"tempC"`
		Humidity float64 `json:"humidity"`
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

	e.TempC = result.TempC
	e.Humidity = result.Humidity

	return nil
}
