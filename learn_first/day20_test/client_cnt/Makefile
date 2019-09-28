SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
$(ELF):$(OBJS)
	gcc $^ -Wall -o $@ -lpthread -lrt
clean:
	rm -rf $(OBJS) $(ELF)

