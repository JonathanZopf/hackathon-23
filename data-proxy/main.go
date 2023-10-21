package main

import (
	"fmt"
	"github.com/lesismal/nbio/nbhttp"
	"io/ioutil"
	"net/http"
	"os"
	"os/signal"
	"syscall"
)

var lastJson string

func postHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method == "POST" {
		// Read the request body
		body, err := ioutil.ReadAll(r.Body)
		if err != nil {
			http.Error(w, "Error reading request body", http.StatusBadRequest)
			return
		}

		lastJson = string(body)

		w.WriteHeader(http.StatusOK)
	} else {
		http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
	}
}

func getHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method == "GET" {

		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Content-Type", "application/json")

		// Write the JSON string to the response
		w.Write([]byte(lastJson))
	} else {
		http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
	}
}

func main() {
	mux := &http.ServeMux{}
	mux.HandleFunc("/post-sensor-data", postHandler)
	mux.HandleFunc("/get-sensor-data", getHandler)

	server := nbhttp.NewServer(nbhttp.Config{
		Network: "tcp",
		Addrs:   []string{"0.0.0.0:10000"},
		Handler: mux,
		IOMod:   nbhttp.IOModNonBlocking, //1 go - routine per conn undtil maxBlocking online reached - then uses epoller
		//MaxBlockingOnline:       500,
		ReleaseWebsocketPayload: true,
		SupportServerOnly:       false,
	})

	server.Start()

	signalChannel := make(chan os.Signal, 1)
	signal.Notify(signalChannel, os.Interrupt, syscall.SIGTERM)

	// Block and wait for a signal
	fmt.Println("Press Ctrl+C to exit...")
	<-signalChannel

}
