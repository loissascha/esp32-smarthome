package main

import (
	"fmt"
	"net/http"
	"os"
	"time"

	"github.com/joho/godotenv"
	"github.com/loissascha/go-http-server/server"
	"github.com/loissascha/go-logger/logger"
	"github.com/loissascha/go-templ-template/internal/handlers/homehandler"
	"github.com/loissascha/go-templ-template/internal/services/espservice"
	"github.com/loissascha/go-templ-template/internal/services/haservice"
)

func main() {
	godotenv.Load()

	port := os.Getenv("PORT")
	if port == "" {
		panic("PORT not defined. Make sure there is a .env file or a environment variable set!")
	}

	s, err := server.NewServer()
	if err != nil {
		panic(err)
	}

	// service
	espS := espservice.New()
	haS := haservice.New()

	err = haS.LoadLightStates()
	if err != nil {
		panic(err)
	}

	for _, l := range haS.Lights {
		haS.TurnOnLight(l)
	}
	time.Sleep(5 * time.Second)
	for _, l := range haS.Lights {
		haS.TurnOffLight(l)
	}

	go func() {
		espS.RunSync()
	}()

	// handler
	homeH := homehandler.New(s, espS)

	// register routes
	homeH.RegisterHandlers(s)

	fs := http.FileServer(http.Dir("./static"))
	s.GetMux().Handle("/static/", http.StripPrefix("/static/", fs))

	logger.Info(nil, "Server starting at port: {port}", port)
	err = s.Serve(fmt.Sprintf(":%v", port))
	if err != nil {
		logger.Error(err, "Server failed to start...")
	}
}
