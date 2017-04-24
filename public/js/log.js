// To insert an element before another one
function insertBefore(el, referenceNode) {
    referenceNode.parentNode.insertBefore(el, referenceNode);
}


var mypath = undefined ; // wil hold thelog  link
var myname = undefined ; // will hold the logname given by the server
var uploder  = undefined ; // will hold the uploder object
var initialdata = document.getElementById("results").innerHTML;

var thethypeoflog = undefined ; // the type of log that will be analysed

var problemsData = undefined ; // will hold problem Data a json object containing a list of prolems

var tTime = undefined ; // will hold time name ( TimeMs or TimeUS ect)

// some statut of graph downloaded
var allgraphs = false ;
var compasssoffsets = false ;
var compasss = false ;
var vibrations = false ;
var mecanicals = false ;
var alimentations = false ;
var gpss = false ;

// last graph shown
var lasts = undefined ;

// set all graph downloaded statut to false
var restarts = function(){
	
	allgraphs = false ;
	compasssoffsets = false ;
	compasss = false ;
	vibrations = false ;
	mecanicals = false ;
	alimentations = false ;
	gpss = false ;
	var lasts = undefined ;
}

var initOptions = function () {
		
		
	// function that manage type of log chosen

	// addEventListener to log options
	var pix = document.getElementById("PixHawk_2");
	var ardu = document.getElementById("ArduCopter_v3");
	
	pix.onclick = function () { 
	
		// set selected to thethypeoflog and unselect the other
		ardu.checked = false ;
		thethypeoflog = "PixHawk 2" ; 
		
	}
	
	ardu.onclick = function () {
		// set selected to thethypeoflog and unselect the other
		pix.checked = false ;
		thethypeoflog = "ArduCopter v3" ; 
		
	}
 
}

initOptions();
 
 
 
