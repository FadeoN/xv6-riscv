#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// Write a simple version of the UNIX find program: find all the files in a directory tree with a specific name. Your solution should be in the file user/find.c.

void find(char *path, char *aExpectedFilename)
{

    struct stat st;
    int fd;

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open '%s'\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        break;
    case T_DIR:

        char buf[512], *p;
        struct dirent de;

        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long");
            break;
        }

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum <= 1)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            
            if(st.type == T_DIR && strcmp(p, ".") != 0){
                find(buf, aExpectedFilename);
            }
            else if(strcmp(p, aExpectedFilename) == 0){
                printf("%s\n", buf);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "find [path] [pattern]");
        return 1;
    }

    char *path = argv[1];
    char *aExpectedFilename = argv[2];

    find(path, aExpectedFilename);

    return 0;
}
