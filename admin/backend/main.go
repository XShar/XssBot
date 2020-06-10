package main

import (
	"log"
	"net/http"

	bots "./handlers/bots"
	gate "./handlers/gate"
	login "./handlers/login"
	modules "./handlers/modules"
	tasks "./handlers/tasks"
	users "./handlers/users"

	"github.com/gin-gonic/gin"
)

// затычка чтобы vue-axios не трахал мозг
func LiberalCORS(ctx *gin.Context) {
	ctx.Header("Access-Control-Allow-Origin", "*")
	if ctx.Request.Method == "OPTIONS" {
		if len(ctx.Request.Header["Access-Control-Request-Headers"]) > 0 {
			ctx.Header("Access-Control-Allow-Headers", ctx.Request.Header["Access-Control-Request-Headers"][0])
		}
		ctx.AbortWithStatus(http.StatusOK)
	}
}

// поставьте тут false, если нужен продакшен билд
var DEBUG_BUILD = false

func main() {
	var router *gin.Engine
	if DEBUG_BUILD {
		// используем роутер с мидлварями
		router = gin.Default()
	} else {
		gin.SetMode(gin.ReleaseMode)
		router = gin.New()
	}

	router.Use(LiberalCORS)

	// добавляем обработку POST-запросов по пути domain.com/gate/*тип гейта*
	router.POST("/gate/:type/", gate.Handler)
	router.POST("/getModule/", modules.GetModule)

	router.POST("/login/", login.Login)
	router.POST("/getCurrentUser/", login.GetCurrentUser)
	router.POST("/getBotsData/", bots.GetBotsData)
	router.POST("/getTasksData/", tasks.GetTasksData)
	router.POST("/taskCreate/", tasks.CreateTask)
	router.POST("/taskStart/", tasks.StartTask)
	router.POST("/taskEdit/", tasks.EditTask)
	router.POST("/taskDelete/", tasks.DeleteTask)
	router.POST("/getTaskErrors/", tasks.GetTaskErrors)
	router.POST("/getModulesData/", modules.GetModulesData)
	router.POST("/uploadModule/", modules.UploadModule)
	router.POST("/delModule/", modules.DelModule)
	router.POST("/getUsersData/", users.GetUsersData)
	router.POST("/updateUser/", users.UpdateUser)
	router.POST("/createUser/", users.CreateUser)
	router.POST("/delUser/", users.DelUser)

	// добавляем роут статк файлов. Позже в этой папке у нас будет продакшен версия фронта
	router.Static("/", "./dist/")

	// стартуем сервер на 80-ом порту
	err := router.Run(":80")
	if err != nil {
		// валимся с ошибкой
		log.Fatal(err)
	}
}
