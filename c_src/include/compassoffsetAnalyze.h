#ifndef COMPASSOFFSETANALYZE_H_INCLUDED
#define COMPASSOFFSETANALYZE_H_INCLUDED
#include <cJSON.h>
void storeerrorCompOffset(char*  ,double** ,char*, int ,char*,cJSON*);
int compassoffsetAnalyze(double **, double **,double **,double **,int*,int*,int*, int);

#endif // COMPASSOFFSETANALYZE_H_INCLUDED
