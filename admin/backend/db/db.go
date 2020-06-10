package db

import (
	"log"

	"github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
)

/*
	Функция возвращает инстанс базы данных
*/
func Connect() *sqlx.DB {
	// пробуем открыть соединение с бд
	db, err := sqlx.Open("sqlite3", "./dump/xss_bot.db")
	if err != nil {
		// не получилось - валимся с ошибкой
		log.Fatal("db.Connect:", err)
	}

	// проверяем соединение с БД
	err = db.Ping()
	if err != nil {
		// что-то пошло не так - валимся с ошибкой
		log.Fatal("db.Connect:", err)
	}

	return db
}
