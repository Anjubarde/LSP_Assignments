// Program to list directory entries with detailed file information
// including name, type, size, permissions, last modification time, and full path

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<sys/stat.h>

#define MAX_PATH_LEN 500

//skip . and .. entries
int IsDotDirectory(const char *name)
{
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        return 1; //skip
    }
    return 0; //do not skip
}

//display file type
void DisplayFileType(struct stat *sb)
{
    if (S_ISREG(sb->st_mode))
        printf("Regular File");
    else if (S_ISDIR(sb->st_mode))
        printf("Directory");
    else if (S_ISLNK(sb->st_mode))
        printf("Symbolic Link");
    else if (S_ISCHR(sb->st_mode))
        printf("Character Device");
    else if (S_ISBLK(sb->st_mode))
        printf("Block Device");
    else if (S_ISFIFO(sb->st_mode))
        printf("FIFO");
    else if (S_ISSOCK(sb->st_mode))
        printf("Socket");
    else
        printf("Unknown");
}

// print file permissions
void DisplayPermissions(struct stat *sb)
{
    printf("File permissions: ");

    // Owner permissions
    printf((sb->st_mode & S_IRUSR) ? "r" : "-");
    printf((sb->st_mode & S_IWUSR) ? "w" : "-");
    printf((sb->st_mode & S_IXUSR) ? "x" : "-");

    // Group permissions
    printf((sb->st_mode & S_IRGRP) ? "r" : "-");
    printf((sb->st_mode & S_IWGRP) ? "w" : "-");
    printf((sb->st_mode & S_IXGRP) ? "x" : "-");

    // Others permissions
    printf((sb->st_mode & S_IROTH) ? "r" : "-");
    printf((sb->st_mode & S_IWOTH) ? "w" : "-");
    printf((sb->st_mode & S_IXOTH) ? "x\n" : "-\n");
}


int main(int argc, char *argv[])
{
    DIR *dp = NULL;
    struct dirent *ptr = NULL;
    char absolutePath[MAX_PATH_LEN];
    int iRet = 0;
    struct stat sb;

    //handle incorrect number of arguments
    if(argc != 2)
    {
        printf("Incorrect number of argument.");
        return -1;
    }

    dp = opendir(argv[1]);

    //check for error
    if(dp == NULL)
    {
        perror("Directory not opened");
        return -1;
    }

    //read directory files
    while((ptr = readdir(dp)) != NULL)
    {
        //skip special entries
        if(IsDotDirectory(ptr->d_name) == 1)
        {
            continue;
        }
       
        //clear buffer
        memset(absolutePath, '\0', MAX_PATH_LEN);

        //create full path using snprintf
        snprintf(absolutePath, sizeof(absolutePath), "%s/%s", argv[1], ptr->d_name);

        //get lstat of file
        iRet = lstat(absolutePath, &sb);
        
        if(iRet == -1)
        {
            printf("lstat failed: %s\n", strerror(errno));
            return -1;
        }

        printf("File Name: %s\n", ptr->d_name);
        printf("File type: ");
        DisplayFileType(&sb);
        printf("\n");
        printf("File Size (bytes): %ld\n", sb.st_size);
        DisplayPermissions(&sb);
        printf("Last Modified Time: %s", ctime(&sb.st_mtime));
        printf("Path: %s\n", absolutePath);
        printf("==========================\n");
    }

    closedir(dp);

    return 0;
}