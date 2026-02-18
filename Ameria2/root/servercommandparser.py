# coding: latin_1

import net
import background
import stringCommander
import constInfo
import app

class ServerCommandParser(object):
	def __init__(self):
		net.SetServerCommandParserWindow(self)
		self.__ServerCommand_Build()

	def __ServerCommand_Build(self):
		serverCommandList = {
			"item_mall" : self.__ItemMall_Open,
		}

		self.serverCommander = stringCommander.Analyzer()
		for serverCommandItem in serverCommandList.items():
			self.serverCommander.SAFE_RegisterCallBack(
				serverCommandItem[0], serverCommandItem[1]
			)

	def BINARY_ServerCommand_Run(self, line):
		try:
			print " BINARY_ServerCommand_Reserve", line
			return self.serverCommander.Run(line)
		except RuntimeError, msg:
			import dbg
			dbg.TraceError(msg)
			return 0

	def __PreserveCommand(self, line):
		net.PreserveServerCommand(line)

	def __ItemMall_Open(self):
		self.__PreserveCommand("item_mall")

parserWnd = ServerCommandParser()
