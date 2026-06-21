OpenMP Directives
============================================================

``parallel`` Regions
------------------------------------------------------------

A *parallel region* is a structured block of code that is to be executed in parallel by a number of threads. Each thread executes the structured block independently. *Note: it is illegal for code to branch out of a parallel region.* The basic structure of the `parallel construct <https://www.openmp.org/spec-html/5.1/openmpse14.html#x59-590002.6>`_ is as follows:

.. code-block:: c

   #pragma omp parallel [clause[ [,] clause] ... ]
   {
       /*structured block*/
   }

As an example, consider the following code:

.. code-block:: c

   #include <omp.h>
   #include <stdio.h>

   int main(void)
   {

     printf("Total number of threads allocated in the parellel section %d \n", omp_get_num_threads() );
     #pragma omp parallel
     {
       printf("This is run by thread %d \n", omp_get_thread_num());
     }

     return 0;
   }

The above code is contained in file ``openmp_parallel_region.c``. Compile it by typing::

   make openmp_parallel_section

.. note::
   Each program can be compiled by: *make <program name>*

.. note::
   All the programs can be compiled in a single go by: *make all*

1. Run the code by typing ``./openmp_parallel_region``

2. Now run the code with 4 threads by first setting the ``OMP_NUM_THREADS`` variable to 4::

      OMP_NUM_THREADS=4 ./openmp_parallel_region

3. Now run the code by setting the ``OMP_DYNAMIC`` to true::

      OMP_DYNAMIC=true ./openmp_parallel_region

What difference do you see between the different runs?

There is no guarantee that the requested number of threads will be allocated. ``omp_get_num_threads()`` provides the actual allocated thread count, and ``omp_get_thread_num()`` retrieves a thread's index. When designing an OpenMP program, focus on building the algorithm for the allocated threads, not the requested number.

Some other useful OpenMP routines are:

* `omp_set_num_threads(np) <https://www.openmp.org/spec-html/5.1/openmpsu120.html#x159-1920003.2.1>`_: sets the number of parallel threads to be used for parallel regions
* `omp_get_max_threads() <https://www.openmp.org/spec-html/5.1/openmpsu122.html#x161-1940003.2.3>`_: gives the maximum number of threads that could be used

The above three functions are used in the program ``openmp_max_threads.c``.

4. Make and run the program ``openmp_max_threads.c``. Run the program using::

      ./openmp_max_threads 10

How many threads are created? What is the maximum threads allowed?

The ``reduction`` Clause
------------------------------------------------------------

A `reduction clause <https://www.openmp.org/spec-html/5.1/openmpsu117.html#x152-1720002.21.5>`_ can be added to the parallel directive. This specifies that the final values of certain variables are combined using the specified operation (or intrinsic function) at the end of the parallel region. For example, consider the program ``openmp_reduction.c``, which demonstrates a number of reduction operations and also shows the use of the `omp_get_thread_num() <https://www.openmp.org/spec-html/5.1/openmpsu123.html#x162-1950003.2.4>`_ routine to uniquely define each thread.

The following table shows the initial value assigned to a reduction variable for each reduction operation.

.. list-table::
   :header-rows: 1

   * - Operator
     - Initial Value
   * - ``+``
     - 0
   * - ``-``
     - 0
   * - ``*``
     - 1
   * - ``&``
     - ~ 0
   * - ``|``
     - 0
   * - ``^``
     - 0
   * - ``&&``
     - 1
   * - ``||``
     - 0
   * - ``max``
     - Least representable number
   * - ``min``
     - Largest representable number

5. Run the program ``openmp_reduction.c`` with four threads and make sure you understand what is happening::

      make openmp_reduction
      OMP_NUM_THREADS=4 ./openmp_reduction

The Datasharing Clause
------------------------------------------------------------

The optional ``clause``\ s can be used to define data sharing as follows:

