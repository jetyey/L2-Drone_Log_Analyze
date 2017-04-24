#ifndef GPS_H_INCLUDED
#define GPS_H_INCLUDED
#include <cJSON.h>
void storeerrorGPS(char*  ,double** ,char*, int , char* ,int,char *, int ,cJSON*);
int gpsAnalyze(double **, double**,double**,int*,int*, int,int,int);

#endif // GPS_H_INCLUDED