var showmultiplelines = function(problemtype,listoflogtype,witchgraphe,errors = true) {
	
	
	
	
	var COLORS = ["black","orange","green","yellow","blue","red"]
	var colors = 1 ;
	// Set the dimensions of the canvas / graph
	var margin = {top: 30, right: 20, bottom: 30, left: 50},
		width = 30000 - margin.left - margin.right,
		height = 600 - margin.top - margin.bottom;
	
	// d3 js global to graph
	var svg = undefined ; 

	// Set the ranges
	var x = d3.scale.linear().range([0, width]);
	var y = d3.scale.linear().range([height, 0]);

	// Define the axes
	var xAxis = d3.svg.axis().scale(x)
		.orient("bottom")
		.ticks(120);
	// Define the axes
	var yAxis = d3.svg.axis().scale(y)
		.orient("left")
		.ticks(10);
		
	// will hold d3js object representing values of time function
	var valuelines = [] ;
	
	
	
	// function to add new values time function
	var addv = function(i,colors){
		
		valuelines.push(
			[
				d3.svg.line()
				.x(function(d) { return x(d[tTime]); })
				.y(function(d) { return y(d[listoflogtype[i]]);}) ,
				
				"line" +  colors.toString() ,
				
				undefined
			]
		)
		
	}
	
	// add number of times the the logtypes
	for (var i = 0 ; i < listoflogtype.length ; i++){
		
		addv(i,colors);
		
		colors += 1 ;
		
	}
	
	
				  
	var mmin = 0 ; // will hold minimum number in all log in the same graph
	var mmax = 0 ; // will hold maximum number in all log in the same graph
	
	var todownload = listoflogtype.length ; // length of number of log to download
	
	var plotgraphs = function(){
		
		for (j = 0 ; j < listoflogtype.length ; j++) {
		
			// Add the valueline path.
			svg.append("path")
				.attr("class", valuelines[j][1])
				.attr("d", valuelines[j][0](valuelines[j][2]));
				
			// add label .
			svg.append("text")
				.attr("class", "x label")
				.attr("text-anchor", "end")
				.attr("x", 230 + j * 70 )
				.attr("y", -15)
				.text( listoflogtype[j] + " " )
				.style('fill', COLORS[j+1]);
		}
		
		
		if (errors) {
			// if error we add error label
			svg.append("text")
			.attr("class", "x label")
			.attr("text-anchor", "end")
			.attr("x", 230 + j * 70 )
			.attr("y", -15)
			.text( "Erreur " )
			.style('fill', "red");
		
		}
	}
	var scalegraph = function(){
		// Adds the svg canvas
		svg = d3.select(witchgraphe)
			.append("svg")
				.attr("width", width + margin.left + margin.right)
				.attr("height", height + margin.top + margin.bottom)
				/*
				.call(d3.behavior.zoom().on("zoom", function () {
					svg.attr("transform", "translate(" + d3.event.translate + ")" + " scale(" + d3.event.scale + ")")
				  }))*/
			.append("g")
				.attr("transform", 
					  "translate(" + margin.left + "," + margin.top + ")");
		// Scale the range of the data
		x.domain(d3.extent(valuelines[0][2], function(d) { return d[tTime]; }));
		y.domain([mmin, mmax]);
		
		// Add the X Axis
		svg.append("g")
			.attr("class", "x axis")
			.attr("transform", "translate(0," + height + ")")
			.call(xAxis);

		// Add the Y Axis
		svg.append("g")
			.attr("class", "y axis")
			.call(yAxis);
			
		// set axis names
		svg.append("text")
		.attr("class", "x label")
		.attr("text-anchor", "end")
		.attr("x", 200)
		.attr("y", -15)
		.text("X = " + tTime + "  Y = " );
		
		if (errors){
			for (var  k = 0 ; k < problemtype["whitch"].length ; k++){
				
				geterrors(k);
			}
			
		
		}
	}
	var nofound = [] ;
	var getlog = function(j) {
		
		// get log from server
		d3.csv(mypath + "/" + problemtype["log"] + "_" + listoflogtype[j] + ".csv" , function(error, data) {
			
			if(error != null){
				// log not found
				nofound.push(listoflogtype[j]);
				if (nofound.length == listoflogtype.length ){
					// all log not found 
					var d = document.getElementById(witchgraphe.substring(1)) ;
					d.style = "" ;
					// print appopritae message
					d.innerHTML = nofound.join(" ") + " are not availabale ";
				}
				return;
			}
			
			// we cast datatype
			data.forEach(function(d) {
				d[tTime] = +d[tTime];
				d[listoflogtype[j]] = +d[listoflogtype[j]];
			});
			// set the data to valuelines end wait 
			valuelines[j][2] = data ;
			
			// we compute max and min of all the log in the same graph
			var ss = d3.min(data, function(d) { return d[listoflogtype[j]];}) ;
			if ( ss < mmin ) mmin = ss ;
			ss = d3.max(data, function(d) { return d[listoflogtype[j]];}) ;
			if ( ss > mmax ) mmax = ss ;
			todownload -= 1 ; 
			
			
			if (todownload == 0 ){
				// we dowloaded all
				if (nofound.length != 0 ){
					// if we didn't get some log print appopritae message
					el = document.createElement("div");
					el.innerHTML = nofound.join(" ") + " logs are not available ";
					insertBefore(el,document.getElementById(witchgraphe.substring(1)))
				}
				
				// we got all the data start drawing
				scalegraph();
				plotgraphs();
						
			}
		});
		
	}	
		
	for (i = 0 ; i < listoflogtype.length ; i++) {
		// gell all logs
		getlog(i) ;
	}
	
	var geterrors = function(ii){
		
		// let's start displaying errors range on top of logs
		
		for (var nbf = 0 ; nbf < problemtype["nbfiles"][ii] ; nbf++ ) {
		
			d3.csv(mypath + "/" + problemtype["whitch"][ii] + "_" +  nbf + ".csv" , function(error, data) {
				
				// cast "string" to binary data 
				data.forEach(function(d) {
					
					d[tTime] = +d[tTime];
					
					for (var ltp = 0 ; ltp  < listoflogtype.length ; ltp++) {
						try {
							d[listoflogtype[ltp]] = +d[listoflogtype[ltp]];
						}catch(err) {}
					}
				});
				
				var valuelines = [] ;

				var addv = function(i){
					// function to add new values time function
					valuelines.push(
						
							d3.svg.line()
							.x(function(d) { return x(d[tTime]); })
							.y(function(d) { return y(d[listoflogtype[i]]);}) 
						
					)
					
				}
				for (var ii = 0 ; ii < listoflogtype.length ; ii++){
					
					addv(ii);
					
				}
				// Add the valueline path.
				for (var ltp = 0 ; ltp  < listoflogtype.length ; ltp++) {
					
					svg.append("path")
					.attr("class", "line5")
					.attr("d", valuelines[ltp](data));
				}
					
			});
		}
	}
}

















