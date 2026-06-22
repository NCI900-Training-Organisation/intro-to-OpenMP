Further Worked Examples
============================================================

These two examples apply the worksharing-loop techniques from the previous
sections to complete numerical applications: an iterative PDE solver and an
iterative linear-system solver. They also introduce one more loop clause,
``collapse``.

The ``collapse`` Clause
------------------------------------------------------------

Many loops are nested. By default ``#pragma omp for`` only distributes the
*outermost* loop across threads. The `collapse clause <https://www.openmp.org/spec-html/5.1/openmpsu48.html#x73-730002.11.4>`_
tells OpenMP to merge a number of perfectly nested loops into a single, larger
iteration space and distribute *that*:

.. code-block:: c

   #pragma omp parallel for collapse(2)
   for (int i = 0; i < N; i++)
       for (int j = 0; j < M; j++)
           /* ... */

This is useful when the outer loop alone has too few iterations to keep all
threads busy (e.g. ``N`` smaller than the number of threads), or simply to
expose more parallelism and improve load balance. The collapsed loops must be
*perfectly nested* (no code between them) and have bounds that do not depend on
each other.

Exercise 6
------------------------------------------------------------

29. The program ``exercise6.c`` solves the :doc:`2D Poisson equation <applications/laplace>`
    by finite differences, using either Jacobi or red-black Gauss-Seidel
    iteration. Each solver sweeps over a 2D :math:`(i,j)` mesh, so it is a
    natural candidate for ``collapse(2)``. Parallelise the residual and the
    solver sweeps. The solution is in ``exercise6_solution.c``.

    .. code-block:: console

       make exercise6_solution

       # arguments: grid size, tolerance, method ("Jacobi" or "Gauss-Seidel")
       OMP_NUM_THREADS=4 ./exercise6_solution 512 1e-5 Jacobi
       OMP_NUM_THREADS=4 ./exercise6_solution 512 1e-5 Gauss-Seidel

    The solution is written to ``laplace-soln.dat`` and can be visualised with
    ``Laplace-plot.py``. Try different thread counts and compare the two
    methods.

Exercise 7
------------------------------------------------------------

30. The program ``exercise7.c`` solves a symmetric positive-definite system
    with the :doc:`Conjugate Gradient method <applications/conjugate-gradient>`.
    Almost all of the run time is spent in a few vector kernels — a
    matrix-vector product, two dot products, and some AXPY/scale updates.
    Parallelise each kernel with a worksharing loop, using ``reduction(+:...)``
    for the dot products. The CG iteration itself stays sequential. The
    solution is in ``exercise7_solution.c``.

    .. code-block:: console

       make exercise7_solution

       # argument: tolerance; the matrix is read from stdin (MatrixMarket)
       OMP_NUM_THREADS=4 ./exercise7_solution 1e-5 < msc04515.dat

    A small system (``Trefethen_20.dat``) is also provided for a quick check.
    Vary ``OMP_NUM_THREADS`` and observe how the solve time changes.
