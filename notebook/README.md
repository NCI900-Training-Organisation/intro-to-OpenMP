# OpenMP-C hands-on notebook

A self-contained Jupyter notebook (`OpenMP-C.ipynb`) that drives the OpenMP
hands-on session through four worked examples:

1. **Monte-Carlo π** — independent loop iterations
2. **Mandelbrot set** — dynamic (load-imbalanced) iterations + separating I/O from compute
3. **Conjugate Gradient** — parallelising the kernels of a loop-carried-dependent solver
4. **Finite-difference Poisson** — Jacobi vs. red-black Gauss-Seidel

All source files referenced by the notebook live in this directory, so it runs
as a unit.

## Running it

Launch Jupyter **from this directory** so the working directory contains the
source files:

```
cd notebook
jupyter lab        # or: jupyter notebook
```

then run the cells top to bottom. The first cell changes into the materials
directory; on NCI ARE that defaults to `/scratch/vp91/$USER/OpenMP-C`, and
otherwise it stays in the directory the notebook was launched from. To point it
somewhere specific, set the `OPENMP_C_DIR` environment variable before launching.

## The exercises

The `*-serial.c` files are complete; you parallelise the matching `*-omp.c`
files (and `mandelbrot-separateIO.c`) by replacing their `#pragma omp FIXME`
markers. Worked solutions for the same problems are also available in the
project's `src/` directory as `exercise3/4/6/7_solution.c`.

`Trefethen_20.dat` and `msc04515.dat` are MatrixMarket inputs for the Conjugate
Gradient example; `Mandelbrot-plot.py` and `Laplace-plot.py` visualise the
outputs.
