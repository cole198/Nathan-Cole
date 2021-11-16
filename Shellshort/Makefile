SRC_C=shell_array.c shell_list.c 
SRC_H=shell_array.h shell_list.h list_of_list.h
TEST_C=main.c
TEST_EXPECTED=expected.txt
TEST_ACTUAL=actual.txt
EXECUTABLE=main
ASG_NICKNAME=HW17
CC=gcc
CFLAGS=-g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic -DDEBUG
CFLAGS_GCOv=$(CFLAGS) -fprofile-arcs -ftest-coverage
SHELL=/bin/bash
SUBMIT_FILES=$(SRC_C) $(SRC_H) $(TEST_C) Makefile huff.txt 

$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) -o $(EXECUTABLE) $(SRC_C) $(SRC_H) $(TEST_C) $(CFLAGS)
submit: 
	264submit $(ASG_NICKNAME) $(SUBMIT_FILES)
test: $(EXECUTABLE) 
	@if diff -a -B <("./$(EXECUTABLE)") $(TEST_EXPECTED) &> /dev/null ; then \
		echo -e '\x1b[32mTest passed: output of $(TEST_ACTUAL) matches $(TEST_EXPECTED)\x1b[0m' ; \
	else \
		echo -e '\x1b[31mTest failed: output of $(TEST_ACTUAL) does NOT match $(TEST_EXPECTED)\x1b[0m' ; \
	fi
clean:
	rm -f $(EXECUTABLE)
	rm -f *.c.gcov *.gcda *.gcno
pretest: submit
	264test $(ASG_NICKNAME)
coverage: $(SRC_C) $(TEST_C)
	gcc -o $(EXECUTABLE) $(SRC_C) $(TEST_C) -ftest-coverage -fprofile-arcs -DNDEBUG
	./$(EXECUTABLE)
	gcov -f $(SRC_C)


.PHONY: submit test clean pretest coverage
