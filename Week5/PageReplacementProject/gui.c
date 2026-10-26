
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "page_replacement.h"

static GtkWidget *pages_entry;
static GtkWidget *frames_entry;
static GtkWidget *algorithm_combo;
static GtkWidget *results_view;

static void show_message(const char *message)
{
    GtkTextBuffer *buffer;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(results_view));
    gtk_text_buffer_set_text(buffer, message, -1);
}

static int parse_pages(const char *input, int pages[], int *count)
{
    char *copy;
    char *token;
    char *end;
    long value;

    *count = 0;

    if (input == NULL || *input == '\0')
        return 0;

    copy = g_strdup(input);
    token = strtok(copy, " \t\n,");

    while (token != NULL)
    {
        char *p = token;

        while (*p != '\0')
        {
            if (!isdigit((unsigned char)*p) && *p != '+' && *p != '-')
            {
                g_free(copy);
                return 0;
            }
            p++;
        }

        value = strtol(token, &end, 10);

        if (*end != '\0' || value < 0 || value > 2147483647L ||
            *count >= MAX_PAGES)
        {
            g_free(copy);
            return 0;
        }

        pages[*count] = (int)value;
        (*count)++;

        token = strtok(NULL, " \t\n,");
    }

    g_free(copy);
    return *count > 0;
}

static void append_result(GString *output, const char *name,
                          int pages[], PageResult *result)
{
    int i;
    int j;
    double hit_ratio;
    double fault_ratio;

    g_string_append_printf(output, "\n=== %s ===\n", name);
    g_string_append(output, "Page\tFrames\t\tStatus\n");
    g_string_append(output, "----------------------------------------\n");

    for (i = 0; i < result->referenceCount; i++)
    {
        g_string_append_printf(output, "%d\t", pages[i]);

        for (j = 0; j < result->frameCount; j++)
        {
            if (result->frames[i][j] == -1)
                g_string_append(output, "- ");
            else
                g_string_append_printf(output, "%d ",
                                        result->frames[i][j]);
        }

        g_string_append_printf(output, "\t\t%s\n",
                               result->hit[i] ? "HIT" : "FAULT");
    }

    if (result->referenceCount > 0)
    {
        hit_ratio = (double)result->pageHits / result->referenceCount;
        fault_ratio = (double)result->pageFaults / result->referenceCount;
    }
    else
    {
        hit_ratio = 0.0;
        fault_ratio = 0.0;
    }

    g_string_append_printf(output,
        "\nPage hits: %d\nPage faults: %d\n"
        "Hit ratio: %.2f%%\nFault ratio: %.2f%%\n",
        result->pageHits, result->pageFaults,
        hit_ratio * 100.0, fault_ratio * 100.0);
}

static void run_simulation(GtkWidget *button, gpointer data)
{
    const char *page_text;
    const char *frame_text;
    const char *algorithm;
    char *end;
    long frame_value;
    int pages[MAX_PAGES];
    int count;
    int frames;
    PageResult result;
    GString *output;

    (void)button;
    (void)data;

    page_text = gtk_entry_get_text(GTK_ENTRY(pages_entry));
    frame_text = gtk_entry_get_text(GTK_ENTRY(frames_entry));

    if (!parse_pages(page_text, pages, &count))
    {
        show_message("Invalid page-reference string.\n"
                     "Enter 1 to 100 non-negative page numbers, "
                     "separated by spaces or commas.");
        return;
    }

    if (frame_text == NULL || *frame_text == '\0')
    {
        show_message("Please enter the number of frames.");
        return;
    }

    frame_value = strtol(frame_text, &end, 10);

    while (*end != '\0' && isspace((unsigned char)*end))
        end++;

    if (end == frame_text || *end != '\0' ||
        frame_value < 1 || frame_value > MAX_FRAMES)
    {
        show_message("Invalid frame count. Enter a number from 1 to 20.");
        return;
    }

    frames = (int)frame_value;
    algorithm = gtk_combo_box_text_get_active_text(
                    GTK_COMBO_BOX_TEXT(algorithm_combo));

    output = g_string_new(NULL);

    if (g_strcmp0(algorithm, "FIFO") == 0)
    {
        run_fifo(pages, count, frames, &result);
        append_result(output, "FIFO", pages, &result);
    }
    else if (g_strcmp0(algorithm, "LRU") == 0)
    {
        run_lru(pages, count, frames, &result);
        append_result(output, "LRU", pages, &result);
    }
    else if (g_strcmp0(algorithm, "Optimal") == 0)
    {
        run_optimal(pages, count, frames, &result);
        append_result(output, "Optimal", pages, &result);
    }
    else if (g_strcmp0(algorithm, "Compare All") == 0)
    {
        PageResult fifo_result;
        PageResult lru_result;
        PageResult optimal_result;

        run_fifo(pages, count, frames, &fifo_result);
        run_lru(pages, count, frames, &lru_result);
        run_optimal(pages, count, frames, &optimal_result);

        append_result(output, "FIFO", pages, &fifo_result);
        append_result(output, "LRU", pages, &lru_result);
        append_result(output, "Optimal", pages, &optimal_result);

        g_string_append(output, "\n=== Comparison Summary ===\n");
        g_string_append_printf(output,
            "FIFO faults: %d\nLRU faults: %d\nOptimal faults: %d\n",
            fifo_result.pageFaults, lru_result.pageFaults,
            optimal_result.pageFaults);
    }

    show_message(output->str);
    g_string_free(output, TRUE);
    g_free((gchar *)algorithm);
}

void start_gui(void)
{
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *title;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *scroll;
    GtkWidget *input_box;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Page Replacement Simulator");
    gtk_window_set_default_size(GTK_WINDOW(window), 850, 650);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    title = gtk_label_new(
        "Simulation and Comparative Analysis of Page Replacement Algorithms");
    gtk_box_pack_start(GTK_BOX(main_box), title, FALSE, FALSE, 5);

    input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_box_pack_start(GTK_BOX(main_box), input_box, FALSE, FALSE, 5);

    label = gtk_label_new(
        "Page reference string (example: 7 0 1 2 0 3):");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(input_box), label, FALSE, FALSE, 0);

    pages_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(
        GTK_ENTRY(pages_entry), "Enter page numbers separated by spaces");
    gtk_box_pack_start(GTK_BOX(input_box), pages_entry, FALSE, FALSE, 0);

    label = gtk_label_new("Number of frames:");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(input_box), label, FALSE, FALSE, 0);

    frames_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(frames_entry), "Example: 3");
    gtk_box_pack_start(GTK_BOX(input_box), frames_entry, FALSE, FALSE, 0);

    label = gtk_label_new("Choose algorithm:");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(input_box), label, FALSE, FALSE, 0);

    algorithm_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "FIFO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LRU");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Optimal");
    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "Compare All");
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithm_combo), 0);
    gtk_box_pack_start(GTK_BOX(input_box), algorithm_combo, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Run Simulation");
    g_signal_connect(button, "clicked",
                     G_CALLBACK(run_simulation), NULL);
    gtk_box_pack_start(GTK_BOX(main_box), button, FALSE, FALSE, 5);

    label = gtk_label_new("Simulation Results:");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(main_box), label, FALSE, FALSE, 0);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), scroll, TRUE, TRUE, 0);

    results_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(results_view), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(results_view), TRUE);
    gtk_container_add(GTK_CONTAINER(scroll), results_view);

    gtk_widget_show_all(window);
}
