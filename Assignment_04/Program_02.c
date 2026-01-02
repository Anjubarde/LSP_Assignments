// Program to copy file contents starting from a specified offset
// into a destination file

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sourceFd, destFd;
    char Buffer[BUFFER_SIZE];
    int iRet = 0;
    int bytesRead = 0;
    int offset = 0;

    if(argc != 4)
    {
        printf("Incorrect argument is passed.\n");
        return -1;
    }

    //open source file
    sourceFd = open(argv[1], O_RDONLY);
    if(sourceFd == -1)
    {
        perror("Error opening source file");
        return -1;
    }

    //open destination file
    destFd = open(argv[2], O_WRONLY | O_CREAT, 0777);
    if(destFd == -1)
    {
        perror("Error opening destination file");
        return -1;
    }

    offset = atoi(argv[3]); //convert the character offset to int

    //set offset of source file
    iRet = lseek(sourceFd, offset, SEEK_SET);
    if(iRet == -1)
    {
        printf("%s\n", strerror(errno));
        return -1;
    }

    //clean buffer
    memset(Buffer, '\0', BUFFER_SIZE);

    while((bytesRead = read(sourceFd, Buffer, BUFFER_SIZE-1)) != 0)
    {
        Buffer[BUFFER_SIZE] = '\0';

        //write bytes to destination file
        iRet = write(destFd, Buffer, bytesRead);
        if(iRet == -1)
        {
            printf("%s\n", strerror(errno));

            //close opened files
            close(sourceFd);
            close(destFd);
            return -1;
        }

        //clear buffer
        memset(Buffer, '\0', BUFFER_SIZE);
    }
    printf("Data successfully copied from the specified offset.\n");

    //close opened files
    close(sourceFd);
    close(destFd);

    return 0;
}