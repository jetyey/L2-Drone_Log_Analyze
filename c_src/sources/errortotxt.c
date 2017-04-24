#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

int copyfiletofile(FILE* out , char* filefrom, char* buff, const int buffsize){
	
	FILE *from;
	size_t nreaded;
	from = fopen(filefrom, "r");
	
	if (from) {
		
		while ((nreaded = fread(buff, 1, buffsize, from)) > 0){
			fwrite(buff, 1, nreaded, out);
		}
			
		if (ferror(from)) {
			printf("cannot open file : %s",filefrom);
			return -1;
		}
		
		fclose(from);
		
	}else{
		
		printf("cannot open file : %s",filefrom);
		return -1;
	}
	
	return 0;
	
	
}


int printerrortotxt(cJSON* root,char* nomtypedelog,char* wheretostore){
	
	const int CHUNK =  10240 ;
	char* buff = malloc(10240);
	
	if (!buff){
		printf("cannot allocte buff for reading");
		return -1;
	}
	
	FILE* txt;
	char name[100];
	strcpy(name,wheretostore);
    strcat(name,"/problems.txt");
	txt=fopen(name,"w");
	
	
	
	cJSON* problem ;
	cJSON* whitchs ;
	cJSON* nbfiles ;
	
	cJSON* problems = cJSON_GetObjectItem(root,"problems");
	
	
	char* thetype;
	double nbfile;
	char* whitch ;
	int i , j , k;
	
	char str_nbfile[15];
	
	
	fprintf(txt,"Controller : %s\n\n\n",nomtypedelog);
	
	
    for (i = 0; i < cJSON_GetArraySize(problems); i++)
		{
			problem = cJSON_GetArrayItem(problems, i);
			thetype =  cJSON_GetObjectItem(problem, "type")->valuestring ;
			
			if (strcmp(thetype,"compassoffsetv2") == 0 ) {
				
				fprintf(txt,"Compass Offset errors\n");
				fprintf(txt,"Explanation of errors:\n");
				fprintf(txt,"COMPASS_OFS_X should be included in [-150 ;15]\n");
				fprintf(txt,"COMPASS_OFS_Y should be included in [-150 ;15]\n");
				fprintf(txt,"COMPASS_OFS_Z should be included in [-150 ;15]\n");
				fprintf(txt,"\n\n");
				
				fprintf(txt,"Values:\n\n");
				fprintf(txt,"%s",cJSON_GetObjectItem(problem, "data")->valuestring);
				fprintf(txt,"\n\n");
				
				
				
			}
			
			else if (strcmp(thetype,"compassoffset") == 0 ) {
				
				fprintf(txt,"Compass Offset errors\n");
				
				whitchs = cJSON_GetObjectItem(problem,"whitch");
				nbfiles = cJSON_GetObjectItem(problem,"nbfiles");
				
				for(j = 0 ; j < cJSON_GetArraySize(whitchs); j++) {
					
					whitch = cJSON_GetArrayItem(whitchs, j)->valuestring;
					
					if (strcmp(whitch,"compassoffsetv11")== 0){
				
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"Ofsx should be included in [-150 ;15]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/compassoffsetv11_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
					
					
					else if (strcmp(whitch,"compassoffsetv12")== 0){
						
						fprintf(txt,"Explanation of errors:\n");

				
						fprintf(txt,"Ofsy should be included in [-150 ;15]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/compassoffsetv12_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
						
					}
					
					else if (strcmp(whitch,"compassoffsetv13")== 0){
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"Ofsz should be included in [-150 ;15]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/compassoffsetv13_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
				}
				
				
			}
			
			else if (strcmp(thetype,"mechanical") == 0 ) {
				
				fprintf(txt,"Mechanical errors\n");
				
				whitchs = cJSON_GetObjectItem(problem,"whitch");
				nbfiles = cJSON_GetObjectItem(problem,"nbfiles");
				
				for(j = 0 ; j < cJSON_GetArraySize(whitchs); j++) {
					
					whitch = cJSON_GetArrayItem(whitchs, j)->valuestring;
					
					if (strcmp(whitch,"mechanical1") == 0){
				
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"The difference between DesRoll and Roll should be included [-5;5]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/mechanical1_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
					
					
					else if (strcmp(whitch,"mechanical2") == 0){
						
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"The difference between DesPitch and Pitch should be included [-5;5]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/mechanical2_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
						
					}
					
					else if (strcmp(whitch,"mechanical3") == 0 ){
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"The difference between DesYaw and Yaw should be included [-15;15]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/mechanical3_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
							
						}
						fprintf(txt,"\n\n");
					}
				}
			}
			
			else if (strcmp(thetype,"gps") == 0 ) {
				
				fprintf(txt,"GPS errors\n");
				
				whitchs = cJSON_GetObjectItem(problem,"whitch");
				nbfiles = cJSON_GetObjectItem(problem,"nbfiles");
				
				for(j = 0 ; j < cJSON_GetArraySize(whitchs); j++) {
					
					whitch = cJSON_GetArrayItem(whitchs, j)->valuestring;
					
					if (strcmp(whitch,"gps1")== 0){
				
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"Hdop should be less than 1.5\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/gps1_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
					
					
					else if (strcmp(whitch,"gps2") == 0){
						
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"NSats should be greater than 9 \n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/gps2_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
						
					}
				}
				
				
			}
			
			else if (strcmp(thetype,"vibration") == 0 ) {
				
				fprintf(txt,"Vibration errors\n");
				
				whitchs = cJSON_GetObjectItem(problem,"whitch");
				nbfiles = cJSON_GetObjectItem(problem,"nbfiles");
				
				for(j = 0 ; j < cJSON_GetArraySize(whitchs); j++) {
					
					whitch = cJSON_GetArrayItem(whitchs, j)->valuestring;
				
					if (strcmp(whitch,"vibration1")== 0){
				
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"AccX  should be included [-5;5]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/vibration1_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
					
					
					else if (strcmp(whitch,"vibration2")== 0){
						
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"AccY  should be included [-5;5]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/vibration2_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
						
					}
					
					else if (strcmp(whitch,"vibration3")== 0){
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"AccZ should be included [-15;15]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/vibration3_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
				}
				
			}
			
			else if (strcmp(thetype,"compass") == 0 ) {
				
				fprintf(txt,"Compass errors\n");
				
				whitchs = cJSON_GetObjectItem(problem,"whitch");
				nbfiles = cJSON_GetObjectItem(problem,"nbfiles");
				
				for(j = 0 ; j < cJSON_GetArraySize(whitchs); j++) {
					
					whitch = cJSON_GetArrayItem(whitchs, j)->valuestring;
					
					if (strcmp(whitch,"compass")== 0){
				
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"The value of mag_field = sqrt(MagX(2) + MagY(2)+ MagZ(2))  should be included [120;550]\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/compass_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
					
				}
				
			}
			
			else if (strcmp(thetype,"alimentation") == 0 ) {
				
				fprintf(txt,"Power errors\n");
				
				whitchs = cJSON_GetObjectItem(problem,"whitch");
				nbfiles = cJSON_GetObjectItem(problem,"nbfiles");
				
				for(j = 0 ; j < cJSON_GetArraySize(whitchs); j++) {
					
					whitch = cJSON_GetArrayItem(whitchs, j)->valuestring;
					
					if (strcmp(whitch,"alimentation")== 0){
				
						fprintf(txt,"Explanation of errors:\n");
						fprintf(txt,"A Vcc variation of more than 150 is considered an error\n");
						fprintf(txt,"\n\n");
						fprintf(txt,"Values:\n\n");
						
						nbfile = cJSON_GetArrayItem(nbfiles, j)->valuedouble;
						for (k = 0 ; k < (int)nbfile ; k++){
							
							strcpy(name,wheretostore);
							strcat(name,"/alimentation_");
							sprintf(str_nbfile, "%d.csv", k);
							strcat(name,str_nbfile);
							if (copyfiletofile(txt,name,buff,CHUNK) == -1 ){
								return -1;
							}
							fprintf(txt,"\n");
						}
						fprintf(txt,"\n\n");
					}
				}
				
			}
			
			
		}

	fclose(txt);
	return 0 ;
	
}
