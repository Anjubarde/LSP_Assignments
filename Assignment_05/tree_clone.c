// Program to display a directory structure in tree format

#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>

#define MAX_PATH 500

//skip . and .. entries
int IsDotDirectory(const char *name)
{
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        return 1; //skip
    }
    return 0; //do not skip
}

//print indentation based on directory depth
void PrintIndent(int depth)
{
    for(int i=0; i<depth; i++)
    {
        printf("     ");
    }
}

void DisplayTree(const char *path, int depth)
{
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    struct stat sb;
    char fullPath[MAX_PATH];

    //open current directory
    dp = opendir(path);
    if(dp == NULL)
    {
        perror("Cannot open directory");
        return;
    }

    //traverse diectory entries
    while((entry = readdir(dp)) != NULL)
    {
        //skip special entries
        if(IsDotDirectory(entry->d_name))
        {
            continue;
        }

        //build full path
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if(lstat(fullPath, &sb) == -1)
        {
            perror("lstat failed");
            continue;
        }

        PrintIndent(depth);

        //check file type
        if(S_ISDIR(sb.st_mode)) //directory file
        {
            printf("[D] %s\n", entry->d_name);

            //call again for new directory
            DisplayTree(fullPath, depth+1);
        }
        else if(S_ISLNK(sb.st_mode)) //symlink file
        {
            printf("[L] %s\n", entry->d_name);
        }
        else if(S_ISREG(sb.st_mode)) //regular file
        {
            printf("[F] %s\n", entry->d_name);
        }
        else //different file
        {
            printf("%s\n", entry->d_name);
        }

    }

    //close directory
    closedir(dp);

}

int main(int argc, char *argv[])
{
    DIR *dp = NULL;
    struct dirent *ptr = NULL;
    if(argc != 2)
    {
        printf("Incorrect number of argument is passed...!");
        return -1;
    }

    DisplayTree(argv[1], 1);

    return 0;
}