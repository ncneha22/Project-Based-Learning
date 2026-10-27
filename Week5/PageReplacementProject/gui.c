
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "page_replacement.h"
#include "chart.h"

/* GUI widgets */
static GtkWidget *pages_entry;
static GtkWidget *frames_entry;
static GtkWidget *algorithm_combo;
static GtkWidget *results_view;
static GtkWidget *chart_label;
static GtkWidget *chart_area;

/* Chart values */
static int show_chart = 0;
static int fifo_faults = 0;
static int lru_faults = 0;
static int optimal_faults = 0;
static int fifo_hits = 0;
static int lru_hits = 0;
static int optimal_hits = 0;

/* Display a message in the results area */
static void show_message(const char *message)
{
    GtkTextBuffer *buffer;

    buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(results_view));

    gtk_text_buffer_set_text(buffer, message, -1);
}

/* Convert the input text into page numbers */
static int parse_pages(const char *input,
                       int pages[],
                       int *count)
{
    char *copy;
    char *token;
    char *end;
    char *p;
    long value;

    *count = 0;

    if (input == NULL || *input == '\0')
        return 0;

    copy = g_strdup(input);
    token = strtok(copy, " \t\n,");

    while (token != NULL)
    {
        p = token;

        if (*p == '+' || *p == '-')
            p++;

        if (*p == '\0')
        {
            g_free(copy);
            return 0;
        }

        while (*p != '\0')
        {
            if (!isdigit((unsigned char)*p))
            {
                g_free(copy);
                return 0;
            }

            p++;
        }

        value = strtol(token, &end, 10);

        if (*end != '\0' ||
            value < 0 ||
            value > 2147483647L ||
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

/* Display the step-by-step results */
static void append_result(GString *output,
                          const char *name,
                          int pages[],
                          PageResult *result)
{
    int i;
    int j;
    double hit_ratio;
    double fault_ratio;

    g_string_append_printf(output,
                           "\n=== %s ===\n", name);

    g_string_append(output,
                    "Page\tFrames\t\tStatus\n");

    g_string_append(output,
                    "----------------------------------------\n");

    for (i = 0; i < result->referenceCount; i++)
    {
        g_string_append_printf(output, "%d\t", pages[i]);

        for (j = 0; j < result->frameCount; j++)
        {
            if (result->frames[i][j] == -1)
                g_string_append(output, "- ");
            else
                g_string_append_printf(
                    output, "%d ",
                    result->frames[i][j]);
        }

        g_string_append_printf(
            output, "\t\t%s\n",
            result->hit[i] ? "HIT" : "FAULT");
    }

    hit_ratio = (double)result->pageHits /
                result->referenceCount;

    fault_ratio = (double)result->pageFaults /
                  result->referenceCount;

    g_string_append_printf(
        output,
        "\nPage hits: %d\n"
        "Page faults: %d\n"
        "Hit ratio: %.2f%%\n"
        "Fault ratio: %.2f%%\n",
        result->pageHits,
        result->pageFaults,
        hit_ratio * 100.0,
        fault_ratio * 100.0);
}

/* Draw the comparison chart */
static gboolean draw_chart(GtkWidget *widget,
                           cairo_t *cr,
                           gpointer data)
{
    (void)widget;
    (void)data;

    if (show_chart)
    {
        draw_comparison_chart(
            cr,
            fifo_hits, fifo_faults,
            lru_hits, lru_faults,
            optimal_hits, optimal_faults);
    }

    return FALSE;
}

/* Run the selected algorithm */
static void run_simulation(GtkWidget *button,
                           gpointer data)
{
    const char *page_text;
    const char *frame_text;
    gchar *algorithm;
    char *end;
    long frame_value;

    int pages[MAX_PAGES];
    int count;
    int frames;

    PageResult result;
    GString *output;

    (void)button;
    (void)data;

    show_chart = 0;
    gtk_widget_hide(chart_label);
    gtk_widget_hide(chart_area);
    gtk_widget_queue_draw(chart_area);

    page_text = gtk_entry_get_text(
        GTK_ENTRY(pages_entry));

    frame_text = gtk_entry_get_text(
        GTK_ENTRY(frames_entry));

    if (!parse_pages(page_text, pages, &count))
    {
        show_message(
            "Invalid page-reference string.\n"
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

    while (*end != '\0' &&
           isspace((unsigned char)*end))
    {
        end++;
    }

    if (end == frame_text ||
        *end != '\0' ||
        frame_value < 1 ||
        frame_value > MAX_FRAMES)
    {
        show_message(
            "Invalid frame count. Enter a number from 1 to 20.");
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

        fifo_faults = fifo_result.pageFaults;
        lru_faults = lru_result.pageFaults;
        optimal_faults = optimal_result.pageFaults;

        fifo_hits = fifo_result.pageHits;
        lru_hits = lru_result.pageHits;
        optimal_hits = optimal_result.pageHits;

        show_chart = 1;
        gtk_widget_show(chart_label);
        gtk_widget_show(chart_area);

        g_string_append(output,
                        "\n=== Comparison Summary ===\n");

        g_string_append_printf(
            output,
            "Algorithm   Hits   Faults   Hit Ratio   Fault Ratio\n"
            "FIFO        %d      %d       %.2f%%       %.2f%%\n"
            "LRU         %d      %d       %.2f%%       %.2f%%\n"
            "Optimal     %d      %d       %.2f%%       %.2f%%\n",
            fifo_result.pageHits, fifo_result.pageFaults,
            fifo_result.pageHits * 100.0 / count,
            fifo_result.pageFaults * 100.0 / count,
            lru_result.pageHits, lru_result.pageFaults,
            lru_result.pageHits * 100.0 / count,
            lru_result.pageFaults * 100.0 / count,
            optimal_result.pageHits, optimal_result.pageFaults,
            optimal_result.pageHits * 100.0 / count,
            optimal_result.pageFaults * 100.0 / count);
    }

    show_message(output->str);

    gtk_widget_queue_draw(chart_area);

    g_string_free(output, TRUE);
    g_free(algorithm);
}

/* Create the GUI window */
void start_gui(void)
{
    GtkWidget *window;
    GtkWidget *outer_scroll;
    GtkWidget *main_box;
    GtkWidget *title;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *scroll;
    GtkWidget *input_box;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(
        GTK_WINDOW(window),
        "Page Replacement Simulator");

    gtk_window_set_default_size(
        GTK_WINDOW(window), 950, 650);

    gtk_container_set_border_width(
        GTK_CONTAINER(window), 15);

    g_signal_connect(
        window, "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    main_box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL, 12);

    outer_scroll = gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(outer_scroll),
        GTK_POLICY_NEVER,
        GTK_POLICY_AUTOMATIC);

    gtk_container_add(
        GTK_CONTAINER(window), outer_scroll);

    gtk_container_add(
        GTK_CONTAINER(outer_scroll), main_box);


    title = gtk_label_new(
        "Simulation and Comparative Analysis "
        "of Page Replacement Algorithms");

    gtk_box_pack_start(
        GTK_BOX(main_box), title,
        FALSE, FALSE, 5);

    input_box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL, 8);

    gtk_box_pack_start(
        GTK_BOX(main_box), input_box,
        FALSE, FALSE, 5);

    label = gtk_label_new(
        "Page reference string (example: 7 0 1 2 0 3):");

    gtk_widget_set_halign(label, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(input_box), label,
        FALSE, FALSE, 0);

    pages_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(pages_entry),
        "Enter page numbers separated by spaces");

    gtk_box_pack_start(
        GTK_BOX(input_box), pages_entry,
        FALSE, FALSE, 0);

    label = gtk_label_new("Number of frames:");

    gtk_widget_set_halign(label, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(input_box), label,
        FALSE, FALSE, 0);

    frames_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(frames_entry), "Example: 3");

    gtk_box_pack_start(
        GTK_BOX(input_box), frames_entry,
        FALSE, FALSE, 0);

    label = gtk_label_new("Choose algorithm:");

    gtk_widget_set_halign(label, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(input_box), label,
        FALSE, FALSE, 0);

    algorithm_combo = gtk_combo_box_text_new();

    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "FIFO");

    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "LRU");

    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "Optimal");

    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "Compare All");

    gtk_combo_box_set_active(
        GTK_COMBO_BOX(algorithm_combo), 0);

    gtk_box_pack_start(
        GTK_BOX(input_box), algorithm_combo,
        FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Run Simulation");

    g_signal_connect(
        button, "clicked",
        G_CALLBACK(run_simulation), NULL);

    gtk_box_pack_start(
        GTK_BOX(main_box), button,
        FALSE, FALSE, 5);

    label = gtk_label_new("Simulation Results:");

    gtk_widget_set_halign(label, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(main_box), label,
        FALSE, FALSE, 0);

    scroll = gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_AUTOMATIC,
        GTK_POLICY_ALWAYS);

    gtk_widget_set_size_request(scroll, -1, 300);
    gtk_widget_set_vexpand(scroll, FALSE);

    gtk_box_pack_start(
        GTK_BOX(main_box), scroll,
        TRUE, TRUE, 0);

    results_view = gtk_text_view_new();

    gtk_text_view_set_editable(
        GTK_TEXT_VIEW(results_view), FALSE);

    gtk_text_view_set_monospace(
        GTK_TEXT_VIEW(results_view), TRUE);

    gtk_text_view_set_wrap_mode(
        GTK_TEXT_VIEW(results_view), GTK_WRAP_NONE);

    gtk_container_add(
        GTK_CONTAINER(scroll), results_view);

    chart_label = gtk_label_new(
        "Page Fault Comparison Chart (Compare All)");

    gtk_box_pack_start(
        GTK_BOX(main_box), chart_label,
        FALSE, FALSE, 0);

    chart_area = gtk_drawing_area_new();

    gtk_widget_set_size_request(
        chart_area, 520, 400);

    g_signal_connect(
        chart_area, "draw",
        G_CALLBACK(draw_chart), NULL);

    gtk_box_pack_start(
        GTK_BOX(main_box), chart_area,
        FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_widget_hide(chart_label);
    gtk_widget_hide(chart_area);
}
