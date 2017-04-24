/**
* \file dranalyse.c
* \brief Il s'agit de main.c
* \author DE JESUS
* \version 1.0
* \date 19 avril 2017
*
* Programme Drone Log Analyze - LS2S2
*
*/
#include <Python.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrange.h"
#include "count.h"
#include "freearray.h"
#include "arrayallocation.h"
#include "gpsAnalyze.h"
#include "compassAnalyze.h"
#include "compassoffsetAnalyze.h"
#include "batteryAnalyze2.h"
#include "vibrationAnalyze.h"
#include "mechanicalAnalyze.h"
#include <unistd.h>
#include <cJSON.h>
#include "errortotxt.h"

static PyObject* 

/**
 * \fn analyse(PyObject *self , PyObject *arg)
 * \brief Fonction main
 * 
 * \param self  	PyObject module qui appele la fonction analyse
 * \param arg		2 PyObject argument nom de fichier et le stockage
 *
 * \return PyLong_FromLong(0)
 */
analyse(PyObject *self , PyObject *arg)
{
	char *filename; /*!< Pointeur vers le fichier à lire */
	char *wheretostore; /*!< Pointeur vers le stockage de fichier*/
	char *nomtypedelog; /*!< Pointeur vers le nom du typedelog*/
	if (!PyArg_ParseTuple(arg, "sss",&filename,&wheretostore,&nomtypedelog)) {
		return NULL ; 
	}
	
	int typedelog ;
	char* thetime ;
	if (strcmp("PixHawk 2",nomtypedelog) == 0) {
		thetime = "TimeUs" ;
		typedelog = 1 ;
	}
	else if (strcmp("ArduCopter v3",nomtypedelog) == 0) {
		
		thetime = "TimeMs" ;
		typedelog = 0 ;
	}
	else {	
		printf("type of log not supported !");
		return PyLong_FromLong(-1);
	}
	
	
    //pointers to counters
    int ATT=0;
    int IMU=0;
    int CURR=0;
    int MAG=0;
    int GPS=0;
    //Array containing word to be search
    char att[5]="ATT";
    char imu[5]="IMU";
    char curr[5]="CURR";
    char mag[5]="MAG";
    char gps[5]="GPS";
    FILE *file = NULL;     // pointeur to the file to be read
    //Pointers to arrays 2D
    double **attptr, **imuptr, **currptr, **magptr, **gpsptr, **parmptr;
    double **atterr1, **atterr2,**atterr3,**imuerr1,**imuerr2,**imuerr3, **currerr, **magerr1,**magerr2,**magerr3, **gpserr1,**gpserr2;

   
	//read
    file = fopen(filename, "r");
	if (file == NULL){
		printf("Error occured !");
		return PyLong_FromLong(-1);
	}

	//functions to count occurence of evert attribute:
	count(file, &ATT, &IMU, &CURR, &MAG, &GPS, att, imu, curr, mag, gps);
	
	if (ATT == 0 && IMU == 0 && CURR == 0 && MAG == 0 && GPS == 0 ) {
		// file is not log file
		printf("bad file Error occured !");
		return PyLong_FromLong(-1);
		
	}
	printf("ATT %d  IMU %d CURR %d MAG %d  GPS %d\n",ATT , IMU , CURR , MAG , GPS );
    //reopen file
    file = fopen(filename, "r");
	if (file == NULL){
		printf("Error occured !");
		return PyLong_FromLong(-1);
	}

	//Initialize allocation
    attptr = arrayallocation(ATT);    //Create line for ATT
    imuptr = arrayallocation(IMU);    //Create line for IMU
    currptr = arrayallocation(CURR);  //Create line for CURR
    magptr = arrayallocation(MAG);    //Create line for MAG
    gpsptr = arrayallocation(GPS);    //Create line for GPS
    parmptr=arrayallocation(1);
    //tableau de double contenant TimeMsMs des erreurs
   //Initialize allocation
    atterr1 = arrayallocation(ATT);    //Create line for ATT
	atterr2 = arrayallocation(ATT);
	atterr3 = arrayallocation(ATT);    //Create line for ATT
    imuerr1 = arrayallocation(IMU);    //Create line for IMU
	imuerr2 = arrayallocation(IMU);    //Create line for IMU
	imuerr3 = arrayallocation(IMU);    //Create line for IMU
    currerr = arrayallocation(CURR);  //Create line for CURR
    magerr1 = arrayallocation(MAG);    //Create line for MAG
	magerr2 = arrayallocation(MAG);    //Create line for MAG
	magerr3 = arrayallocation(MAG);    //Create line for MAG
    gpserr1 = arrayallocation(GPS);    //Create line for GPS
	gpserr2 = arrayallocation(GPS);    //Create line for GPS

	
	
	
    //FUNTION: read file then sort to the proper array
    arrange(file, attptr, imuptr, currptr, magptr, gpsptr, parmptr, att, imu, curr, mag, gps);
	
	
     //store to csv files
	int i = 0 ;
    char name[200] ; 
	int fields ;
	int timepos ;
	
	if (ATT > 0 ){
		fields = 6 ;
		char* attfields[6] = { "DesRoll","Roll","DesPitch","Pitch","DesYaw","Yaw"};
		for(i=0;i<fields;i++){
		strcpy(name,wheretostore);
		strcat(name,"att_");
		strcat(name,attfields[i]);
		strcat(name,".csv");
		array_to_csv(name,thetime,0,attfields[i],i+1,attptr,ATT);
		}
	}

	if (IMU > 0 ){
		fields = 6 ;
		char* imufields[6] = {"GyrX","GyrY","GyrZ","AccX","AccY","AccZ"};  
		for(i=0;i<fields;i++){
			strcpy(name,wheretostore);
			strcat(name,"imu_");
			strcat(name,imufields[i]);
			strcat(name,".csv");
		
			array_to_csv(name,thetime,0,imufields[i],i+1,imuptr,IMU);
		}
	}
  
	if (CURR > 0 ){
		if (typedelog == 0){
			fields = 6 ;
			char* curfields[6] = {"ThrOut","ThrInt","Volt","Curr","Vcc","CurrTot"};
			if (CURR > 0){
				// store to csv files
				for(i=0;i<fields;i++){
				strcpy(name,wheretostore);
				strcat(name,"curr_");
					strcat(name,curfields[i]);
					strcat(name,".csv");
					array_to_csv(name,thetime,0,curfields[i],i+1,currptr,CURR);
				}
			}
		 }
	}
	
	if (MAG > 0){
		fields = 6 ;
		char* magfields[6] = {"MagX","MagY","MagZ","OfsX","OfsY","OfsZ"};
		// store to csv files
		for(i=0;i<fields;i++){
		strcpy(name,wheretostore);
		strcat(name,"mag_");
			strcat(name,magfields[i]);
			strcat(name,".csv");
			array_to_csv(name,thetime,0,magfields[i],i+1,magptr,MAG);
		}
	}

	
	if (GPS > 0 ) {
	
		if (typedelog == 0 ) {
			fields = 2 ;
			char* gpsfields0[2]  = { "NSats","HDop"};
			timepos = 1 ;
			// store to csv files
			for(i=0;i<fields;i++){
				strcpy(name,wheretostore);
				strcat(name,"gps_");
				strcat(name,gpsfields0[i]);
				strcat(name,".csv");
				array_to_csv(name,thetime,timepos,gpsfields0[i],i+3,gpsptr,GPS);
				
			}
		}else if (typedelog == 1 ){
			fields = 2 ;
			char* gpsfields1[2] = { "NSats","HDop" };
			timepos = 0 ;
			// store to csv files
			for(i=0;i<fields;i++){
			strcpy(name,wheretostore);
			strcat(name,"gps_");
			strcat(name,gpsfields1[i]);
			strcat(name,".csv");
			array_to_csv(name,thetime,timepos,gpsfields1[i],i+4,gpsptr,GPS);
			}
		}
		
	}
   
	
	cJSON* root = cJSON_CreateObject(); // will hold problems files structures
	if(!root){
		return PyLong_FromLong(-1);
	}
	cJSON* jproblems = cJSON_CreateArray() ;// will hold a lis tof problem
	if(!jproblems){
		return PyLong_FromLong(-1);
	}
	cJSON* oneproblem ; // will hold details for one problem
	cJSON_AddItemToObject(root, "problems", jproblems);
	
	
	
	
	FILE *problems_f;
	strcpy(name,wheretostore);
    strcat(name,"problems.json");
	problems_f=fopen(name,"w");
	
	
    //Analyze GPS
	// ptr to null
	int hdoppos , nsatpos ;
	if (typedelog == 0 ) {
		hdoppos = 4 ;
		nsatpos = 3 ;
		
	}else if (typedelog == 1 ){
		hdoppos = 5 ;
		nsatpos = 4 ;
	}
	for(i=0;i<GPS;i++){
		gpserr1[i] = NULL ;
		gpserr2[i] = NULL ;
	}
	
	int gps1 = 0 , gps2 = 0 ;
    if (gpsAnalyze(gpsptr,gpserr1,gpserr2,&gps1,&gps2, GPS ,hdoppos , nsatpos )) {
		oneproblem = cJSON_CreateObject();
		cJSON_AddStringToObject(oneproblem, "type", "gps");
		cJSON_AddStringToObject(oneproblem, "log", "gps");
		cJSON_AddItemToObject(oneproblem,"nbfiles",cJSON_CreateArray());
		cJSON_AddItemToObject(oneproblem,"whitch",cJSON_CreateArray());
		cJSON_AddItemToArray(jproblems,oneproblem);
		if (gps1){
			
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("gps1"));
			if (typedelog == 0 ) {
				storeerrorGPS(wheretostore,gpserr1,"gps1",GPS,"TimeMs",1,"HDop",4,oneproblem);
			}else if (typedelog == 1 ){
				storeerrorGPS(wheretostore,gpserr1,"gps1",GPS,"TimeUs",0,"HDop",5,oneproblem);
			}
			
		}
		if (gps2){
		
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("gps2"));
			if (typedelog == 0 ) {
				storeerrorGPS(wheretostore,gpserr2,"gps2",GPS,"TimeMs",1,"NSats",3,oneproblem);
			}else if (typedelog == 1 ){
				storeerrorGPS(wheretostore,gpserr2,"gps2",GPS,"TimeUs",0,"NSats",4,oneproblem);
			}
			
		}
    };
	
	// ptr to null
	for(i=0;i<IMU;i++){
		imuerr1[i] = NULL ;
		imuerr2[i] = NULL ;
		imuerr3[i] = NULL ;
	}
	int imu1 = 0 , imu2 = 0 , imu3 = 0;
    //Analyze vibrations
   if (vibrationAnalyze(imuptr, imuerr1,imuerr2,imuerr3,&imu1,&imu2,&imu3, IMU)) {
		oneproblem = cJSON_CreateObject();
		cJSON_AddStringToObject(oneproblem, "type", "vibration");
		cJSON_AddStringToObject(oneproblem, "log", "imu");
		cJSON_AddItemToObject(oneproblem,"nbfiles",cJSON_CreateArray());
		cJSON_AddItemToObject(oneproblem,"whitch",cJSON_CreateArray());
		cJSON_AddItemToArray(jproblems,oneproblem);
		
		if (imu1){
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("vibration1"));
			storeerrorVib(wheretostore,imuerr1,"vibration1",IMU,thetime,oneproblem);
		}
		if (imu2){
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("vibration2"));
			storeerrorVib(wheretostore,imuerr2,"vibration2",IMU,thetime,oneproblem);		
		}
		if (imu3){
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("vibration3"));
			storeerrorVib(wheretostore,imuerr3,"vibration3",IMU,thetime,oneproblem);
		}
    }
	
	//Analyze Compass
	// ptr to null
	for(i=0;i<MAG;i++){
		magerr1[i] = NULL ;
	}
   
    if(compassAnalyze(magptr, magerr1 , MAG)) {
		oneproblem = cJSON_CreateObject();
		cJSON_AddStringToObject(oneproblem, "type", "compass");
		cJSON_AddStringToObject(oneproblem, "log", "mag");
		cJSON_AddItemToObject(oneproblem,"nbfiles",cJSON_CreateArray());
		cJSON_AddItemToObject(oneproblem,"whitch",cJSON_CreateArray());
		cJSON_AddItemToArray(jproblems,oneproblem);
		cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("compass"));
		storeerrorCompass(wheretostore,magerr1,MAG,thetime,oneproblem);
    };
	
	
    //Analyze voltage interference
	// ptr to null
	if(typedelog == 0 ){
		for(i=0;i<CURR;i++){
			currerr[i] = NULL ;
		}
		if (batteryAnalyze2(currptr,currerr, CURR)) {
			oneproblem = cJSON_CreateObject();
			cJSON_AddStringToObject(oneproblem, "type", "alimentation");
			cJSON_AddStringToObject(oneproblem, "log", "curr");
			cJSON_AddItemToObject(oneproblem,"nbfiles",cJSON_CreateArray());
			cJSON_AddItemToObject(oneproblem,"whitch",cJSON_CreateArray());
			cJSON_AddItemToArray(jproblems,oneproblem);
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("alimentation"));
			storeerrorBat2(wheretostore,currerr,CURR,thetime,oneproblem);
		};
	}
	

    //Analyze Compass Offset
    if(parmptr[0][0]==1)
    {	
		
		// ptr to null
		for(i=0;i<MAG;i++){
			magerr1[i] = NULL ;
			magerr2[i] = NULL ;
			magerr3[i] = NULL ;
		}
		int mag1 = 0 , mag2 = 0 , mag3  = 0;
        if (compassoffsetAnalyze(magptr, magerr1,magerr2,magerr3,&mag1,&mag2,&mag3 ,MAG) ){
			oneproblem = cJSON_CreateObject();
			cJSON_AddStringToObject(oneproblem, "type", "compassofsset");
			cJSON_AddStringToObject(oneproblem, "log", "mag");
			cJSON_AddItemToObject(oneproblem,"nbfiles",cJSON_CreateArray());
			cJSON_AddItemToObject(oneproblem,"whitch",cJSON_CreateArray());
			cJSON_AddItemToArray(jproblems,oneproblem);
			
			if(mag1){
				cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("compassoffsetv11"));
				storeerrorCompOffset(wheretostore,magerr1,"compassoffsetv11",MAG,thetime,oneproblem);
			}
			if(mag2){
			
				cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("compassoffsetv12"));
				storeerrorCompOffset(wheretostore,magerr2,"compassoffsetv12",MAG,thetime,oneproblem);
			}
			if(mag3){
				cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("compassoffsetv13"));
				storeerrorCompOffset(wheretostore,magerr3,"compassoffsetv13",MAG,thetime,oneproblem);
			}
			
    	
		}
		
    
	}else {
		
	   char* m = malloc(300);
	   sprintf(m,"COMPASS_OFS_X = %f\nCOMPASS_OFS_Y = %f\nCOMPASS_OFS_Z = %f\n",parmptr[0][0],parmptr[0][1],parmptr[0][2]);
	 
	   cJSON_AddStringToObject(root, "offset", m);
	   if(compassoffsetAnalyze2(parmptr, 1)) {
		   
			oneproblem = cJSON_CreateObject();
			cJSON_AddStringToObject(oneproblem, "type", "compassoffsetv2");
			cJSON_AddStringToObject(oneproblem, "data", m);
			
			cJSON_AddItemToArray(jproblems,oneproblem);
    
		}
		
    }
	
	int at1 = 0 , at2 = 0 , at3  = 0;
	// ptr to null
	for(i=0;i<ATT;i++){
		atterr1[i] = NULL ;
		atterr2[i] = NULL ;
		atterr3[i] = NULL ;
	}
	
    //Analyze Mechanical problems
	if( mechanicalAnalyze(attptr,atterr1,atterr2,atterr3,&at1,&at2,&at3,ATT)){
		
		oneproblem = cJSON_CreateObject();
		cJSON_AddStringToObject(oneproblem, "type", "mechanical");
		cJSON_AddStringToObject(oneproblem, "log", "att");
		cJSON_AddItemToObject(oneproblem,"nbfiles",cJSON_CreateArray());
		cJSON_AddItemToObject(oneproblem,"whitch",cJSON_CreateArray());
		cJSON_AddItemToArray(jproblems,oneproblem);
		
		if (at1){
			
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("mechanical1"));
			storeerrorMec(wheretostore,atterr1,"mechanical1",ATT,thetime,oneproblem);
		}
		if (at2){
			
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("mechanical2"));
			storeerrorMec(wheretostore,atterr2,"mechanical2",ATT,thetime,oneproblem);
			
		}if (at3){
			
			
			cJSON_AddItemToArray(cJSON_GetObjectItem(oneproblem,"whitch") , cJSON_CreateString("mechanical3"));
			storeerrorMec(wheretostore,atterr3,"mechanical3",ATT,thetime,oneproblem);
		}
    };
	
	
	
	
	
	if (printerrortotxt(root,nomtypedelog,wheretostore) == -1){
		printf("Error occured !");
		return PyLong_FromLong(-1);
	}
	
	fprintf(problems_f,"%s",cJSON_Print(root));
	cJSON_Delete(root);
	fclose(problems_f);
	
	
   //Free every allocation done
    
    freearray(attptr, ATT);
    freearray(imuptr, IMU);
    freearray(currptr, CURR);
    freearray(magptr, MAG);
    freearray(gpsptr, GPS);
    freearray(parmptr, 0);
	
	free(atterr1);
	free(atterr2);
	free(atterr3);
    free(imuerr1);
	free(imuerr2);
	free(imuerr3);
    free(currerr);
    free(magerr1);
	free(magerr2);
	free(magerr3);
    free(gpserr1);
	free(gpserr2);
   
	
    //printf("\nI Didn't forgot to free!\n");

	
	
	return PyLong_FromLong(0);

}





static PyMethodDef DranalyseMethods[] = {

	{ "analyse", analyse , METH_VARARGS,

	  "drone analyse implementation." },

	{ NULL, NULL, 0, NULL }

};



PyMODINIT_FUNC

initdranalyse(void)

{
	 
	(void)Py_InitModule("dranalyse", DranalyseMethods);

}
