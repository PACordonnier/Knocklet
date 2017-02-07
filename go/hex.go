//Code test, utilisation du paquet hex de go
package main

import (
	"encoding/hex"
	"fmt"
)

func main() {
	hex, err := hex.DecodeString("4B4E4F000002")
	if(err != nil) {
		fmt.Println("error")
		return
	}
	fmt.Println(len(hex))
	fmt.Println("Hello world !")
}
