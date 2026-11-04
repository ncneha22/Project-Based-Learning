#include <gtk/gtk.h>
#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "app.h"
#include "deadlock.h"
#include "resource_manager.h"

static SystemState state;
static int loaded = 0;

static GtkWidget *window, *notebook, *output, *status_label;
static GtkWidget *process_spin, *resource_spin, *total_entry;
static GtkWidget *matrix_box, *available_box;
static GtkWidget *graph_area;
static GtkWidget *pid_entry, *values_entry, *action_combo;

static GtkWidget *alloc_entry[MAX_PROCESSES][MAX_RESOURCES];
static GtkWidget *max_entry[MAX_PROCESSES][MAX_RESOURCES];
static GtkWidget *need_label[MAX_PROCESSES][MAX_RESOURCES];
static GtkWidget *available_value[MAX_RESOURCES];

static const int classic_alloc[5][3] = {
    {0,1,0}, {2,0,0}, {3,0,2}, {2,1,1}, {0,0,2}
};
static const int classic_max[5][3] = {
    {7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}
};

static void message(const char *text)
{
    GtkTextBuffer *buffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(output));
    gtk_text_buffer_set_text(buffer, text, -1);
}

static GtkWidget *make_label(const char *text)
{
    GtkWidget *label = gtk_label_new(text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}

static GtkWidget *make_entry(const char *text)
{
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
    gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
    gtk_entry_set_text(GTK_ENTRY(entry), text);
    return entry;
}

static int read_entry(GtkWidget *entry, int *value)
{
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    char *end;
    long number = strtol(text, &end, 10);

    while (*end == ' ' || *end == '\t')
        end++;

    if (text[0] == '\0' || *end != '\0' ||
        number < 0 || number > 100000)
        return 0;

    *value = (int)number;
    return 1;
}

static void update_status(const char *text)
{
    char label[256];
    snprintf(label, sizeof(label), "SYSTEM STATUS: %s", text);
    gtk_label_set_text(GTK_LABEL(status_label), label);
}

static void clear_box(GtkWidget *box)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(box));
    GList *item;

    for (item = children; item != NULL; item = item->next)
        gtk_widget_destroy(GTK_WIDGET(item->data));

    g_list_free(children);
}

static void update_matrix_values(void)
{
    int i, j;
    char text[30];

    for (i = 0; i < state.process_count; i++) {
        for (j = 0; j < state.resource_count; j++) {
            snprintf(text, sizeof(text), "%d", state.need[i][j]);
            gtk_label_set_text(GTK_LABEL(need_label[i][j]), text);
        }
    }

    for (j = 0; j < state.resource_count; j++) {
        snprintf(text, sizeof(text), "%d", state.available[j]);
        gtk_label_set_text(GTK_LABEL(available_value[j]), text);
    }
}

