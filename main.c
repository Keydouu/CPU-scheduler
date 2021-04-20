#include <stdio.h>
#include <stdlib.h>
#include "FilesManipulation.h"
#include "CPUmanagement.h"
int main()
{
	task* T_tasks;
	FILE* inputFile=NULL;
	char c;
	int tasksNumber,i;
	inputFile=fopen("tasksFile.txt","r");
	if(inputFile==NULL)
	{
        printf("file not found\n");
        sleep(10);
        exit(0);
	}
	tasksNumber=numberOfTasks(inputFile);
	T_tasks=(task*)malloc(sizeof(task)*tasksNumber);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);
	for(i=0;i<tasksNumber;i++)//on verifie que la lecture à partir du fichier a fonctionné
		printf("%s E=%d D=%d P=%d E/S=%d\n",(T_tasks+i)->Name,(T_tasks+i)->entry,(T_tasks+i)->length,(T_tasks+i)->priority,(T_tasks+i)->inOutLength);
	/*FCFS(T_tasks,tasksNumber);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);*/
	//SJF(T_tasks,tasksNumber,0);
	/*for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);
	SJF(T_tasks,tasksNumber,1);*/
	/*for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);
	fclose(inputFile);
	tourniquet(T_tasks,tasksNumber);*/
	fclose(inputFile);
	return 0;
}