var showcompassoffsetdiv = function(){
	var problems = problemsData;
	
	if (!compasssoffsets){
		var comof1 = false ;
		var comof2 = false ;
		var comof3 = false ;
		var como2f = false ;
		
		document.getElementById("compassofssetdiv").style.display = "block" ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			
			var problem = problems["problems"][i];
			
			if (problem.type == 'compassoffsetv11'){
				document.getElementById("compassofssetv1").style.display = "block" ;
				document.getElementById("OfsXOfsYOfsZ1").style.display = "block" ;
				showmultiplelines(problem, ["OfsX","OfsY","OfsZ"], "#OfsXOfsYOfsZ");
				comof = true ;
				break;
			}
			else if(problem.type == 'compassoffsetv21' ){
				document.getElementById("compassofssetv1").style.display = "none" ;
				document.getElementById("compassofssetv2").style.display = "block" ;
				document.getElementById("compassoffsetv21").style.display = "block" ;
				document.getElementById("compassoffsetv21").innerHTML = problems.offset.split("/n").join("</br>") ;
				como2f = true ;
				break;
			}
		}
		if (problems.hasOwnProperty("offset")){
			if (!como2f){
				document.getElementById("compassofssetv1").style.display = "none" ;
				document.getElementById("compassofssetv2").style.display = "block" ;
				document.getElementById("compassoffsetv21").style.display = "block" ;
				document.getElementById("compassoffsetv21").innerHTML = problems.offset.split("\n").join("</br>") ;
			}
		}else {
			document.getElementById("compassofssetv1").style.display = "block" ;
			if(!comof){
				document.getElementById("OfsXOfsYOfsZ1").style.display = "block" ;
				showmultiplelines({"log":"mag"}, ["OfsX","OfsY","OfsZ"], "#OfsXOfsYOfsZ1",false);
			}
		}
		compasssoffsets = true ;
	}
	if (lasts != undefined){
		lasts.style.display = "none" ;
	}
	lasts = document.getElementById("compassofssetdiv");
	lasts.style.display = "block" ;
	
}
var showcompassdiv = function(){
	
	var problems = problemsData;
	if(!compasss ){
		
		document.getElementById("compassdiv").style.display = "block" ;
		var comps = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			if (problem.type == 'compass' ){
				document.getElementById("MagXMagYMagZ").style.display = "block" ;
				showmultiplelines(problem, ["MagX","MagY","MagZ"], "#MagXMagYMagZ");
				comps = true ;
				break
			}
		}
		if (!comps){
			document.getElementById("MagXMagYMagZ").style.display = "block" ;
			showmultiplelines({"log":"mag"}, ["MagX","MagY","MagZ"], "#MagXMagYMagZ", false);
		}
		compasss = true ;
	}
	if (lasts != undefined){
		lasts.style.display = "none" ;
	}
	lasts = document.getElementById("compassdiv");
	lasts.style.display = "block" ;
}
var showvibfrationdiv = function(){
	var problems = problemsData;
	
	if(!vibrations){
		

		
		document.getElementById("vibrationdiv").style.display = "block" ;
		
		var vib = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			if (problem.type == 'vibration') { 
				document.getElementById("AccXAccYAccZ").style.display = "block" ;
				showmultiplelines(problem, ["AccX","AccY","AccZ"], "#AccXAccYAccZ");
				vib = true ;
			}
			
		}
		if (!vib){
			document.getElementById("AccXAccYAccZ").style.display = "block" ;
			showmultiplelines({"log":"imu"}, ["AccX","AccY","AccZ"], "#AccXAccYAccZ", false);
		}		
		vibrations = true
	}
	if (lasts != undefined){
		lasts.style.display = "none" ;
	}
	lasts = document.getElementById("vibrationdiv");
	lasts.style.display = "block" ;
	
	
}
var showgpsdiv = function(){
	var problems = problemsData;
	
	if(!gpss){
		
		document.getElementById("gpsdiv").style.display = "block" ;
		var gpp = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			if (problem.type == 'gps' ) { 
				document.getElementById("HdopNsats").style.display = "block" ;
				showmultiplelines(problem, ["HDop","NSats"], "#HdopNsats");
				gpp = true ;
			}
		}
		if (!gpp){
			document.getElementById("HdopNsats").style.display = "block" ;
			showmultiplelines({"log":"gps"}, ["HDop","NSats"], "#HdopNsats",false);
		}
		gpss = true ;
	}
	if (lasts != undefined){
		lasts.style.display = "none" ;
	}
	lasts = document.getElementById("gpsdiv");
	lasts.style.display = "block" ;
	
}
var showpowerdiv = function(){
	var problems = problemsData;
	
	if(!alimentations){
		document.getElementById("powerdiv").style.display = "block" ;
		var ali = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			
			var problem = problems["problems"][i];
			if(problem.type == "alimentation") { 
				document.getElementById("VCC").style.display = "block" ;
				showmultiplelines(problem, ["Vcc"], "#VCC");
				ali = true ;
				break
			}
		}
		if (!ali){
			document.getElementById("VCC").style.display = "block" ;
			showmultiplelines({"log":"curr"}, ["Vcc"], "#VCC",false);
		}
		
		alimentations = true ;
	}
	if (lasts != undefined){
		lasts.style.display = "none" ;
	}
	lasts = document.getElementById("powerdiv");
	lasts.style.display = "block" ;
}

