package users

import (
	"log"
	"net/http"

	database "../../db"
	login "../login"
	"golang.org/x/crypto/bcrypt"

	"github.com/gin-gonic/gin"
)

var db = database.Connect()

/*
	Обработчик пути /getUsersData/
	Ответ: список юзеров
*/
func GetUsersData(ctx *gin.Context) {
	// не забываем проверить авторизацию
	if !login.CtxAuthCheck(ctx) {
		return
	}

	var usersData []struct {
		Username string `db:"Username" json:"username"`
	}
	// достаём всех юзеров из бд
	err := db.Select(&usersData, "SELECT `Username` FROM `users` ORDER BY `Id` DESC")
	if err != nil {
		log.Println("users.GetUsersData:", err)
	}

	ctx.JSON(http.StatusOK, usersData)
}

/*
	Обработчик пути /updateUser/
*/
func UpdateUser(ctx *gin.Context) {
	var userForm struct {
		Cookie string `json:"cookie"`
		Form   struct {
			Username string `json:"username"`
			Password string `json:"password"`
		} `json:"form"`
	}

	// биндим в структуру
	ctx.BindJSON(&userForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(userForm.Cookie) {
		return
	}

	// получаем имя юзера из куки
	username := login.IsUserValid(userForm.Cookie)

	// если указан юзернейм - меняем
	if userForm.Form.Username != "" {
		_, err := db.Exec("UPDATE `users` SET `Username` = ? WHERE `Username` = ?", userForm.Form.Username, username)
		if err != nil {
			log.Println("users.UpdateUser:", err)
		}
	}

	// если указан пароль - меняем
	if userForm.Form.Password != "" {
		// хэшируем пароль
		hash, err := bcrypt.GenerateFromPassword([]byte(userForm.Form.Password), bcrypt.MinCost)
		if err != nil {
			log.Println("users.UpdateUser:", err)
		}

		var usernameUpdated string

		// если юзер так же обновил имя - обновляем пароль для юзера с нвоым именем
		if userForm.Form.Username != "" {
			usernameUpdated = userForm.Form.Username
		} else {
			// иначе используем старое имя
			usernameUpdated = username
		}
		// пишем хэш нового пароля в базу
		_, err = db.Exec("UPDATE `users` SET `PasswordHash` = ? WHERE `Username` = ?", string(hash), usernameUpdated)
		if err != nil {
			log.Println("users.UpdateUser:", err)
		}
	}

	// интересно, эти комментарии кто-то читает?
	ctx.Status(http.StatusOK)
}

/*
	Обработчик пути /createUser/
*/
func CreateUser(ctx *gin.Context) {
	var userForm struct {
		Cookie string `json:"cookie"`
		Form   struct {
			Username string `json:"username"`
			Password string `json:"password"`
		} `json:"form"`
	}

	// биндим в структуру
	ctx.BindJSON(&userForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(userForm.Cookie) {
		return
	}

	// хэшируем пароль
	hash, err := bcrypt.GenerateFromPassword([]byte(userForm.Form.Password), bcrypt.MinCost)
	if err != nil {
		log.Println("users.CreateUser:", err)
	}

	// записываем в таблицу с юзерами
	_, err = db.Exec("INSERT INTO `users` (`Username`, `PasswordHash`, `Token`) VALUES(?, ?, ?)", userForm.Form.Username, string(hash), 0)
	if err != nil {
		log.Println("users.CreateUser:", err)
	}

	ctx.Status(http.StatusOK)
}

/*
	Обработчик пути /delUser/
*/
func DelUser(ctx *gin.Context) {
	var userForm struct {
		Cookie   string `json:"cookie"`
		Username string `json:"username"`
	}

	// биндим в структуру
	ctx.BindJSON(&userForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(userForm.Cookie) {
		return
	}

	// удаляем юзера из таблицы
	_, err := db.Exec("DELETE FROM `users` WHERE `Username` = ?", userForm.Username)
	if err != nil {
		log.Println("users.DelUser:", err)
	}

	ctx.Status(http.StatusOK)
}
