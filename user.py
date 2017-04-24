import string
from db import DB
from twisted.web.server import NOT_DONE_YET
from functools import partial
import os
import random
from twisted.internet import reactor
import json
from jinja2 import Environment, PackageLoader
from datetime import datetime
import os

from twisted.internet import reactor, threads
from functools import partial
from loganalyser.logmanager import processlog

from multiprocessing import Pool

# processes
#this is a pool of worker (python and c ) higher level in python do sate management  low level written in c do long running heavy computation 
pool = Pool(processes=3)

#some states
ERROR = -1
UPLOADING = 0
PROCESSING = 1
LOGVIEWABLE = 2

#this function generate a string of length N
def generate_id(N=10):
	s = string.ascii_uppercase + string.digits
	return ''.join(random.SystemRandom().choice(s) for _ in range(N))
	
def startprocessing(logid,name,typeoflog):
	
	# for now the processing do only the transformation of log data to some useful arrays and store them to disk  
	
	# this will block in until processing finish
	# hiher level python will update the state
	
	
	pool.apply(processlog, args = (logid,name,typeoflog))
	
	# we ignore the result
def gestateofanalyse(request):
	
	if len(request.postpath) > 0 :
		name = request.postpath[0]
	else :
		print "Bad Request no log File name provided"
		request.redirect("/")
		return ""
	def gotlogs(data):
		
		if data and  data[0][4] == LOGVIEWABLE :
			
			request.write("1")
			request.finish()
		elif data and  data[0][4] == ERROR :
			request.write("-1")
			request.finish()
		else :
			request.write("0")
			request.finish()
	
	DB.getfromtable("LogFiles",["*"],[["Name"],["="],[name],[]],gotlogs)
	return NOT_DONE_YET	
def renderLogPage(request):

	
	if len(request.postpath) > 1 :
		name = request.postpath[0]
                logfile = request.postpath[1]
		
	else :
		print "Bad Request no log File name provided"
		request.redirect("/")
		return ""
		
	def gotlogs(data):
		
		if data and  data[0][4] == LOGVIEWABLE :
			try :
				with open("data/%s/bin/%s"%(name,logfile), "r") as f :
					request.write(f.read())
			except :
				request.setResponseCode(404)
			request.finish()
			
		else :
			# redirect to home
			request.redirect("/")
			request.finish()
	
	DB.getfromtable("LogFiles",["*"],[["Name"],["="],[name],[]],gotlogs)
	return NOT_DONE_YET
	
def newlogupload(request,filedata) :
	
	
	try :
		name =  request.args["upload_token"][0]
		current = int(request.args["resumableChunkNumber"][0])
		total = int(request.args["resumableTotalChunks"][0])
		logtype = request.args["typeoflog"][0]
	except Exception as e :
		# we had bad request 
		print "Bad  request %s"%str(e)
		request.redirect("/")
		return ""
		
	
	def gotlogs(data):
	
		if not data :
			# we should not get here
			# fail silently
			print "bad request name doesn't exists : %s"%name
			request.finish()
			return
			
			
		
		state = int(data[0][4])
		
		if state <> UPLOADING :
			# we should not get here
			# fail silently
			print "we already uploaded all this file : %s"%name
			request.finish()
			return
		
		if int(data[0][3]) > current :
			# we should not get here
			# fail silently
			print "we already have this part : %s"%current
			#request.setResponseCode(200)
			request.finish()
			return
				
		# write data to file
		
		d = threads.deferToThread(newchunk,name,filedata)
		d.addCallback(partial(newchunkwritten,request,total,current,name,logtype,data))
		
	
	DB.getfromtable("LogFiles",["*"],[["Name"],["="],[name],[]],gotlogs)
	return NOT_DONE_YET
	
def newchunkwritten(request,total,current,name,logtype,data,ignored):
	
	state = int(data[0][4])
	
	if total == current :
		# we got the last chunk
		# tell browser to redirect to file url
		request.write(json.dumps( { "redirect" : "/dnm/logs/%s"%name } ))
		
		# we update the state 
		state = PROCESSING 
		
	def updated(ignored):
		request.finish()
		
		#start processing log data 
		if state == PROCESSING :
			reactor.callInThread(startprocessing , int(data[0][0]) , str(name) , logtype )
		
	DB.makeupdate(table="LogFiles",fields=["Tchunk","Cchunk","State"],values=[total,current,state],
						conditionf=["Id"],condition=["="],
						conditionv=[int(data[0][0])],joiners=[],cb=updated)
	
def newchunk(name,data):
	
	with open("data/%s/upload/upload.log"%name , "a") as f :
		f.write(data)
		
	
		
def verifchunk(request) :
	
	try :
		name =  request.args["upload_token"][0]
		current = int(request.args["resumableChunkNumber"][0])
		total = int(request.args["resumableTotalChunks"][0])
	except Exception as  e :
		print "we had bad request for verif chunk"
		print e	
		return
	
	def gotlogs(data):
		if not data :
			# we should not get here
			# fail silently
			print "bad request name doesn't exists : %s"%name
			request.finish()
			return
		if int(data[0][4]) <> UPLOADING  :
			# we should not get here
			# fail silently
			print "we already uploaded all this file : %s"%name
			request.finish()
			return
		
		if current <= int(data[0][3]) :
			# we already have this part
			# end request with 200 Ok

			request.setResponseCode(200)
			request.finish()
		else :
			# this make the uploader to send the actual chunk
			request.setResponseCode(204)
			request.finish()

			
	
	DB.getfromtable("LogFiles",["*"],[["Name"],["="],[name],[]],gotlogs)
	
	return NOT_DONE_YET
	
def getnewlogurl(request) :
	
	
	def nameexists(name,data):
		
		if data :
			# name exists get new one
			newname()
		else :
			def added(ignored) :
				#prepare some dirs to hold upload
				os.mkdir("data/%s"%name)
				os.mkdir("data/%s/upload"%name)
				os.mkdir("data/%s/bin"%name)
				
				#return name to the browser
				request.write(name)
				request.finish()
				
			# name doesn't exists insert
			DB.makeinsert("LogFiles",("Name","Tchunk","Cchunk","State"),(name,0,0,UPLOADING) , cb = added)
			
			
	def newname():
		# generate new id and verify if it is not already in db
		name = generate_id()
		DB.getfromtable("LogFiles",["Id"],[["Name"],["="],[name],[]],partial(nameexists,name))
		
	newname()
	return NOT_DONE_YET
	
def log(request) :

	pass
