from user import newlogupload , log , verifchunk , getnewlogurl , renderLogPage , gestateofanalyse


def getrequestname(request,t):
	path = request.path.split("/")
	if len(path) < 3 : 
		request.name = "404"
		return 
	request.postpath = path[3:]
	
	if t == "g" :
		if GAction.has_key(path[2]) :
			request.name = path[2]
			return
	else :
		if PAction.has_key(path[2]) :
			request.name = path[2]
			return
	request.name = "404"

def r404(request,*args):
	return ""
		
GAction = { "log"   :  log ,
            "404" : r404 ,
	"newlogupload" : verifchunk ,
	"getnewlogurl" : getnewlogurl ,
	"logs" :renderLogPage	,
	"gestateofanalyse" : gestateofanalyse}
PAction = { "newlogupload"  : newlogupload ,
			"404" : r404}
