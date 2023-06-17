#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "file_functions.h"

void printFilenames(char **filenames, int fileCount) 
{
    for (int i = 0; i < fileCount; i++) 
    {
        printf("%s\n", filenames[i]);
    }
}

char **allocateFilenamesArray(int maxFiles) 
{
    char **filenames = malloc(maxFiles * sizeof(char *));
    for (int i = 0; i < maxFiles; i++) 
    {
        filenames[i] = malloc(MAX_FILENAME_LENGTH * sizeof(char));
    }
    return filenames;
}

void deallocateFilenamesArray(char **filenames, int fileCount) 
{
    for (int i = 0; i < fileCount; i++) 
    {
        free(filenames[i]);
    }
    free(filenames);
}

