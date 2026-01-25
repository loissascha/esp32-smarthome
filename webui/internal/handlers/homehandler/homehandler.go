package homehandler

import (
	"net/http"

	"github.com/loissascha/go-http-server/server"
	"github.com/loissascha/go-templ-template/internal/ui/pages"
	"github.com/loissascha/go-templ-template/internal/ui/partials"
)

type HomeHandler struct {
	s *server.Server
}

func New(s *server.Server) *HomeHandler {
	return &HomeHandler{
		s: s,
	}
}

func (h *HomeHandler) RegisterHandlers(s *server.Server) {
	s.GET("/", h.homeRoute)
	s.GET("/partials/infos", h.partialsInfos)
}

func (h *HomeHandler) partialsInfos(w http.ResponseWriter, r *http.Request) {
	partials.HomeInfoComponents().Render(r.Context(), w)
}

func (h *HomeHandler) homeRoute(w http.ResponseWriter, r *http.Request) {
	homeComponent := pages.Home()
	homeComponent.Render(r.Context(), w)
}
