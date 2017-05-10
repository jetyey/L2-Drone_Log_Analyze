# Project_L2S2-Drone_Log_Analyze
This application is a lof file analyze that detects basic problems on your drone's log file.
This was developped by Ahmedou Galledou, Chakib Soulimas, Jethro De Jesus annd Zakariae Idrissi of the University of Paris Descartes as part of our project L2S2.


Requirement :

	support only linux for now

	libraries : python , twisted
	installation de python :
		sudo apt-get install -y python2.7-dev python-pip build-essential 
	installation de twisted :
		sudo pip install pyopenssl
		sudo pip install twisted
	
Execution :
	cd racine
	le serveur s'execute sur le port 8020 par defaut
	python main.py 
	ou	
	python main.py port
	
Info :
	to build the c source :
		cd path/to/project/c_src
		python setup.py build
		
		and you will have the .so module in directory build