static void build_matrices(void)
{
    GtkWidget *outer, *frame, *grid, *label, *row;
    int i, j;
    char text[30];
    const char *names[] = {"Allocation", "Maximum", "Need"};

    clear_box(matrix_box);
    memset(alloc_entry, 0, sizeof(alloc_entry));
    memset(max_entry, 0, sizeof(max_entry));
    memset(need_label, 0, sizeof(need_label));

    outer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_pack_start(GTK_BOX(matrix_box), outer, FALSE, FALSE, 0);

    for (int section = 0; section < 3; section++) {
        frame = gtk_frame_new(names[section]);
        gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
        gtk_box_pack_start(GTK_BOX(outer), frame, TRUE, TRUE, 0);

        grid = gtk_grid_new();
        gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
        gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
        gtk_container_add(GTK_CONTAINER(frame), grid);

        label = make_label("Process");
        gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

        for (j = 0; j < state.resource_count; j++) {
            snprintf(text, sizeof(text), "R%d", j);
            label = gtk_label_new(text);
            gtk_grid_attach(GTK_GRID(grid), label, j + 1, 0, 1, 1);
        }

        for (i = 0; i < state.process_count; i++) {
            snprintf(text, sizeof(text), "P%d", i);
            label = gtk_label_new(text);
            gtk_grid_attach(GTK_GRID(grid), label, 0, i + 1, 1, 1);

            for (j = 0; j < state.resource_count; j++) {
                if (section == 0) {
                    snprintf(text, sizeof(text), "%d",
                             state.allocation[i][j]);
                    row = make_entry(text);
                    alloc_entry[i][j] = row;
                } else if (section == 1) {
                    snprintf(text, sizeof(text), "%d",
                             state.maximum[i][j]);
                    row = make_entry(text);
                    max_entry[i][j] = row;
                } else {
                    snprintf(text, sizeof(text), "%d",
                             state.need[i][j]);
                    row = gtk_label_new(text);
                    need_label[i][j] = row;
                }

                gtk_widget_set_size_request(row, 48, 30);
                gtk_grid_attach(GTK_GRID(grid), row, j + 1, i + 1, 1, 1);
            }
        }
    }

    frame = gtk_frame_new("Available Resources");
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start(GTK_BOX(matrix_box), frame, FALSE, FALSE, 8);

    available_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(available_box), 10);
    gtk_container_add(GTK_CONTAINER(frame), available_box);

    for (j = 0; j < state.resource_count; j++) {
        snprintf(text, sizeof(text), "R%d", j);
        row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_box_pack_start(GTK_BOX(available_box),
                           gtk_label_new(text), FALSE, FALSE, 0);

        snprintf(text, sizeof(text), "%d", state.available[j]);
        available_value[j] = gtk_label_new(text);
        gtk_widget_set_size_request(available_value[j], 40, 28);
        gtk_box_pack_start(GTK_BOX(row), available_value[j], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(available_box), row, FALSE, FALSE, 0);
    }

    gtk_widget_show_all(matrix_box);
}

static void refresh_all(void)
{
    if (!loaded)
        return;

    update_matrix_values();
    gtk_widget_queue_draw(graph_area);
}

static void reset_system(GtkWidget *button, gpointer data)
{
    (void)button;
    (void)data;

    memset(&state, 0, sizeof(state));
    loaded = 0;

    clear_box(matrix_box);
    memset(alloc_entry, 0, sizeof(alloc_entry));
    memset(max_entry, 0, sizeof(max_entry));
    memset(need_label, 0, sizeof(need_label));
    memset(available_value, 0, sizeof(available_value));

    gtk_entry_set_text(GTK_ENTRY(total_entry), "");
    gtk_entry_set_text(GTK_ENTRY(pid_entry), "");
    gtk_entry_set_text(GTK_ENTRY(values_entry), "");

    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(output)),
        "System has been reset. Create a new system or load the classic example.",
        -1
    );

    update_status("NOT LOADED");
    gtk_widget_queue_draw(graph_area);
}

static void create_system(GtkWidget *button, gpointer data)
{
    int p, r, i, j;
    (void)button;
    (void)data;

    p = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(process_spin));
    r = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(resource_spin));

    memset(&state, 0, sizeof(state));
    state.process_count = p;
    state.resource_count = r;

    for (i = 0; i < p; i++)
        state.active[i] = 1;

    for (j = 0; j < r; j++)
        state.total[j] = 10;

    loaded = 1;
    build_matrices();
    refresh_all();
    update_status("READY");
    message("Processes and resources created.\nEdit Allocation and Maximum values in the Matrices tab, then click Apply matrix edits.");
}

static void load_classic(GtkWidget *button, gpointer data)
{
    int i, j;
    (void)button;
    (void)data;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(process_spin), 5);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(resource_spin), 3);
    gtk_entry_set_text(GTK_ENTRY(total_entry), "10 5 7");

    memset(&state, 0, sizeof(state));
    state.process_count = 5;
    state.resource_count = 3;

    for (i = 0; i < 5; i++) {
        state.active[i] = 1;
        for (j = 0; j < 3; j++) {
            state.allocation[i][j] = classic_alloc[i][j];
            state.maximum[i][j] = classic_max[i][j];
        }
    }

    for (j = 0; j < 3; j++)
        state.total[j] = (int[]){10,5,7}[j];

    calculate_need(&state);

    for (j = 0; j < 3; j++) {
        int allocated = 0;
        for (i = 0; i < 5; i++)
            allocated += state.allocation[i][j];
        state.available[j] = state.total[j] - allocated;
    }

    loaded = 1;
    build_matrices();
    refresh_all();
    update_status("READY");
    message("Classic Banker's Algorithm example loaded.\nExpected safe sequence: P1 -> P3 -> P4 -> P5 -> P2.");
}

