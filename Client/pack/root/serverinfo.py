import app
import localeInfo
import net
from constInfo import TextColor
app.ServerName = None

def GetServerID():
	serverID = 0
	for k, server_data in SERVER_LIST.items():
		if server_data["main"]["name"] == net.GetServerInfo().split(",")[0]:
			serverID = k
			break
	return serverID

SERVER_LIST = {}
def __AddServerToServerList(server_data):
	server_index = len(SERVER_LIST)

	mark_name = server_data["mark_name"]
	SERVER_LIST[server_index] = {  # serverIndex
		"main": server_data,
		"channel": {},
		"auth": {},
		"mark": {"mark": "%s.tga" % mark_name, "symbol_path": mark_name},
	}

STATE_NONE = TextColor(localeInfo.CHANNEL_STATUS_OFFLINE, "FF0000") #RED
STATE_DICT = {
	0: TextColor(localeInfo.CHANNEL_STATUS_OFFLINE, "FF0000"), 		#RED
	1: TextColor(localeInfo.CHANNEL_STATUS_RECOMMENDED, "00ff00"), 	#GREEN
	2: TextColor(localeInfo.CHANNEL_STATUS_BUSY, "ffff00"), 		#YELLOW
	3: TextColor(localeInfo.CHANNEL_STATUS_FULL, "ff8a08") 			#ORANGE
}

SERVER_DEBUG = {
	"name":TextColor("Local", "ffd500"), #GOLD
	"host":"192.168.1.4",
	"auth_base_port": 17105,
	"auth_port_increment": 0,
	"auth_port_channel_increment": 0,
	"auth_count": 1,
	"channel_base_port": 28000,
	"channel_port_increment": 10,
	"channel_count": 2,
	"mark":13800,
	"mark_name": "30",
	"premium_channels": (2,),
}

SERVER_INTERNAL = {
	"name":TextColor("Internal", "ffd500"), #GOLD
	"host":"192.168.1.4",
	"auth_base_port": 17105,
	"auth_port_increment": 0,
	"auth_port_channel_increment": 10,
	"auth_count": 1,
	"channel_base_port": 28000,
	"channel_port_increment": 10,
	"channel_count": 4,
	"mark":13800,
	"mark_name": "20",
	"premium_channels": (4,),
}

SERVER_PRODUCTION = {
	"name":TextColor("Mt2009", "ffd500"), #GOLD
	"host":"192.168.1.4",
	"auth_base_port": 17105,
	"auth_port_increment": 0,
	"auth_port_channel_increment": 0,
	"auth_count": 1,
	"channel_base_port": 28000,
	"channel_port_increment": 10,
	"channel_count": 8,
	"mark":13800,
	"mark_name": "10",
	"premium_channels": (8,),
}

if app.DEBUG_BUILD:
	__AddServerToServerList(SERVER_DEBUG)

if app.INTERNAL_BUILD or app.DEBUG_BUILD:
	__AddServerToServerList(SERVER_INTERNAL)

__AddServerToServerList(SERVER_PRODUCTION)

## channel data
for server_id, server_data in SERVER_LIST.items():
	for i in range(server_data["main"]["channel_count"]):
		channelIndex = i+1
		channelPort = server_data["main"]["channel_base_port"] + server_data["main"]["channel_port_increment"] * i
		isPremium = channelIndex in server_data["main"]["premium_channels"]
		server_data["channel"][i] = {
			"name": TextColor("CH%d%s" % (channelIndex, " |Eother/premium|e" if isPremium else ""), "FFffFF"),
			"ip": server_data["main"]["host"],
			"tcp_port": channelPort,
			"udp_port": channelPort,
			"state": STATE_NONE,
		}

## auth data
for server_id, server_data in SERVER_LIST.items():
	for i in range(server_data["main"]["channel_count"]):
		server_data["auth"][i] = {
			"ip": server_data["main"]["host"],
			"port": [],
		}

		for j in range(server_data["main"]["auth_count"]):
			authNumber = j + 1
			port = server_data["main"]["auth_base_port"] + server_data["main"]["auth_port_channel_increment"] * i + server_data["main"]["auth_port_increment"] * authNumber
			server_data["auth"][i]["port"].append(port)
