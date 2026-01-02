// Program to combine multiple input files into a single output file

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>

#define BUFFER_SIZE 200

int main(int argc, char *argv[])
{   
    int sourceFd, destFd;
    char Buffer[BUFFER_SIZE];
    int iRet, bytesRead;

    //validate minimum required arguments
    if(argc < 3)
    {
        printf("Incorrect number of argument.\n");
        return -1;
    }

    //open destination file
    destFd = open(argv[1], O_WRONLY | O_CREAT, 0777);
    if(destFd == -1)
    {
        perror("Error while opening destination file");
        return -1;
    }

    //iterate for each input file
    for(int i=2; i<argc; i++)
    {
        //open source file
        sourceFd = open(argv[i], O_RDONLY);

        //clear buffer
        memset(Buffer, '\0', BUFFER_SIZE);

        //read file bytes
        while((bytesRead = read(sourceFd, Buffer, BUFFER_SIZE-1)) != 0)
        {
            Buffer[BUFFER_SIZE] = '\0';
            //write into destination file
            iRet = write(destFd, Buffer, bytesRead);
            if(iRet == -1)
            {
                printf("Error during write: %s\n", strerror(errno));
                //close opened files
                close(sourceFd);
                close(destFd);
                return -1;
            }
            //clear buffer
            memset(Buffer, '\0', BUFFER_SIZE);
        }
        //close source file
        close(sourceFd);
    }

    printf("Files data merged successfully.\n");

    //close destination file
    close(destFd);

    return 0;
}