#include <stdio.h>
//#include "geometry_gen.h"
//#include "linear_geometry_gen.h"
#include "linear_geometry_gen_rejection.h"

int main(int argc, char *argv[]) {
    // Problem constants
    // Chords
    double start_value_0 = 101.0 / 20.0;  // cm
    double end_value_0 = 99.0 / 10.0;  // cm
    double start_value_1 = 101.0 / 20.0;  // cm
    double end_value_1 = 11.0 / 10.0;  // cm
    // Distances
    double end_dist = 10.0;  // cm
    long num_divs = 50;

    // Initially allocate to 1-length
    double *x_delta = malloc(sizeof *x_delta * num_divs);
    double *x_arr = malloc(sizeof *x_arr * num_divs);
    int *materials = malloc(sizeof *materials * num_divs);
    long num_cells = 0;

    //get_geometry(start_value_0, start_value_1, end_dist, num_divs, &x_delta, &x_arr, &materials, &num_cells);
    get_geometry(start_value_0, end_value_0, start_value_1, end_value_1, end_dist, num_divs, &x_delta, &x_arr, &materials, &num_cells);

    // Save material information
    FILE *fp;
    fp = fopen("../csv/material_plot.csv", "w");
    fprintf(fp, "xdist,material,xdelta\n");
    for (int i = 0; i < num_cells; i++) {
        fprintf(fp, "%f,%d,%f\n", x_arr[i], materials[i], x_delta[i]);
    }

    // Cleanup
    fclose(fp);
    free(x_delta);
    free(x_arr);
    free(materials);

    return 0;
}
