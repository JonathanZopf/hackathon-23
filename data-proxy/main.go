package main

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/hex"
	"fmt"
	"github.com/lesismal/llib/std/crypto/tls"
	"github.com/lesismal/nbio/nbhttp"
	"io/ioutil"
	"net/http"
	"os"
	"os/signal"
	"syscall"
)

var lastJson string
var key string

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

func decryptFile(keyString string, fileToDecode string) ([]byte, error) {

	key, err := hex.DecodeString(keyString)
	if err != nil {
		return nil, err
	}

	encoded, err := os.ReadFile(fileToDecode)
	if err != nil {
		return nil, err
	}

	//Create a new Cipher Block from the key
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	//Create a new GCM
	aesGCM, err := cipher.NewGCM(block)
	if err != nil {
		return nil, err
	}

	//Get the nonce size
	nonceSize := aesGCM.NonceSize()

	//Extract the nonce from the encrypted data
	nonce, ciphertext := encoded[:nonceSize], encoded[nonceSize:]

	//Decrypt the data
	return aesGCM.Open(nil, nonce, ciphertext, nil)
}

func getWebserverTlsConfig() *tls.Config {

	rsaServerCertExternal, err := decryptFile(key, "/opt/DataProxy/fullchain.pem.enc")
	if err != nil {
		fmt.Println("Could not parse external Server Cert")
	}
	rsaServerPrivExternal, err := decryptFile(key, "/opt/DataProxy/privkey.pem.enc")
	if err != nil {
		fmt.Println("Could not parse external Server Key")
	}
	certExternal, err := tls.X509KeyPair(rsaServerCertExternal, rsaServerPrivExternal)
	if err != nil {
		fmt.Println("tls.X509KeyPair failed for server")
	}

	return &tls.Config{
		Certificates:       []tls.Certificate{certExternal},
		InsecureSkipVerify: true,
	}
}

func main() {
	mux := &http.ServeMux{}
	mux.HandleFunc("/post-sensor-data", postHandler)
	mux.HandleFunc("/get-sensor-data", getHandler)

	server := nbhttp.NewServer(nbhttp.Config{
		Network:  "tcp",
		AddrsTLS: []string{"0.0.0.0:10000"},
		Handler:  mux,
		IOMod:    nbhttp.IOModNonBlocking, //1 go - routine per conn until maxBlocking online reached - then uses epoller
		//MaxBlockingOnline:       500,
		ReleaseWebsocketPayload: true,
		SupportServerOnly:       true,
		TLSConfig:               getWebserverTlsConfig(),
	})

	server.Start()

	signalChannel := make(chan os.Signal, 1)
	signal.Notify(signalChannel, os.Interrupt, syscall.SIGTERM)

	// Block and wait for a signal
	fmt.Println("Press Ctrl+C to exit...")
	<-signalChannel

}
