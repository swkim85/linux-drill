// part1b.c
#include <omp.h>
#include <stdio.h>

int main() {
    omp_set_max_active_levels(2); // Set the maximum number of nested active parallel regions to 3

    #pragma omp parallel num_threads(2)
    {
        #pragma omp parallel num_threads(2)
        {
            #pragma omp parallel num_threads(2)
            {
                printf("Nested level: %d\n", omp_get_level());
            }
        }
    }

    return 0;
}
