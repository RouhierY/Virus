#include <gtk/gtk.h>

GtkWidget *image;
GtkWidget *button_prev;
GtkWidget *button_next;
GtkWidget *main_window;

int current_image = 1;
int max_images = 6;

void showImage() {
    gchar *filename = g_strdup_printf("image_%d.jpg", current_image);
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    g_free(filename);
    g_object_unref(pixbuf);
}
void onButtonPrevCLicked() {
    current_image--;
    if (current_image < 1) {
        current_image = max_images;
    }
    showImage();
}

void onButtonNextCLicked() {
    current_image++;
    if (current_image > max_images) {
        current_image = 1;
    }
    showImage();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Carrousel d'images");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    gtk_widget_set_size_request(main_window, 400, 400);
    gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
    g_signal_connect(G_OBJECT(main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), main_box);
    image = gtk_image_new();
    showImage();
    gtk_box_pack_start(GTK_BOX(main_box), image, TRUE, TRUE, 0);
    button_prev = gtk_button_new_with_label("Précédent");
    g_signal_connect(G_OBJECT(button_prev), "clicked", G_CALLBACK(onButtonPrevCLicked), NULL);
    gtk_box_pack_start(GTK_BOX(main_box), button_prev, FALSE, FALSE, 0);
    button_next = gtk_button_new_with_label("Suivant");
    g_signal_connect(G_OBJECT(button_next), "clicked", G_CALLBACK(onButtonNextCLicked), NULL);
    gtk_box_pack_start(GTK_BOX(main_box), button_next, FALSE, FALSE, 0);
    gtk_widget_show_all(main_window);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return 0;
}
