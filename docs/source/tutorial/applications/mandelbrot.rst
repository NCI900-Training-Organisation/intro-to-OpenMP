Finding Mandelbrot Set Using Monte Carlo Sampling
============================================================

.. image:: ../../figs/mandelbrot.png
   :alt: Mandelbrot set
   :align: center

Mandelbrot set generates a list of complex numbers using the formulae

.. math::

   z_{n} = z^{2}_{n-1} + c

where

.. math::

   z_{0} = 0

If the values of the complex numbers generated gets larger and larger then the choice of :math:`c` is not in the Mandelbrot set. To plot an image the points inside the Mandelbrot set is given a particular colour and all other points are given a different colour based on the iteration required to determine they are outside the Mandelbrot set.

In the Monte Carlo method, we generate pseudo random points in the complex plane, and then these points are tested if they are in the general Mandelbrot set or not. As can be imagined, for different :math:`c` the calculation required to determine whether :math:`c` is part of the Mandelbrot set is going to be different. We can use this property of Mandelbrot set to demonstrate some features of OpenMP.
