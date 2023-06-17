#ifndef PIPE_FUNCTIONS_H
#define PIPE_FUNCTIONS_H

#define MAX_RESULT_LENGTH 512

int **createPipes(int fileCount);
pid_t *createChildProcesses(int fileCount, int **pipes, char **filenames);
void performParentOperations(int fileCount, int **pipes, pid_t *childProcesses);
void readResults(int fileCount, int **pipes, pid_t *childProcesses);
void deallocateMemory(int fileCount, int **pipes, char **filenames, pid_t *childProcesses);

#endif

