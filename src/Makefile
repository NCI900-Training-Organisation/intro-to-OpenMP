CC 	= gcc
CFLAGS	= -O0 
#CC	= icx
#CFLAGS	= -O3 -qopenmp
LDFLAGS	= -lpapi -lm -fopenmp

all:	openmp_parallel_region openmp_reduction openmp_max_threads papi\
	openmp_tasks openmp_depend openmp_datasharing openmp_if openmp_taskgroups openmp_simd \
	openmp_schedule openmp_master openmp_single openmp_taskloop openmp_tied openmp_sections openmp_nowait \
	openmp_parallel_for openmp_false_sharing openmp_false_sharing_avoid\
	exercise1 exercise1_solution exercise2 exercise2_solution exercise3 exercise3_solution \
	exercise4 exercise4_solution exercise5 exercise5_solution \
	exercise6 exercise6_solution exercise7 exercise7_solution

papi:	papi.c
	$(CC) $(CFLAGS) -o $@ $? -lpapi

cache:	cache.c
	$(CC) $(CFLAGS) -o $@ $? -lpapi

openmp_parallel_region:	openmp_parallel_region.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_reduction:	openmp_reduction.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_max_threads:	openmp_max_threads.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_datasharing: openmp_datasharing.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_schedule:	openmp_schedule.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_single:	openmp_single.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_false_sharing_avoid:	openmp_false_sharing_avoid.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_false_sharing:	openmp_false_sharing.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_parallel_for:	openmp_parallel_for.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_master:	openmp_master.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_tasks:	openmp_tasks.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)
openmp_depend:	openmp_depend.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_sections: openmp_sections.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_nowait:	openmp_nowait.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_if:	openmp_if.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_taskgroups:	openmp_taskgroups.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)	

openmp_taskloop:	openmp_taskloop.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)	

openmp_tied	:	openmp_tied.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

openmp_simd: openmp_simd.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)	
	
exercise1:	exercise1.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise1_solution:	exercise1_solution.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise2:	exercise3.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise2_solution:	exercise3_solution.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise3:	exercise3.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise3_solution:	exercise3_solution.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise4:	exercise4.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise4_solution:	exercise4_solution.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise5:	exercise5.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

exercise5_solution:	exercise5_solution.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

# Conjugate Gradient and Laplace do not use PAPI; build them with -O3 -lm.
exercise6:	exercise6.c
	$(CC) -O3 -fopenmp -o $@ $? -lm

exercise6_solution:	exercise6_solution.c
	$(CC) -O3 -fopenmp -o $@ $? -lm

exercise7:	exercise7.c
	$(CC) -O3 -fopenmp -o $@ $? -lm

exercise7_solution:	exercise7_solution.c
	$(CC) -O3 -fopenmp -o $@ $? -lm

.SUFFIXES:	.o .c

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	/bin/rm -f a.out *.o 
	/bin/rm -f papi cache
	/bin/rm -f openmp_tasks openmp_depend openmp_datasharing openmp_if openmp_taskgroups openmp_parallel_region
	/bin/rm -f openmp_simd openmp_parallel_section openmp_max_threads openmp_reduction openmp_master openmp_schedule openmp_single
	/bin/rm -f openmp_taskloop openmp_tied openmp_sections openmp_nowait openmp_parallel_for openmp_barrier openmp_false_sharing openmp_false_sharing_avoid
	/bin/rm -f exercise1 exercise1_solution exercise2 exercise2_solution exercise3 exercise3_solution 
	/bin/rm -f exercise4 exercise4_solution exercise5 exercise5_solution
	/bin/rm -f exercise6 exercise6_solution exercise7 exercise7_solution
	/bin/rm -f *.csv laplace-soln.dat