* ``shared(list)`` specifies variables that are visible to all threads. If you specify a variable as shared, you are stating that all threads can safely share a single copy of the variable.
* ``private(list)`` specifies that each thread has its own uninitialized local copy of each variable listed.
* ``firstprivate(list)`` specifies that each thread has its own local copy of each variable listed, which is initialized to the value that the variable has on entry to the block.
* ``default(data-sharing-attribute)`` - where for C/C++ the ``data-sharing-attribute`` is either ``shared`` or none. When you specify the default ``data-sharing-attribute``, you declare the default for all variables in the code block to be shared or to have no default (none). *Note - Fortran also permits a default of* ``private``\ *. This is not available in C/C++ since many of the standard libraries use global variables, and scoping these as local would give errors.*

6. Run the program ``openmp_datasharing.c`` with four threads and identtify the difference between the different clauses::

      make openmp_datasharing
      OMP_NUM_THREADS=4 ./openmp_datasharing

Exercise 1
------------------------------------------------------------

7. The program ``exercise1.c`` computes the sum of all integers from 1 to ``num_elem``, and creates ``p`` OpenMP threads. Currently, this task is performed using the following loop, using only the main thread:

   .. code-block:: c

      sum = 0;
      i = 0;

      while (i < nele) {
        i++;
        sum += i;
      }

   Parallelize this summation by using OpenMP to manually divide (this means you are not to convert this to a ``for`` loop and use ``#pragma omp for``) up the loop operations amongst the available OpenMP threads. Your parallel code must continue to use a ``while`` construct. Solution is available in ``exercise1_solution.c``.

Race Condition and Critical Sections
------------------------------------------------------------

.. image:: ../figs/race.png
   :alt: Race condition
   :align: center

A *race condition* arises when two threads simultaneously access a shared variable. Initially, the first thread reads the variable, and shortly thereafter, the second thread reads the same value from that variable. Subsequently, both the first and second threads carry out their respective operations on the value, and a race ensues to determine which thread can write its value last to the shared variable. The final value preserved in the shared variable is that which is written by the thread that manages to write its value last, effectively overwriting any prior values set by other threads.

A *critical section* refers to a segment of code responsible for accessing shared resources, such as variables or data structures. This section necessitates exclusive execution by a single process at any given moment to prevent the occurrence of race conditions and other synchronization-related problems.

In the solution of the previous excercise you can see this race condition. Every time you run the program you may get a different value for sum. This problem can be addressed in OpenMP using ``critical`` or ``atomic`` construct.

The ``critical`` Construct
------------------------------------------------------------

The `critical construct <https://www.openmp.org/spec-html/5.1/openmpsu99.html#x132-1420002.19.1>`_ ensures that a block of code is only executed by one processor at a time. Effectively this serializes portions of a parallel region.

.. code-block:: c

   #pragma omp critical [(name)]
   {
     /*structured block*/
   }

A thread will wait at the beginning of the critical section until no other thread in the team is executing that (named) section.

The ``atomic`` Construct
------------------------------------------------------------

The `atomic construct <https://www.openmp.org/spec-html/5.1/openmpsu105.html#x138-1480002.19.7>`_ ensures that memory locations are accessed atomically, to help avoid race conditions or reads or writes that would result in indeterminate values.

``#pragma omp atomic``

The directive refers to the line of code immediately following it. Be aware that there may be an overhead associated with the setting and unsetting of locks - so use this directive and/or critical sections only when necessary. For example, we could use the atomic construct to parallelize an inner product:

.. code-block:: c

   #pragma omp parallel for shared(a,b,sum) private(I,tmp)
   for (i = 0; i < n; i++) {
     tmp = a[i] * b[i];
     #pragma omp atomic
     sum = sum + tmp;
   }

but the performance would be very poor!

Exercise 2
------------------------------------------------------------

8. The program ``exercise1_solution.c`` has a race condition. Solve this race condition using the the construct ``atomic`` or ``critical``. The solutions are availble in ``exercise2_solution.c``.

False Sharing
------------------------------------------------------------

.. image:: ../figs/false_sharing.png
   :alt: False sharing
   :align: center

All modern processors use cache. Accessing a memory location not only copy that memory location, but a slice of memory to me moved to the cache. This slice of memory is called **cache line**. For example when you aceess an array element ``A[N]`` there is a good chance ``A[N+1]`` and ``A[N+2]`` is also moved to the cache.

Concurrent updated to separate elements within a shared cache line by different processors cause the entire cache line to be invalidated, despite the logically independent nature of these updates. Each update to an element within the cache line flags the entire line as invalid, affecting other threads attempting to access different elements within the same line. Consequently, they are compelled to retrieve a fresher version of the line from memory or an alternate source, even if the specific element they're accessing hasn't been altered. This occurs because cache coherence operates at the level of cache lines, not individual elements. Consequently, it leads to amplified interconnect activity and additional processing overhead. Furthermore, during the update of the cache line, access to the elements within it is restricted.

For instance if Thread T1 changes the data ``A[N]`` it will make the entire cache line invalid. Which means the data ``A[N+1]`` and ``A[N+2]`` also becomes invalid. So, if Threard T2 tries to access ``A[N+1]`` it will see that the the cache line is invalid and will fetch the data from the memory. This is **false sharing**.

.. image:: ../figs/cache1.png
   :alt: Cache invalidation
   :align: center

Some methods to avoid false sharing are:

* Refrain from modifying global data accessed by multiple threads.
* Ensure that shared global data is aligned with cache line boundaries.
* Avoid using an array indexed by thread ID or rank to store temporary, thread-specific data.
* When parallelizing an algorithm, partition data sets along cache lines, rather than across them.

.. image:: ../figs/cache2.png
   :alt: Cache line partitioning
   :align: center

9. The programs ``openmp_false_sharing.c`` and ``openmp_false_sharing_avoid.c`` demonstrates false sharing and a method to avoid it.

The Worksharing-Loop Construct (``for``)
------------------------------------------------------------

In the program ``exercise2_solution.c``, we parallelized a loop by manually assigning different loop indices to different threads. With ``for`` loops, OpenMP provides the `worksharing-loop construct <https://www.openmp.org/spec-html/5.1/openmpsu48.html#x73-730002.11.4>`_ to do this for you. This directive is placed immediately before a for loop and automatically partitions the loop iterations across the available threads.

.. code-block:: c

   #pragma omp for [clause[[,]clause ...]
   for (...) { ... }

``openmp_parallel_for.c`` demonstrates how the work the ``for`` construct works. Note that ``for`` construct only handles the distribution of work to different threads. We still have to manage the critical sections and make sure there are no race conditions.

Exercise 3
------------------------------------------------------------

10. ``exercise3.c`` calculates the value :doc:`π Using Monte Carlo Method <applications/pi>`. Parallelize the program using the ``for`` construct. The solution is available in ``exercise3_solution.c``.

The ``schedule`` Construct
------------------------------------------------------------

An important optional clause is the ``schedule(type[,chunk])`` clause. This can be used to define specifically how the iterations are divided amongst the different threads. Two distribution schemes are:

* ``(static,chunk-size)``: iterations are divided into pieces of a size specified by chunk and these chunks are then assigned to threads in a round-robin fashion.
* ``(dynamic,chunk-size)``: iterations are divided into pieces of a size specified by chunk. As each thread finishes a chunk, it dynamically obtains the next available chunk of loop indices.

11. ``openmp_schedule.c`` demonstrates how the two scheduling options differ from one another::

       ./openmp_schedule

Exercise 4
------------------------------------------------------------

12. The program ``exercise4.c`` generates the :doc:`mandelbrot <applications/mandelbrot>` set. Paralleize the program using different OpenMP directives. Test how ``static`` and ``dynamic`` influences the performance of the program. The solution is available in ``exercise4_solution.c``.

The ``barrier`` Construct
------------------------------------------------------------

In any parallel program, there will be certain points where you wish to synchronize all your threads. This is achieved by using the `barrier construct <https://www.openmp.org/spec-html/5.1/openmpsu100.html#x133-1430002.19.2>`_.

``#pragma omp barrier``

All threads must arrive at the barrier before any thread can continue. Some OpenMP constructs have implicit barriers.

13. Program ``openmp_barrier.c`` demonstrates the working of implicit and explicit barriers.

``nowait`` Construct
------------------------------------------------------------

The ``nowait`` clause overrides any synchronization that would otherwise occur at the end of a construct.

.. code-block:: c

   #pragma omp for nowait [clause[[,]clause ...]
   for (...) { ... }

14. Program ``openmp_nowait.c`` demonstrates how we can use nowait with ``for`` construct.

The ``single`` and ``master`` Construct
------------------------------------------------------------

Certain pieces of code you may only want to run on one thread - even though multiple threads are executing. For example, you often only want output to be printed once from one thread. This can be achieved using the `single construct <https://www.openmp.org/spec-html/5.1/openmpsu43.html#x67-670002.10.2>`_:

.. code-block:: c

   #pragma omp single [clause]
   {
     /*structured block*/
   }

or using the `master construct <https://www.openmp.org/spec-html/5.0/openmpse24.html>`_

.. code-block:: c

   #pragma omp master [clause]
   {
     /*structured block*/
   }

In the ``single`` construct the thread that encounters the code block first, executes it. While in the ``master`` construct the master thread always executes the code.

By default, all other threads will wait at the end of the structured block until the thread executing that block has completed. You can avoid this by augmenting the single directive with a ``nowait`` clause.

15. ``openmp_single.c`` demonstrates how the ``single`` construct works
16. ``openmp_master.c`` demonstrates how the ``master`` construct works.

The ``sections`` Construct
------------------------------------------------------------

.. image:: ../figs/sections.png
   :alt: Sections construct
   :align: center

A program can be divided into different sections. Each of these section can be completed by a separate thread. This is especially usefull when the sections are independent of one another.

.. code-block:: c

   #pragma omp parallel
   {
       #pragma omp sections
       {
           #pragma omp section
           int sum = a + b;

           #pragma omp section
           int diff = a - b;

           #pragma omp section
           int prod = a * b;
       }
   }

17. ``openmp_sections.c`` demonstrates how the ``sections`` construct works.

The ``if`` Clause
------------------------------------------------------------

``if`` clause specifies whether a code segment should be executed in parallel or in serial. ``if`` clause applies to ``parallel``, ``for`` and ``sections``.

.. code-block:: c

   #pragma omp parallel if (val)
   if (omp_in_parallel()) {
       ....
   }
   else {
       ....
   }

All parallel programs are bound by the `Amdhal's Law <https://en.wikipedia.org/wiki/Amdahl%27s_law>`_ - "*The overall performance improvement gained by optimizing a single part of a system is limited by the fraction of time that the improved part is actually used*".

.. image:: ../figs/amdahls.png
   :alt: Amdahl's Law
   :align: center

That is, the performance gain from parallelization is limited by the part of the program that can be parallelised. In the above diagram *Program 1* will benefit more from parallelization when compared to *Program 2*.
In addition lanching threads have a non-trivial cost. So running things in parallel may not be helpful if the work to paralleised is trivial. We can use the you ``if`` clause to run things in parallel only if we have non-trivial work to parellelise.

18. The program ``openmp_if.c`` demonstrates how you can use the ``if`` clause. Run the program with different combination of *threads* and *elements*. What difference do you see?
19. Change the *THRESHOLD* value in the program. What difference do you see?

The ``simd`` Clause
------------------------------------------------------------

The ``simd`` construct facilitates the concurrent execution of multiple iterations within associated loops by utilizing SIMD instructions. Each parallel iteration is processed by a distinct SIMD lane.

.. code-block:: c

   #pragma omp parallel for simd reduction(+: acc)
   for (int k = 0; k < LIMIT; k++) {
       float mul = a[k] * b[k];
       acc += mul;
   }

.. image:: ../figs/simd.png
   :alt: SIMD lanes
   :align: center

20. The program ``openmp_simd.c`` demonstrates how you can use the ``simd`` clause.
21. How does the performance differ with and without the ``simd`` clause?
