// Program to calculate data from given directory:
// 1) total regular files
// 2) total directories visited
// 3) cumulative size of regular files (bytes)
// 4) largest file name with its size

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>
#include<sys/stat.h>

#define MAX_PATH 500

long totalFiles = 0;
long totalDirs = 0;
long long totoalSize = 0;
char largestFile[MAX_PATH];
long long largestSize = 0;

//skip . and .. entries
int IsDotDirectory(const char *name)
{
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        return 1; //skip
    }
    return 0; //do not skip
}

void ScanDirectory(const char *path)
{
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    struct stat sb;
    char Path[MAX_PATH];
    
    //increase directory count when call ScanDirectory
    totalDirs++;

    //open directory
    dp = opendir(path);
    if(dp == NULL)
    {
        perror("Cannot open directory");
        return;
    }

    //read directory entries
    while((entry = readdir(dp)) != NULL)
    {
        //skip special entires
        if(IsDotDirectory(entry->d_name))
        {
            continue;
        }

        //build path for file
        snprintf(Path, sizeof(Path), "%s/%s", path, entry->d_name);

        //get stat for file
        if(lstat(Path, &sb) == -1)
        {
            perror("lstat failed");
            continue;
        }

        //check for regular file
        if(S_ISREG(sb.st_mode))
        {
            //increase file count
            totalFiles++;
            //increase file size
            totoalSize += sb.st_size;

            //check if file is large than previous large file
            if(sb.st_size > largestSize)
            {
                largestSize = sb.st_size;
                strncpy(largestFile, entry->d_name, MAX_PATH - 1);
                largestFile[MAX_PATH - 1] = '\0';
            }
        }
        else if(S_ISDIR(sb.st_mode)) //check for directory file
        {
            //call again for directory
            ScanDirectory(Path);
        }
    }
    //close directory
    closedir(dp);
    
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Incorrect number of argument.");
        return -1;
    }

    ScanDirectory(argv[1]);

    printf("\n========== Directory Analysis ==========\n");
    printf("Total Files: %ld\n", totalFiles);
    printf("Total Directories: %ld\n", totalDirs);
    printf("Total Size: %lld bytes\n", totoalSize);

    if(largestSize > 0)
    {
        printf("Largest File: %s (%lld bytes)\n", largestFile, largestSize);
    }

    return 0;
}