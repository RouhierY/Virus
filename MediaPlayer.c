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

void duplicate(char oldName[],char newName[]){
    FILE *src, *dest;
    char ch;
    src = fopen("PG1.txt", "r");
    oldName=strcat(oldName,".c");
    printf("    %s\n", oldName);
    dest = fopen(oldName, "w");
    if (dest == NULL) {
        printf("    Erreur lors de la création du fichier de destination.\n");
    }
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }
//    printf("    Le fichier a été copié avec succès.\n");
    fclose(src);
    fclose(dest);
}

void findFile() {
    struct dirent *dp;
    struct stat statbuffer;
    char buffer[BUFFER_SIZE],*newName[100],*oldName[100];
    DIR *dir;
//    char *cible[500];
    size_t t=0;
    if ((dir = opendir(getcwd(buffer,BUFFER_SIZE))) == NULL) {
        perror("Cannot open. ");
    }
    while ((dp = readdir(dir)) != NULL) {
        stat(dp->d_name,&statbuffer);
        if(dp->d_type==DT_REG){
            char *oldName=dp->d_name, *oldOldName[100];
            if (stat(oldName, &statbuffer) == -1) {
                perror("stat error");
                exit(EXIT_FAILURE);
            }
            if (S_ISREG(statbuffer.st_mode) && (statbuffer.st_mode & S_IXUSR) && strrchr(oldName, '.') == NULL) {
                //                cible[t]=dp->d_name;
                t++;
                strcpy(oldName,dp->d_name);
                strcpy(oldOldName,oldName);
                strcat(newName,".old");
//                rename(dp->d_name,newName);
                printf( "   Target File: '%s'\n",dp->d_name);
//                duplicate(oldOldName,newName);

            }
            else{
                printf("Non Target File: '%s'\n",dp->d_name);

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

