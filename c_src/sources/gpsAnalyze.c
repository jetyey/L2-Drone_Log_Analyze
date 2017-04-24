/**
 * \file gpsAnalyze.c
 * \brief Programme qui detecte les problemes de GPS
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include  "gpsAnalyze.h"
/**
 * \fn int gpsAnalyze(gpsptr,gpserr, *GPS)
 * \brief Fonction qui detecte les probleme de gps et le stocke dans un tableau gpserr
 * 
 * \param[in] gpsptr le tableau de double qui contient les valeurs de gps
 * \param[in/out] gpserr1 est le tableau qui contiendra les valeurs des erreurs
 * \param[in/out] gpserr2 est le tableau qui contiendra les valeurs des erreurs
 * \param[in] gps1 pour indiquer la presence d'un erreur
 * \param[in] gps2 pour indiquer la presence d'un erreur
 * \param[in] ligne nombre de ligne
 *
 * \return 0 s'il n'y a pas d'erreur.
 */
int gpsAnalyze(double **gpsptr, double **gpserr1,double **gpserr2, int* gps1 , int* gps2 , int ligne,int hdoppos , int nsatpos )
{
    printf("\n--------------Analyzing GPS glitches-----------------\n");
    if(ligne!=0)                                                                        //Verify if there's GPS in log
    {
        int i;
        int c=0;                                              //counter of error
        for(i=0;i<ligne;i++){
            if(gpsptr[i][hdoppos]>1.5 )
			{
				
				gpserr1[i] = gpsptr[i];
				c++;
				*gps1 = 1 ;
				
			}
			if (gpsptr[i][nsatpos]<9){ 			//condition 1
			
                gpserr2[i] = gpsptr[i];
				*gps2 = 1 ;
                c++;
				
            }
        }
        printf(">>>>>>>>>\t Number of GPS errors: %d\n",c);
        if(c>0) return 1;                                                           //return 1 if error detected
    }else{printf(">>>>>>>>>\t No MAG attribute in these log file\n");}
    return 0;
}

/**
 * \fn storeerrorGPS(char* where ,double** gpserr, char* whichmec, int size ,cJSON* problem){
 * \brief Fonction qui convertit le gpserr en csv
 * 
 * \param where		Le nom de reportoire qu'on enrigestre le fichier csv
 * \param gpserr	Le tableau qui contiendra les valeurs des erreurs
 * \param whichmec 	Pour identifier quel probleme est detecté parmi les 2
 * \param size		Le nombre de lignes
 * \param thetime	Le Time dans le logfile
 * \param thetimei 	L'index du Time
 * \param thename 	Le nom de attribut lu de temps (soit TimeMs ou TimeUs)
 * \param theindex 	L'index du nom
 * \param problem	Objet JSON qui decrit rencontrer dans ce type de log
 */
void storeerrorGPS(char* where ,double** gpserr ,char* whichmec, int size ,char* thetime,int thetimei, char* thename,int theindex ,cJSON* problem){
	
	char name[100];
	
	FILE *csv_file = NULL;
	int files = 0 ;
	int i;
	int started = 0 ;
	
	for (i=0;i<size;i++)
	{
		if (started){
			
			if (gpserr[i]) {
				fprintf(csv_file,"%f,%f\n",  gpserr[i][thetimei],gpserr[i][theindex]);
			}else {
				started = 0 ;
				fclose(csv_file);
			}
		}
		else {
			
			if (gpserr[i]){
				i-- ;
				sprintf(name,"%s/%s_%d.csv",where,whichmec,files);
				
				csv_file=fopen(name,"w");
				
				fprintf(csv_file,"%s,%s\n",thetime,thename);
				
				started = 1 ;
							
				files++ ;
				
			}
			
			
		}
		
		
	}
		
	cJSON_AddItemToArray(cJSON_GetObjectItem(problem,"nbfiles") , cJSON_CreateNumber((double)files));
	
}