static void apply_matrices(GtkWidget *button, gpointer data)
{
    int i, j, total[MAX_RESOURCES];
    char *copy, *token;
    (void)button;
    (void)data;

    if (!loaded) {
        message("Create the system first.");
        return;
    }

    copy = g_strdup(gtk_entry_get_text(GTK_ENTRY(total_entry)));
    token = strtok(copy, " ,\t\n");

    for (j = 0; j < state.resource_count; j++) {
        char *end;
        long value;

        if (token == NULL) {
            g_free(copy);
            message("Enter all total resource values on Dashboard.");
            return;
        }

        value = strtol(token, &end, 10);
        if (*end != '\0' || value < 0 || value > 100000) {
            g_free(copy);
            message("Invalid total resource value.");
            return;
        }

        total[j] = (int)value;
        token = strtok(NULL, " ,\t\n");
    }
    g_free(copy);

    for (i = 0; i < state.process_count; i++) {
        for (j = 0; j < state.resource_count; j++) {
            if (!read_entry(alloc_entry[i][j], &state.allocation[i][j]) ||
                !read_entry(max_entry[i][j], &state.maximum[i][j])) {
                message("Enter valid non-negative numbers in every matrix box.");
                return;
            }

            if (state.allocation[i][j] > state.maximum[i][j]) {
                message("Allocation cannot be greater than Maximum.");
                return;
            }
        }
    }

    for (j = 0; j < state.resource_count; j++) {
        int allocated = 0;
        for (i = 0; i < state.process_count; i++)
            allocated += state.allocation[i][j];

        if (allocated > total[j]) {
            message("Allocated resources cannot exceed total resources.");
            return;
        }

        state.total[j] = total[j];
        state.available[j] = total[j] - allocated;
    }

    calculate_need(&state);
    refresh_all();
    message("Matrix values applied successfully.");
}

static void check_safety(GtkWidget *button, gpointer data)
{
    int sequence[MAX_PROCESSES], length = 0;
    int deadlocked[MAX_PROCESSES];
    char text[1000] = "";
    int i;
    (void)button;
    (void)data;

    if (!loaded) {
        message("Create or load a system first.");
        return;
    }

    if (banker_is_safe(&state, sequence, &length)) {
        strcat(text, "SYSTEM IS SAFE\nSafe sequence: ");
        for (i = 0; i < length; i++) {
            char part[20];
            snprintf(part, sizeof(part), "P%d", sequence[i]);
            strcat(text, part);
            if (i < length - 1)
                strcat(text, " -> ");
        }
        update_status("SAFE");
    } else {
        strcat(text, "SYSTEM IS NOT IN A SAFE STATE.");
        update_status("UNSAFE");
    }

    if (detect_deadlock(&state, deadlocked)) {
        strcat(text, "\nDeadlock detected in: ");
        for (i = 0; i < state.process_count; i++) {
            if (deadlocked[i]) {
                char part[20];
                snprintf(part, sizeof(part), "P%d ", i);
                strcat(text, part);
            }
        }
    } else {
        strcat(text, "\nNo deadlock detected.");
    }

    message(text);
    refresh_all();
}

