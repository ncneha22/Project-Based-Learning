#ifndef CHART_H
#define CHART_H

#include <cairo.h>

void draw_comparison_chart(cairo_t *cr,
                           int fifo_hits, int fifo_faults,
                           int lru_hits, int lru_faults,
                           int optimal_hits, int optimal_faults);

#endif
