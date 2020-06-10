package structures

type TaskData struct {
	Id         int    `db:"Id" json:"id"`
	Type       string `db:"Type" json:"type"`
	ModuleName string `db:"ModuleName" json:"module_name"`
	Param      string `db:"Param" json:"param"`
	Limit      int    `db:"Limit" json:"limit"`
	Loads      int    `db:"Loads" json:"loads"`
	Runs       int    `db:"Runs" json:"runs"`
	Errors     int    `db:"Errors" json:"errors"`
	Uids       string `db:"Uids" json:"uids"`
	IsEnabled  bool   `db:"IsEnabled" json:"is_enabled"`
}

type CompletedTaskData struct {
	TaskId           string `db:"TaskId" json:"task_id"`
	Uid              string `db:"Uid" json:"bot_uid"`
	Error            string `db:"Error" json:"error"`
	GetLastErrorCode string `db:"GetLastErrorCode" json:"getlasterror_code"`
}

type BotData struct {
	Id               int    `db:"Id" json:"id"`
	Uid              string `db:"Uid" json:"uid"`
	Ip               string `db:"Ip" json:"ip"`
	Country          string `db:"Country" json:"country"`
	WinVer           string `db:"WinVer" json:"win_ver"`
	Username         string `db:"Username" json:"username"`
	Computername     string `db:"Computername" json:"computername"`
	CpuName          string `db:"CpuName" json:"cpu_name"`
	GpuName          string `db:"GpuName" json:"gpu_name"`
	RamAmount        string `db:"RamAmount" json:"ram_amount"`
	ScreenResolution string `db:"ScreenResolution" json:"screen_resolution"`
	ActiveWindow     string `db:"ActiveWindow" json:"active_window"`
	Joined           int64  `db:"Joined" json:"joined"`
	Seen             int64  `db:"Seen" json:"seen"`
	TaskId           int    `db:"TaskId" json:"task_id"`
}
