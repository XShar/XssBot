package config

import (
	"io/ioutil"
	"log"
)

func GetDomain() string {
	file, err := ioutil.ReadFile("./domain")
	if err != nil {
		log.Fatal("config.GetConfig:", err)
	}

	return string(file)
}
