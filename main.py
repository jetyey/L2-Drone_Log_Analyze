#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import json
from sys import platform

#set program name in linux 

if platform == "linux" or platform == "linux2":
	import ctypes
	libc = ctypes.cdll.LoadLibrary('libc.so.6')
	libc.prctl(15, 'LogViwer', 0, 0, 0)

from twisted.web import server, resource
from twisted.internet import reactor, endpoints 
from setup import init
from twisted.python.logfile import DailyLogFile
from twisted.python import log
import sys
from twisted.web.server import Site
from twisted.web.static import File
import where
log.startLogging(sys.stdout)

#log.startLogging(DailyLogFile.fromFullPath("log/twisted.txt"))


#reactor.suggestThreadPoolSize(30)


class ServerApp(resource.Resource):
	
	isLeaf = True
	
	def render_GET(self, request):
		#hangle get , look request name and call the function 
		request.name = None
		where.getrequestname(request,"g")
		return where.GAction[request.name](request)
		
	
	def render_POST(self, request):
		#hangle post , look request name and call the function with data 
		try :
			data = request.content.read()
		except Exception as e :
			print e
			return ""
		request.name = None
		where.getrequestname(request,"p")
		return where.PAction[request.name](request,data)

		



if __name__ == "__main__" :

	root = File('public')
	s = ServerApp()
	root.putChild('dnm', s)
	factory = Site(root)
	init()
	if len(sys.argv) > 1 :
		p = str(sys.argv[1])
	else :
		p = "8020"
	endpoints.serverFromString(reactor, "tcp:" + p  ).listen(factory)
	reactor.run()