var showmecdiv = function(){
	var problems = problemsData;
	
	if(!mecanicals){
		
		document.getElementById("mecdiv").style.display = "block" ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			console.log(problem);
			if (problem.type == "mechanical") {
				
				z = problem.whitch.indexOf('mechanical1') ;
				document.getElementById("mechanicaldiv1").style.display = "block" ;
				document.getElementById("mechanical1").style.display = "block" ;
				if (z > -1){
					
					showmultiplelines({ log: "att" , whitch :[problem.whitch[z]] , nbfiles : [problem.nbfiles[z]] }, ["Roll","DesRoll"], "#mechanical1");
				}else {
					
					showmultiplelines({ log: "att" }, ["Roll","DesRoll"], "#mechanical1",false);
				}
			
			
				z = problem.whitch.indexOf('mechanical2') ;
				document.getElementById("mechanicaldiv2").style.display = "block" ;
				document.getElementById("mechanical2").style.display = "block" ;
				if (z > -1){
					showmultiplelines({ log: "att" , whitch :[problem.whitch[z]] , nbfiles : [problem.nbfiles[z]] },["Pitch","DesPitch"], "#mechanical2");
				}else {
					showmultiplelines({ log: "att" }, ["Pitch","DesPitch"], "#mechanical2",false);
				}
	
				
				z = problem.whitch.indexOf('mechanical3') ;
				document.getElementById("mechanicaldiv3").style.display = "block" ;
				document.getElementById("mechanical3").style.display = "block" ;
				if (z > -1){
					showmultiplelines({ log: "att" , whitch :[problem.whitch[z]] , nbfiles : [problem.nbfiles[z]] },["Yaw","DesYaw"], "#mechanical3");
				}else {
					showmultiplelines({ log: "att" }, ["Yaw","DesYaw"], "#mechanical3",false);
				}
			mecanicals = true ;
			break;	
			}
			mecanicals = true ;
		}
	}
	
	if (lasts != undefined){
		lasts.style.display = "none" ;
	}
	lasts = document.getElementById("mecdiv");
	lasts.style.display = "block" ;
}



