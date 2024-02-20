CC=gcc
CFL=-Wall -Werror -Wextra -lstdc++
PFL=-std=c++17 -Wpedantic
OFL=-fPIC -O3 -march=native
TFL=-g -lgtest
COVFL=--coverage
LCOVFL=-fprofile-arcs -ftest-coverage --coverage
LIB=S21_matrix_oop.cc
TEST=test.cc 
OBJ=S21_matrix_oop.o

all: clean s21_matrix_oop.a test

clean:
	rm -rf test
	rm -rf report
	rm -rf gcov_report
	rm -rf *.dSYM
	rm *.a
	rm *.o
	rm *.gc*


test:
	$(CC) $(CFL) $(PFL) $(TFL) $(LIB) $(TEST) -o test
	./test

s21_matrix_oop.a:
	$(CC) -Wall -Werror -Wextra -std=c++17 -Wpedantic -c $(LIB) -o s21_matrix_oop.o
	ar rc S21_matrix_oop.a $(OBJ)
	ranlib S21_matrix_oop.a

lcov_report:
	$(CC) $(CFL) $(TFL) $(PFL) $(LCOVFL) $(LIB) $(TEST)  -o report.out
	./report.out
	lcov --exclude /usr --exclude test.cc -t "report" -o report.info -c -d .
	genhtml -o ./report report.info
	open -a "Safari" report/index.html
	rm report.* *.gc*
.PHONY: lcov_report

leaks:
	$(CC) $(CFL) $(PFL) $(TFL) $(LIB) $(TEST) -o leaks
	leaks -atExit -- ./leaks
	rm leaks
.PHONY: leaks

style_check:
	clang-format -style=google *.cc *.h -n
.PHONY: check_style

style_format:
	clang-format -style=google *.cc *.h -i
.PHONY: format
