// write a program to delete the empty files within directory

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH 256

int main()
{
    char directoryPath[MAX_PATH];
    char filePath[MAX_PATH];

    DIR *dirStream = NULL;
    struct dirent *dirEntry = NULL;
    struct stat fileStat;

    int deletedFilesCount = 0;


    //get directory name from user
    printf("Enter directory name: ");
    scanf("%s", directoryPath);

    //open directory
    dirStream = opendir(directoryPath);
    if(dirStream == NULL)
    {
        perror("opendir failed");
        return -1;
    }

    //read directory
    while((dirEntry = readdir(dirStream)) != NULL)
    {
        //skip . and ..
        if((strcmp(dirEntry->d_name, ".") == 0) ||
            (strcmp(dirEntry->d_name, "..") == 0))
        {
            continue;
        }
        
        //create full path
        strcpy(filePath, directoryPath);
        strcat(filePath, "/");
        strcat(filePath, dirEntry->d_name);

        //get stat of file to get size
        if(lstat(filePath, &fileStat) == -1)
        {
            continue;
        }

        //check for empty regular file
        if(S_ISREG(fileStat.st_mode) && fileStat.st_size == 0)
        {
            //delete file
            if(unlink(filePath) == 0)
            {
                printf("Deleted: %s\n", dirEntry->d_name);
                deletedFilesCount++;
            }
        }
    }

    //close directory
    closedir(dirStream);

    printf("Total empty files deleted: %d\n", deletedFilesCount);
    return 0;
}
