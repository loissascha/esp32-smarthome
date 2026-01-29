package homehandler

import (
	"fmt"
	"net/http"

	"github.com/loissascha/go-http-server/server"
	"github.com/loissascha/go-templ-template/internal/services/espservice"
	"github.com/loissascha/go-templ-template/internal/ui/pages"
	"github.com/loissascha/go-templ-template/internal/ui/partials"
)

type HomeHandler struct {
	s       *server.Server
	espserv *espservice.ESPService
}

func New(s *server.Server, esps *espservice.ESPService) *HomeHandler {
	return &HomeHandler{
		s:       s,
		espserv: esps,
	}
}

func percent(start, end int, value float64) int {
	// avoid division by zero
	if start == end {
		return 0
	}

	p := (value - float64(start)) / float64(end-start) * 100

	// clamp to [0, 100]
	if p < 0 {
		p = 0
	}
	if p > 100 {
		p = 100
	}

	return int(p)
}

func (h *HomeHandler) RegisterHandlers(s *server.Server) {
	s.GET("/", h.homeRoute)
	s.GET("/dashboard", h.dashboardRoute)
	s.GET("/temps", h.tempsRoute)
	s.GET("/light", h.lightsRoute)
	s.GET("/partials/infos", h.partialsInfos)
	s.GET("/partials/components/{component}", h.handleComponent)
	s.POST("/tischlampe/mode/{mode}", h.tischlampeMode)
	s.POST("/tischlampe/manual/{status}", h.tischlampeManualStatus)
}

func (h *HomeHandler) handleComponent(w http.ResponseWriter, r *http.Request) {
	component := r.PathValue("component")

	switch component {
	case "temp":
		p := percent(15, 35, h.espserv.Sensors.TempC)
		partials.TempGauge(h.espserv.Sensors.TempC, p).Render(r.Context(), w)
	case "humidity":
		p := percent(0, 100, h.espserv.Sensors.Humidity)
		partials.HumidityGauge(h.espserv.Sensors.Humidity, p).Render(r.Context(), w)
	case "voice":
		fmt.Fprintf(w, "%.0f", h.espserv.Sensors.VoiceLevel)
	case "tischlampestatus":
		partials.TischlampeStatusButton(h.espserv.Sensors.TischlampeStatus).Render(r.Context(), w)
	case "tischlampemode":
		partials.TischlampeMode(h.espserv.Sensors.TischlampeManualMode).Render(r.Context(), w)
	case "pcstatus":
		partials.PCStatus(h.espserv.Sensors.PCOnline).Render(r.Context(), w)
	case "timemode":
		partials.TimeMode(h.espserv.Sensors).Render(r.Context(), w)
	default:
		partials.Error("Unknown component").Render(r.Context(), w)
	}
}

func (h *HomeHandler) tischlampeManualStatus(w http.ResponseWriter, r *http.Request) {
	status := r.PathValue("status")
	fmt.Println("new status:", status)
	if status == "on" {
		h.espserv.UpdateTischlampeManualStatus(true)
	} else {
		h.espserv.UpdateTischlampeManualStatus(false)
	}
	partials.TischlampeStatusButton(h.espserv.Sensors.TischlampeStatus).Render(r.Context(), w)
}

func (h *HomeHandler) tischlampeMode(w http.ResponseWriter, r *http.Request) {
	mode := r.PathValue("mode")
	fmt.Println("new mode:", mode)
	if mode == "manual" {
		h.espserv.UpdateTischlampeMode(true)
	} else {
		h.espserv.UpdateTischlampeMode(false)
	}
	partials.TischlampeMode(h.espserv.Sensors.TischlampeManualMode).Render(r.Context(), w)
}

func (h *HomeHandler) partialsInfos(w http.ResponseWriter, r *http.Request) {
	partials.HomeInfoComponents(h.espserv.Sensors).Render(r.Context(), w)
}

func (h *HomeHandler) homeRoute(w http.ResponseWriter, r *http.Request) {
	homeComponent := pages.Home(h.espserv.Sensors)
	homeComponent.Render(r.Context(), w)
}

func (h *HomeHandler) dashboardRoute(w http.ResponseWriter, r *http.Request) {
	pages.Dashboard().Render(r.Context(), w)
}

func (h *HomeHandler) tempsRoute(w http.ResponseWriter, r *http.Request) {
	pages.Temps().Render(r.Context(), w)
}

func (h *HomeHandler) lightsRoute(w http.ResponseWriter, r *http.Request) {
	pages.Lights().Render(r.Context(), w)
}