var showalldiv = function(){
	
    // graph all log

	// function that graph in allgraph zone
  if (!allgraphs){
	    
		document.getElementById("alldiv").style.display = "block" ;
	
		
		var problems = problemsData ;
		
	
		if (problemsData.hasOwnProperty("offset")){
			
			var d = '<div><h3 class="title-description">Compass Offset</h3>' +
			'<h4 class="title-description">' +
			'COMPASS_OFS_X ϵ [-150 ;15]</br>' +
			'COMPASS_OFS_Y ϵ [-150 ;15]</br>' +
			'COMPASS_OFS_Z ϵ [-150 ;15]</br>' +
			'</h4><h5>' + problemsData.offset.split("\n").join("</br>") + '</h5></div>';
			
			var mel = document.createElement('div');	
			mel.innerHTML = d ;
			document.getElementById("result2").appendChild(mel);
			
		}else{ 
			var d = '<div><h3 class="title-description">Compass Ofsset</h3>'	+
			'<h4 class="title-description">' +
			'Ofsx ϵ [-150 ;15]</br>' +
			'Ofsy ϵ [-150 ;15]</br>' +
			'Ofsz ϵ [-150 ;15]</br>' +
			'</h4>' + 
			'<div id="allOfsXOfsYOfsZ"  style="background:white;overflow:scroll"></div></div>' ;
			var mel = document.createElement('div');	
			mel.innerHTML =  d ;
			document.getElementById("result2").appendChild(mel.firstChild);
			var comof = false ;
			for (var i = 0 ; i <  problems["problems"].length ; i++) {
			
				var problem = problems["problems"][i];
				
				
				if (problem.type == 'compassoffsetv11'){
					showmultiplelines(problem, ["OfsX","OfsY","OfsZ"], "#allOfsXOfsYOfsZ");
					comof = true ;
					break;
				}
			}
			if (!comof){
				
				showmultiplelines({"log":"mag"}, ["OfsX","OfsY","OfsZ"], "#allOfsXOfsYOfsZ",false);
			}
		}
		
		
		
		var d = '<div><h3 class="title-description">Compass</h3>' +
				'<h4 class="title-description">' +
					'The value of mag_field = sqrt(MagX(2) + MagY(2)+ MagZ(2)) ϵ [120;550]</br>' +
				'</h4>' +
				'<div id="allMagXMagYMagZ"  style="background:white;overflow:scroll"></div></div>';
			
		var mel = document.createElement('div');	
		mel.innerHTML = d ;
		document.getElementById("result2").appendChild(mel.firstChild);
		var comps = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			if (problem.type == 'compass' ){
				showmultiplelines(problem, ["MagX","MagY","MagZ"], "#allMagXMagYMagZ");
				comps = true ;
				break
			}
		}
		if (!comps){
			showmultiplelines({"log":"mag"}, ["MagX","MagY","MagZ"], "#allMagXMagYMagZ", false);
		}
		
		
	
	
		var d = '<div>'
		'<h3 class="title-description">Vibration</h3>' +				
		'<h4 class="title-description">' +
		'AccX ϵ [-3 ;3]</br>' +
		'AccY ϵ [-3 ;3]</br>' +
		'AccZ ϵ [-3 ;3]</br>' +
		'</h4>' +
		'<div id="allAccXAccYAccZ"  style="background:white;overflow:scroll"></div></div>' ;
		var mel = document.createElement('div');	
		mel.innerHTML = d ;
		document.getElementById("result2").appendChild(mel.firstChild);
		var vib = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			if (problem.type == 'vibration') { 
				showmultiplelines(problem, ["AccX","AccY","AccZ"], "#allAccXAccYAccZ");
				vib = true ;
			}
			
		}
		if (!vib){
			
			showmultiplelines({"log":"imu"}, ["AccX","AccY","AccZ"], "#allAccXAccYAccZ", false);
		}		
		
		
		
		var d = '<div>'+
		'<h3 class="title-description">GPS</h3>' +	
		'<h4 class="title-description">' +
		'Hdop should be less than 1.5 </br>' +
		'NSats should be greater than 9 ' +
		'</h4>' +
		'<div id="allHdopNsats"  style="background:white;overflow:scroll"></div></div>' ;
		var mel = document.createElement('div');	
		mel.innerHTML = d ;
		document.getElementById("result2").appendChild(mel.firstChild);
		var gpp = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			if (problem.type == 'gps' ) { 
				
				showmultiplelines(problem, ["HDop","NSats"], "#allHdopNsats");
				gpp = true ;
			}
		}
		if (!gpp){
			showmultiplelines({"log":"gps"}, ["HDop","NSats"], "#allHdopNsats",false);
		}
		
		
		

		var d = '<div><h3 class="title-description">Power </h3>'+
		'<h4 class="title-description">'+
		'A Vcc variation of more than 150 is considered an error</br>'+
		'</h4>'+
		'<div id="allVCC"  style="background:white;overflow:scroll"></div>' ;
		var mel = document.createElement('div');	
		mel.innerHTML = d ;
		document.getElementById("result2").appendChild(mel.firstChild);
		
		var ali = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			
			var problem = problems["problems"][i];
			if(problem.type == "alimentation") { 
				showmultiplelines(problem, ["Vcc"], "#allVCC");
				ali = true ;
				break
			}
		}
		if (!ali){
			showmultiplelines({"log":"curr"}, ["Vcc"], "#allVCC",false);
		}
		
		
		
		
		var f = function(problem){
			var d = '<div><h3 class="title-description">DesRoll and Roll</h3>' +
					'<h4 class="title-description">' +
					'The difference between DesRoll and Roll should be ϵ [-5;5]</br>' +
					'</h4><div id="allRollDesRoll"  style="background:white;overflow:scroll"></div></div>' ;
			
			z = problem.whitch.indexOf('mechanical1') ;
			var mel = document.createElement('div');	
			mel.innerHTML = d ;
			document.getElementById("result2").appendChild(mel.firstChild);
			if (z > -1){
				
				showmultiplelines({ log: "att" , whitch :[problem.whitch[z]] , nbfiles : [problem.nbfiles[z]] }, ["Roll","DesRoll"], "#allRollDesRoll");
			}else {
				
				showmultiplelines({ log: "att" }, ["Roll","DesRoll"], "#allRollDesRoll",false);
			}
		
			var d = '<div><h3 class="title-description">DesPitch and Pitch</h3>' +
					'<h4 class="title-description">' +
					'The difference between DesPitch and Pitch  should be ϵ [-5;5]</br>' +
					'</h4><div id="allPitchDesPitch"  style="background:white;overflow:scroll"></div></div>' ;
			z = problem.whitch.indexOf('mechanical2') ;
			var mel = document.createElement('div');	
			mel.innerHTML = d ;
			document.getElementById("result2").appendChild(mel.firstChild);
			if (z > -1){
				showmultiplelines({ log: "att" , whitch :[problem.whitch[z]] , nbfiles : [problem.nbfiles[z]] },["Pitch","DesPitch"], "#allPitchDesPitch");
			}else {
				showmultiplelines({ log: "att" }, ["Pitch","DesPitch"], "#allPitchDesPitch",false);
			}

			var d = '<div><h3 class="title-description">DesYaw and Yaw</h3>' +
					'<h4 class="title-description">' +
					'The difference between DesYaw and Yaw  should be ϵ [-15;15]</br>' +
					'</h4><div id="allYawNavYaw"  style="background:white;overflow:scroll"></div></div>' ;
			z = problem.whitch.indexOf('mechanical3') ;
			var mel = document.createElement('div');	
			mel.innerHTML = d ;
			document.getElementById("result2").appendChild(mel.firstChild);
			if (z > -1){
				showmultiplelines({ log: "att" , whitch :[problem.whitch[z]] , nbfiles : [problem.nbfiles[z]] },["Yaw","DesYaw"], "#allYawNavYaw");
			}else {
				showmultiplelines({ log: "att" }, ["Yaw","DesYaw"], "#allYawNavYaw",false);
			}
		}
		
		var mec = false ;
		for (var i = 0 ; i <  problems["problems"].length ; i++) {
			var problem = problems["problems"][i];
			if (problem.type == "mechanical") {
				f(problem)
				mec = true ;
				break;
			}
		}
		if(!mec)f({ whitch :[] , nbfiles : [] });
		
		allgraphs = true ;
		
		
	}
	if (lasts != undefined){
		lasts.style.display = "none" ;
	}
	lasts = document.getElementById("alldiv");
	lasts.style.display = "block" ;
	
	
}

