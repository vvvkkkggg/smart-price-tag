package main

import (
	"fmt"
	tgbotapi "gopkg.in/telegram-bot-api.v4"
	"log"
)

func sendGreeting(u tgbotapi.Update, msgChan chan<- tgbotapi.MessageConfig) {
	msgChan <- tgbotapi.MessageConfig{
		BaseChat:              tgbotapi.BaseChat{ChatID: u.Message.Chat.ID, ReplyToMessageID: u.Message.MessageID},
		Text:                  "Здравствуйте! Установка цены осуществляется с помощью команды \"/setprice *артикул* *цена*\"",
		DisableWebPagePreview: false,
	}
}

func handleMsg(update tgbotapi.Update, msgChan chan tgbotapi.MessageConfig) {
	if update.Message.Text == "/start" {
		sendGreeting(update, msgChan)
		return
	}
	var article int64
	var price float64
	var msgText string
	if n, err := fmt.Sscanf(update.Message.Text, "/setprice %d %f", &article, &price); n != 2 || err != nil {
		log.Println("Unable to set price: ", err.Error())
		msgText = "Неверный формат команды \"/setprice *артикул* *цена*\""
	} else {
		// TODO: send request to backend
		msgText = "Цена обновлена"
	}
	msgChan <- tgbotapi.MessageConfig{
		BaseChat:              tgbotapi.BaseChat{ChatID: update.Message.Chat.ID, ReplyToMessageID: update.Message.MessageID},
		Text:                  msgText,
		DisableWebPagePreview: false,
	}
	return
}
