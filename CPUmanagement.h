#ifndef CPUmanagement
#define CPUmanagement
#include "FilesManipulation.h"
void OrgranisationParPrio(int* Q,int Qsize,task* T,int newTask);
void OrgranisationSJF(int* Q,int Qsize,task* T,int newTask);
void OrgranisationFCFS(int* Q,int newTask);
void QueueDown(int* Q,int Qsize);
void scheduling(task* T_tasks,int tasksNumber,int algorithm ,int preemption);
#endif
