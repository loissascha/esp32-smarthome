package espservice

import (
	"time"
)

type ESPService struct {
}

func New() *ESPService {
	return &ESPService{}
}

func (e *ESPService) RunSync() {
	for {
		time.Sleep(10 * time.Second)
		e.sync()
	}
}

func (e *ESPService) sync() {

}
