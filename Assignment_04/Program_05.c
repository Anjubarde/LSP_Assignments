// Program to display the contents of a file in reverse order (chunk-wise)

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int fd, bytesRead;
    char Buffer[BUFFER_SIZE];

    //check argument length for error
    if(argc != 2)
    {
        printf("Incorrect number of argument is paased");
        return -1;
    }

    //open file 
    fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        perror("Error occured");
        return -1;
    }

    memset(Buffer, '\0', BUFFER_SIZE);

    printf("Data from file in reverse order: \n");

    //read file data
    while((bytesRead = read(fd, Buffer, BUFFER_SIZE-1)) != 0)
    {
        Buffer[BUFFER_SIZE] = '\0';

        //print in reverse
        for(int i = bytesRead; i >= 0; i--)
        {
            printf("%c",Buffer[i]);
        }
        //clear buffer
        memset(Buffer, '\0', BUFFER_SIZE);
    }

    //close file
    close(fd);

    return 0;

}