static void draw_arrow(cairo_t *cr, double x1, double y1,
                       double x2, double y2, int request)
{
    double angle = atan2(y2 - y1, x2 - x1);

    if (request)
        cairo_set_source_rgb(cr, 0.85, 0.2, 0.2);
    else
        cairo_set_source_rgb(cr, 0.1, 0.55, 0.35);

    cairo_set_line_width(cr, 1.6);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);

    cairo_move_to(cr, x2, y2);
    cairo_line_to(cr, x2 - 8 * cos(angle - 0.4),
                  y2 - 8 * sin(angle - 0.4));
    cairo_line_to(cr, x2 - 8 * cos(angle + 0.4),
                  y2 - 8 * sin(angle + 0.4));
    cairo_close_path(cr);
    cairo_fill(cr);
}

static gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    RAGEdge edges[MAX_PROCESSES * MAX_RESOURCES * 2];
    int count, i;
    double width = gtk_widget_get_allocated_width(widget);
    double px = 150, rx = width - 150;
    (void)data;

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    if (!loaded) {
        cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
        cairo_move_to(cr, 30, 40);
        cairo_show_text(cr, "Create or load a system to display the graph.");
        return FALSE;
    }

    count = build_rag_edges(&state, edges,
                            MAX_PROCESSES * MAX_RESOURCES * 2);

    for (i = 0; i < count; i++) {
        double py = 60 + edges[i].process_id * 65;
        double ry = 60 + edges[i].resource_id * 65;

        if (edges[i].edge_type == ALLOCATION_EDGE)
            draw_arrow(cr, rx - 30, ry, px + 30, py, 0);
        else
            draw_arrow(cr, px + 30, py, rx - 30, ry, 1);
    }

    for (i = 0; i < state.process_count; i++) {
        double y = 60 + i * 65;
        char label[20];

        cairo_set_source_rgb(cr, 0.3, 0.25, 0.85);
        cairo_rectangle(cr, px - 23, y - 15, 46, 30);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 1, 1, 1);
        snprintf(label, sizeof(label), "P%d", i);
        cairo_move_to(cr, px - 9, y + 5);
        cairo_show_text(cr, label);
    }

    for (i = 0; i < state.resource_count; i++) {
        double y = 60 + i * 65;
        char label[20];

        cairo_set_source_rgb(cr, 0.05, 0.55, 0.45);
        cairo_rectangle(cr, rx - 23, y - 15, 46, 30);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 1, 1, 1);
        snprintf(label, sizeof(label), "R%d", i);
        cairo_move_to(cr, rx - 9, y + 5);
        cairo_show_text(cr, label);
    }

    return FALSE;
}

static void refresh_graph(GtkWidget *button, gpointer data)
{
    (void)button;
    (void)data;
    gtk_widget_queue_draw(graph_area);
}

static void perform_action(GtkWidget *button, gpointer data)
{
    int pid, values[MAX_RESOURCES], i, result;
    char text[300];
    gchar *action;
    (void)button;
    (void)data;

    if (!loaded) {
        message("Create or load a system first.");
        return;
    }

    pid = atoi(gtk_entry_get_text(GTK_ENTRY(pid_entry)));

    if (pid < 0 || pid >= state.process_count || !state.active[pid]) {
        message("Enter a valid process ID (P0, P1, P2...).");
        return;
    }

    action = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(action_combo));

    if (strcmp(action, "Recover Process") != 0) {
        char *copy = g_strdup(gtk_entry_get_text(GTK_ENTRY(values_entry)));
        char *token = strtok(copy, " ,\t\n");

        for (i = 0; i < state.resource_count; i++) {
            char *end;
            long value;

            if (token == NULL) {
                g_free(copy);
                g_free(action);
                message("Enter one value for every resource.");
                return;
            }

            value = strtol(token, &end, 10);
            if (*end != '\0' || value < 0 || value > 100000) {
                g_free(copy);
                g_free(action);
                message("Invalid resource value.");
                return;
            }

            values[i] = (int)value;
            token = strtok(NULL, " ,\t\n");
        }
        g_free(copy);
    }

    if (strcmp(action, "Request Resources") == 0) {
        int waiting = 0;

        for (i = 0; i < state.resource_count; i++)
            state.request[pid][i] = 0;

        result = request_resources(&state, pid, values);

        if (result == 1) {
            snprintf(text, sizeof(text), "Resources granted to P%d.", pid);
        } else if (result == 0) {
            for (i = 0; i < state.resource_count; i++) {
                if (values[i] > state.available[i])
                    waiting = 1;
            }

            if (waiting) {
                for (i = 0; i < state.resource_count; i++)
                    state.request[pid][i] = values[i];

                snprintf(text, sizeof(text),
                         "P%d is waiting for unavailable resources. Red arrows show pending requests.", pid);
            } else {
                snprintf(text, sizeof(text),
                         "Request denied because it would make the system unsafe.");
            }
        } else {
            snprintf(text, sizeof(text), "Invalid resource request.");
        }
    } else if (strcmp(action, "Release Resources") == 0) {
        for (i = 0; i < state.resource_count; i++) {
            if (values[i] > state.allocation[pid][i]) {
                g_free(action);
                message("Cannot release more than the allocated amount.");
                return;
            }
        }
        release_resources(&state, pid, values);
        snprintf(text, sizeof(text), "Resources released by P%d.", pid);
    } else {
        if (recover_process(&state, pid))
            snprintf(text, sizeof(text), "Recovery completed for P%d.", pid);
        else
            snprintf(text, sizeof(text), "Recovery failed.");
    }

    g_free(action);
    message(text);
    refresh_all();
}

