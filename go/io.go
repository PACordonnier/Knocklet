//Code test, lecture de JSOn dans un fichier
package main

import (
	"fmt"
	"io/ioutil"
)

type Address struct {
	address string
type List struct {
	List []Address `json:"list"`
}
func check(e error) {
    if e != nil {
	    panic(e)
    }
}


func main() {
	dat, err := ioutil.ReadFile("/home/pi/go/address.json")
	check(err)
	fmt.Println(string(dat))
}
