#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
from twisted.web.server import NOT_DONE_YET
import sqlite3
import os
import shutil


def init():
	
	# here we get database config 
	with open("conf/db.json","r") as io :
		confs = json.loads(io.read())
	dbconfs = confs["dbconfig"]

	# if it's first time we create all table etc ....

	if confs["firstime"] :
		initsqlitedb(dbconfs)
		if confs["clean_data"] :
			
			try :
				shutil.rmtree('data')
			except :
				pass
			os.makedirs("data")
			confs["clean_data"] = False 
		confs["firstime"] = False
		with open("conf/db.json","w") as io :
			io.write(json.dumps(confs))
	
def initsqlitedb(dbconfs):
	con =  sqlite3.connect(dbconfs["db"])
	with open("conf/install-db-req.json","r") as io :
		reqs = json.loads(io.read())
	
	with con:
		cur = con.cursor()
		for part , partreqs in reqs.iteritems() :
			for req in partreqs :
				cur.execute(req)
	con.close()






