package haservice

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"time"

	"github.com/loissascha/go-templ-template/internal/entities"
)

const token string = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI5ZTgzNDEzZjYxN2Q0MjE3YTJiOGM0NzIyZDI2ZmQ2ZSIsImlhdCI6MTc2OTY5OTc5NywiZXhwIjoyMDg1MDU5Nzk3fQ.8XaS8S2UCFTzdyc4yK5t3L_9Cdkeiv1gCwfayPuzr-I"

var registeredLights = []string{"light.tischlampe", "light.stehlampe"}
var registeredThermostates = []string{"climate.wohnzimmer_thermostat"}

type HAService struct {
	Lights []*entities.EntityState
}

func New() *HAService {
	h := &HAService{
		Lights: []*entities.EntityState{},
	}
	return h
}

func (h *HAService) LoadLightStates() error {
	for _, l := range registeredLights {
		res, err := h.loadLightStatus(l)
		if err != nil {
			return err
		}
		fmt.Println(res)
		h.Lights = append(h.Lights, res)
	}
	return nil
}

func (h *HAService) TurnOffLight(e *entities.EntityState) error {
	type Body struct {
		EntityId string `json:"entity_id"`
	}
	b := Body{
		EntityId: e.EntityID,
	}
	bj, err := json.Marshal(b)
	if err != nil {
		return err
	}
	req, err := http.NewRequest(
		http.MethodPost,
		"http://192.168.0.96:8123/api/services/light/turn_off",
		bytes.NewReader(bj),
	)
	if err != nil {
		return err
	}

	req.Header.Set("Authorization", fmt.Sprintf("Bearer %s", token))
	req.Header.Set("Content-Type", "application/json")

	client := http.Client{
		Timeout: 10 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	// body, err := io.ReadAll(resp.Body)
	// if err != nil {
	// 	return err
	// }

	fmt.Println("Status:", resp.Status)
	// fmt.Println("Body:", string(body))

	if resp.StatusCode != 200 {
		return fmt.Errorf("Non successful status code")
	}

	return nil
}

func (h *HAService) TurnOnLight(e *entities.EntityState) error {
	type Body struct {
		EntityId string `json:"entity_id"`
	}
	b := Body{
		EntityId: e.EntityID,
	}
	bj, err := json.Marshal(b)
	if err != nil {
		return err
	}
	req, err := http.NewRequest(
		http.MethodPost,
		"http://192.168.0.96:8123/api/services/light/turn_on",
		bytes.NewReader(bj),
	)
	if err != nil {
		return err
	}

	req.Header.Set("Authorization", fmt.Sprintf("Bearer %s", token))
	req.Header.Set("Content-Type", "application/json")

	client := http.Client{
		Timeout: 10 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return err
	}

	fmt.Println("Status:", resp.Status)
	fmt.Println("Body:", string(body))

	return nil
}

func (h *HAService) loadLightStatus(id string) (*entities.EntityState, error) {
	fmt.Println("loading light status from api for", id)
	req, err := http.NewRequest(
		http.MethodGet,
		fmt.Sprintf("http://192.168.0.96:8123/api/states/%s", id),
		nil,
	)
	if err != nil {
		return nil, err
	}

	req.Header.Set("Authorization", fmt.Sprintf("Bearer %s", token))
	req.Header.Set("Content-Type", "application/json")

	client := http.Client{
		Timeout: 10 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	fmt.Println("Status:", resp.Status)
	fmt.Println("Body:", string(body))

	var result entities.EntityState
	err = json.Unmarshal(body, &result)
	if err != nil {
		return nil, err
	}
	return &result, nil
}
