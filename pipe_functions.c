#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "pipe_functions.h"
#include "file_functions.h"
#include "math_functions.h"

int **createPipes(int fileCount) //create pipes based on number of files
{
    int **pipes = malloc(fileCount * sizeof(int *));

   for (int i = 0; i < fileCount; i++) 
    {
        pipes[i] = malloc(2 * sizeof(int));

        if (pipe(pipes[i]) == -1) 
	{
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    return pipes;
}


pid_t *createChildProcesses(int fileCount, int **pipes, char **filenames) //create child processes based on number of files
{
    pid_t *childProcesses = malloc(fileCount * sizeof(pid_t));
    
    for (int i = 0; i < fileCount; i++)
    {
        childProcesses[i] = fork();
        if (childProcesses[i] == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (childProcesses[i] == 0)
        {
            //child process
            close(pipes[i][1]); //close write end of the pipe in the child process

            char filename[MAX_FILENAME_LENGTH];
            int bytesRead = read(pipes[i][0], filename, MAX_FILENAME_LENGTH - 1);
            if (bytesRead == -1)
            {
                perror("Failed to read from pipe");
                exit(EXIT_FAILURE);
            }
            filename[bytesRead] = '\0';

            int file = open(filename, O_RDONLY);
            if (file == -1)
            {
                perror("Failed to open file");
                exit(EXIT_FAILURE);
            }

            char uniqueID[MAX_RESULT_LENGTH];
            bytesRead = read(file, uniqueID, MAX_RESULT_LENGTH - 1);
            if (bytesRead == -1)
            {
                perror("Failed to read unique ID from file");
                exit(EXIT_FAILURE);
            }
            uniqueID[bytesRead] = '\0';

            int num1, num2;
            char operator;

            //read the first operand from the file
            bytesRead = read(file, &num1, sizeof(int));
            if (bytesRead != sizeof(int))
            {
                write(STDERR_FILENO, "Failed to read the first operand from file\n", 43);
                exit(EXIT_FAILURE);
            }

            //read the operator from the file
            bytesRead = read(file, &operator, sizeof(char));
            if (bytesRead != sizeof(char))
            {
                write(STDERR_FILENO, "Failed to read the operator from file\n", 38);
                exit(EXIT_FAILURE);
            }

            //read the second operand from the file
            bytesRead = read(file, &num2, sizeof(int));
            if (bytesRead != sizeof(int))
            {
                write(STDERR_FILENO, "Failed to read the second operand from file\n", 44);
                exit(EXIT_FAILURE);
            }

            int result = solveEquation(num1, operator, num2);

            close(file);

            char resultStr[MAX_RESULT_LENGTH];
            int resultLength = snprintf(resultStr, sizeof(resultStr), "%s:%d", uniqueID, result);

            if (write(pipes[i][0], resultStr, resultLength + 1) == -1)
            {
                perror("Failed to write to pipe");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
        else
        {
            //parent process
            close(pipes[i][0]); //close read end of the pipe in the parent process

            int filenameLength = strlen(filenames[i]);
            if (write(pipes[i][1], filenames[i], filenameLength + 1) == -1)
            {
                perror("Failed to write to pipe");
                exit(EXIT_FAILURE);
            }
        }
    }

    return childProcesses;
}


void readResults(int fileCount, int **pipes, pid_t *childProcesses) //write results to a text file
{
    int resultsFile = open("results.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (resultsFile == -1) 
    {
        perror("Failed to create results.txt file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fileCount; i++) 
    {
        char resultStr[MAX_RESULT_LENGTH];
        if (read(pipes[i][0], resultStr, MAX_RESULT_LENGTH) == -1) 
	{
            perror("Failed to read from pipe");
            exit(EXIT_FAILURE);
        }

        printf("Child process %d result: %s\n", childProcesses[i], resultStr);
	printf("Writing result to results.txt file\n");
        printf("%s\n", resultStr);
        dprintf(resultsFile, "%s\n", resultStr);
    }

    close(resultsFile);
}

void deallocateMemory(int fileCount, int **pipes, char **filenames, pid_t *childProcesses) //memory deallocation for all files and processes created
{
    //close all remaining pipe ends in the parent process
    for (int i = 0; i < fileCount; i++) 
    {
        close(pipes[i][1]); //close write end of the pipe in the parent process
    }

    //wait for all child processes to finish
    for (int i = 0; i < fileCount; i++) 
    {
        int status;
        if (waitpid(childProcesses[i], &status, 0) == -1) 
	{
            perror("Failed to wait for child process");
            exit(EXIT_FAILURE);
        }
    }

    //free allocated memory for pipes
    for (int i = 0; i < fileCount; i++) 
    {
        free(pipes[i]);
    }
    free(pipes);

    //free allocated memory for filenames
    deallocateFilenamesArray(filenames, fileCount);

    //free allocated memory for child process IDs
    free(childProcesses);
}

