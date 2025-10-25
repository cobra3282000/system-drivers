/*
 * System Drivers - Driver Management for Arch Linux
 * Main entry point with privilege escalation
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include "../include/gui.h"
#include "../include/privilege.h"

int main(int argc, char *argv[]) {
    // Check if running with root privileges
    if (geteuid() != 0) {
        printf("System Drivers requires root privileges.\n");
        printf("Attempting to escalate privileges...\n");

        // Try to re-execute with pkexec (polkit)
        char *pkexec_args[argc + 2];
        pkexec_args[0] = "pkexec";
        pkexec_args[1] = argv[0];

        // Copy remaining arguments
        for (int i = 1; i < argc; i++) {
            pkexec_args[i + 1] = argv[i];
        }
        pkexec_args[argc + 1] = NULL;

        // Execute with pkexec
        execvp("pkexec", pkexec_args);

        // If pkexec fails, try sudo
        fprintf(stderr, "pkexec failed, trying sudo...\n");

        char *sudo_args[argc + 2];
        sudo_args[0] = "sudo";
        sudo_args[1] = argv[0];

        for (int i = 1; i < argc; i++) {
            sudo_args[i + 1] = argv[i];
        }
        sudo_args[argc + 1] = NULL;

        execvp("sudo", sudo_args);

        // If both fail
        fprintf(stderr, "Error: Could not escalate privileges.\n");
        fprintf(stderr, "Please run with: sudo %s\n", argv[0]);
        return 1;
    }

    printf("System Drivers starting with root privileges...\n");

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create and show the main window
    GtkWidget *window = create_main_window();

    if (window == NULL) {
        fprintf(stderr, "Failed to create main window\n");
        return 1;
    }

    gtk_widget_show_all(window);

    // Start GTK main loop
    gtk_main();

    return 0;
}
