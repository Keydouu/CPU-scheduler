#include <stdio.h>
#include <stdlib.h>
#include "FilesManipulation.h"
int readNumber (FILE* fichier)
{
	char c=0;
	int N=0;
	c=fgetc(fichier);
	while((c!=',')&&(c!=';'))
	{
		N = N * 10 + c -48;
		c=fgetc(fichier);
	}
	return N;
}
int numberOfTasks(FILE* fichier)
{
	int i=0;
	char c=10;
	rewind(fichier);
	while(c!=EOF)
	{
		c=fgetc(fichier);
		if (c==';')
			i++;
	}
	rewind(fichier);
	return i;
}
task tasksReader(FILE* fichier)
{
	task T;
	char c;
	T.Name[1]=0;
	T.Name[2]=0;
	c=fgetc(fichier);
	if (c=='\n')
		c=fgetc(fichier);
	T.Name[0]=c;
	c=fgetc(fichier);
	if(c!=',')
	{
		T.Name[1]=c;
		c=fgetc(fichier);
	}
	T.priority=10;//si on ne donne pas le priorité alors elle est minimale
	T.inOutLength=0;//si E/S n'est pas spécifiée alors elle n'existe pas
	T.entry=readNumber (fichier);
	T.length=readNumber (fichier);
	T.priority=readNumber (fichier);
	T.inOutLength=readNumber (fichier);
	c=fgetc(fichier);
	return T;
}
void writeFromCPUtoFile(FILE* fichier,task* T_tasks,CPUtable CPU,int Qsize)
{
	char c;
	int i;
	c=fgetc(fichier);
	rewind(fichier);
	for(i=0;i<(2*Qsize);i++)
	{
		while((c!=10)&&(c!=EOF))
			c=fgetc(fichier);
		fseek( fichier, -1, SEEK_CUR );
		if(i<Qsize)
		{
			if(CPU.entreeSortie[i]>=0)
				fprintf(fichier, ",%s",(T_tasks+CPU.entreeSortie[i])->Name);
			else
				fprintf(fichier, ",");
		}
		else
		{
			if(CPU.fileAttente[i-Qsize]>=0)
				fprintf(fichier, ",%s",(T_tasks+CPU.fileAttente[i-Qsize])->Name);
			else
				fprintf(fichier, ",");
		}
		c=fgetc(fichier);
	}
}
