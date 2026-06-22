Conjugate Gradient Method
============================================================

The Conjugate Gradient (CG) method is an iterative solver for a linear system

.. math::

   A x = b,

where :math:`A` is **symmetric positive-definite**. Starting from an initial
guess :math:`x_0` with residual :math:`r_0 = b - A x_0` and search direction
:math:`p_0 = r_0`, each iteration performs:

.. math::

   \begin{aligned}
   \alpha_k &= \frac{r_k^\top r_k}{p_k^\top A p_k} \\
   x_{k+1} &= x_k + \alpha_k p_k \\
   r_{k+1} &= r_k - \alpha_k A p_k \\
   \beta_k &= \frac{r_{k+1}^\top r_{k+1}}{r_k^\top r_k} \\
   p_{k+1} &= r_{k+1} + \beta_k p_k
   \end{aligned}

In exact arithmetic CG converges in at most :math:`n` iterations for an
:math:`n \times n` system. For background see *Iterative Methods for Sparse
Linear Systems* (Saad) and *Matrix Computations* (Golub & Van Loan).

What matters for parallelism is that **each iteration is built from a handful
of simple vector kernels**, and almost all the run time is spent in them:

* one **matrix-vector product** :math:`A p_k` — a loop over the rows, each
  computing a dot product (the dominant cost);
* two **dot products** :math:`r_k^\top r_k` and :math:`p_k^\top A p_k` — each a
  sum over :math:`n` elements, i.e. a **reduction**;
* a few **AXPY / scale** updates of the form :math:`y \mathrel{+}= \alpha x`.

Each kernel is a single loop over :math:`n` (or :math:`n^2`) elements with no
dependence between iterations, so each can be parallelised with a worksharing
loop — the dot products using ``reduction(+:...)``. The CG recurrence itself
stays sequential; only the kernels run in parallel.

The program reads its matrix in MatrixMarket coordinate format from standard
input (e.g. ``msc04515.dat`` or the small ``Trefethen_20.dat``). This is the
application behind Exercise 7.
