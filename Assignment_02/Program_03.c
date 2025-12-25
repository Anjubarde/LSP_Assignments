// write a program to list all files from directory

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<dirent.h>

#define MAX_PATH 100

//accept directory name from the user
void GetDirectoryName(char *dirName)
{
    //clear the buffer before use
    memset(dirName, '\0', MAX_PATH);

    printf("Enter directory name to open: ");
    scanf("%s", dirName);
}

//check whether the directory entry is "." or ".."
int SkipSpecialEntries(const char *name)
{
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        return 1;
    }
    return 0;
}

int main()
{
    char dirName[MAX_PATH];
    DIR *dp = NULL;
    struct dirent *ptr = NULL;

    //read directory name from user
    GetDirectoryName(dirName);

    //open directory
    dp = opendir(dirName);

    //check for error
    if(dp == NULL)
    {
        printf("%s\n", strerror(errno));
        return -1;
    }
    
    printf("Directory opened successfully...!\n");

    //iterate through all entries in the directory
    while((ptr = readdir(dp)) != NULL)
    {
        //ignore "." and ".." entries
        if(SkipSpecialEntries(ptr->d_name))
        {
            continue;
        }
        //print file names
        printf("File name: %s\n", ptr->d_name);
    }

    //close directory
    closedir(dp);

    return 0;
}