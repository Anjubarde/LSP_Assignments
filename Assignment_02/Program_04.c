// Program to list all files in a directory along with their file types

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH_SIZE 100
#define FULL_PATH_SIZE 200

//accept directory name from the user
void GetDirectoryName(char *dirName)
{
    //clear the buffer before taking input
    memset(dirName, '\0', MAX_PATH_SIZE);

    printf("Enter directory name to open: ");
    scanf("%s", dirName);
}

//check the special entries of . and ..
int SkipSpecialEntries(const char *name)
{
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        return 1;
    }
    return 0;
}

//display the type of file based on stat structure
void DisplayFileType(struct stat *sb)
{
    if (S_ISREG(sb->st_mode))
        printf("Regular File");
    else if (S_ISDIR(sb->st_mode))
        printf("Directory");
    else if (S_ISLNK(sb->st_mode))
        printf("Symbolic Link");
    else if (S_ISCHR(sb->st_mode))
        printf("Character Device");
    else if (S_ISBLK(sb->st_mode))
        printf("Block Device");
    else if (S_ISFIFO(sb->st_mode))
        printf("FIFO");
    else if (S_ISSOCK(sb->st_mode))
        printf("Socket");
    else
        printf("Unknown");
}

int main()
{
    char dirName[MAX_PATH_SIZE];
    char fullPath[FULL_PATH_SIZE];
    DIR *dp = NULL;
    struct dirent *ptr = NULL;
    struct stat sb;
    int iRet = 0;

    //get directory name from user
    GetDirectoryName(dirName);

    //open directory
    dp = opendir(dirName);

    //check for error
    if(dp == NULL)
    {
        printf("%s\n", strerror(errno));
        return -1;
    }
    
    printf("Directory opened successfully.\n");

    //read directory one by one for all files
    while((ptr = readdir(dp)) != NULL)
    {
        //skip special entires
        if(SkipSpecialEntries(ptr->d_name))
        {
            continue;
        }

        //clear full path buffer
        memset(fullPath, '\0', FULL_PATH_SIZE);

        //create full path
        strcpy(fullPath, dirName);
        strcat(fullPath, "/");
        strcat(fullPath, ptr->d_name);

        //get lstat of file
        iRet = lstat(fullPath, &sb);
        
        if(iRet == -1)
        {
            printf("lstat failed: %s\n", strerror(errno));
            return -1;
        }
        //file file name and file Type
        printf("File Name: %s", ptr->d_name);
        printf("\tFile type: ");
        DisplayFileType(&sb);
        printf("\n");
    }

    //close directory
    closedir(dp);

    return 0;
}