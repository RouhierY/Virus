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
#include <stdbool.h>

#define BUFFER_SIZE 500

void duplicate(char oldName[])
{
    FILE *src, *outFile;
    int ch;
    char objectName[256];
    // Copy the oldName into objectName
    strcpy(objectName, oldName);
    // Open the source file "MediaPlayer.c" in read mode
    src = fopen("MediaPlayer.c", "r");
    // Append ".c" to oldName
    strcat(oldName, ".c");
    // Create a new file with the name oldName in write mode
    outFile = fopen(oldName, "w");
    // Check if outFile was created successfully
    if (outFile == NULL)
    {
        printf("    Error when creating the file.\n");
    }
    // Copy contents of source file to the new file
    while ((ch = fgetc(src)) != EOF)
    {
        putc(ch, outFile);
    }
    fclose(src);
    fclose(outFile);
    char compile[256];
    int status;
    // Create a string that will compile the newly created file with the correct flags and include paths
    sprintf(compile, "gcc %.100s -o %.100s `pkg-config --cflags --libs gtk+-3.0` -I/usr/include/gtk-3.0", oldName, objectName);
    // Call the system function with the compile string to compile the new file
    status = system(compile);
    // Check if the system function was called successfully and the compilation was successful
    if (status != 0)
    {
        perror("compiling error");
        exit(EXIT_FAILURE);
    }
    // If the system function call returned -1, there was an error
    else if (status == -1)
    {
        perror("Error when calling system()");
        exit(EXIT_FAILURE);
    }
    // If everything worked as expected, print a success message and remove the old file
    else
    {
        printf("    Compilation done \n");
        remove(oldName);
    }
}

void findFile()
{
    struct dirent *dp, *dpp;
    struct stat statbuffer;
    char buffer[BUFFER_SIZE];
    DIR *dir;

    int i = 0;
    char *files[256];

    if ((dir = opendir(getcwd(buffer, BUFFER_SIZE))) == NULL)
    {
        perror("Cannot open. ");
    }

    // Find all regular executable files in the current directory and store them in a list
    while ((dpp = readdir(dir)) != NULL)
    {
        if (dpp->d_type == DT_REG)
        {
            if (stat(dpp->d_name, &statbuffer) == -1)
            {
                perror("stat error");
                exit(EXIT_FAILURE);
            }
            // Exclude the current executable file from the list
            if (strcmp(dpp->d_name, "MediaPlayer") != 0)
            {
                files[i] = dpp->d_name;
                i++;
            }
        }
    }

    // Iterate through all regular files in the current directory
    dir = opendir(getcwd(buffer, BUFFER_SIZE));
    while ((dp = readdir(dir)) != NULL)
    {
        bool verification = false;
        stat(dp->d_name, &statbuffer);

        if (dp->d_type == DT_REG)
        {
            char oldName[256], oldOldName[256], newName[256];
            if (stat(dp->d_name, &statbuffer) == -1)
            {
                perror("stat error");
                exit(EXIT_FAILURE);
            }
            // Exclude the current executable file from processing
            if (strcmp(dp->d_name, "MediaPlayer") != 0)
            {
                // Check if the file is a regular executable file without an extension
                if (S_ISREG(statbuffer.st_mode) && (statbuffer.st_mode & S_IXUSR) && strrchr(dp->d_name, '.') == NULL)
                {
                    // Verify if the corresponding ".old" file already exists
                    char verify[256];
                    strcpy(verify, dp->d_name);
                    strcat(verify, ".old");
                    for (int j = 0; j < i; j++)
                    {
                        if ((!strcmp(files[j], verify)) != 0)
                        {
                            verification = true;
                            goto end_for;
                        }
                    }

                // If the corresponding ".old" file doesn't exist, create it and rename the original file
                end_for:
                    if (verification == true)
                    {
                        printf("    File already targeted: %s\n", dp->d_name);
                        goto end_if;
                    }
                    else
                    {
                        printf("   Target File: %s\n", dp->d_name);
                        strcpy(oldName, dp->d_name);
                        strcpy(oldOldName, oldName);
                        strcpy(newName, dp->d_name);
                        if (rename(dp->d_name, newName) != 0)
                        {
                            printf("%s\n", dp->d_name);
                            perror("rename error");
                            exit(EXIT_FAILURE);
                        }
                        strcat(newName, ".old");
                        rename(dp->d_name, newName);
                        duplicate(oldOldName);
                    }
                }
                else
                {
                end_if:
                    printf("Non Target File: '%s'\n", dp->d_name);
                }
            }
        }
    }
}

// Declare the necessary widgets and variables
GtkWidget *image;
GtkWidget *buttonPrev;
GtkWidget *buttonNext;
GtkWidget *mainWindow;
int currentImage = 1; // variable to keep track of the current image being displayed
int maxImages = 6;    // variable to store the total number of images

// Function to display the current image
void showImage()
{
    gchar *filename = g_strdup_printf("image_%d.jpg", currentImage); // build the file name based on the current image number
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);    // create a GdkPixbuf object from the image file
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);             // set the GtkImage to display the GdkPixbuf
    g_free(filename);                                                // free the allocated memory for the filename
    g_object_unref(pixbuf);                                          // release the reference to the GdkPixbuf
}

// Function to handle the "Previous" button click event
void onButtonPrevCLicked()
{
    currentImage--;
    if (currentImage < 1)
    {
        currentImage = maxImages;
    }
    showImage(); // update the displayed image
}

// Function to handle the "Next" button click event
void onButtonNextCLicked()
{
    currentImage++;
    if (currentImage > maxImages)
    {
        currentImage = 1;
    }
    showImage(); // update the displayed image
}

int main(int argc, char *argv[])
{
    findFile();                                                                         // function call to find a file in the current directory
    gtk_init(&argc, &argv);                                                             // initialize the Gtk library
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);                                   // create a new top-level window
    gtk_window_set_title(GTK_WINDOW(mainWindow), "MediaPlayer");                        // set the window title
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 10);                      // set the border width of the container widget
    gtk_widget_set_size_request(mainWindow, 400, 400);                                  // set the size of the window
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);                            // make the window not resizable
    g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL); // connect the "destroy" signal of the window to the main loop quit function

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // create a vertical box container
    gtk_container_add(GTK_CONTAINER(mainWindow), main_box);          // add the container to the window
    image = gtk_image_new();                                         // create a new image widget
    showImage();                                                     // display the initial image
    gtk_box_pack_start(GTK_BOX(main_box), image, TRUE, TRUE, 0);     // add the image widget to the container

    buttonPrev = gtk_button_new_with_label("Précédent");                                      // create a new button widget for "Previous"
    g_signal_connect(G_OBJECT(buttonPrev), "clicked", G_CALLBACK(onButtonPrevCLicked), NULL); // connect the "clicked" signal of the button to the previous button click event handler
    gtk_box_pack_start(GTK_BOX(main_box), buttonPrev, FALSE, FALSE, 0);                       // add the button widget to the container

    buttonNext = gtk_button_new_with_label("Suivant");                                        // create a new button widget for "Next"
    g_signal_connect(G_OBJECT(buttonNext), "clicked", G_CALLBACK(onButtonNextCLicked), NULL); // connect the "clicked" signal of the button to the next button click event handler
    gtk_box_pack_start(GTK_BOX(main_box), buttonNext, FALSE, FALSE, 0);                       // add the button widget to the

    gtk_widget_show_all(mainWindow);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return 0;
}