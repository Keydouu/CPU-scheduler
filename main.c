#include <stdio.h>
#include <stdlib.h>
#include "CPUmanagement.h"
int Menu()
{
	char input=0;
	while((input<'1')||(input>'6'))
	{
		#if defined(_WIN32) || defined(WIN32)
		system("cls");
		#else
		system("clear");
		#endif
		printf("1 FCFS\n");
		printf("2 SJF sans preemption\n");
		printf("3 SJF avec preemption\n");
		printf("4 Par priorite sans preemption\n");
		printf("5 Par priorite avec preemption\n");
		printf("6 RR\n");
		printf("entrez votre choix : \n");
		scanf("%c",&input);

	}
	return (input-'0');
}
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
    fclose(inputFile);
    switch(Menu()) {
      case 1 :
            scheduling(T_tasks,tasksNumber,1,0);
         break;
      case 2 :
            scheduling(T_tasks,tasksNumber,2,0);
         break;
      case 3 :
            scheduling(T_tasks,tasksNumber,2,1);
         break;
      case 4 :
            scheduling(T_tasks,tasksNumber,3,0);
         break;
      case 5 :
            scheduling(T_tasks,tasksNumber,3,1);
         break;
      case 6 :
            RR(T_tasks,tasksNumber);
         break;
    }
	return 0;
}
