package login

import (
	"log"
	"net/http"

	config "../../config"
	database "../../db"
	obf "../gate/obf"

	"github.com/gin-gonic/gin"
	"golang.org/x/crypto/bcrypt"
)

var db = database.Connect()

/*
	Генерация и запись токена в бд
*/
func SetToken(user string) string {
	token := obf.GenRandStr(32)
	_, err := db.Exec("UPDATE `users` SET `Token` = ? WHERE `Username` = ?", token, user)
	if err != nil {
		log.Println("login.SetToken:", err)
	}

	return token
}

type LoginData struct {
	Username string `json:"username"`
	Password string `json:"password"`
}

type UsersData struct {
	Id           int    `db:"Id" json:"id"`
	Username     string `db:"Username" json:"username"`
	PasswordHash string `db:"PasswordHash" json:"password_hash"`
	Token        string `db:"Token" json:"token"`
}

/*
	Функция возвращает список всех юзеров
*/
func GetUsersData() (usersData []UsersData) {
	err := db.Select(&usersData, "SELECT * FROM `users` ORDER BY `Id` DESC")
	if err != nil {
		log.Println("login.GetUsersData:", err)
	}
	return
}

type CookieData struct {
	Cookie string `json:"cookie"`
	Domain string `json:"domain"`
}

/*
	Обработчик пути /login/
*/
func Login(ctx *gin.Context) {
	var data LoginData

	// биндим данные из запроса в структуру
	ctx.BindJSON(&data)

	var passwordHash string
	err := db.Get(&passwordHash, "SELECT `PasswordHash` FROM `users` WHERE `Username` = ?", data.Username)
	if err != nil {
		log.Println("login.Login:", err)
	}

	// сверяем хэш пароля из бд и пароль, введённый юзером
	if bcrypt.CompareHashAndPassword([]byte(passwordHash), []byte(data.Password)) == nil {
		// если пароль совпадает - обновляем токен
		token := SetToken(data.Username)
		tokenHash, err := bcrypt.GenerateFromPassword([]byte(token), bcrypt.MinCost)
		if err != nil {
			log.Println("login.Login:", err)
		}
		cookieData := CookieData{
			Cookie: string(tokenHash),
			Domain: config.GetDomain(),
		}

		// в ответ высылаем куки
		ctx.JSON(http.StatusOK, cookieData)
	}
}

/*
	Функция возвращает имя юзера по хэшу токена
*/
func IsUserValid(cookie string) (username string) {
	usersData := GetUsersData()
	for _, user := range usersData {
		if bcrypt.CompareHashAndPassword([]byte(cookie), []byte(user.Token)) == nil {
			username = user.Username
			return username
		}
	}

	return ""
}

func CtxAuthCheck(ctx *gin.Context) bool {
	var cookie CookieData
	ctx.BindJSON(&cookie)
	username := IsUserValid(cookie.Cookie)
	if username == "" {
		ctx.Status(http.StatusUnauthorized)
		return false
	}
	return true
}

func AuthCheck(cookie string) bool {
	username := IsUserValid(cookie)
	if username == "" {
		return false
	}
	return true
}

/*
	Обработчик пути /getCurrentUser/
*/
func GetCurrentUser(ctx *gin.Context) {
	var cookie CookieData

	// биндим данные в структуру
	ctx.BindJSON(&cookie)

	// проверяем, валидна ли кука
	username := IsUserValid(cookie.Cookie)
	if username != "" {
		// еслив алидна - отвечаем шлём имя юзера
		ctx.JSON(http.StatusOK, gin.H{
			"username": username,
		})
	}
}
