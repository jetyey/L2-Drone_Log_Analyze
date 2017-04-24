#ifndef MECHANICALANALYZE_H_INCLUDED
#define MECHANICALANALYZE_H_INCLUDED
#include <cJSON.h>
void storeerrorMec(char*  ,double** ,char*, int ,char*,cJSON*);
int mechanicalAnalyze(double **, double **, double **,double **,int* ,int* ,int* , int);

#endif // MECHANICALANALYZE_H_INCLUDED
