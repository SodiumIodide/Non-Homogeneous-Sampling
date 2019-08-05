#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define SEED 123456

#define HOMOG

#ifdef HOMOG
#include "geometry_gen.h"
#endif

#ifndef HOMOG
#include "linear_geometry_gen.h"
#include "linear_geometry_gen_rejection.h"
#endif

static inline unsigned long rdtsc() {
#if defined(__ia64)
    /* for Intel Itanium 64 bit processor */
    unsigned long x;
    __asm__ __volatile__("mov &#37;0=ar.itc" : "=r"(x) :: "memory");
    while (__builtin_expect ((int) x == -1, 0))
        __asm__ __volatile__("mov %0=ar.itc" : "=r"(x) :: "memory");
    return x;
#else
    /* for all others */
    unsigned long long x;
    unsigned int low,high;
    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high));
    x = high;
    x = (x<<32)+low;
    return x;
#endif
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Problem constants
    // Chords
    double start_value_0 = 99.0 / 100.0;  // cm
    double start_value_1 = 11.0 / 100.0;  // cm
#ifndef HOMOG
    double end_value_0 = 99.0 / 10.0;  // cm
    double end_value_1 = 11.0 / 10.0;  // cm
#endif
    // Distances
    double end_dist = 100.0;  // cm
    long num_divs = 50;

    // Initially allocate to 1-length
    double *x_delta = malloc(sizeof *x_delta * num_divs);
    double *x_arr = malloc(sizeof *x_arr * num_divs);
    int *materials = malloc(sizeof *materials * num_divs);
    long num_cells = 0;

    // File handle
    FILE *fp;
    // Iterator
    unsigned volatile int i;
    /*
    Note here that "volatile" increases overhead due to forcing the counter to
    be memory-accessed, but as both methods utilize the same amount of loops, the
    overhead introduced should be equivalent, allowing for a comparison.
    */

#ifndef HOMOG
    // Timing data
    uint64_t t_linear, t_linear_rejection;
    // Number of cycles to time
    unsigned int num_time = 100000;
#endif

#ifdef HOMOG
    get_geometry(start_value_0, start_value_1, end_dist, num_divs, &x_delta, &x_arr, &materials, &num_cells);
#endif

#ifndef HOMOG
    //get_geometry_linear(start_value_0, end_value_0, start_value_1, end_value_1, end_dist, num_divs, &x_delta, &x_arr, &materials, &num_cells);
    get_geometry_linear_rejection(start_value_0, end_value_0, start_value_1, end_value_1, end_dist, num_divs, &x_delta, &x_arr, &materials, &num_cells);
#endif

    // Save material information
    fp = fopen("../csv/material_plot.csv", "w");
    fprintf(fp, "xdist,material,xdelta\n");
    for (i = 0; i < num_cells; i++) {
        fprintf(fp, "%f,%d,%f\n", x_arr[i], materials[i], x_delta[i]);
    }

#ifndef HOMOG
    // Compare timing
    t_linear = rdtsc();
    for (i = 0; i < num_time; i++) {
        get_geometry_linear(start_value_0, end_value_0, start_value_1, end_value_1, end_dist, num_divs, &x_delta, &x_arr, &materials, &num_cells);
        // Cycle-less noop to prevent optimizing loops
        __asm__ __volatile__("");
    }
    t_linear = rdtsc() - t_linear;

    t_linear_rejection = rdtsc();
    for (i = 0; i < num_time; i++) {
        get_geometry_linear_rejection(start_value_0, end_value_0, start_value_1, end_value_1, end_dist, num_divs, &x_delta, &x_arr, &materials, &num_cells);
        // Cycle-less nooop to prevent optimizing loops
        __asm__ __volatile__("");
    }
    t_linear_rejection = rdtsc() - t_linear_rejection;

    printf("Clock cycles for %d iterations of linear function: %ld\n", num_time, t_linear);
    printf("Approximate cycles per function call: %ld\n\n", (long)(ceil((double)t_linear / num_time)));
    printf("Clock cycles for %d iterations of linear rejection function: %ld\n", num_time, t_linear_rejection);
    printf("Approximate cycles per function call: %ld\n\n", (long)(ceil((double)t_linear_rejection / num_time)));
#endif

    // Cleanup
    fclose(fp);
    free(x_delta);
    free(x_arr);
    free(materials);

    return 0;
}
