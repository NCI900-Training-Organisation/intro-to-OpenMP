2D Poisson Equation by Finite Differences
============================================================

We solve a model 2D Poisson problem with Dirichlet boundary conditions on the
unit square :math:`[0,1]^2`:

.. math::

   -\Delta u = 2\pi^2 \sin(\pi x)\sin(\pi y) \quad \text{in } [0,1]^2,
   \qquad u = \sin(\pi x)\sin(\pi y) \ \text{ on the boundary.}

The square is covered by a uniform mesh with spacing :math:`h`, and the
Laplacian is approximated at each interior point by the standard **five-point
stencil**:

.. math::

   \frac{4 u_{i,j} - u_{i-1,j} - u_{i+1,j} - u_{i,j-1} - u_{i,j+1}}{h^2} = f_{i,j}.

This produces a large sparse linear system, which we solve **iteratively**
rather than directly. Two classic relaxation schemes are provided:

* **Jacobi.** Every new value is computed purely from the *old* neighbouring
  values:

  .. math::

     u^{\text{new}}_{i,j} = \tfrac{h^2}{4} f_{i,j}
       + \tfrac14\left(u_{i-1,j} + u_{i+1,j} + u_{i,j-1} + u_{i,j+1}\right).

  Because every update reads only the previous iterate, the entire interior
  sweep is independent and can be run in parallel with no special care.

* **Gauss-Seidel (red-black).** Gauss-Seidel updates in place, which would
  normally create dependencies between neighbouring points. Colouring the grid
  like a checkerboard breaks this: a *red* point :math:`(i+j\text{ even})`
  depends only on *black* neighbours and vice-versa. Each colour sweep
  therefore updates points that are mutually independent and can be
  parallelised, while the two sweeps run one after the other.

Both solvers iterate over a two-dimensional :math:`(i,j)` index space, which
makes them a natural fit for the :doc:`collapse clause <../further-examples>`.
This is the application behind Exercise 6.
