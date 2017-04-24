import os
from dranalyse import analyse
import json

import json
from twisted.web.server import NOT_DONE_YET
import sqlite3
import os
import shutil
#some states
ERROR = -1
UPLOADING = 0
PROCESSING = 1
LOGVIEWABLE = 2
#load some db confs
with open("conf/db.json","r") as io :
	confs = json.loads(io.read())
dbconfs = confs["dbconfig"]

def processlog(logid,name,typeoflog):
	
	logManager = LogManager(logid,name,typeoflog)
	
	# let start analysing 
	
	logManager.analyse()

	


class LogManager(object) :









	def __init__(self, logid , name, typeoflog) :
	
	
		self.logid = logid
		
		self.name = name
		
		self.typeoflog = typeoflog
	
	
	
	def analyse(self) :
		
		# call analyser
		
		logfilelocation = "data/" + self.name + "/upload/upload.log"
		wheretostoreresult = "data/" + self.name + "/bin/"
		try :
			notdone = analyse(logfilelocation,wheretostoreresult,self.typeoflog)
		except Exception as e:
			notdone = 1
			print e
		con =  sqlite3.connect(dbconfs["db"])
	
		with con:
			cur = con.cursor()
			if notdone == 0 :
				cur.execute("UPDATE LogFiles SET State=?  WHERE Id=?",(LOGVIEWABLE,self.logid))
			else :
				cur.execute("UPDATE LogFiles SET State=?  WHERE Id=?",(ERROR,self.logid))
		con.close()
		
