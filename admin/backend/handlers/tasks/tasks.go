package tasks

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
	Обработчик пути /getTasksData/
	Ответ: список тасков
*/
func GetTasksData(ctx *gin.Context) {
	// не забываем проверить авторизацию
	if !login.CtxAuthCheck(ctx) {
		return
	}

	var tasksData []structures.TaskData
	// достаём все задания из бд
	err := db.Select(&tasksData, "SELECT * FROM `tasks` ORDER BY `Id` DESC")
	if err != nil {
		log.Println("tasks.GetTasksData:", err)
	}

	// если заданий нет - так и ответим
	if len(tasksData) < 1 {
		ctx.JSON(http.StatusOK, "")
		return
	}
	ctx.JSON(http.StatusOK, tasksData)
}

type TaskForm struct {
	Cookie string              `json:"cookie"`
	Form   structures.TaskData `json:"form"`
}

/*
	Создание задания
*/
func CreateTask(ctx *gin.Context) {
	var taskForm TaskForm

	// биндим в структуру
	ctx.BindJSON(&taskForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(taskForm.Cookie) {
		return
	}

	// записываем в бд
	_, err := db.Exec("INSERT INTO `tasks` (`Type`, `ModuleName`, `Param`, `Limit`, `Uids`) VALUES(?, ?, ?, ?, ?)",
		taskForm.Form.Type,
		taskForm.Form.ModuleName,
		taskForm.Form.Param,
		taskForm.Form.Limit,
		taskForm.Form.Uids,
	)
	if err != nil {
		log.Println("tasks.CreateTask:", err)
	}

	// и всегда отвечаем кодом 200, пока что у нас ошибки на стороне клиента нигде не отображаются
	ctx.Status(http.StatusOK)
}

/*
	Запуск задания
*/
func StartTask(ctx *gin.Context) {
	var taskForm TaskForm

	// биндим в структуру
	ctx.BindJSON(&taskForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(taskForm.Cookie) {
		return
	}

	// обновляем запись в бд
	_, err := db.Exec("UPDATE `tasks` SET `IsEnabled` = NOT `IsEnabled` WHERE `Id` = ?",
		taskForm.Form.Id,
	)
	if err != nil {
		log.Println("tasks.StartTask:", err)
	}

	// отвечаем кодом 200
	ctx.Status(http.StatusOK)
}

/*
	Редактирование задания
*/
func EditTask(ctx *gin.Context) {
	var taskForm TaskForm

	// биндим в структуру
	ctx.BindJSON(&taskForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(taskForm.Cookie) {
		return
	}

	// обновляем запись в бд
	_, err := db.Exec("UPDATE `tasks` SET `Type` = ?, `ModuleName` = ?, `Param` = ?, `Limit` = ?, `Uids` = ? WHERE `Id` = ?",
		taskForm.Form.Type,
		taskForm.Form.ModuleName,
		taskForm.Form.Param,
		taskForm.Form.Limit,
		taskForm.Form.Uids,
		taskForm.Form.Id,
	)
	if err != nil {
		log.Println("tasks.EditTask:", err)
	}

	// отвечаем кодом 200
	ctx.Status(http.StatusOK)
}

/*
	Удаление задания
*/
func DeleteTask(ctx *gin.Context) {
	var taskForm TaskForm

	// биндим в структуру
	ctx.BindJSON(&taskForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(taskForm.Cookie) {
		return
	}

	// удаляем запись из бд
	_, err := db.Exec("DELETE FROM `tasks` WHERE `Id` = ?", taskForm.Form.Id)
	if err != nil {
		log.Println("tasks.DeleteTask:", err)
	}

	// удаляем все ошибки с этим задание из тыблицы
	_, err = db.Exec("DELETE FROM `task_errors` WHERE `TaskId` = ?", taskForm.Form.Id)
	if err != nil {
		log.Println("tasks.DeleteTask:", err)
	}

	// отвечаем кодом 200
	ctx.Status(http.StatusOK)
}

/*
	Обработчик пути /getTaskErrors/
*/
func GetTaskErrors(ctx *gin.Context) {
	var taskForm struct {
		Cookie string `json:"cookie"`
		TaskId int    `json:"task_id"`
	}

	// биндим в структуру
	ctx.BindJSON(&taskForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(taskForm.Cookie) {
		return
	}

	var taskErrors []structures.CompletedTaskData
	err := db.Select(&taskErrors, "SELECT * FROM `task_errors` WHERE `TaskId` = ?", taskForm.TaskId)
	if err != nil {
		log.Println("tasks.GetTaskErrors:", err)
	}

	// если ошибок нет - так и ответим
	if len(taskErrors) < 1 {
		ctx.JSON(http.StatusOK, "")
		return
	}
	ctx.JSON(http.StatusOK, taskErrors)
}
