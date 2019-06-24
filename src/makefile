C=gcc
RM=rm -f
CFLAGS=-O3 -lpthread -Wall -pedantic
SO=-soname

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	SO=-install_name
endif

main: main.c ga.o utility.o crossover.o mutate.o selection.o sort.o
	$(C) -o main main.c -lm ga.o utility.o crossover.o mutate.o selection.o sort.o $(CFLAGS)

tests: run_tests.o utility.o crossover.o mutate.o selection.o CuTest.o sort.o tests.o
	$(C) -o tests run_tests.c -lm utility.o crossover.o mutate.o selection.o CuTest.o sort.o tests.o $(CFLAGS)

so: ga.c utility.o crossover.o mutate.o selection.o sort.o
	$(C) -shared -Wl,$(SO),ga.so -o ga.so -fPIC ga.c -lm utility.o crossover.o mutate.o selection.o sort.o $(CFLAGS)

ga.o: ga.c
	$(C) -c -fPIC ga.c $(CFLAGS)

utility.o: utility.c
	$(C) -c -fPIC utility.c $(CFLAGS)

crossover.o: crossover.c
	$(C) -c -fPIC crossover.c $(CFLAGS)

mutate.o: mutate.c
	$(C) -c -fPIC mutate.c $(CFLAGS)

selection.o: selection.c
	$(C) -c -fPIC selection.c $(CFLAGS)

sort.o: sort.c
	$(C) -c -fPIC sort.c $(CFLAGS)

run_tests.o: run_tests.c
		$(C) -c run_tests.c $(CFLAGS)

tests.o: tests.c
	$(C) -c tests.c $(CFLAGS)

CuTest.o: CuTest.c
	$(C) -c CuTest.c $(CFLAGS)

clean:
	$(RM) main tests *.o *.so
