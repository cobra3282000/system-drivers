/*
 * GUI Implementation - GTK interface
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/gui.h"
#include "../include/driver.h"
#include "../include/hardware.h"

// Global variables for UI elements
static GtkWidget *driver_list_box = NULL;
static GtkWidget *main_window_ref = NULL;
static GtkWidget *status_bar = NULL;
static DriverInfo *current_drivers = NULL;
static int driver_count = 0;

// Structure to pass driver info to button callbacks
typedef struct {
    int driver_index;
} DriverButtonData;

// Helper function to update status bar
static void update_status(const char *message) {
    if (status_bar != NULL) {
        gtk_label_set_text(GTK_LABEL(status_bar), message);
        // Force GUI to update
        while (gtk_events_pending()) {
            gtk_main_iteration();
        }
    }
}

// Callback for window close
static void on_window_destroy(GtkWidget *widget, gpointer data) {
    (void)widget;  // Unused
    (void)data;    // Unused

    // Cleanup
    if (current_drivers != NULL) {
        free_driver_list(current_drivers, driver_count);
    }
    gtk_main_quit();
}

// Callback for individual driver install button
static void on_driver_install_clicked(GtkButton *button, gpointer user_data) {
    (void)button;  // Unused

    DriverButtonData *data = (DriverButtonData *)user_data;
    int driver_idx = data->driver_index;

    if (driver_idx < 0 || driver_idx >= driver_count) {
        return;
    }

    DriverInfo *driver = &current_drivers[driver_idx];

    // Show confirmation (only for non-installed drivers)
    char confirm_msg[512];
    snprintf(confirm_msg, sizeof(confirm_msg),
            "Install %s?\n\nPackage: %s\n%s",
            driver->name, driver->package, driver->description);

    GtkWidget *confirm_dialog = gtk_message_dialog_new(GTK_WINDOW(main_window_ref),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_QUESTION,
                                                       GTK_BUTTONS_YES_NO,
                                                       "%s", confirm_msg);
    int response = gtk_dialog_run(GTK_DIALOG(confirm_dialog));
    gtk_widget_destroy(confirm_dialog);

    if (response != GTK_RESPONSE_YES) {
        update_status("Installation cancelled.");
        return;
    }

    // Update status bar
    char status_msg[256];
    snprintf(status_msg, sizeof(status_msg), "Installing %s...", driver->name);
    update_status(status_msg);

    // Install the driver
    printf("Installing %s...\n", driver->name);
    bool success = install_driver(driver);

    if (success) {
        snprintf(status_msg, sizeof(status_msg), "Successfully installed %s!", driver->name);
        update_status(status_msg);

        // Check if reboot needed
        if (driver->needs_reboot) {
            show_reboot_dialog(main_window_ref);
        } else {
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(main_window_ref),
                                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                                               GTK_MESSAGE_INFO,
                                                               GTK_BUTTONS_OK,
                                                               "Successfully installed %s!",
                                                               driver->name);
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        }

        // Refresh the list
        refresh_driver_list(driver_list_box);
        update_status("Ready.");
    } else {
        snprintf(status_msg, sizeof(status_msg), "Failed to install %s", driver->name);
        update_status(status_msg);

        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg),
                "Failed to install %s\n\nCheck terminal output for details.",
                driver->name);
        show_error_dialog(main_window_ref, error_msg);
    }
}

// Create the main window
GtkWidget* create_main_window(void) {
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "System Drivers");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    main_window_ref = window;

    // Connect destroy signal
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Create main vertical box
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create header label
    GtkWidget *header = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header),
        "<big><b>System Driver Manager</b></big>\n"
        "Scan and install drivers for your hardware");
    gtk_box_pack_start(GTK_BOX(vbox), header, FALSE, FALSE, 10);

    // Create toolbar with buttons
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);

    GtkWidget *refresh_btn = gtk_button_new_with_label("Refresh Drivers");
    g_signal_connect(refresh_btn, "clicked", G_CALLBACK(on_refresh_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), refresh_btn, FALSE, FALSE, 5);

    // Add spacer
    GtkWidget *spacer = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(toolbar), spacer, TRUE, TRUE, 0);

    // Create scrolled window for driver list
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);

    // Create list box for drivers
    driver_list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled), driver_list_box);

    // Create status bar
    status_bar = gtk_label_new("Ready. Click 'Refresh Drivers' to scan for available drivers.");
    gtk_label_set_xalign(GTK_LABEL(status_bar), 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), status_bar, FALSE, FALSE, 5);

    // Initial scan
    refresh_driver_list(driver_list_box);

    return window;
}

// Refresh the driver list
void refresh_driver_list(GtkWidget *list_box) {
    // Clear existing items
    GList *children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Free old driver data
    if (current_drivers != NULL) {
        free_driver_list(current_drivers, driver_count);
        current_drivers = NULL;
    }

    // Scan hardware
    HardwareInfo *hw_list = NULL;
    int hw_count = scan_hardware(&hw_list);

    if (hw_count <= 0) {
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new("No hardware detected or scan failed.");
        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), row, -1);
        gtk_widget_show_all(list_box);
        return;
    }

    // Detect drivers
    driver_count = detect_drivers(hw_list, hw_count, &current_drivers);
    free_hardware_list(hw_list, hw_count);

    if (driver_count <= 0) {
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new("No additional drivers needed. System is up to date!");
        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), row, -1);
        gtk_widget_show_all(list_box);
        return;
    }

    // Add drivers to list
    for (int i = 0; i < driver_count; i++) {
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_container_add(GTK_CONTAINER(row), hbox);

        // Driver info
        char info_text[512];
        snprintf(info_text, sizeof(info_text),
                "<b>%s</b> (%s)\n<small>%s</small>%s",
                current_drivers[i].name,
                current_drivers[i].package,
                current_drivers[i].description,
                current_drivers[i].is_recommended ? "\n<small><b>Recommended</b></small>" : "");

        GtkWidget *label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label), info_text);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 5);

        // Install/Installed button
        GtkWidget *install_btn;

        if (current_drivers[i].is_installed) {
            // Already installed - show disabled "Installed" button
            install_btn = gtk_button_new_with_label("Installed");
            gtk_widget_set_sensitive(install_btn, FALSE);  // Disable the button
            gtk_widget_set_size_request(install_btn, 100, -1);
        } else {
            // Not installed - show clickable "Install" button
            DriverButtonData *btn_data = malloc(sizeof(DriverButtonData));
            btn_data->driver_index = i;

            install_btn = gtk_button_new_with_label("Install");
            g_signal_connect(install_btn, "clicked", G_CALLBACK(on_driver_install_clicked), btn_data);
            gtk_widget_set_size_request(install_btn, 100, -1);
        }

        gtk_box_pack_start(GTK_BOX(hbox), install_btn, FALSE, FALSE, 5);

        gtk_list_box_insert(GTK_LIST_BOX(list_box), row, -1);
    }

    gtk_widget_show_all(list_box);
}


// Refresh button callback
void on_refresh_clicked(GtkButton *button, gpointer user_data) {
    (void)button;     // Unused
    (void)user_data;  // Unused
    refresh_driver_list(driver_list_box);
}

// Show reboot dialog
void show_reboot_dialog(GtkWidget *parent) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Driver installation requires a system reboot.\n\n"
                                               "Would you like to reboot now?");

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_YES) {
        system("systemctl reboot");
    }
}

// Show error dialog
void show_error_dialog(GtkWidget *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Show progress dialog
void show_progress_dialog(GtkWidget *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_NONE,
                                               "%s", message);
    gtk_widget_show_all(dialog);

    // Process events to show dialog
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
}
