// write a program to check file present in directory or not

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h> 

#define MAX_PATH_SIZE 100
#define FULL_PATH_SIZE 250

//get directory and file name
void GetDirAndFile(char *directory, char *filename)
{
    //set null bytes into file path buffer
    memset(directory, '\0', MAX_PATH_SIZE);
    memset(filename, '\0', MAX_PATH_SIZE);

    printf("Enter name of directory: ");
    scanf("%s", directory);

    printf("Enter name of file: ");
    scanf("%s", filename);
}

int main()
{
    char directory[MAX_PATH_SIZE];
    char filename[MAX_PATH_SIZE];
    char relativePath[FULL_PATH_SIZE];
    char absolutePath[FULL_PATH_SIZE];

    DIR *dirPtr = NULL;
    struct dirent *dirEntry = NULL;

    //get directory and file name from user
    GetDirAndFile(directory, filename);

    //open directory
    dirPtr = opendir(directory);

    //check for error
    if(dirPtr == NULL)
    {
        perror("Directory not opened");
        return -1;
    }

    //read directory files
    while((dirEntry = readdir(dirPtr)) != NULL)
    {
        if (strcmp(dirEntry->d_name, filename) == 0)
        {
            memset(relativePath, '\0', FULL_PATH_SIZE);

            //create relative path
            strcpy(relativePath, directory);
            strcat(relativePath, "/");
            strcat(relativePath, filename);

            //create absolute path
            if(realpath(relativePath, absolutePath) != NULL)
            {
                printf("File found at: %s\n", absolutePath);
            }
            else
            {
                perror("realpath failed");
            }

            closedir(dirPtr);
            return 0;
        }
    }

    closedir(dirPtr);
    printf("File not found in directory.\n");
    
    return -1;
}