# include <dirent.h> 
# include <stdio.h>
# include <sys/stat.h>
# include <time.h>
# include <sys/ioctl.h>
# include <stdbool.h>

# include <string.h>
# include <stdlib.h>

// Config
const char* DEFAULT_TIME_FORMAT = "%H:%M:%S %d/%m/%Y";

const bool HAS_DATE = false;
const bool HAS_PERMISSIONS = true;
const bool HAS_PERMS_SPACES = true;
const bool HAS_FILE_SIZE = true;
const bool HAS_NAME = true;

// From https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c (answer from Jonathon Leffler)
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


// Based on https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c (answer from Jonathon Leffler)
static char* lsperms (int mode) {
    static const char *rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};       
   
    const bool has_perms_spaces = HAS_PERMS_SPACES;
 
    int other_pos = 3;
    int bits_size = 12;
    if (has_perms_spaces)  {
        bits_size += 3;
        other_pos++;
    }
    
    /* static */ char bits[bits_size];
    bits[0] = filetypeletter(mode);
    int offset = 0;
    
    if (has_perms_spaces) {
        bits[1] = ' ';
        offset++;
    }
    strcpy(&bits[1 + offset], rwx[(mode >> 6)& 7]);
    
    if (has_perms_spaces) {
        bits[5] = ' ';
        offset++;
    }
    strcpy(&bits[4 + offset], rwx[(mode >> 3)& 7]);
    
    if (has_perms_spaces) {
        bits[9] = ' ';
        offset++;
    }
    strcpy(&bits[7 + offset], rwx[(mode & 7)]);

    if (mode & S_ISUID)
        bits[other_pos] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        bits[other_pos * 2] = (mode & S_IXGRP) ? 's' : 'l';
    if (mode & S_ISVTX)
        bits[other_pos * 3] = (mode & S_IXOTH) ? 't' : 'T';

    bits_size -= 2;
    bits[bits_size] = ' ';
    
    bits_size++;
    bits[bits_size] = '\0';

    printf(bits);
    return bits;
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

char* readable_fs(double size/*in bytes*/, char *buf) {
    int i = 0;
    const char* units[] = {" B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }

    sprintf(buf, "%.*f%s", i, size, units[i]);
    printf("%8s ", buf);
    return buf;
}

void all_files(DIR* dir, int columns, int max_filename_size) {
    // print all the files and directories within directory
    struct dirent *ent;
    struct stat buf;   

	int string_item_size = max_filename_size; 
	if (HAS_DATE) {
		string_item_size += 20;
	}

    if (HAS_PERMISSIONS) {
        string_item_size += 11;
        if (HAS_PERMS_SPACES) {
            string_item_size += 3;
        }
    }
    
    if (HAS_FILE_SIZE) {
         string_item_size += 8;   
    }
	
    printf("string item size: %d\n", string_item_size);
    int sum = string_item_size;

    ent = readdir(dir);
    int item_count;
    for (item_count = 0; ent != NULL; item_count++) {
        char* filename = ent -> d_name;

        ent = readdir(dir);
        stat(filename, &buf); 
        
        if(HAS_FILE_SIZE){
            char file_size[6];
            sprintf(file_size, "%5.ld", buf.st_size);

            char bufls[8];
            readable_fs(buf.st_size, file_size);
        }

        if (HAS_PERMISSIONS) {
            lsperms(buf.st_mode);
        }

        if (HAS_DATE) {
            char time[20]; 
            strftime(time, 20, DEFAULT_TIME_FORMAT, localtime(&buf.st_mtime)); // 2020-10-06 11:54:11
            printf(time);
        }

        // last access, size, perm, filename
        
        //colors
        // https://www.theurbanpenguin.com/4184-2/
       
        // printf("%6s %s %-*s", file_size, nice_perms, max_filename_size, filename); 
        printf("%-*s", max_filename_size, filename); 

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

int main(int argc, char *argv[]) {
    printf("Hello\n");

    char* str_dir = ".";
    if (argc > 1) {
        str_dir = argv[1];
    }

    DIR* dir = opendir(str_dir);// only filename works correctly if the filepath is not .

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

        dir = opendir(str_dir);

        all_files(dir, columns, max_filename_size);
    }

    printf("\n");
    return(0);
}
