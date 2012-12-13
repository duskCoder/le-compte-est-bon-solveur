#ifndef SOLVER_H
#define SOLVER_H

typedef struct {
    int *array;
    unsigned len;
} vect_t;

void solve(const vect_t *vect, int result);

#endif /* SOLVER_H */
