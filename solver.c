#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include "solver.h"

#define _log(...)   fprintf(stderr, __VA_ARGS__)
#define _abs(x)     (((x) < 0) ? -(x) : (x))

typedef uint8_t byte;

enum {
    addition       ,
    substraction   ,
    multiplication ,
    division       ,
};

typedef struct {
    int v1;
    int v2;
    byte op;
} computation_t;

typedef struct {
    computation_t *computation;
    int level;
} solution_t;

static solution_t solution_g;
static const vect_t *vect_g;

static int result;
static int closer;

/* {{{ display */

static void display_solution(void)
{
    puts("found a (better) solution:");

    for (int i = vect_g->len - 1; i >= solution_g.level; i--) {
	char op;
	int _result;

	switch (solution_g.computation[i].op) {
	    case addition:
		op = '+';
		_result = solution_g.computation[i].v1 + solution_g.computation[i].v2;
		break;
	    case substraction:
		op = '-';
		_result = solution_g.computation[i].v1 - solution_g.computation[i].v2;
		break;
	    case multiplication:
		op = '*';
		_result = solution_g.computation[i].v1 * solution_g.computation[i].v2;
		break;
	    case division:
		op = '/';
		_result = solution_g.computation[i].v1 / solution_g.computation[i].v2;
		break;
	    default:
		assert (0);
		break;
	}

	printf("%d %c %d = %d\n", solution_g.computation[i].v1
		, op
		, solution_g.computation[i].v2
		, _result);
    }
    puts("--------------------------");
}

/* }}} */

    static inline
void construct_new_vect(vect_t *new_vect, const vect_t *vect, unsigned v1, unsigned v2)
{
    unsigned j = 0;

    for (unsigned i = 0; i < vect->len; ++i) {
	/* check is this is one of the taken values */
	if (i == v1 || i == v2)
	    continue;

	new_vect->array[j++] = vect->array[i];
    }
}

static void solve_rec(const vect_t *vect)
{
    vect_t new_vect;

    if (solution_g.level != 0 && ((unsigned)solution_g.level >= vect->len)) {
	/* we have already found a better solution */
	return;
    }

    if (vect->len == 0)
	return;

    for (unsigned i = 0; i < vect->len; ++i) {
	if ((closer == -1)
		|| (_abs(vect->array[i] - result) <= _abs(closer - result)))
	{
	    closer = vect->array[i];
	    if (closer == result) {
		solution_g.level = vect->len;
		display_solution();
		return;
	    }

	}
    }

    new_vect.array = malloc(sizeof(int) * (vect->len - 1));
    new_vect.len   = vect->len - 1;
    for (unsigned i = 0; i < vect->len; ++i) {
	for (unsigned j = 0; j < vect->len; ++j) {
	    /* we cannot use the same value twice */
	    if (i == j)
		continue;

	    /* construct the new vector */
	    construct_new_vect(&new_vect, vect, i, j);

	    solution_g.computation[vect->len - 1].v1 = vect->array[i];
	    solution_g.computation[vect->len - 1].v2 = vect->array[j];

	    /* try adding (no condition) */
	    new_vect.array[new_vect.len - 1] = vect->array[i] + vect->array[j];
	    solution_g.computation[vect->len - 1].op = addition;
	    solve_rec(&new_vect);

	    /* try substracting (must be > 0) */
	    if (vect->array[i] >= vect->array[j]) {
		new_vect.array[new_vect.len - 1] = vect->array[i] - vect->array[j];
		solution_g.computation[vect->len - 1].op = substraction;
		solve_rec(&new_vect);
	    }

	    /* try multiplicating (no condition) */
	    new_vect.array[new_vect.len - 1] = vect->array[i] * vect->array[j];
	    solution_g.computation[vect->len - 1].op = multiplication;
	    solve_rec(&new_vect);

	    /* try dividing (no rest needed and cannot divide by 0) */
	    if ((vect->array[j] != 0) && (vect->array[i] % vect->array[j] == 0)) {
		new_vect.array[new_vect.len - 1] = vect->array[i] / vect->array[j];
		solution_g.computation[vect->len - 1].op = division;
		solve_rec(&new_vect);
	    }
	}
    }
    free(new_vect.array);
}

/* public function to use */
void solve(const vect_t *vect, int _result)
{
    closer = -1;
    vect_g = vect;
    result = _result;

    solution_g.computation = malloc(sizeof(computation_t) * vect->len);
    solution_g.level       = 0;

    solve_rec(vect);

    if (closer == result) {
	puts("Le compte est bon!");
    } else {
	result = closer;
	closer = -1;
	solve_rec(vect);
    }
    free(solution_g.computation);
}
