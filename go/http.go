// Code exemple, utilisation de HTTP
package main

import (
	"bytes"
	"fmt"
	"encoding/json"
	"net/http"
)

type Params struct {
	Apikey string `json:"apikey"`
	BraceletId int `json:"braceletId"`
	ModuleId int `json:"moduleId"`
	Rssi int `json:"rssi"`
}

type Payload struct {
	Method string `json:"method"`
	Jsonrpc string `json:"jsonrpc"`
	Params Params `json:"params"`
}
func main() {
	params := Params{
		Apikey: "osdozhuizifzeui",
		BraceletId: 1,
		ModuleId: 1,
		Rssi: -68,
	}
	payload := Payload{
		Method: "knock",
		Jsonrpc: "2.0",
		Params: params,
	}
	b,_ := json.Marshal(payload)
	url := "http://jeedom.local/plugins/knocklet/core/api/knocklet.api.php"
	req, err := http.NewRequest("POST", url, bytes.NewBuffer(b))
	req.Header.Set("Content-Type",  "application/json")
	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		panic(err)
	}
	defer resp.Body.Close()

	fmt.Println("response Status:", resp.Status)
}
