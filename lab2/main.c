#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

static int err_code;

/*
 * here are some function signatures and macros that may be helpful.
 */

void handle_error(char* fullname, char* action);
bool test_file(char* pathandname);
bool is_dir(char* pathandname);
const char* ftype_to_str(mode_t mode);
void list_file(char* pathandname, char* name, bool list_long);
void list_dir(char* dirname, bool list_long, bool list_all, bool recursive);

/*
 * You can use the NOT_YET_IMPLEMENTED macro to error out when you reach parts
 * of the code you have not yet finished implementing.
 */
#define NOT_YET_IMPLEMENTED(msg)                  \
    do {                                          \
        printf("Not yet implemented: " msg "\n"); \
        exit(255);                                \
    } while (0)

/*
 * PRINT_ERROR: This can be used to print the cause of an error returned by a
 * system call. It can help with debugging and reporting error causes to
 * the user. Example usage:
 *     if ( error_condition ) {
 *        PRINT_ERROR();
 *     }
 */
#define PRINT_ERROR(progname, what_happened, pathandname)               \
    do {                                                                \
        printf("%s: %s %s: %s\n", progname, what_happened, pathandname, \
               strerror(errno));                                        \
    } while (0)

/* PRINT_PERM_CHAR:
 *
 * This will be useful for -l permission printing.  It prints the given
 * 'ch' if the permission exists, or "-" otherwise.
 * Example usage:
 *     PRINT_PERM_CHAR(sb.st_mode, S_IRUSR, "r");
 */
#define PRINT_PERM_CHAR(mode, mask, ch) printf("%s", (mode & mask) ? ch : "-");

/*
 * Get username for uid. Return 1 on failure, 0 otherwise.
 */
static int uname_for_uid(uid_t uid, char* buf, size_t buflen) {
    struct passwd* p = getpwuid(uid);
    if (p == NULL) {
        return 1;
    }
    strncpy(buf, p->pw_name, buflen);
    return 0;
}

/*
 * Get group name for gid. Return 1 on failure, 0 otherwise.
 */
static int group_for_gid(gid_t gid, char* buf, size_t buflen) {
    struct group* g = getgrgid(gid);
    if (g == NULL) {
        return 1;
    }
    strncpy(buf, g->gr_name, buflen);
    return 0;
}

/*
 * Format the supplied `struct timespec` in `ts` (e.g., from `stat.st_mtime`) as a
 * string in `char *out`. Returns the length of the formatted string (see, `man
 * 3 strftime`).
 */
static size_t date_string(struct timespec* ts, char* out, size_t len) {
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    struct tm* t = localtime(&ts->tv_sec);
    if (now.tv_sec < ts->tv_sec) {
        // Future time, treat with care.
        return strftime(out, len, "%b %e %Y", t);
    } else {
        time_t difference = now.tv_sec - ts->tv_sec;
        if (difference < 31556952ull) {
            return strftime(out, len, "%b %e %H:%M", t);
        } else {
            return strftime(out, len, "%b %e %Y", t);
        }
    }
}

/*
 * Print help message and exit.
 */
static void help() {
    /* TODO: add to this */
    printf("ls: List files\n");
    printf("\t--help: Print this help\n");
    printf("\t-a: List all files, including hidden ones\n");
    printf("\t-l: Use long listing format\n");
    printf("\t-R: Recursively list subdirectories\n");
    printf("\t-n: Display the number of files instead of listing them\n");
    exit(0);
}

/*
 * call this when there's been an error.
 * The function should:
 * - print a suitable error message (this is already implemented)
 * - set appropriate bits in err_code
 */
void handle_error(char* what_happened, char* fullname) {
    PRINT_ERROR("ls", what_happened, fullname);

    // TODO: your code here: inspect errno and set err_code accordingly.
    PRINT_ERROR("ls", what_happened, fullname);

    if (errno == ENOENT) {
        err_code |= 0x08;
    } else if (errno == EACCES) {
        err_code |= 0x10;
    } else {
        err_code |= 0x20;
    }
    err_code |= 0x40;
}

/*
 * test_file():
 * test whether stat() returns successfully and if not, handle error.
 * Use this to test for whether a file or dir exists
 */
bool test_file(char* pathandname) {
    struct stat sb;
    if (stat(pathandname, &sb)) {
        handle_error("cannot access", pathandname);
        return false;
    }
    return true;
}

/*
 * is_dir(): tests whether the argument refers to a directory.
 * precondition: test_file() returns true. that is, call this function
 * only if test_file(pathandname) returned true.
 */
