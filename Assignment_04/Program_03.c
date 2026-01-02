// Program to copy file contents using pread() and pwrite() without modifying the file descriptor offset

#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[])
{
    int sourceFd, destFd;
    char Buffer[BUFFER_SIZE];
    int iRet, bytesRead, offset;

    //check argument error
    if(argc != 4)
    {
        printf("Incorrect argument is passed.\n");
        return -1;
    }

    //open source file
    sourceFd = open(argv[1], O_RDONLY);
    if(sourceFd == -1)
    {
        perror("Error during opening source file");
        return -1;
    }

    //open destination file
    destFd = open(argv[2], O_WRONLY | O_CREAT, 0777);
    if(destFd == -1)
    {
        perror("Error during opening destination file");
        return -1;
    }

    //clear buffer
    memset(Buffer, '\0', BUFFER_SIZE);

    while((bytesRead = pread(sourceFd, Buffer, BUFFER_SIZE-1, offset)) != 0)
    {
        Buffer[BUFFER_SIZE] = '\0';

        //write at offset
        iRet = pwrite(destFd, Buffer, bytesRead, offset);

        //move offset
        offset += iRet;
        
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

    printf("Data copied successfully.\n");

    //close opened files
    close(sourceFd);
    close(destFd);


    return 0;
}