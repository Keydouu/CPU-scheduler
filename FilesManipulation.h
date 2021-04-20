#ifndef FilesManipulation
#define FilesManipulation
typedef struct task task;
typedef struct CPUtable CPUtable;
struct task
{
	char Name[3];
	int entry,length,priority,inOutLength;
};
struct CPUtable
{
	int *entreeSortie,*fileAttente;
};
int readNumber (FILE* fichier);
int numberOfTasks(FILE* fichier);
task tasksReader(FILE* fichier);
void writeFromCPUtoFile(FILE* fichier,task* T_tasks,CPUtable CPU,int Qsize);
#endif
