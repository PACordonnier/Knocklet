// +build

package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"strings"
	"time"
	"encoding/json"
	"bytes"
	"net/http"
	"encoding/binary"
	//"encoding/hex"

	"github.com/paypal/gatt"
	"github.com/paypal/gatt/examples/option"
)

var done = make(chan struct{})
var lastTime time.Time = time.Now()

type Params struct {
	Apikey string `json:"apikey"`
	BraceletId string `json:"braceletId"`
	ModuleId string `json:"moduleId"`
	Rssi int `json:"rssi"`
	Knocks int `json:"knocks"`
}

type Payload struct {
	Method string `json:"method"`
	Jsonrpc string `json:"jsonrpc"`
	Params Params `json:"params"`
}

func onStateChanged(d gatt.Device, s gatt.State) {
	fmt.Println("State:", s)
	switch s {
	case gatt.StatePoweredOn:
		fmt.Println("Scanning...")
		//hex, err := hex.DecodeString("4B4E4F000002")
		//if (err != nil){
		//	log.Printf("error")
		//	return
		//}
		//var arr [6]byte
		//copy(arr[:], hex)
		//d.AddDeviceToWhiteList(arr)
		d.Scan([]gatt.UUID{}, true)
		return
	default:
		d.StopScanning()
	}
}

func sendToJeedom(braceletId string, moduleId string, knocks int, rssi int) {
	params := Params{
		Apikey: "lbippE2VJkuxVdfI4sKtZWQkCTwNJxH517kYVYEDLBbUe2eD",
		BraceletId: braceletId,
		ModuleId: moduleId,
		Rssi: rssi,
		Knocks: knocks,
	}
	method :=""
	if knocks == 255 {
		method = "init"
	} else {
		method = "knock"
	}
	payload := Payload{
		Method: method,
		Jsonrpc: "2.0",
		Params: params,
	}
	b,_ := json.Marshal(payload)
	url := "http://jeedom.local/plugins/knocklet/core/api/knocklet.api.php"
	req, err := http.NewRequest("POST", url, bytes.NewBuffer(b))
	req.Header.Set("Content-Type",  "application/json")
	client := &http.Client{}
	resp, err := client.Do(req)
	fmt.Println(string(b))
	if err != nil {
		panic(err)
	}
	defer resp.Body.Close()
}

func onPeriphDiscovered(p gatt.Peripheral, a *gatt.Advertisement, rssi int) {
	id := strings.ToUpper(flag.Args()[0])
	if strings.ToUpper(p.ID()) != id {
		return
	}
	fmt.Println("coucou")
	if (time.Since(lastTime).Seconds() > 2.55555) {
		fmt.Printf("\nPeripheral ID:%s, NAME:(%s)\n", p.ID(), p.Name())
		fmt.Println("  Local Name        =", a.LocalName)
		fmt.Println("  TX Power Level    =", a.TxPowerLevel)
		fmt.Println("  Manufacturer Data =", a.ManufacturerData)
		fmt.Println("  Service Data      =", a.ServiceData)
		fmt.Println("  RSSI		 =", rssi)
		fmt.Println("")
		lastTime = time.Now()
		h := []byte{0}
		val := append(h, a.ServiceData[0].Data[0])
		sendToJeedom(p.ID(), "5555555", int(binary.BigEndian.Uint16(val)), rssi)
		fmt.Println("end")
	}
}

func main() {
	flag.Parse()
	if len(flag.Args()) != 1 {
		log.Fatalf("usage: %s [options] peripheral-id\n", os.Args[0])
	}

	d, err := gatt.NewDevice(option.DefaultClientOptions...)
	if err != nil {
		log.Fatalf("Failed to open device, err: %s\n", err)
		return
	}

	// Register handlers.
	d.Handle(
		gatt.PeripheralDiscovered(onPeriphDiscovered),
	)

	lastTime = time.Now()
	d.Init(onStateChanged)
	<-done
	fmt.Println("Done")
}
