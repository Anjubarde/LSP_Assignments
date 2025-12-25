// Write a program to open file. Accept name from user and print fd.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = 0;
    char fileName[100];

    printf("Enter file name to open the file: \n");
    scanf("%s", fileName);

    // open file in read mode
    fd = open(fileName, O_RDONLY);

    if (fd == -1)
    {
        perror("UNABLE TO OPEN FILE");
        return -1;
    }

    // if success printf fd
    printf("File opened successfully with fd: %d\n", fd);

    // close file
    close(fd);

    return 0;
}