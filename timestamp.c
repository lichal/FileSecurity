#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

char *timeStampToTime(long int timeStamp){
    return "3";
}

void generatePath(const char *path, char *filename, char *filepath)
{
    strcpy(filepath, path);
    strcat(filepath, "/");
    strcat(filepath, filename);
}

void convertTime(time_t time, char *buffer)
{
    time_t t = time;
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(buffer, 100, "%m-%d-%Y:%H:%M:%S", &tm);
}

/* This function recursively visits subdirectories. */
void logfile(const char *name, int indent, FILE *logging)
{
    DIR *dir;
    struct dirent *entry;
    char filestr[1024];
    
    if (!(dir = opendir(name)))
        return;
    
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            /* Ignore the if its . or .. (current and parent directory) */
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            /* Store the directory into log. */
            fprintf(logging, "%*s[%s]\n", indent, "", entry->d_name);
//            if(fgets(filestr, 1024, logging)!=NULL){
//                printf("%s", filestr);
//            }
            logfile(path, indent + 2, logging);
        } else {
            struct stat statBuf;
            
            /* Concatenate the filename with the path to the file. */
            char filepath[100];
            char time[100];
            generatePath(name, entry->d_name, filepath);
            /* Grab file status and print out informations. */
            stat(filepath, &statBuf);
            printf("%*s %s", indent, "", entry->d_name);
            printf(" %lld", statBuf.st_size);
            printf(" %ld", statBuf.st_mtime);
            printf(" %ld\n", statBuf.st_atime);
            
            /* Store filename, byte, date modified, and date accessed. */
            fprintf(logging, "%*s %s", indent, "", entry->d_name);
            fprintf(logging, " %lld", statBuf.st_size); // Size
            
            /* Convert last modified time stamp to string. */
            convertTime(statBuf.st_mtime, time);
            fprintf(logging, " %s", time); // time string in mm-dd-YYYY:HH:MM:SS
            fprintf(logging, " %ld", statBuf.st_mtime); // Last Modified timestamp
            
            /* Convert last accessed time stamp to string. */
            convertTime(statBuf.st_atime, time);
            fprintf(logging, " %s", time); // time string in mm-dd-YYYY:HH:MM:SS
            fprintf(logging, " %ld\n", statBuf.st_atime); // Last accessed timestamp
            
//            char filename[100];
//            if(fgets(filestr, 1024, logging)!=NULL){
//                long int size;
//                long int lastmodified;
//                long int lastaccessed;
//                sscanf(filestr, "%s %ld %ld %ld", filename, &size, &lastmodified, &lastaccessed);
//                printf("name %s size %ld modified %ld accessed %ld\n", filename, size, lastmodified, lastaccessed);
//                if (strcmp(entry->d_name, filename) != 0){
//                    printf("%*s WARNING: File %s might be modified or deleted.\n\n", indent, "", filename);
//                }
//                if(size != statBuf.st_size){
//                    printf("%*s WARNING: Size of %s have changed since last login.\n\n", indent, "",  filename);
//                }
//            }
        }
    }
    closedir(dir);
}

int main(void) {
    /* Open up the log file. */
    FILE *log;
    log = fopen("../log.txt", "w");
//    if (log == NULL){
//        fclose(log);
//        log = fopen("../log.txt", "w");
//        logfile("/Users/d/courses/cis452/finalproject/", 0, log);
//        printf("Log file initialization complete.\n");
//        fclose(log);
//        return 1;
//    }
    logfile("/Users/d/courses/cis452/finalproject", 0, log);
    fclose(log);
    return 1;
}
