// write a program to move all files from one directory to another

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define MAX_PATH 256

int CopyAndDeleteFiles(const char *sourcePath, const char *destinationPath)
{
    int sourceFd = 0;
    int destinationFd = 0;
    char buffer[BUFFER_SIZE];
    ssize_t readBytes = 0;
    ssize_t writtenBytes = 0;
    struct stat fileStat;

    //open file
    sourceFd = open(sourcePath, O_RDONLY);
    if(sourceFd == -1)
    {
        perror("open source");
        return -1;
    }

    //get stat of file for get permissions
    if(lstat(sourcePath, &fileStat) == -1)
    {
        perror("lstat");
        close(sourceFd);
        return -1;
    }

    //create new file in destination directory
    destinationFd = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, fileStat.st_mode);
    if(destinationFd == -1)
    {
        perror("open destination");
        close(sourceFd);
        return -1;
    }

    //read and write all bytes of files from source to destination file
    while((readBytes = read(sourceFd, buffer, BUFFER_SIZE)) > 0)
    {
        //write bytes to destination file
        writtenBytes = write(destinationFd, buffer, readBytes);
        if (writtenBytes != readBytes)
        {
            perror("write");
            close(sourceFd);
            close(destinationFd);
            return -1;
        }
    }

    //close files
    close(sourceFd);
    close(destinationFd);

    //delete file from source directory
    unlink(sourcePath);
    return 0;
}

int main()
{
    char sourceDirectory[MAX_PATH];
    char destinationDirectory[MAX_PATH];
    char sourceFilePath[MAX_PATH];
    char destinationFilePath[MAX_PATH];
    DIR *dirStream = NULL;
    struct dirent *dirEntry = NULL;
    struct stat entryStat;
    int movedFilesCount = 0;

    //get source directory name
    printf("Enter source directory: ");
    scanf("%s", sourceDirectory);

    //get destination directory name
    printf("Enter destination directory: ");
    scanf("%s", destinationDirectory);

    //open directory
    dirStream = opendir(sourceDirectory);
    if(dirStream == NULL)
    {
        perror("opendir");
        return -1;
    }

    while((dirEntry = readdir(dirStream)) != NULL)
    {
        if((strcmp(dirEntry->d_name, ".") == 0) || (strcmp(dirEntry->d_name, "..") == 0))
        {
            continue;
        }

        //build source path
        strcpy(sourceFilePath, sourceDirectory);
        strcat(sourceFilePath, "/");
        strcat(sourceFilePath, dirEntry->d_name);

        //build destination path
        strcpy(destinationFilePath, destinationDirectory);
        strcat(destinationFilePath, "/");
        strcat(destinationFilePath, dirEntry->d_name);

        if(lstat(sourceFilePath, &entryStat) == -1)
        {
            continue;
        }

        // move only regular files
        if(S_ISREG(entryStat.st_mode))
        {
            if(rename(sourceFilePath, destinationFilePath) == -1)
            {
                // copy files to destination and delete from source
                if(CopyAndDeleteFiles(sourceFilePath, destinationFilePath) == 0)
                {
                    movedFilesCount++;
                }
            }
            else
            {
                movedFilesCount++;
            }
        }
    }

    //close directory
    closedir(dirStream);

    printf("Total files moved: %d\n", movedFilesCount);
    return 0;
}
