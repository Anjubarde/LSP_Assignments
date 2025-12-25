// write a program to read N bytes from file

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char fileName[100];
    int iNum = 0;
    int fd = 0;
    int iRet = 0;

    // get file name from user
    printf("Enter file name to open: ");
    scanf("%s", fileName);

    // get number of bytes to read
    printf("Enter number of bytes to read: ");
    scanf("%d", &iNum);

    fd = open(fileName, O_RDONLY);

    if (fd == -1)
    {
        printf("Unable to open file: %s\n", strerror(errno));
        return -1;
    }

    char Buffer[iNum+1];

    memset(Buffer, '\0', iNum+1);

    // read bytes from file
    iRet = read(fd, Buffer, iNum);

    //check error
    if (iRet == -1)
    {
        printf("Error is occured: %s\n", strerror(errno));
        return -1;
    }

    //display the bytes
    printf("Number of bytes from file: %s\n", Buffer);

    close(fd);

    return 0;
}
