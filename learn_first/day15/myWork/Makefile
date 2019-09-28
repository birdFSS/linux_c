SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=server
$(ELF):$(OBJS)
	gcc $^ -Wall -o $@ -lpthread
clean:
	rm -rf $(OBJS) $(ELF)

