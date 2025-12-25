// Write a program to display complete file information

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// Display file type
const char *DisplayFileType(struct stat *sobj)
{
    if (S_ISREG(sobj->st_mode))
    {
        return "Regular file";
    }
    else if (S_ISDIR(sobj->st_mode))
    {
        return "Directory\n";
    }
    else if (S_ISLNK(sobj->st_mode))
    {
        return "Symbolic link\n";
    }
    else if (S_ISCHR(sobj->st_mode))
    {
        return "Character device";
    }
    else if (S_ISBLK(sobj->st_mode))
    {
        return "Block device";
    }
    else
    {
        return "Unknown";
    }
}

// Display file permissions
void DisplayFilePermissions(struct stat *sobj)
{
    printf("File permissions: ");

    // Owner permissions
    printf((sobj->st_mode & S_IRUSR) ? "r" : "-");
    printf((sobj->st_mode & S_IWUSR) ? "w" : "-");
    printf((sobj->st_mode & S_IXUSR) ? "w" : "-");

    // Group permissions
    printf((sobj->st_mode & S_IRGRP) ? "r" : "-");
    printf((sobj->st_mode & S_IWGRP) ? "w" : "-");
    printf((sobj->st_mode & S_IXGRP) ? "x" : "-");

    // Others permissions
    printf((sobj->st_mode & S_IROTH) ? "r" : "-");
    printf((sobj->st_mode & S_IWOTH) ? "w" : "-");
    printf((sobj->st_mode & S_IXOTH) ? "x\n" : "-\n");
}

// Display stat of file
void DisplayStat(struct stat *sobj)
{
    printf("File size (bytes): %ld\n", sobj->st_size);
    printf("Block size: %ld\n", sobj->st_blksize);
    printf("Inode number: %ld\n", sobj->st_ino);
    DisplayFilePermissions(sobj);
    printf("Hard link count: %ld\n", sobj->st_nlink);
    printf("Owner UID: %d\n", sobj->st_uid);
    printf("Owner GID: %d\n", sobj->st_gid);
    printf("File Type: %s\n", DisplayFileType(sobj));
    printf("Last access time: %s\n", ctime(&sobj->st_atime));
    printf("Last modify time: %s\n", ctime(&sobj->st_mtime));
}

int main()
{
    struct stat sobj;
    int iRet = 0;
    char fileName[50];

    // get file name from user
    printf("Enter file name: ");
    scanf("%s", fileName);

    iRet = stat(fileName, &sobj);

    if (iRet == -1)
    {
        printf("Error Occured: %s\n", strerror(errno));
        return -1;
    }

    DisplayStat(&sobj);

    return 0;
}