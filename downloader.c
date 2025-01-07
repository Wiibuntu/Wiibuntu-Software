#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BOXES 10

GtkWidget *boxes[NUM_BOXES];
const char *download_links[NUM_BOXES];

const char *x86_links[NUM_BOXES] = {
    "https://github.com/Wiibuntu/Wiibuntu-Packages/blob/main/packages/swap-editor-x86.deb",
    "https://github.com/Wiibuntu/Wiibuntu-Packages/blob/main/packages/Breakout-x86.deb",
    "https://example.com/package3.deb",
    "https://example.com/package4.deb",
    "https://example.com/package5.deb",
    "https://example.com/package6.deb",
    "https://example.com/package7.deb",
    "https://example.com/package8.deb",
    "https://example.com/package9.deb",
    "https://example.com/package10.deb"
};

const char *powerpc_links[NUM_BOXES] = {
    "https://github.com/Wiibuntu/Wiibuntu-Packages/blob/main/packages/swap-editor-ppc.deb",
    "https://github.com/Wiibuntu/Wiibuntu-Packages/blob/main/packages/Breakout-ppc.deb",
    "https://powerpc.example.com/package3.deb",
    "https://powerpc.example.com/package4.deb",
    "https://powerpc.example.com/package5.deb",
    "https://powerpc.example.com/package6.deb",
    "https://powerpc.example.com/package7.deb",
    "https://powerpc.example.com/package8.deb",
    "https://powerpc.example.com/package9.deb",
    "https://powerpc.example.com/package10.deb"
};

const char *box_labels[NUM_BOXES] = {
    "Swap Editor",
    "Breakout-Clone",
    "Download Package 3",
    "Download Package 4",
    "Download Package 5",
    "Download Package 6",
    "Download Package 7",
    "Download Package 8",
    "Download Package 9",
    "Download Package 10"
};

void detect_architecture() {
    FILE *fp = popen("uname -m", "r");
    if (fp == NULL) {
        g_print("Failed to detect architecture. Defaulting to x86.\n");
        memcpy(download_links, x86_links, sizeof(x86_links));
        return;
    }

    char buffer[128];
    fgets(buffer, sizeof(buffer), fp);
    pclose(fp);

    if (strstr(buffer, "powerpc") != NULL) {
        memcpy(download_links, powerpc_links, sizeof(powerpc_links));
        g_print("PowerPC architecture detected. Using PowerPC links.\n");
    } else {
        memcpy(download_links, x86_links, sizeof(x86_links));
        g_print("x86 architecture detected. Using x86 links.\n");
    }
}

void download_and_install(const char *link) {
    if (link) {
        char command[512];
        snprintf(command, sizeof(command), "wget %s -O /tmp/package.deb && sudo dpkg -i /tmp/package.deb", link);
        system(command);
    } else {
        g_print("No link specified for this box.\n");
    }
}

void on_box_clicked(GtkWidget *widget, gpointer data) {
    int box_index = GPOINTER_TO_INT(data);
    download_and_install(download_links[box_index]);
}

void on_update_all_clicked(GtkWidget *widget, gpointer data) {
    for (int i = 0; i < NUM_BOXES; i++) {
        download_and_install(download_links[i]);
    }
}

GtkWidget* create_clickable_box(int index) {
    GtkWidget *box = gtk_button_new_with_label(box_labels[index]);
    g_signal_connect(box, "clicked", G_CALLBACK(on_box_clicked), GINT_TO_POINTER(index));
    return box;
}

void run_cli_mode() {
    printf("Running in CLI mode. Downloading all packages...\n");
    for (int i = 0; i < NUM_BOXES; i++) {
        download_and_install(download_links[i]);
    }
    printf("All downloads and installations completed.\n");
}

int main(int argc, char *argv[]) {
    detect_architecture();

    if (argc > 1 && strcmp(argv[1], "-cli") == 0) {
        run_cli_mode();
        return 0;
    }

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Package Downloader");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 500);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    for (int i = 0; i < NUM_BOXES; i++) {
        boxes[i] = create_clickable_box(i);
        gtk_grid_attach(GTK_GRID(grid), boxes[i], i % 3, i / 3, 1, 1);
    }

    GtkWidget *update_all_button = gtk_button_new_with_label("Update All");
    g_signal_connect(update_all_button, "clicked", G_CALLBACK(on_update_all_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), update_all_button, 0, 4, 3, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

