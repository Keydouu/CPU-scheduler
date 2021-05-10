#include <stdio.h>
#include <stdlib.h>
#include "CPUmanagement.h"
void OrgranisationDeFile(int* Q,int Qsize,task* T,int newTask)
{
	Qsize--;
	while((Q[Qsize]<0)&&(Qsize>=0))//ignore -1 (empty cases)
		Qsize--;
	while((Qsize>=0)&&((T+Q[Qsize])->priority>(T+newTask)->priority))//if case below newbi have lower prio(bigger prio number)
	{
		Q[Qsize+1]=Q[Qsize];//move lower case up
		Qsize--;//compare newbie with the new lewer case
	}
	Q[Qsize+1]=newTask;//fix the newbie in the position he is at
}
void QueueDown(int* Q,int Qsize)
{
	int i;
	Qsize--;
	for(i=0;i<Qsize;i++)
		Q[i]=Q[i+1];
	Q[i]=-1;
}
void SJF(task* T_tasks,int tasksNumber, int preemption)
{
	int i,Pos=0;
	int debutDeFile,continueTasks;//bool
	CPUtable CPU;
	FILE* outputFile;
	CPU.entreeSortie=(int*)malloc(sizeof(int)*tasksNumber);
	CPU.fileAttente=(int*)malloc(sizeof(int)*tasksNumber);
	for(i=0;i<tasksNumber;i++)
	{
		*(CPU.entreeSortie+i)=-1;
		*(CPU.fileAttente+i)=-1;
	}
	if (preemption)
	{
		outputFile=fopen("SFJavecPreemption","w+");
		debutDeFile=0;
	}
	else
	{
		outputFile=fopen("SFJsansPreemption","w+");
		debutDeFile=1;
	}
	fileInitialisation(outputFile, (2*tasksNumber));
	do
	{
		/*ACTUAL CODE START HERE */
		for(i=0;i<tasksNumber;i++)
		{
			if((T_tasks+i)->entry==Pos)// entrée des nouvelles taches à la liste d'attente
				OrgranisationDeFile((CPU.fileAttente+debutDeFile),(tasksNumber-debutDeFile),T_tasks,i);
		}
		if(CPU.entreeSortie[0]>=0)// s'il y une tache en E/S
		{
			(T_tasks+CPU.entreeSortie[0])->inOutLength--;//reduire sa durée de ES de 1
			if((T_tasks+CPU.entreeSortie[0])->inOutLength==0)//si la tache se termine
				QueueDown(CPU.entreeSortie,tasksNumber);
		}
		if(CPU.fileAttente[0]>=0)//s'il y a une tache active
		{
			(T_tasks+CPU.fileAttente[0])->length--;//on reduit sa durée d'execution de 1
			if((T_tasks+CPU.fileAttente[0])->length==0)//si la tache se termine
			{
				if((T_tasks+CPU.fileAttente[0])->inOutLength>0)//si elle a une durée d'entrée sortie
				{
					for(i=0;CPU.entreeSortie[i]>=0;i++);//on cherche la celule libre en E/S
                        CPU.entreeSortie[i]=CPU.fileAttente[0];//on y met la tache qui était active
				}
				QueueDown(CPU.fileAttente,tasksNumber);
			}
		}
		else
            QueueDown(CPU.fileAttente,tasksNumber);
		continueTasks=0;
		i=0;
		while((i<tasksNumber)&&(continueTasks==0))
		{
			if(((T_tasks+i)->length!=0)||((T_tasks+i)->inOutLength!=0))
				continueTasks=1;
			i++;
		}
		Pos++;
		writeFromCPUtoFile(outputFile,T_tasks,CPU,tasksNumber);//change
	}while(continueTasks);
	free(CPU.entreeSortie);
	free(CPU.fileAttente);
	fclose(outputFile);
}
