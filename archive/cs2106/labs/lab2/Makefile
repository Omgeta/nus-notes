CC=gcc
CFLAGS=-g -std=c99 -Wall -Wextra -D_GNU_SOURCE
LDFLAGS=-ldl

OBJS_SHELL=myshell.o driver.o fork-wrapper.o
OBJS_MONITOR=monitor.o

all: myshell monitor programs

myshell: $(OBJS_SHELL)
	$(CC) $(OBJS_SHELL) $(LDFLAGS) -o myshell

monitor: $(OBJS_MONITOR)

.PHONY: programs clean

programs:
	$(MAKE) -C programs all

clean:
	$(MAKE) -C programs clean
	rm -f myshell monitor $(OBJS_SHELL) $(OBJS_MONITOR)
