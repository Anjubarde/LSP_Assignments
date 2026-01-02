// Program to duplicate data from a source file into a target file

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sourceFD, targetFD;
    ssize_t readCount;
    char Buffer[BUFFER_SIZE];
    int iRet = 0;

    //validate command line arguments
    if(argc != 3)
    {
        printf("Incorrect number of argument\n");
        return -1;
    }

    //open source file
    sourceFD = open(argv[1], O_RDONLY);
    if(sourceFD == -1)
    {
        switch (errno)
        {
        case EACCES:
            printf("You don't have permission to access file\n");
            break;
        
        case ENOENT:
            printf("File not found\n");
            break;
        
        default:
            printf("%s\n", strerror(errno));
        }
        return -1;
    }

    //open target file
    targetFD = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(targetFD == -1)
    {
        perror("Failed to open destination file\n");
        return -1;
    }

    //clear buffer
    memset(Buffer, '\0', BUFFER_SIZE);

    //reading file content and writting it in other file
    while((readCount = read(sourceFD, Buffer, BUFFER_SIZE-1)) != 0)
    {
        Buffer[BUFFER_SIZE] = '\0';
        iRet = write(targetFD, Buffer, readCount);
        if(iRet == -1)
        {
            perror("Error while writting data into file\n");
            close(sourceFD);
            close(targetFD);
            return -1;
        }
        //clear buffer
        memset(Buffer, '\0', BUFFER_SIZE);
    }

    printf("File copied successfully.\n");

    //close file
    close(sourceFD);
    close(targetFD);
    return 0;
}