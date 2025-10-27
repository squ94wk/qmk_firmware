#include "metrics.h"
#include "print.h"
#include <stdio.h>
#include "quantum.h"

histogram_t magic_timing_histogram = {0};

void metrics_observe(histogram_t *hist, uint16_t duration_ms) {
    hist->count++;

    int bucket = 0;
    if (duration_ms >= 100) {
        bucket = MAGIC_HISTOGRAM_BUCKETS - 1;
    } else if (duration_ms >= 1) {
        uint16_t temp = duration_ms;
        while (temp >= 2 && bucket < MAGIC_HISTOGRAM_BUCKETS - 2) {
            temp >>= 1;
            bucket++;
        }
    }
    hist->buckets[bucket]++;
}

void metrics_print_histogram(histogram_t *hist, const char *name) {
    char buffer[64];

    SEND_STRING("=== ");
    SEND_STRING(name);
    SEND_STRING(" Histogram ===\n");

    snprintf(buffer, sizeof(buffer), "Total calls: %lu\n", hist->count);
    send_string(buffer);

    static const char *labels[MAGIC_HISTOGRAM_BUCKETS] = {
        "<1ms",
        "1-2ms",
        "2-4ms",
        "4-8ms",
        "8-16ms",
        "16-32ms",
        "32-64ms",
        ">64ms"
    };

    for (int i = 0; i < MAGIC_HISTOGRAM_BUCKETS; i++) {
        if (hist->buckets[i] > 0) {
            SEND_STRING("  ");
            SEND_STRING(labels[i]);
            SEND_STRING(": ");
            snprintf(buffer, sizeof(buffer), "%lu", hist->buckets[i]);
            send_string(buffer);
            SEND_STRING("\n");
        }
    }
}
