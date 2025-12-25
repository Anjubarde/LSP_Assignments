// write a program to copy all file contents from source to destination

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>


#define MAX_PATH_SIZE 100
#define BUFFER_SIZE 1024

void GetSourceAndDestination(char *srcPath, char *destPath)
{
    //set null bytes into file path buffer
    memset(srcPath, '\0', MAX_PATH_SIZE);
    memset(destPath, '\0', MAX_PATH_SIZE);

    printf("Enter name of source file: ");
    scanf("%s", srcPath);

    printf("Enter name of destination file: ");
    scanf("%s", destPath);
}

int main()
{
    char srcPath[MAX_PATH_SIZE];
    char destPath[MAX_PATH_SIZE];
    char buffer[BUFFER_SIZE];

    int srcFd = 0;
    int destFd = 0;
    int bytesRead = 0;
    int bytesWritten = 0;  

    struct stat fileStat;

    GetSourceAndDestination(srcPath, destPath);

    //open soruce file in read mode
    srcFd = open(srcPath, O_RDONLY);
    if(srcFd == -1)
    {
        perror("Source file open failed");
    }

    //check stat for permission
    if(stat(srcPath, &fileStat) == -1)
    {
        perror("stat failed");
        close(srcFd);
        return -1;
    }

    //open destination file in write mode
    destFd = open(destPath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(destFd == -1)
    {
        perror("Destination file open failed");
    }

    //read data and write into file
    while((bytesRead = read(srcFd, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[BUFFER_SIZE] = '\0';

        bytesWritten = write(destFd, buffer, bytesRead);
        if(bytesWritten != bytesRead)
        {
            perror("Write failed");
            close(srcFd);
            close(destFd);
            return -1;
        }
    }

    printf("File contents copied successfully...!\n");

    //preserve permissions
    chmod(destPath, fileStat.st_mode);

    close(srcFd);
    close(destFd);


    return 0;
}