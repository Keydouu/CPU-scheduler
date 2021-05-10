#include <stdio.h>
#include <stdlib.h>
#include "FilesManipulation.h"
void fileInitialisation(FILE* fp, int numberOfLines)
{
    char c;
    c='\n';
    while(numberOfLines>0)
    {
        fputc(c,fp);
        numberOfLines--;
    }
}
void GoToLine(FILE* fp,int lineNumber)
{
	char c=32;
	rewind(fp);
	while(lineNumber)
	{
		c=fgetc(fp);
		if ((c=='\n')||(c==EOF))
			lineNumber--;
	}
	if(c!=EOF)
        fseek(fp,-1,SEEK_CUR);
}
void writeInMiddleOfFile(FILE* fp, char* charTable, int charTableSize)
{
    char* memory;
    int i=0,j;
    char c=10;
    while(c!=EOF)
    {
        c=fgetc(fp);
        i++;
    }
    c=',';
    i--;
    fseek(fp,-i,SEEK_END);
    memory=(char*)malloc(i*sizeof(char));
    for(j=0;j<i;j++)
        memory[j]=fgetc(fp);
    fseek(fp,-i,SEEK_END);
    fputc(c,fp);
    for(j=0;j<charTableSize;j++)
        fputc(charTable[j],fp);
    for(j=0;j<i;j++)
        fputc(memory[j],fp);
    fseek(fp,-i,SEEK_END);
}
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
	int i,j;
	rewind(fichier);
	for(i=0;i<(2*Qsize);i++)
	{
		j=2;
		if(i<Qsize)
		{
			GoToLine(fichier,(Qsize-i));
			if(CPU.entreeSortie[i]>=0)
			{
				if((T_tasks+CPU.entreeSortie[i])->Name[1]==0)
					j=1;
				writeInMiddleOfFile(fichier,(T_tasks+CPU.entreeSortie[i])->Name,j);
			}
			else
			{
				writeInMiddleOfFile(fichier,0,0);
			}
		}
		else
		{
			GoToLine(fichier,(3*Qsize-i));
			if(CPU.fileAttente[i-Qsize]>=0)
			{
				if((T_tasks+CPU.fileAttente[i-Qsize])->Name[1]==0)
					j=1;
				writeInMiddleOfFile(fichier,(T_tasks+CPU.fileAttente[i-Qsize])->Name,j);
			}
			else
			{
				writeInMiddleOfFile(fichier,0,0);
			}
		}
	}
}
