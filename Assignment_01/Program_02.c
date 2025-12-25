// Write a program to open file in given mode and display fd

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// convert mode to flags
int ConvertModeToFlags(char *mode)
{
    if (strcmp(mode, "R") == 0)
    {
        return O_RDONLY;
    }
    else if (strcmp(mode, "W") == 0)
    {
        return O_WRONLY;
    }
    else if (strcmp(mode, "RW") == 0)
    {
        return O_RDWR;
    }
    else if (strcmp(mode, "A") == 0)
    {
        return O_APPEND | O_CREAT;
    }
    else
    {
        return -1;
    }
}

int main()
{
    int fd = 0;
    char mode[3];
    int flag;

    // get file mode from user
    printf("Enter mode (R / W / RW / A): \n");
    scanf("%s", mode);

    // convert mode to flags
    flag = ConvertModeToFlags(mode);

    if (flag == -1)
    {
        printf("File open skipped due to invalid mode.\n");
        return -1;
    }

    // open file in given mode
    fd = open("Hello.txt", flag);

    // if file is unable to open handle error
    if (fd == -1)
    {
        perror("Unable to open file");
        return -1;
    }

    printf("File is opened successfully with fd: %d\n", fd);

    return 0;
}