/* =================================================================
mandelbrot-omp.c

Written by Frederick Fung for NCI OpenMP Workshop March 2022, updated
by Frederick Fung in 2025

This program computes Mandelbrot set within a pre-defined region.
For plotting, the output is filled as the iteration goes.

Output: an 2D array. Each element corresponds to a pixel to be drawn.

The code is accelerated by openmp multi-threading.

Compile: gcc -fopenmp -g -Wall -O3 -o mandelbrot-omp mandelbrot-omp.c

Usage: ./mandelbrot-omp

.....................................................................

Produced for NCI Training.

Frederick Fung 2022, 2025
====================================================================*/


#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<omp.h>

#define MAXITER 100
#define IO

//#define OPENMP_TIMER

static const double XMIN = -2.0, XMAX = 0.47;
static const double YMIN = -1.12, YMAX = 1.12;

typedef struct { double re, im; } Complex;

static int mandelbrot_escape(Complex c) {
   Complex z = {0.0, 0.0};
   for (int i = 0; i < MAXITER; ++i) {
       /* z = z^2 + c */
       double zr2 = z.re * z.re;
       double zi2 = z.im * z.im;
       double two_zr_zi = 2.0 * z.re * z.im;

       z.re = zr2 - zi2 + c.re;
       z.im = two_zr_zi + c.im;

       /* escape test: |z|^2 > 4  diverges */
       if (z.re * z.re + z.im * z.im > 4.0)
           return i + 1;
   }
   return MAXITER;
}

static void gen_mandelbrot(int points){

  if (points <= 0) {
      fprintf(stderr, "points must be positive\n");
      return;
  }

  /* Include both endpoints by using N samples over a closed interval */
  const double stepx = (points > 1) ? (XMAX - XMIN) / (points - 1) : 0.0;
  const double stepy = (points > 1) ? (YMAX - YMIN) / (points - 1) : 0.0;

  /* allocate mem for the image */
  int *img[points];
  for (int n = 0; n < points; ++n){
    img[n] = (int *) malloc(points * sizeof(int));
  }

  int i, j;

  /* computation  no I/O - Each (i, j) writes to its own img[i][j], so there is no race and no
     ordering is needed.  */
  #ifdef OPENMP_TIMER
  double start = omp_get_wtime();
  #endif

  /* parallel construct */
  #pragma omp parallel private(i, j)
  {
     /* think about why dynamic scheduling is used here. */
     #pragma omp for schedule(dynamic)
      for (i = 0; i < points; i++){
         double x = XMIN + i * stepx;
         for (j = 0; j < points; j++){
            double y = YMIN + j * stepy;
            Complex c_num = { x, y };
            img[i][j] = mandelbrot_escape(c_num);
         }
      }
  }

  #ifdef OPENMP_TIMER
  double end = omp_get_wtime();
  printf("openmp walltime %f seconds\n ", end - start);
  #endif

  /* I/O phase*/
  #ifdef IO
  char path[128];
  snprintf(path, sizeof path, "mandelbrot_set_%d.csv", points);
  FILE *fp = fopen(path, "w");
  if (!fp) {
      perror("fopen");
  } else {
      for (i = 0; i < points; i++){
         for (j = 0; j < points; j++){
            fprintf(fp, (j + 1 < points) ? "%d," : "%d\n", img[i][j]);
         }
      }
      fclose(fp);
  }
  #endif

  /* free space */
  for (int n = 0; n < points; n++)
     free(img[n]);
}



int main(){


    int npoints_array[]= {100,1000,10000};

    for (int i=0; i< sizeof(npoints_array) / sizeof(npoints_array[0]); i++){

       int point = npoints_array[i];

       printf("Resolution #%d, Points %d by %d \n", i, point, point);

       gen_mandelbrot(point);

    }

    return 0;

}
