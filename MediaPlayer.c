//
// Created by yanni on 03/03/2023.
//
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 500

void findFile() {
    DIR *dir;
    struct dirent *dp;
    char buffer[BUFFER_SIZE];

    if ((dir = opendir(getcwd(buffer,BUFFER_SIZE))) == NULL) {
        perror("Cannot open. ");
    }


    while ((dp = readdir(dir)) != NULL) {

    }
}

int main(int argc, char *argv[]) {
    // printf() displays the string inside quotation
    printf("Hello, World!\n");
//    findFile();
    struct dirent *dp;
    struct stat statbuffer;
    char buffer[BUFFER_SIZE];
    DIR *dir;
    char *cible[500];
    size_t t=sizeof(cible)/sizeof(char );
//    printf("'%s'\n", getcwd(buffer,BUFFER_SIZE));
    if ((dir = opendir(getcwd(buffer,BUFFER_SIZE))) == NULL) {
        perror("Cannot open. ");
    }
    while ((dp = readdir(dir)) != NULL) {
        stat(dp->d_name,&statbuffer);
//        if(statbuffer.st_mode &S_IXUSR){

            if(dp->d_type==DT_REG){
                char *filename=dp->d_name;
                if (stat(filename, &statbuffer) == -1) {
                    perror("stat");
                    exit(EXIT_FAILURE);
                }
//            if(statbuffer.st_mode &S_IFREG){

                int len = strlen(filename);
                if (S_ISREG(statbuffer.st_mode) && (statbuffer.st_mode & S_IXUSR) && strstr(filename, ".") == NULL) {
                    //                cible[t]=dp->d_name;
                    t++;
                    char oldname[255];
                    strcpy(oldname,dp->d_name);
                    char *newName;
//                    newName= strcat(oldname,".old");
//                rename(dp->d_name,newName);
                    printf( "   Fichier cible: '%s'\n",dp->d_name);
                }
                else{
                    printf("Fichier non cible: '%s'\n",dp->d_name);

//                }
            }
        }
    }
    //lecture tableau cible
//    for (int i = 0; i < t; i++) {
//        printf("%s ", cible[i]);
//    }
    return 0;
}

