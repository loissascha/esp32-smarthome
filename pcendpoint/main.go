package main

import (
	"fmt"
	"net/http"
)

func pingHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Received ping request")
	fmt.Fprint(w, "pong")
}

func main() {
	fmt.Println("starting...")
	http.HandleFunc("/ping", pingHandler)
	err := http.ListenAndServe("0.0.0.0:4444", nil)
	if err != nil {
		panic(err)
	}
}
