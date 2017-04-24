#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
from twisted.enterprise.adbapi import ConnectionPool
from twisted.web.server import NOT_DONE_YET
from twisted.python import log
import os



class DataBase(object):
	
	def __init__(self):
	
	
		# here we get database config 
		with open("conf/db.json","r") as io :
			self.inits = json.loads(io.read())
		dbconfs= self.inits["dbconfig"]
		
		self.dbpool = ConnectionPool(dbconfs["dbengine"], dbconfs["db"]) 
		
			
	def runInteraction(self,f):
		return self.dbpool.runInteraction(f)

	def databaseerror(self,f):
		"""
		log.msg(str(f.value))
		log.msg(str(f.getTraceback()))
		"""
		print f
	
	
	def makecondition(self,conditionf=[],condition=[],joiners=[]):
		if joiners and conditionf:			
			d = [" " + conditionf[i] + condition[i] + "? "  for i in xrange(len(conditionf))]
			i = 1
			for join in joiners :
				d.insert(i,join)
				i += 2
			return "".join(d)	
		elif condition :
			return " ".join([conditionf[i] + condition[i] + "?"  for i in range(len(conditionf))])
		else :
			return conditionf[0]
	
	
	def getlastidquery(self):
		return "SELECT LAST_INSERT_ID()"
		
	def getmakeinsertquery(self,table, fields=(), values=() ):
		return 'INSERT INTO  %s (%s) VALUES (%s)' % (
			table,
			', '.join(fields),
			', '.join(["?"]*len(values))
		)	
	def makeinsert(self, table, fields=(), values=() , cb = None ):
		
		query = self.getmakeinsertquery(table,fields=fields,values=values)
		print query
		d = self.dbpool.runQuery(query,values)
		if cb is not None :
			d.addCallback(cb)
		d.addErrback(self.databaseerror)
		return d

	def getfromtable(self,table,fields,condition,cb):
		if fields == "all" :	
			query = "SELECT * FROM %s WHERE %s"%(table,self.makecondition(
			conditionf=condition[0],condition = condition[1] ,joiners=condition[3]))
		else :
			query = "SELECT %s FROM %s WHERE %s"%(" ,".join(fields),table,self.makecondition(
			conditionf=condition[0],condition = condition[1] ,joiners=condition[3]))
		
		d = self.dbpool.runQuery(query,tuple(condition[2]))
		d.addCallback(cb)
		d.addErrback(self.databaseerror)
		return d
	def runrawsql(sql,cb):
		d = self.dbpool.runQuery(sql)
		d.addCallback(cb)
		d.addErrback(self.databaseerror)
		return d
		
		
		
	def makeupdate(self, table=None,fields=[],values=[],
		conditionf=[],condition=[],
		conditionv=[],joiners=[],cb=None):
		query = 'UPDATE %s SET %s  WHERE %s' % (
			table,
			', '.join([f + "=?" for f in fields]),
			self.makecondition(conditionf=conditionf,condition=condition,joiners=joiners)
		)
		d = self.dbpool.runQuery(query,tuple(values+conditionv))
		if cb is not None :
			d.addCallback(cb)
		d.addErrback(self.databaseerror)
		return d
		
	

DB = DataBase()
