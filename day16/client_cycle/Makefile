SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
$(ELF):$(OBJS)
	gcc $^ -Wall -o $@ -lpthread
clean:
	rm -rf $(OBJS) $(ELF)

