#ifndef _GEOMETRY_GEN_H
#define _GEOMETRY_GEN_H

#include <math.h>
#include <stdlib.h>

// Using C standard pseudo random number generator
// This may be replaced with any pseudo-rng as desired

void get_geometry(double chord_0, double chord_1, double end_dist, long num_divs, double** x_delta, double** x_arr, int** materials, long* num_cells) {
    // Computational values
    int material_num;
    double rand_num;
    double chord;

    // Updateable values for sizing
    double cons_dist = 0.0;  // cm
    double dist = 0.0;  // cm
    long curr_size = num_divs;

    // Return value and reallocation sizing
    *num_cells = 0;

    // Seed generator
    srand(SEED);

    // Determine first material to use
    const double prob_0 = chord_0 / (chord_0 + chord_1);
    material_num = ((rand() / (double)RAND_MAX) < prob_0) ? 0 : 1;

    while (cons_dist < end_dist) {
        // Generate a random number
        rand_num = rand() / (double)RAND_MAX;

        // Assign a chord length based on material number
        chord = (material_num == 0) ? chord_0 : chord_1;  // cm

        // Calculate and append the material length
        dist = -chord * log(rand_num);  // cm
        cons_dist += dist;  // cm

        // Check on thickness to not overshoot the boundary
        if (cons_dist > end_dist) {
            dist += end_dist - cons_dist;  // cm
            cons_dist = end_dist;  // cm
        }

        // Further discretize geometry
        for (int i = 0; i < num_divs; i++) {
            *num_cells += 1;

            // Expand arrays in increments of the number of divisions in each sublayer
            if (*num_cells > curr_size) {
                curr_size += num_divs;
                *x_delta = realloc(*x_delta, sizeof *x_delta * curr_size);
                *x_arr = realloc(*x_arr, sizeof *x_arr * curr_size);
                *materials = realloc(*materials, sizeof *materials * curr_size);
            }

            // The width of each cell
            if (*x_delta != NULL) {
                *((*x_delta) + *num_cells - 1) = dist / (double)num_divs;
            }

            // The initial x-location of each cell
            if (*x_arr != NULL) {
                *((*x_arr) + *num_cells - 1) = cons_dist - dist + (dist / (double)num_divs * (double)i);
            }

            // The material present in each cell
            if (*materials != NULL) {
                *((*materials) + *num_cells - 1) = material_num;
            }
        }

        // Update material number
        material_num = (material_num == 0) ? 1 : 0;
    }
}

#endif
