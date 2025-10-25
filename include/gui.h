/*
 * GUI Header - GTK interface definitions
 */

#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

// Main window creation
GtkWidget* create_main_window(void);

// Driver list management
void refresh_driver_list(GtkWidget *list_box);
void on_refresh_clicked(GtkButton *button, gpointer user_data);

// Dialog functions
void show_reboot_dialog(GtkWidget *parent);
void show_error_dialog(GtkWidget *parent, const char *message);
void show_progress_dialog(GtkWidget *parent, const char *message);

#endif // GUI_H
