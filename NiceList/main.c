# include <dirent.h> 
# include <stdio.h>
# include <sys/stat.h>
# include <time.h>
# include <sys/ioctl.h>

# include <string.h>
# include <stdlib.h>

const char* DEFAULT_TIME_FORMAT = "%H:%M:%S %d/%m/%Y";

static int filetypeletter(int mode) {
    char c;

    if (S_ISREG(mode))
        c = '-';
    else if (S_ISDIR(mode))
        c = 'd';
    else if (S_ISBLK(mode))
        c = 'b';
    else if (S_ISCHR(mode))
        c = 'c';

#ifdef S_ISFIFO
    else if (S_ISFIFO(mode))
        c = 'p';
#endif  /* S_ISFIFO */
#ifdef S_ISLNK
    else if (S_ISLNK(mode))
        c = 'l';
#endif  /* S_ISLNK */
#ifdef S_ISSOCK
    else if (S_ISSOCK(mode))
        c = 's';
#endif  /* S_ISSOCK */
#ifdef S_ISDOOR
    /* Solaris 2.6, etc. */
    else if (S_ISDOOR(mode))
        c = 'D';
#endif  /* S_ISDOOR */
    else
    {
        /* Unknown type -- possibly a regular file? */
        c = '?';
    }
    return(c);
}

/* Convert a mode field into "ls -l" type perms field. */
static char* lsperms(int mode) {
    static const char *rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    static char bits[11];

    bits[0] = filetypeletter(mode);

    strcpy(&bits[1], rwx[(mode >> 6)& 7]);
    strcpy(&bits[4], rwx[(mode >> 3)& 7]);
    strcpy(&bits[7], rwx[(mode & 7)]);

    if (mode & S_ISUID)
        bits[3] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        bits[6] = (mode & S_IXGRP) ? 's' : 'l';
    if (mode & S_ISVTX)
        bits[9] = (mode & S_IXOTH) ? 't' : 'T';
    bits[10] = '\0';

    return(bits);
} 


int find_max_filename_size(DIR* dir) {
    struct dirent* ent;
    ent = readdir(dir);
    
    int max_filename_size = 0;
    int current_filename_size = 0;

    while (ent != NULL) {
        char* filename = ent -> d_name;
        
        ent = readdir(dir);

        current_filename_size = strlen(filename);
        if (current_filename_size > max_filename_size) {
            max_filename_size = current_filename_size;
        }
    }
    closedir(dir);

    return max_filename_size;
}

void all_files(DIR* dir, int columns, int max_filename_size) {
    // print all the files and directories within directory
    struct dirent *ent;
    struct stat buf;   

    int string_item_size = 36 + max_filename_size;

    int sum = string_item_size;

    

    ent = readdir(dir);
    int item_count;
    for (item_count = 0; ent != NULL; item_count++) {
        char* filename = ent -> d_name;

        ent = readdir(dir);
        stat(filename, &buf);
        int mode = buf.st_mode;
        char* perm = lsperms(mode); // 11

        char file_size[6];
        sprintf(file_size, "%5.ld", buf.st_size);

        //char* time = ctime(&buf.st_mtime);

        char time[20]; 
        strftime(time, 20, DEFAULT_TIME_FORMAT, localtime(&buf.st_mtime)); // 2020-10-06 11:54:11

        // last access, size, perm, filename
        printf("%s %s %s %-17s", time, file_size, perm, filename); 

        sum += string_item_size + 3;
        if (sum > columns) {
            printf("\n");
            sum = string_item_size;
        } else {
            printf(" # ");
            sum += 3;
            
        }
        
    }

    printf("%d items\n\n", item_count);

    //print_result(result, item_count, max_column_size, columns);
    closedir(dir);
}

int main(void) {
    printf("\n");
    DIR* dir = opendir(".");

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    int columns = w.ws_col;
    printf("columns %d\n", columns);

    if (dir == NULL) {
        // could not open directory
        perror("Could not open directory");
        return 1;

    } else {

        int max_filename_size = find_max_filename_size(dir);

        printf("max filename size %d\n\n", max_filename_size);

        dir = opendir(".");

        all_files(dir, columns, max_filename_size);
    }

    

    printf("\n");
    return(0);
}