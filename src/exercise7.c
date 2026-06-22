/* =================================================================
exercise7.c

Standard Conjugate Gradient (CG) method for a symmetric positive-definite
system A x = b. For background see "Iterative Methods for Sparse Linear
Systems" by Yousef Saad and "Matrix Computations" by Golub & Van Loan.

The accepted linear system input format is MatrixMarket (read from stdin).

EXERCISE: CG spends almost all of its time in a few vector kernels — the
dot product, the matrix-vector product, and the AXPY/scale updates. Each is
a simple loop over n elements. Parallelise them with worksharing loops by
replacing every `#pragma omp FIXME` below. The dot product needs a reduction.
The CG iteration itself stays sequential. The solution is in
exercise7_solution.c.

Compile: gcc -fopenmp -g -Wall -O3 -lm -o exercise7 exercise7.c

Example usage: ./exercise7 1e-5 < msc04515.dat

Produced for NCI Training.

Frederick Fung 2022, 2025
====================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


static double dotProduct(const double *restrict a, const double *restrict b, int n) {
    double sum = 0.0;
    /* FIXME: parallelise; sum is a reduction variable */
    #pragma omp FIXME
    for (int i = 0; i < n; ++i) sum += a[i] * b[i];
    return sum;
}

static void alpha_pscalar(double *restrict y, const double *restrict x, double alpha, int n) {
    /* FIXME: parallelise (y += alpha * x) */
    #pragma omp FIXME
    for (int i = 0; i < n; ++i) y[i] += alpha * x[i];
}

static void scalar(double *x, double alpha, int n) {
    /* FIXME: parallelise (x *= alpha) */
    #pragma omp FIXME
    for (int i = 0; i < n; ++i) x[i] *= alpha;
}

static void copy(double *restrict dst, const double *restrict src, int n) {
    memcpy(dst, src, (size_t)n * sizeof *dst);
}

static void matvec(double *restrict y, const double *restrict A, const double *restrict x, int n) {
    /* FIXME: parallelise over the rows i (s and Ai stay private) */
    #pragma omp FIXME
    for (int i = 0; i < n; ++i) {
        double s = 0.0;
        const double *Ai = A + (size_t)i * n;
        for (int j = 0; j < n; ++j) s += Ai[j] * x[j];
        y[i] = s;
    }
}

/* y = a - b */
static void vec_sub(double *restrict y, const double *restrict a, const double *restrict b, int n) {
    /* FIXME: parallelise (y = a - b) */
    #pragma omp FIXME
    for (int i = 0; i < n; ++i) y[i] = a[i] - b[i];
}

/* ----- Conjugate Gradient for SPD A ----- */
static int cg_solve(const double *A,
                    const double *b,
                    double *x,
                    int n,
                    int max_iter,
                    double tol,
                    int verbose)
{
    int it = 0;
    double *r  = (double*)malloc((size_t)n * sizeof *r);
    double *p  = (double*)malloc((size_t)n * sizeof *p);
    double *Ap = (double*)malloc((size_t)n * sizeof *Ap);
    if (!r || !p || !Ap) { free(r); free(p); free(Ap); return 1; }

    /* r = b - A x; p = r */
    if (x) {
        matvec(Ap, A, x, n);
        vec_sub(r, b, Ap, n);
    } else {
        /* If passes NULL x, treat as zero initial guess */
        memset(r, 0, (size_t)n * sizeof *r);
        copy(r, b, n);
        x = (double*)calloc((size_t)n, sizeof *x);
        if (!x) { free(r); free(p); free(Ap); return 1; }
    }
    copy(p, r, n);

    double rs_old = dotProduct(r, r, n);
    const double tol2 = tol * tol;

    for (it = 0; it < max_iter; ++it) {
        matvec(Ap, A, p, n);
        double pAp = dotProduct(p, Ap, n);
        if (pAp == 0.0) { /* breakdown */
            if (verbose) fprintf(stderr, "CG breakdown: p^T A p = 0 at iter %d\n", it);
            break;
        }
        double alpha = rs_old / pAp;

        /* x_i+1 = x_i + alpha * p */
        alpha_pscalar(x, p,  alpha, n);

        /* r_i+1 = r_i - alpha * Ap */
        alpha_pscalar(r, Ap, -alpha, n);

        double rs_new = dotProduct(r, r, n);
        if (verbose) {
            fprintf(stderr, "iter %4d  |r| = %.6e\n", it+1, sqrt(rs_new));
        }
        if (rs_new <= tol2) break;

        /* beta = rs_new / rs_old */
        double beta = rs_new / rs_old;

        /* p = r + beta * p */
        scalar(p, beta, n);
        alpha_pscalar(p, r, 1.0, n);

        rs_old = rs_new;
    }

    free(r);
    free(p);
    free(Ap);
    return it;  /* iterations performed */
}

/* ----- main: read dense matrix from triples, run CG ----- */
int main(int argc, char **argv)
{
    double tol = 1e-5;
    if (argc > 1) tol = atof(argv[1]);

    int M; /* number of rows */
    int N; /* number of cols */
    int length; /* length of data, #non-zeros */

    if (scanf("%d %d %d", &M, &N, &length) != 3) {
        fprintf(stderr, "Failed to read M N length\n");
        return 1;
    }
    if (M != N) {
        fprintf(stderr, "Matrix must be square (got %d x %d)\n", M, N);
        return 1;
    }
    const int dim = M;

    /* Dense row-major matrix, zero-initialized */
    double *A = (double*)calloc((size_t)M * N, sizeof *A);
    double *b = (double*)malloc((size_t)dim * sizeof *b);
    double *x = (double*)calloc((size_t)dim, sizeof *x);

    if (!A || !b || !x) {
        perror("alloc");
        free(A); free(b); free(x);
        return 1;
    }

    /* Read length triples: 1-based rows/cols */
    for (int k = 0; k < length; ++k) {
        int row, col;
        double val;
        if (scanf("%d %d %lf", &row, &col, &val) != 3) {
            fprintf(stderr, "Bad triple at k=%d\n", k);
            free(A); free(b); free(x);
            return 1;
        }
        if (row < 1 || row > M || col < 1 || col > N) {
            fprintf(stderr, "Index out of range at k=%d: (%d,%d)\n", k, row, col);
            free(A); free(b); free(x);
            return 1;
        }
        A[(size_t)(row - 1) * N + (col - 1)] = val;
    }

    /* RHS = 1 */
    for (int i = 0; i < dim; ++i) b[i] = 1.0;

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    int iters = cg_solve(A, b, x, dim, /*max_iter=*/dim, /*tol=*/tol, /*verbose=*/1);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    printf("CG finished in %d iterations, time = %.6f s\n", iters, elapsed);

    free(A); free(b); free(x);
    return 0;
}