bool is_dir(char* pathandname) {
    /* TODO: fillin */
    struct stat sb;
    
    if (stat(pathandname, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return true;
    }
    
    return false;
}

/* convert the mode field in a struct stat to a file type, for -l printing */
const char* ftype_to_str(mode_t mode) {
    /* TODO: fillin */
    
    if (S_ISREG(mode))
        return "-";
    if (S_ISDIR(mode))
        return "d";
    
    return "?";
}

/* list_file():
 * implement the logic for listing a single file.
 * This function takes:
 *   - pathandname: the directory name plus the file name.
 *   - name: just the name "component".
 *   - list_long: a flag indicated whether the printout should be in
 *   long mode.
 *
 *   The reason for this signature is convenience: some of the file-outputting
 *   logic requires the full pathandname (specifically, testing for a directory
 *   so you can print a '/' and outputting in long mode), and some of it
 *   requires only the 'name' part. So we pass in both. An alternative
 *   implementation would pass in pathandname and parse out 'name'.
 */
void list_file(char* pathandname, char* name, bool list_long) {
    /* TODO: fill in*/
    struct stat sb;
    
 
    if (stat(pathandname, &sb) == -1) {
        handle_error("cannot stat", pathandname);
        return;
    }
    
    if(list_long){
        printf("%s", ftype_to_str(sb.st_mode));
        
        PRINT_PERM_CHAR(sb.st_mode, S_IRUSR, "r");
        PRINT_PERM_CHAR(sb.st_mode, S_IWUSR, "w");
        PRINT_PERM_CHAR(sb.st_mode, S_IXUSR, "x");
        PRINT_PERM_CHAR(sb.st_mode, S_IRGRP, "r");
        PRINT_PERM_CHAR(sb.st_mode, S_IWGRP, "w");
        PRINT_PERM_CHAR(sb.st_mode, S_IXGRP, "x");
        PRINT_PERM_CHAR(sb.st_mode, S_IROTH, "r");
        PRINT_PERM_CHAR(sb.st_mode, S_IWOTH, "w");
        PRINT_PERM_CHAR(sb.st_mode, S_IXOTH, "x");
        
        printf(" %ld", (long)sb.st_nlink);
        
        char uname[256], gname[256];
        
        if (uname_for_uid(sb.st_uid, uname, sizeof(uname)) == 0) {
            printf(" %s", uname);
        } 
        else {
            printf(" %d", sb.st_uid);
            err_code |= 0x20;
        }
        if (group_for_gid(sb.st_gid, gname, sizeof(gname)) == 0) {
            printf(" %s", gname);
        } 
        else {
            printf(" %d", sb.st_gid);
            err_code |= 0x20;
        }
        
        printf(" %ld", sb.st_size);

                
        char date[64];
        date_string(&sb.st_mtim, date, sizeof(date));
        printf(" %s", date);
    }

    if (S_ISDIR(sb.st_mode) && strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
        printf(" %s/", name);
    } 
    else {
        printf(" %s", name);
    }
    printf("\n");
    
}

/* list_dir():
 * implement the logic for listing a directory.
 * This function takes:
 *    - dirname: the name of the directory
 *    - list_long: should the directory be listed in long mode?
 *    - list_all: are we in "-a" mode?
 *    - recursive: are we supposed to list sub-directories?
 */
void list_dir(char* dirname, bool list_long, bool list_all, bool recursive) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(dirname)) == NULL) {
        handle_error("cannot open directory", dirname);
        return;
    }

    if (recursive) {
        printf("%s:\n", dirname);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!list_all && entry->d_name[0] == '.') {
            continue;
        }

        char pathandname[1024];
        snprintf(pathandname, sizeof(pathandname), "%s/%s", dirname, entry->d_name);
        list_file(pathandname, entry->d_name, list_long);
    }

    if (recursive) {
        rewinddir(dir);
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char pathandname[1024];
                snprintf(pathandname, sizeof(pathandname), "%s/%s", dirname, entry->d_name);
                printf("\n");
                list_dir(pathandname, list_long, list_all, recursive);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    // This needs to be int since C does not specify whether char is signed or
    // unsigned.
    int opt;
    err_code = 0;
    bool list_long = false, list_all = false, recursive = false;
    // We make use of getopt_long for argument parsing, and this
    // (single-element) array is used as input to that function. The `struct
    // option` helps us parse arguments of the form `--FOO`. Refer to `man 3
    // getopt_long` for more information.
    struct option opts[] = {
        {.name = "help", .has_arg = 0, .flag = NULL, .val = '\a'}};

    // This loop is used for argument parsing. Refer to `man 3 getopt_long` to
    // better understand what is going on here.
    while ((opt = getopt_long(argc, argv, "1alR", opts, NULL)) != -1) {
        switch (opt) {
            case '\a':
                // Handle the case that the user passed in `--help`. (In the
                // long argument array above, we used '\a' to indicate this
                // case.)
                help();
                break;
            case '1':
                // Safe to ignore since this is default behavior for our version
                // of ls.
                break;
            case 'a':
                list_all = true;
                break;
                // TODO: you will need to add items here to handle the
                // cases that the user enters "-l" or "-R"
            case 'l':
                list_long = true;
                break;
            case 'R':
                recursive = true;
                break;
            default:
                printf("Unimplemented flag %d\n", opt);
                break;
        }
    }

    // TODO: Replace this.
    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            if (is_dir(argv[i])) {
                if (i > optind) {
                    printf("\n%s:\n", argv[i]);
                }
                list_dir(argv[i], list_long, list_all, recursive);
            }
            else if (test_file(argv[i])) {
                list_file(argv[i], argv[i], list_long);
            }
        }
    }
    else {
        list_dir(".", list_long, list_all, recursive);
    }

    exit(err_code);
}
