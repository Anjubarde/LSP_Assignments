// Program to write a user provided string into a file

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#define BUFFER_SIZE 1024

int main()
{
    char fileName[100];
    char iBuffer[BUFFER_SIZE];
    int fd = 0;
    int iRet = 0;

    //read file name from the user
    printf("Enter name of file: ");
    scanf("%s", fileName);

    //consume lefover newline character from input buffer
    getchar();

    //clear input buffer
    memset(iBuffer, '\0', BUFFER_SIZE);

    //read string input from the user
    printf("Enter string to write in file: ");
    fgets(iBuffer, BUFFER_SIZE, stdin);

    //open the file in write and append mode (create if it does not exist)
    fd = open(fileName, O_WRONLY | O_APPEND | O_CREAT, 0777);

    //check if the opening failed
    if(fd == -1)
    {
        printf("Unable to open file: %s\n", strerror(errno));
        return -1;
    }

    //write the string data into the file
    iRet = write(fd, iBuffer, strlen(iBuffer));

    //check if write operation failed
    if(iRet == -1)
    {
        printf("Unable to write in file: %s\n", strerror(errno));
        return -1;
    }

    //display the total number of bytes written
    printf("%d bytes written successfully into file.\n", iRet);

    //close file
    close(fd);

    return 0;
}

