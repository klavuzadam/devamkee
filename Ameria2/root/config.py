LOCAL_SERVER = False

SERVER_IP = "94.23.77.86"
AUTH_PORT = 39994
CH1_PORT = 13002

if LOCAL_SERVER:
	SERVER_IP = "127.0.0.1"
	AUTH_PORT = 11000
	CH1_PORT = 13000

SERVER_SETTINGS = {
	"SERVER_NAME" : "Ameria",
	"IP" : SERVER_IP,
	"CH1" :	CH1_PORT,
	"CH2" : 13102,
	"CH3" : 13202,
	"CH4" : 13302,
	"CH5" : 13402,	
	"AUTH" : AUTH_PORT,	
	"MARK" : CH1_PORT,		
}

MARKADDR	= 13002

CHANNELS_DICT = {
	1:{"key":11,"port":SERVER_SETTINGS["CH1"],},
	2:{"key":12,"port":SERVER_SETTINGS["CH2"],},
	3:{"key":13,"port":SERVER_SETTINGS["CH3"],},
	4:{"key":14,"port":SERVER_SETTINGS["CH4"],},
	5:{"key":15,"port":SERVER_SETTINGS["CH5"],},
}


MARKADDR_DICT = {
		10 : { "ip" :SERVER_SETTINGS["IP"], "tcp_port" : 29001, "mark" : "10.tga", "symbol_path" : "10", }, #
		20 : { "ip" :SERVER_SETTINGS["IP"], "tcp_port" : 29101, "mark" : "20.tga", "symbol_path" : "20", }, #
		30 : { "ip" :SERVER_SETTINGS["IP"], "tcp_port" : 29201, "mark" : "30.tga", "symbol_path" : "30", }, #
		40 : { "ip" :SERVER_SETTINGS["IP"], "tcp_port" : 29301, "mark" : "40.tga", "symbol_path" : "40", }, #
}
