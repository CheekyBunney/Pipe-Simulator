#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#define MAX_FILENAME_LENGTH 256

void printFilenames(char **filenames, int fileCount);
char **allocateFilenamesArray(int maxFiles);
void deallocateFilenamesArray(char **filenames, int fileCount);

#endif

