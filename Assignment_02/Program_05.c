// write a program to find largest file in directory

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include<sys/types.h>


#define MAX_PATH_SIZE 100
#define FULL_PATH_SIZE 200

//get directory name from user
void GetDirectoryName(char *directoryName)
{
    memset(directoryName, '\0', MAX_PATH_SIZE);

    printf("Enter directory name to open: ");
    scanf("%s", directoryName);
}

//check the special entries of . and ..
int SkipSpecialEntries(const char *entryName)
{
    if((strcmp(entryName, ".") == 0) || (strcmp(entryName, "..") == 0))
    {
        return 1;
    }
    return 0;
}

int main()
{
    char directoryName[MAX_PATH_SIZE];
    char filePath[FULL_PATH_SIZE];
    char largestFileName[MAX_PATH_SIZE];

    DIR *dirStream = NULL;
    struct dirent *dirEntry = NULL;
    struct stat fileStat;
    int iRet = 0;

    off_t largestFileSize = 0;

    //get directory name from user
    GetDirectoryName(directoryName);

    //open directory
    dirStream = opendir(directoryName);

    //check for error
    if(dirStream == NULL)
    {
        perror("opendir failed");
        return -1;
    }
    
    printf("Directory opened successfully...!\n");

    //read directory one by one for all files
    while((dirEntry = readdir(dirStream)) != NULL)
    {
        //skip special entires
        if(SkipSpecialEntries(dirEntry->d_name))
        {
            continue;
        }

        strcpy(filePath, directoryName);
        strcat(filePath, "/");
        strcat(filePath, dirEntry->d_name);

        iRet = lstat(filePath, &fileStat);

        if(iRet == -1)
        {
            perror("lstat failed");
            closedir(dirStream);
            return -1;
        }

        //consider only regular file
        if(S_ISREG(fileStat.st_mode))
        {
            if(fileStat.st_size > largestFileSize)
            {
                largestFileSize = fileStat.st_size;
                strcpy(largestFileName, dirEntry->d_name);
            }
        }
    }

    if(largestFileSize > 0)
    {
        printf("Largets File: %s\n", largestFileName);
        printf("Size: %ld\n", (long)largestFileSize);
    }
    else{
        printf("No regular files found.\n");
    }

    closedir(dirStream);

    return 0;
}