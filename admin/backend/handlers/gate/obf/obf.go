package obf

import (
	"encoding/base64"
	"log"
	"math/rand"
	"strings"
	"time"

	"gopkg.in/goyy/goyy.v0/util/crypto/rc4"
)

// массив случайных символов (рун)
var letterRunes = []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")

// инициализация ГСЧ
func init() {
	rand.Seed(time.Now().UnixNano())
}

/*
	Функция возвращает случайную строку длинной n символов
	Да, не совсем секьюрно юзать обычный math/rand, вместо него можно использовать crypto/rand
*/
func GenRandStr(n int) string {
	// создаём массив из n символов (рун)
	b := make([]rune, n)
	for i := range b {
		// в цикле берём случайный символ из нашего массива символов и пишем в созданный нами массив выше
		b[i] = letterRunes[rand.Intn(len(letterRunes))]
	}
	// кастуем к строке
	return string(b)
}

var keyLen = 32 // длинна ключа 32 символа

/*
	Функция возвращает зашифрованную строку data
*/
func Encrypt(data string) string {
	// генерируем случайный ключ
	encKey := GenRandStr(keyLen)

	// шифруем наши данные с помощью RC4 с ключём, что мы сгенерировали выше
	data_rc4, err := rc4.Encrypt([]byte(data), []byte(encKey))
	if err != nil {
		// если что-то пошло не так - выводим в терминал ошибку
		log.Println("obf.Encrypt:", err)
	}

	// перегоняем зашифрованные данные в base64 строку
	encrypted_data := base64.StdEncoding.EncodeToString(data_rc4)

	// дописываем ключ в начало base64 строки и возврвщаем
	return encKey + encrypted_data
}

/*
	Функция возвращает расшифрованную строку data
*/
func Decrypt(data string) string {
	dataLen := len(data)
	encKey := data[0:keyLen] // обрезаем ключ

	// заменяем все пробелы на +, обрезаем ключ
	data_base64 := strings.ReplaceAll(data[keyLen:dataLen], " ", "+")

	// снимаем base64
	data_rc4, err := base64.StdEncoding.DecodeString(data_base64)
	if err != nil {
		log.Println("obf.Decode:", err)
	}

	// расшифровываем RC4
	data_decrypted, err := rc4.Decrypt(data_rc4, []byte(encKey))
	if err != nil {
		log.Println("obf.Decode:", err)
	}

	// кастуем к строке
	return string(data_decrypted)
}