var drawproblems = function(){
	
	
	// we got problesm start printing
	var mystr = "" ;
	
	for (var i = 0 ; i <  problemsData["problems"].length ; i++) {
		
		problem = problemsData["problems"][i];
		
		mystr += problem.type + " problems detected </br>"	
		
	}
	/*
	Compass offset problems detected
	Compass problems detected
	Vibration problems detected
	GPS problems detected
	Power problems detected
	Mechanical failures detected
	*/
	document.getElementById("problemsdiv").innerHTML = mystr ;
	
}

var drawgraphs = function(){
   var getstate = undefined ;
   var _getstate = function(){

	// we call this function every  2 seconds to verfy if processing is over in the server side
	$.ajax({
		
       url : "dnm/gestateofanalyse/" + myname ,

       type : 'GET',

       success : function(code_html, statut){
		if (code_html == "1") {
			// update progress bar
			
			$("#myprogress").contents().find("#myprogressbar").css('width', '100%').attr('aria-valuenow', 100);
			document.getElementById("myprogressdiv").style.display = "none" ;
			document.getElementById("downloaderror").style.display = "block" ;
			document.getElementById("downloaderror").firstChild.href = mypath + "/problems.txt"
			clearInterval(getstate);
		    // processig is over let display data
			$.ajax({
		
			   url :  mypath + "/problems.json" ,

			   type : 'GET',

			   success : function(code_html, statut){
						
						problemsData = JSON.parse(code_html);
						
						
						// print detected problems
						drawproblems();
						

						
				}
			});
  
		}else if (code_html == "-1"){
			// invalid file
			document.getElementById("sierror").innerHTML = "Fichier Log Invalid réessayer"
			document.getElementById("sierror").style.color = "red" ;
			clearInterval(getstate);
		}
	   }
       });

   }
   var getstate = setInterval(_getstate , 2000) ;
  
}
var newupload = function(){
	
	
// the uploader take some arguemnt please see Runnable site
var r = uploder = new Resumable({
  target: 'dnm/newlogupload/' ,
  chunkSize : 5000000 ,
  query:{upload_token:'my_token' , typeoflog :"lol"} ,
  simultaneousUploads : 1 ,
  method : "octet"
});
r.assignBrowse(document.getElementById('browseButton'));

r.on('fileSuccess', function(file ,message){
	
   // r = JSON.parse(message);
   drawgraphs();
   uploder = undefined ;
   document.getElementById("toremove").innerHTML = '<input class="button button-large" id="browseButton" type="file"></input><a class="button button-large" onclick="analyse();" href="#work" >Lancer l\'analyse</a>';
   newupload();
  });
r.on('fileProgress', function(file){
    //console.debug('fileProgress', file);
  });
r.on('fileAdded', function(file, event){
    
    console.debug('fileAdded', event);
	document.getElementById("sierror").innerHTML = "";
	document.getElementById("problemsdiv").innerHTML = "";
	document.getElementById("browseButton").style.display = "none"; 
	document.getElementById("lunchanalyse").style.backgroundColor = "#DE5E60" ;
	
  });
r.on('filesAdded', function(array){
    //r.upload();
    console.debug('filesAdded', array);
  });
r.on('fileRetry', function(file){
    //console.debug('fileRetry', file);
  });
r.on('fileError', function(file, message){
    //console.debug('fileError', file, message);
	uploder = undefined ;
	document.getElementById("sierror").innerHTML = "Impossible d'uploder le Fichier Log";
	document.getElementById("sierror").style.color = "red" ;
    document.getElementById("toremove").innerHTML = '<input class="button button-large" id="browseButton" type="file"></input><a class="button button-large" onclick="analyse();" href="#work" >Lancer l\'analyse</a>';
	newupload();
  });
r.on('uploadStart', function(){
    //console.debug('uploadStart');
  });
r.on('complete', function(){
    console.debug('complete');
 
  });
r.on('progress', function(){
	
	var p  = r.progress() * 50 ;
	//$('.progress-bar').css('width', p+'%').attr('aria-valuenow', p);
	$("#myprogress").contents().find("#myprogressbar").css('width', p+'%').attr('aria-valuenow', p);
  });
r.on('error', function(message, file){
   // console.debug('error', message, file);
  });
r.on('pause', function(){
    //console.debug('pause');
  });
r.on('cancel', function(){
    //console.debug('cancel');
  });
}

