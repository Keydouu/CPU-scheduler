#include <stdio.h>
#include <stdlib.h>
#include "CPUmanagement.h"
void OrgranisationDeFile(int* Q,int Qsize,task* T)
{
	int i=0;
	int memory;
	while((i<Qsize)&&((*(Q+i)>=0)||(*(Q+i+1)>=0)))
	{
		if(*(Q+i)<0)
		{
			if(*(Q+i+1)>=0)
			{
				memory=*(Q+i);
				*(Q+i)=*(Q+i+1);
				*(Q+i+1)=memory;
			}
		}
		else if (((T+i)->priority>(T+i+1)->priority)&&(*(Q+i+1)>=0))
		{
			memory=*(Q+i);
			*(Q+i)=*(Q+i+1);
			*(Q+i+1)=memory;
			i=-1;
		}
		i++;
	}
}

SJF(task* T_tasks,int tasksNumber, int preemption)
{
	int i,j,Pos=0;
	int debutDeFile,continueTasks;//bool
	CPUtable CPU;
	File* outputFile;
	CPU.entreeSortie=(int*)malloc(sizeof(int)*tasksNumber);
	CPU.fileAttente=(int*)malloc(sizeof(int)*tasksNumber);
	for(i=0;i<tasksNumber;i++)
	{
		*(CPU.entreeSortie+i)=-1;
		*(CPU.fileAttente+i)=-1;
	}
	if (preemption)
	{
		outputfile=fopen("SFJavecPreemption",w+);
		debutDeFile=0;
	}
	else
	{
		outputfile=fopen("SFJsansPreemption",w+);
		debutDeFile=1;
	}
	do
	{
		/*ACTUAL CODE START HERE */
		for(i=0;i<tasksNumber;i++)
		{
			if((T_task+i)->entry==Pos)// entrée des nouvelles taches à la liste d'attente
			{
				j=0;
				while(*(CPU.fileAttente+j)>=0)
					j++;
				*(CPU.fileAttente+j)=i;
			}
		}
		//orgranisation de la file d'attente selon la prioritée
		OrgranisationDeFile((CPU.fileAttente+debutDeFile),(tasksNumber-debutDeFile),T_tasks);
		OrgranisationDeFile((CPU.entreeSortie+1),(tasksNumber-1),T_tasks);
		if(CPU.entreeSortie[0]>=0)// s'il y une tache en E/S
		{
			(T_task+CPU.entreeSortie[0])->inOutLength--;//reduire sa durée de ES de 1
			if((T_task+CPU.entreeSortie[0])->inOutLength==0)//si la tache se termine
			{
				CPU.entreeSortie[0]=-1;
				OrgranisationDeFile(CPU.entreeSortie,tasksNumber,T_tasks);
			}
		}
		if(CPU.fileAttente[0]>=0)//s'il y a une tache active
		{
			(T_task+CPU.fileAttente[0])->length--;//on reduit sa durée d'execution de 1
			if((T_task+CPU.fileAttente[0])->length==0)//si la tache se termine
			{
				if((T_task+CPU.fileAttente[0])->inOutLength>0)//si elle a une durée d'entrée sortie
				{
					i=0;
					while((i<tasksNumber)&&(*(CPU.entreeSortie+i)>=0))//on cherche la premiere case libre à la file E/S
						i++;
					*(CPU.entreeSortie+i)=CPU.fileAttente[0];//on y met la tache qui était active
				}
				CPU.fileAttente[0]=-1;//il n'y a plus de tache active puisqu'elle vient de terminer
			}
		}
		continueTasks
		Pos++;
		/*ACTUAL CODE END HERE*/
		/*
		+	Nouvelles taches -> listes d'attente
		Organisation de la liste d'attente selon la prioritée sticte
		+	Tache actuelle de E/S -- SI ELLE EXISTE
		si elle est terminée on la surprime
		+	Durée de la tache actuelle -- SI ELLE EXISTE
		+	Si tache actuelle  == finie -> e/s
		+	Si il n'y a pas de tache actuelle alors faire descendre la liste d'attente
		*/
		writeFromCPUtoFile(outputFile,T_tasks,*CPU,tasksNumber);
	}while(CPU.fileAttente[0]>=0);
	free(CPU.entreeSortie);
	free(CPU.fileAttente);
	fclose(outputefile);
}
