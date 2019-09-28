SRCS:=$(wildcard *.c)
TARGETS:=$(SRCS:%.C=%)
all:$(TARGETS)
%:%.c
	gcc $< -g -o $@ -lpthread
clean:
	rm $(TARGETS)
