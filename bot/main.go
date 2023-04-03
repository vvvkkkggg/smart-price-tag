package main

import (
	tgbotapi "gopkg.in/telegram-bot-api.v4"
	"log"
	"os"
)

func main() {
	u := tgbotapi.NewUpdate(0)
	u.Timeout = 60
	msgChan := make(chan tgbotapi.MessageConfig)

	var bot, _ = tgbotapi.NewBotAPI(os.Getenv("TG_BOT_TOKEN"))

	// send messages from msg channel
	go func(messages chan tgbotapi.MessageConfig) {
		for msg := range messages {
			bot.Send(msg)
		}
	}(msgChan)

	if updates, err := bot.GetUpdatesChan(u); err == nil {
		for u := range updates {
			switch {
			case u.Message != nil:
				go handleMsg(u, msgChan)
			}
		}
	} else {
		log.Fatal(err.Error())
	}
}
