#include <stdlib.h>
#include <stdio.h>
#include "solver.h"

int main(int argc, char **argv)
{
    const char *arg0 = *argv++;
    vect_t vect;

    if (argc < 3)
	goto usage;

    vect.len   = argc - 2;
    vect.array = malloc(sizeof(int) * vect.len);

    for (int i = 0; i < (signed)vect.len; ++i) {
	vect.array[i] = atoi(*argv++);
    }

    solve(&vect, atoi(*argv));
    free(vect.array);

    return 0;

usage:
    fprintf(stderr, "Usage: %s number ... result\n", arg0);
    return -1;
}
