#ifndef CPUmanagement
#define CPUmanagement
#include "FilesManipulation.h"
void OrgranisationDeFile(int* Q,int Qsize,task* T,int newTask);
void QueueDown(int* Q,int Qsize);
void SJF(task* T_tasks,int tasksNumber, int preemption);
#endif
