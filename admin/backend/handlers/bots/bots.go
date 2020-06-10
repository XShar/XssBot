package bots

import (
	"log"
	"net/http"

	database "../../db"
	structures "../../structures"
	login "../login"

	"github.com/gin-gonic/gin"
)

var db = database.Connect()

/*
	Обработчик пути /getBotsData/
	Ответ: список ботов
*/
func GetBotsData(ctx *gin.Context) {
	// не забываем проверить авторизацию. Мы же не хотим, чтобы на наших ботов кто-то смотрел помимо нас?
	if !login.CtxAuthCheck(ctx) {
		return
	}

	var botsData []structures.BotData
	// достаём всех ботов из бд
	err := db.Select(&botsData, "SELECT * FROM `bots` ORDER BY `Id` DESC")
	if err != nil {
		log.Println("bots.GetBotsData:", err)
	}

	// если ботов нет - так и ответим
	if len(botsData) < 1 {
		ctx.JSON(http.StatusOK, "")
		return
	}
	ctx.JSON(http.StatusOK, botsData)
}