static GtkWidget *make_page(void)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(box), 14);
    return box;
}

void start_gui(void)
{
    GtkWidget *root, *header, *title_box, *title, *subtitle;
    GtkWidget *page, *grid, *button, *row, *frame, *scroll;
    GtkWidget *label, *legend;
    GtkCssProvider *css;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Deadlock Simulation System — Week 4");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css,
        "window { background: #f4f4f4; }"
        "notebook tab { padding: 9px 16px; }"
        "frame { border: 1px solid #c8c8c8; }"
        "entry { padding: 5px; }"
        "button { padding: 7px; }"
        ".heading { font-size: 22px; font-weight: bold; }",
        -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(), GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(root), 12);
    gtk_container_add(GTK_CONTAINER(window), root);

    header = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_box_pack_start(GTK_BOX(root), header, FALSE, FALSE, 0);

    title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title),
        "<span size='x-large' weight='bold'>Deadlock Simulation System</span>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(header), title, FALSE, FALSE, 0);

    subtitle = gtk_label_new("Operating Systems Lab · Interactive Week 4 Project");
    gtk_widget_set_halign(subtitle, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(header), subtitle, FALSE, FALSE, 0);

    status_label = gtk_label_new("SYSTEM STATUS: NOT INITIALIZED");
    gtk_widget_set_halign(status_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(header), status_label, FALSE, FALSE, 5);

    notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(root), notebook, TRUE, TRUE, 0);

    /* DASHBOARD */
    page = make_page();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page,
                             gtk_label_new("Dashboard"));

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_box_pack_start(GTK_BOX(page), grid, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID(grid), make_label("Processes:"), 0, 0, 1, 1);
    process_spin = gtk_spin_button_new_with_range(1, MAX_PROCESSES, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(process_spin), 5);
    gtk_grid_attach(GTK_GRID(grid), process_spin, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), make_label("Resource types:"), 2, 0, 1, 1);
    resource_spin = gtk_spin_button_new_with_range(1, MAX_RESOURCES, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(resource_spin), 3);
    gtk_grid_attach(GTK_GRID(grid), resource_spin, 3, 0, 1, 1);

    button = gtk_button_new_with_label("Create processes / resources");
    g_signal_connect(button, "clicked", G_CALLBACK(create_system), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 0, 1, 1);

    button = gtk_button_new_with_label("Load classic example");
    g_signal_connect(button, "clicked", G_CALLBACK(load_classic), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 5, 0, 1, 1);

    button = gtk_button_new_with_label("Reset");
    g_signal_connect(button, "clicked", G_CALLBACK(reset_system), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 6, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), make_label("Total resources:"), 0, 1, 1, 1);
    total_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(total_entry), "10 5 7");
    gtk_grid_attach(GTK_GRID(grid), total_entry, 1, 1, 2, 1);

    button = gtk_button_new_with_label("Apply matrix edits");
    g_signal_connect(button, "clicked", G_CALLBACK(apply_matrices), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 1, 3, 1);

    button = gtk_button_new_with_label("Banker's Algorithm");
    g_signal_connect(button, "clicked", G_CALLBACK(check_safety), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);

    button = gtk_button_new_with_label("Detect deadlock");
    g_signal_connect(button, "clicked", G_CALLBACK(check_safety), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 2, 2, 1);

    button = gtk_button_new_with_label("System status");
    g_signal_connect(button, "clicked", G_CALLBACK(check_safety), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 2, 2, 1);

    /* MATRICES */
    page = make_page();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page,
                             gtk_label_new("Matrices"));

    label = gtk_label_new("Editable Allocation, Maximum and Available values");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(page), label, FALSE, FALSE, 0);

    matrix_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), matrix_box);
    gtk_box_pack_start(GTK_BOX(page), scroll, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Apply matrix edits");
    g_signal_connect(button, "clicked", G_CALLBACK(apply_matrices), NULL);
    gtk_box_pack_start(GTK_BOX(page), button, FALSE, FALSE, 0);

    /* RESOURCE ALLOCATION GRAPH */
    page = make_page();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page,
                             gtk_label_new("Resource Allocation Graph"));

    label = gtk_label_new("Resource Allocation Graph");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(page), label, FALSE, FALSE, 0);

    graph_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(graph_area, 850, 350);
    g_signal_connect(graph_area, "draw", G_CALLBACK(draw_graph), NULL);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), graph_area);
    gtk_box_pack_start(GTK_BOX(page), scroll, TRUE, TRUE, 0);

    legend = gtk_label_new(
        "Green arrows: Allocation (Resource -> Process)     Red arrows: Request (Process -> Resource)");
    gtk_box_pack_start(GTK_BOX(page), legend, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Refresh graph");
    g_signal_connect(button, "clicked", G_CALLBACK(refresh_graph), NULL);
    gtk_box_pack_start(GTK_BOX(page), button, FALSE, FALSE, 0);

    /* RESULTS DISPLAY - directly below the graph */
    output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(output), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(output), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(output), GTK_WRAP_WORD_CHAR);

    {
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(
            provider,
            "textview { font-family: monospace; font-size: 14pt; padding: 8px; }",
            -1, NULL
        );
        gtk_style_context_add_provider(
            gtk_widget_get_style_context(output),
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
        g_object_unref(provider);
    }

    gtk_widget_set_size_request(output, -1, 110);
    gtk_box_pack_start(GTK_BOX(page), output, FALSE, TRUE, 0);

    /* REQUESTS AND RECOVERY */
    page = make_page();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page,
                             gtk_label_new("Requests & Recovery"));

    frame = gtk_frame_new("Resource Request / Release / Recovery");
    gtk_container_set_border_width(GTK_CONTAINER(frame), 12);
    gtk_box_pack_start(GTK_BOX(page), frame, FALSE, FALSE, 0);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(frame), grid);

    gtk_grid_attach(GTK_GRID(grid), make_label("Process ID:"), 0, 0, 1, 1);
    pid_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(pid_entry), "Example: 0");
    gtk_grid_attach(GTK_GRID(grid), pid_entry, 1, 0, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), make_label("Resource values:"), 0, 1, 1, 1);
    values_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(values_entry), "Example: 1 0 2");
    gtk_grid_attach(GTK_GRID(grid), values_entry, 1, 1, 2, 1);

    action_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(action_combo),
                                   "Request Resources");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(action_combo),
                                   "Release Resources");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(action_combo),
                                   "Recover Process");
    gtk_combo_box_set_active(GTK_COMBO_BOX(action_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), action_combo, 0, 2, 2, 1);

    button = gtk_button_new_with_label("Execute action");
    g_signal_connect(button, "clicked", G_CALLBACK(perform_action), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 2, 1, 1);

    message("Welcome. Load the classic example to test Banker's Algorithm.");
    gtk_widget_show_all(window);
}
