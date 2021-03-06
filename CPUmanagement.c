#include <stdio.h>
#include <stdlib.h>
#include "CPUmanagement.h"
void OrgranisationParPrio(int* Q,int Qsize,task* T,int newTask)
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
void OrgranisationSJF(int* Q,int Qsize,task* T,int newTask)
{
	Qsize--;
	while((Q[Qsize]<0)&&(Qsize>=0))//ignore -1 (empty cases)
		Qsize--;
	while((Qsize>=0)&&((T+Q[Qsize])->length>(T+newTask)->length))//if case below newbi have lower prio(bigger prio number)
	{
		Q[Qsize+1]=Q[Qsize];//move lower case up
		Qsize--;//compare newbie with the new lewer case
	}
	Q[Qsize+1]=newTask;//fix the newbie in the position he is at
}
void OrgranisationFCFS(int* Q,int newTask)
{
	int i;
	for(i=1;Q[i]>=0;i++);
	Q[i]=newTask;
}
void QueueDown(int* Q,int Qsize)
{
	int i;
	Qsize--;
	for(i=0;i<Qsize;i++)
		Q[i]=Q[i+1];
	Q[i]=-1;
}
void scheduling(task* T_tasks,int tasksNumber,int algorithm ,int preemption)
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
	outputFile=createFile(algorithm,preemption);
	if (preemption)
		debutDeFile=0;
	else
		debutDeFile=1;
	fileInitialisation(outputFile, (2*tasksNumber));
	do
	{
		/*ACTUAL CODE START HERE */
		if(CPU.entreeSortie[0]>=0)// s'il y une tache en E/S
		{
			(T_tasks+CPU.entreeSortie[0])->inOutLength--;//reduire sa durée de ES de 1
			if((T_tasks+CPU.entreeSortie[0])->inOutLength==0)//si la tache se termine
            		{
                		(T_tasks+CPU.entreeSortie[0])->tSejour=Pos-(T_tasks+CPU.entreeSortie[0])->entry;
                		QueueDown(CPU.entreeSortie,tasksNumber);
            		}
		}
		if(CPU.fileAttente[0]>=0)//s'il y a une tache active
		{
			if((T_tasks+CPU.fileAttente[0])->tReponse<0)
                		(T_tasks+CPU.fileAttente[0])->tReponse=Pos-((T_tasks+CPU.fileAttente[0])->entry)-1;
			(T_tasks+CPU.fileAttente[0])->length--;//on reduit sa durée d'execution de 1
			if((T_tasks+CPU.fileAttente[0])->length==0)//si la tache se termine
			{
				(T_tasks+CPU.fileAttente[0])->tSejour=Pos-(T_tasks+CPU.fileAttente[0])->entry;
				if((T_tasks+CPU.fileAttente[0])->inOutLength>0)//si elle a une durée d'entrée sortie
				{
					for(i=0;CPU.entreeSortie[i]>=0;i++);//on cherche la celule libre en E/S
                        		CPU.entreeSortie[i]=CPU.fileAttente[0];//on y met la tache qui était active
				}
				QueueDown(CPU.fileAttente,tasksNumber);
			}
		}
		for(i=0;i<tasksNumber;i++)//On verifie s'il y a des nouvelles taches
		{
			if((T_tasks+i)->entry==Pos)// entrée des nouvelles taches à la liste d'attente
			{
				switch(algorithm) {
				case 1 :
					OrgranisationFCFS(CPU.fileAttente,i);
				break;
				case 2 :
					OrgranisationSJF((CPU.fileAttente+debutDeFile),(tasksNumber-debutDeFile),T_tasks,i);
				break;
				case 3 :
					OrgranisationParPrio((CPU.fileAttente+debutDeFile),(tasksNumber-debutDeFile),T_tasks,i);
				break;
				}
			}
		}
		if(CPU.fileAttente[0]<0)
            		QueueDown(CPU.fileAttente,tasksNumber);
		continueTasks=0;
		for(i=1;i<tasksNumber;i++)
		{
            		if(CPU.fileAttente[i]>=0)
                	(T_tasks+CPU.fileAttente[i])->tAttente++;
		}
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
	finalTouch(outputFile, T_tasks,tasksNumber,Pos);
	fclose(outputFile);
	openWhenDone(algorithm,preemption);
}
void RR(task* T_tasks,int tasksNumber)
{
	int i,Pos=0,inOutTime=0,actualCycle=0,lateEntry=-1,lateEntry2=-1;
	int continueTasks;//bool
	CPUtable CPU;
	FILE* outputFile;
	CPU.entreeSortie=(int*)malloc(sizeof(int)*tasksNumber);
	CPU.fileAttente=(int*)malloc(sizeof(int)*tasksNumber);
	for(i=0;i<tasksNumber;i++)
	{
		*(CPU.entreeSortie+i)=-1;
		*(CPU.fileAttente+i)=-1;
	}
	outputFile=createFile(4,0);
	fileInitialisation(outputFile, (2*tasksNumber));
	do
	{
		if(inOutTime>0)// s'il y une tache en E/S
		{
			inOutTime--;
            		if(inOutTime==0)
           		{
				if((T_tasks+CPU.entreeSortie[0])->length>0)
                		{
                    			lateEntry2=CPU.entreeSortie[0];
                    			(T_tasks+CPU.entreeSortie[0])->tSejour=Pos-(T_tasks+CPU.entreeSortie[0])->entry;
				}
				QueueDown(CPU.entreeSortie,tasksNumber);
				
				if(CPU.entreeSortie[0]>=0)
                   			 inOutTime=(T_tasks+CPU.entreeSortie[0])->inOutLength;
            		}
		}

		if((CPU.fileAttente[0]>=0)&&(actualCycle==0))//s'il y a une tache active
          		  actualCycle=(T_tasks+CPU.fileAttente[0])->cycle;
        	if(actualCycle)
		{
			if((T_tasks+CPU.fileAttente[0])->tReponse<0)
                		(T_tasks+CPU.fileAttente[0])->tReponse=Pos-((T_tasks+CPU.fileAttente[0])->entry)-1;
			actualCycle--;
			(T_tasks+CPU.fileAttente[0])->length--;//on reduit sa durée d'execution de 1
			if((T_tasks+CPU.fileAttente[0])->length==0)//si la tache se termine
			{
				(T_tasks+CPU.fileAttente[0])->tSejour=Pos-(T_tasks+CPU.fileAttente[0])->entry;
				actualCycle=0;
				if((T_tasks+CPU.fileAttente[0])->inOutLength>0)//si elle a une durée d'entrée sortie
				{
					for(i=0;CPU.entreeSortie[i]>=0;i++);//on cherche la celule libre en E/S
						CPU.entreeSortie[i]=CPU.fileAttente[0];//on y met la tache qui était active
					if(i==0)
						inOutTime=(T_tasks+CPU.fileAttente[0])->inOutLength;
				}
				QueueDown(CPU.fileAttente,tasksNumber);
			}
			else if(actualCycle==0)//si le cycle se termine
			{
                		i=0;
                		while((i<tasksNumber)&&((T_tasks+i)->entry!=Pos))
                    			i++;
                		if((i==tasksNumber)&&(inOutTime==0)&&(CPU.fileAttente[1]<0)&&(lateEntry2<0))
                    			actualCycle=(T_tasks+CPU.fileAttente[0])->cycle;
				else
              			{
                			if((T_tasks+CPU.fileAttente[0])->inOutLength>0)//si elle a une durée d'entrée sortie
					{
						for(i=0;CPU.entreeSortie[i]>=0;i++);//on cherche la celule libre en E/S
                    				CPU.entreeSortie[i]=CPU.fileAttente[0];//on y met la tache qui était active
						if(i==0)
							inOutTime=(T_tasks+CPU.fileAttente[0])->inOutLength;
					}
					else
                				lateEntry=CPU.fileAttente[0];
                			QueueDown(CPU.fileAttente,tasksNumber);
                		}
			}
		}
		for(i=0;i<tasksNumber;i++)//On verifie s'il y a des nouvelles taches
		{
			if((T_tasks+i)->entry==Pos)// entrée des nouvelles taches à la liste d'attente
					OrgranisationFCFS(CPU.fileAttente,i);
		}
		if(lateEntry>=0)
		{
          		  OrgranisationFCFS(CPU.fileAttente,lateEntry);
           		 lateEntry=-1;
		}
		if(lateEntry2>=0)
		{
            		OrgranisationFCFS(CPU.fileAttente,lateEntry2);
            		lateEntry2=-1;
		}
		if(CPU.fileAttente[0]<0)
			QueueDown(CPU.fileAttente,tasksNumber);
		continueTasks=0;
		for(i=1;i<tasksNumber;i++)
		{
           		if(CPU.fileAttente[i]>=0)
               			(T_tasks+CPU.fileAttente[i])->tAttente++;
		}
		i=0;
		if(inOutTime>0)
		continueTasks=1;
		while((i<tasksNumber)&&(continueTasks==0))
		{
			if((T_tasks+i)->length!=0)
				continueTasks=1;
			i++;
		}
		Pos++;
		writeFromCPUtoFile(outputFile,T_tasks,CPU,tasksNumber);//change
	}while(continueTasks);
	free(CPU.entreeSortie);
	free(CPU.fileAttente);
	finalTouch(outputFile, T_tasks,tasksNumber,Pos);
	fclose(outputFile);
	openWhenDone(4,0);
}
