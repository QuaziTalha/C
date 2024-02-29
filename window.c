#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a top-level window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    // Set the window title
    gtk_window_set_title(GTK_WINDOW(window), "Hello, Window!");

    // Set the window size
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Connect the "destroy" signal to exit the application when the window is closed
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all the widgets inside the window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
