#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void getMode(mode_t mode, char str[]);

int main() {
    struct dirent **namelist;
    struct stat file_stat;
    int n;
    n = scandir(".", &namelist, NULL, alphasort);
    if (n < 0)
        perror("scandir");

    else {
        while(n--) {
            stat(namelist[n]->d_name, &file_stat);

            char mode[11];
            memset(mode, '-', sizeof(char) * 11);
            getMode(file_stat.st_mode, mode);

            struct passwd *pwd;
            pwd = getpwuid(file_stat.st_uid);
            struct group *grp;
            grp = getgrgid(file_stat.st_gid);

            printf("%s %s %s %8ld %.12s %s\n", mode, pwd->pw_name, grp->gr_name, file_stat.st_size, 4 + ctime(&file_stat.st_mtime), namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }
    return 0;
}

void getMode(mode_t mode, char str[]) {
    switch (mode & S_IFMT) {
        case S_IFREG:
            str[0] = '-';
            break;
        case S_IFSOCK:
            str[0] = 's';
            break;
        case S_IFLNK:
            str[0] = 'l';
            break;
        case S_IFBLK:
            str[0] = 'b';
            break;
        case S_IFDIR:
            str[0] = 'd';
            break;
        case S_IFCHR:
            str[0] = 'c';
            break;
        case S_IFIFO:
            str[0] = 'p';
    }
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
    str[10] = '\0';
}
