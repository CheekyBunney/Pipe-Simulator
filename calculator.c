#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "file_functions.h"
#include "math_functions.h"
#include "pipe_functions.h"

#define MAX_FILES 100

int main() {
    DIR *dir;
    struct dirent *entry;
    char **filenames;
    int fileCount = 0;
    int **pipes;
    pid_t *childProcesses;

    //dynamically allocate memory for the filenames array
    filenames = allocateFilenamesArray(MAX_FILES);

    //open the current directory
    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    //read directory entries
    while ((entry = readdir(dir)) != NULL) {
        //check if the entry is a regular file and ends with ".usp"
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".usp") != NULL) {
            //copy the filename to the array
            strncpy(filenames[fileCount], entry->d_name, MAX_FILENAME_LENGTH - 1);
            filenames[fileCount][MAX_FILENAME_LENGTH - 1] = '\0';
            fileCount++;

            //resize the filenames array if necessary
            if (fileCount >= MAX_FILES) 
            {
                printf("Maximum file count reached\n");
                break;
            }
        }
    }

    closedir(dir);

    //print retrieved filenames 
    printf("Found %d .usp file(s):\n", fileCount);
    printFilenames(filenames, fileCount);

    //create pipes for each file retrieved
    pipes = createPipes(fileCount);
    childProcesses = createChildProcesses(fileCount, pipes, filenames);

    //wait for child processes to finish and read the results
    readResults(fileCount, pipes, childProcesses);

    //free allocated memory
    deallocateMemory(fileCount, pipes, filenames, childProcesses);

    return 0;
}

