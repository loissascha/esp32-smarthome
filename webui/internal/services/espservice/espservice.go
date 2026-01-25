package espservice

import (
	"fmt"
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
		fmt.Println("Syncing data...")
	}
}
