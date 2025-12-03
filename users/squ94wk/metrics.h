#pragma once

#include <stdint.h>

#define MAGIC_HISTOGRAM_BUCKETS 8

typedef struct {
    uint32_t buckets[MAGIC_HISTOGRAM_BUCKETS];
    uint32_t count;
} histogram_t;

extern histogram_t magic_timing_histogram;

void metrics_observe(histogram_t *hist, uint16_t duration_ms);
void metrics_print_histogram(histogram_t *hist, const char *name);
