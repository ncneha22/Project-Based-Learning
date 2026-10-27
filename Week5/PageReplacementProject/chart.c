#include "chart.h"
#include <stdio.h>

void draw_comparison_chart(cairo_t *cr,
                           int fifo_hits, int fifo_faults,
                           int lru_hits, int lru_faults,
                           int optimal_hits, int optimal_faults)
{
    const char *names[3] = {"FIFO", "LRU", "Optimal"};
    int hits[3] = {fifo_hits, lru_hits, optimal_hits};
    int faults[3] = {fifo_faults, lru_faults, optimal_faults};
    int max_value = 1;
    int i;
    double bar_width = 35;
    double group_gap = 65;
    double bar_gap = 20;
    double chart_left = 65;
    double chart_top = 105;
    double chart_bottom = 300;
    double chart_height = chart_bottom - chart_top;
    double start_x = 100;

    for (i = 0; i < 3; i++)
    {
        if (hits[i] > max_value) max_value = hits[i];
        if (faults[i] > max_value) max_value = faults[i];
    }

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 19);
    cairo_move_to(cr, 75, 32);
    cairo_show_text(cr, "Page Hits and Faults Comparison");

    /* Legend */
    cairo_set_source_rgb(cr, 0.20, 0.65, 0.45);
    cairo_rectangle(cr, 135, 48, 14, 14);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_set_font_size(cr, 12);
    cairo_move_to(cr, 154, 60);
    cairo_show_text(cr, "Hits");

    cairo_set_source_rgb(cr, 0.25, 0.50, 0.85);
    cairo_rectangle(cr, 220, 48, 14, 14);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_move_to(cr, 239, 60);
    cairo_show_text(cr, "Faults");

    cairo_set_line_width(cr, 1.5);
    cairo_move_to(cr, chart_left, chart_top);
    cairo_line_to(cr, chart_left, chart_bottom);
    cairo_line_to(cr, 470, chart_bottom);
    cairo_stroke(cr);

    for (i = 0; i < 3; i++)
    {
        double x = start_x + i * (2 * bar_width + bar_gap + group_gap);
        double hit_height = (double)hits[i] / max_value * chart_height;
        double fault_height = (double)faults[i] / max_value * chart_height;
        double hit_y = chart_bottom - hit_height;
        double fault_y = chart_bottom - fault_height;
        char value[20];

        /* Hits bar */
        cairo_set_source_rgb(cr, 0.20, 0.65, 0.45);
        cairo_rectangle(cr, x, hit_y, bar_width, hit_height);
        cairo_fill(cr);

        /* Faults bar */
        cairo_set_source_rgb(cr, 0.25, 0.50, 0.85);
        cairo_rectangle(cr, x + bar_width + bar_gap,
                        fault_y, bar_width, fault_height);
        cairo_fill(cr);

        /* Values above bars */
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_set_font_size(cr, 13);
        snprintf(value, sizeof(value), "%d", hits[i]);
        cairo_move_to(cr, x + bar_width / 2 - 4, hit_y - 6);
        cairo_show_text(cr, value);

        snprintf(value, sizeof(value), "%d", faults[i]);
        cairo_move_to(cr, x + bar_width + bar_gap + bar_width / 2 - 4,
                      fault_y - 6);
        cairo_show_text(cr, value);

        cairo_set_font_size(cr, 14);
        cairo_move_to(cr, x + 15, chart_bottom + 25);
        cairo_show_text(cr, names[i]);
    }

    cairo_save(cr);
    cairo_translate(cr, 20, 230);
    cairo_rotate(cr, -1.57079632679);
    cairo_set_font_size(cr, 13);
    cairo_move_to(cr, 0, 0);
    cairo_show_text(cr, "Number of Pages");
    cairo_restore(cr);
}
