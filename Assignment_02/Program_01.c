// Program to read and display the contents of a file

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

#define BUFFER_SIZE 1024

int main()
{
    int fd = 0;
    char fileName[100];
    char Buffer[BUFFER_SIZE];
    int bytesRead = 0;
    int totalBytes = 0;

    //read file name from the user
    printf("Enter file name to open the file: \n");
    scanf("%s", fileName);

    //oepn the file in read-only mode
    fd = open(fileName, O_RDONLY);

    //check if file opening failed
    if(fd == -1)
    {
        printf("Unable to open file: %s\n", strerror(errno));
        return -1;
    }

    //initialize buffer with null characters
    memset(Buffer, '\0', BUFFER_SIZE);

    //read file contents until end of the file is reached
    while((bytesRead = read(fd, Buffer, BUFFER_SIZE-1)) > 0)
    {
        Buffer[BUFFER_SIZE] = '\0';
        printf("%s", Buffer);
        totalBytes += bytesRead;
        
        //clear buffer
        memset(Buffer, '\0', BUFFER_SIZE);
    }

    //handle error during reading file
    if(bytesRead == -1) 
    {
        printf("\nError while reading file: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    //print total bytes read from file
    printf("\nTotal bytes read from file: %d\n", totalBytes);

    //close the file descriptor
    close(fd);

    return 0;
}