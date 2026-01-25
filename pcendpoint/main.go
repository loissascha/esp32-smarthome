package main

import (
	"fmt"
	"net/http"
)

const webhookTurnOnURL = "http://192.168.0.96:8123/api/webhook/-rPs1a5lV1u48t_Pl9twvFonH"
const webhookTurnOffURL = "http://192.168.0.96:8123/api/webhook/-oEHY9OBuaU2bJ5PHoWEvpsHp"
const webhookURL = "http://192.168.0.96:8123/api/webhook/-eZjgtjaFiZW8_bz3vcDO2bkO"

func pingHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Received ping request")
	fmt.Fprint(w, "pong")

	// Trigger webhook asynchronously
	// go func() {
	// 	resp, err := http.Post(webhookTurnOffURL, "application/json", nil)
	// 	if err != nil {
	// 		fmt.Println("Error triggering webhook:", err)
	// 		return
	// 	}
	// 	defer resp.Body.Close()
	// 	if resp.StatusCode >= 200 && resp.StatusCode < 300 {
	// 		fmt.Println("Webhook triggered successfully")
	// 	} else {
	// 		fmt.Printf("Webhook failed with status: %d\n", resp.StatusCode)
	// 	}
	// }()
}

func main() {
	http.HandleFunc("/ping", pingHandler)
	http.ListenAndServe("0.0.0.0:4444", nil)
}