var analyse = function(){

if (thethypeoflog == undefined ) {
	alert("Select the type of log");
	return ;
}


document.getElementById("browseButton").style.display = "block"; 
// let start analysing 

// query a new name (id) for this log 
$.ajax({
		
       url : "dnm/getnewlogurl/",

       type : 'GET',

       success : function(code_html, statut){
			// set confs and start uploading
		   mypath = "/dnm/logs/" + code_html ;
		   myname = code_html ;
		   uploder.opts.query.upload_token = code_html ;
		   uploder.opts.query.typeoflog = thethypeoflog ;
		   
		   if (thethypeoflog == "ArduCopter v3" ){tTime = "TimeMs";}
		   else {tTime = "TimeUs";}
		   // remove last download
		   document.getElementById("downloaderror").style.display = "none" ;
		   
		   // remove all analytics
		   document.getElementById("results").innerHTML = initialdata ;
		   document.getElementById("problemsdiv").innerHTML = "" ;
		   // set global show to false
		   restarts();
		   // display and update progressbar
		   document.getElementById("myprogressdiv").style.display = "block" ;
		   $("#myprogress").contents().find("#myprogressbar").css('width', "0%").attr('aria-valuenow', 0);
		   // start uploading
		   uploder.upload();
		   // when upload is over fileSuccess event is called r.on(filesuccess ,  func ) defined earlier
			}
       });

}
newupload();

		