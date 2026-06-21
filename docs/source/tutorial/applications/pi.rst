Calculating the Value of ``π`` Using Monte Carlo Method
============================================================

.. image:: ../../figs/pi.png
   :alt: Monte Carlo estimation of pi
   :align: center

we have a circle of radius 0.5, enclosed by a 1 × 1 square. The area of the circle is:

.. math::

   \pi r^2 = \pi \times 0.5^2 = \pi/4

The area of the square is 1 and the ratio of the area of the circle to the area of the square is:

.. math::

   area(circle) / area(square) = (\pi/4) / 1 = \pi/4

If we generate a large number of uniform points that falls within the square, some of the points will fall within circle and some outside it. The ratio of points inside the circle to the toatal number of points will be approximately equal to the ratio of areas of the circle and the square, ie,

.. math::

   area(circle) / area(square) = N_{inner} / N_{total}

.. math::

   \pi/4 = N_{inner} / N_{total}

.. math::

   \pi = 4 \times (N_{inner} / N_{total})

These calculations are *embarassingle parellel* and they can benefit from multi-threaded programming.
