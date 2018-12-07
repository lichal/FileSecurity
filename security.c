#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void createlog(const char *name, int indent, FILE *logging)
{
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(name)))
        return;
    
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            fprintf(logging, "%*s[%s]\n", indent, "", entry->d_name);
            createlog(path, indent + 2, logging);
        } else {
            struct stat statBuf;
            stat(entry->d_name, &statBuf);
            printf("%*s- %s", indent, "", entry->d_name);
            printf(" %lld %hu\n", statBuf.st_size, statBuf.st_mode);
            /* filename byte datemodified dateaccessed */
            fprintf(logging, "%*s- %s", indent, "", entry->d_name);
            fprintf(logging, " %lld", statBuf.st_size); // Size
            fprintf(logging, " %ld", statBuf.st_mtime); // Last Modified
            fprintf(logging, " %ld\n", statBuf.st_atime); // Last accessed
        }
    }
    closedir(dir);
}

//void logging(const char *name, int indent)
//{
//    DIR *dir;
//    struct dirent *entry;
//
//    if(!(dir = opendir(name)))
//        return;
//
//    while((entry = readdir(dir))){
//        if(entry->d_type == DT_DIR){
//
//        }
//    }
//}

int main(void) {
    /* Open up the log file. */
    FILE *prevLog;
    prevLog = fopen("../log.txt", "w");
//    if (prevLog == NULL){
//        fclose(prevLog);
//        prevLog = fopen("log.txt", "w");
//        listdir(".", 0, prevLog);
//        printf("Log file initialization complete.\n");
//        fclose(prevLog);
//        return 1;
//    }
    createlog("/Users/d/courses/cis452/finalproject/", 0, prevLog);
    fclose(prevLog);
    return 0;
}
