#include <stdlib.h>
#include <stdio.h>
#include "solver.h"

int main(int argc, char **argv)
{
    vect_t vect;

    if (argc != 8) {
	puts("takes exactly 7 args");
	return -1;
    }
    vect.array = malloc(sizeof(int) * 6);
    vect.array[0] = atoi(argv[1]);
    vect.array[1] = atoi(argv[2]);
    vect.array[2] = atoi(argv[3]);
    vect.array[3] = atoi(argv[4]);
    vect.array[4] = atoi(argv[5]);
    vect.array[5] = atoi(argv[6]);

    vect.len = 6;
    solve(&vect, atoi(argv[7]));
    free(vect.array);

    return 0;
}
