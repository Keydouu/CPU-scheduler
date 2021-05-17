#include <stdio.h>
#include <stdlib.h>
#include "CPUmanagement.h"
int main()
{
	task* T_tasks;
	FILE* inputFile=NULL;
	int tasksNumber,i;
	inputFile=fopen("tasksFile.txt","r");
	if(inputFile==NULL)
	{
        printf("file not found\n");
        exit(0);
	}
	tasksNumber=numberOfTasks(inputFile);
	T_tasks=(task*)malloc(sizeof(task)*tasksNumber);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);

	scheduling(T_tasks,tasksNumber,1,0);
	
	rewind(inputFile);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);
	
	scheduling(T_tasks,tasksNumber,2,0);
	
	rewind(inputFile);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);
	
	scheduling(T_tasks,tasksNumber,2,1);
	
	rewind(inputFile);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);
	
	scheduling(T_tasks,tasksNumber,3,0);
	
	rewind(inputFile);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);
	
	scheduling(T_tasks,tasksNumber,3,1);
	
	rewind(inputFile);
	for(i=0;i<tasksNumber;i++)
		*(T_tasks+i)=tasksReader(inputFile);

    	RR(T_tasks,tasksNumber);
	
	fclose(inputFile);
	return 0;
}
