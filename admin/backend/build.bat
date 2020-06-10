set GOOS=windows
set GOARCH=amd64
go build -ldflags="-w -s" main.go